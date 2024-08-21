// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Client.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    TCPClient::TCPClient(Ref<Reactor> Executor, Socket&& Channel)
        : mExecutor  { Executor },
          mSocket    { Move(Channel) },
          mEncoder   { 65'535 },
          mDecoder   { 65'535 },
          mState     { State::Closed },
          mFlushable { false }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    TCPClient::~TCPClient()
    {
        Close(true);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::Connect(CStr Address, UInt16 Port)
    {
        State Expected = State::Closed;

        if (mState.compare_exchange_strong(Expected, State::Connecting))
        {
            const SPtr<asio::ip::tcp::resolver> Resolver
                = NewPtr<asio::ip::tcp::resolver>(mSocket.get_executor());

            const auto OnCompletion = [Self = shared_from_this(), Resolver](const auto Error, auto Result)
            {
                Self->WhenResolve(Error, Result);
            };
            Resolver->async_resolve(Address.data(), std::to_string(Port), OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::Start()
    {
        mState.store(State::Connected, std::memory_order_release);

        mSocket.set_option(asio::ip::tcp::no_delay(true));

        DoRead(false, sizeof(Header));

        mFlushable.store(true, std::memory_order_release);

        const auto OnCallback = [Self = shared_from_this()]()
        {
            if (ConstSPtr<Protocol> Protocol = Self->GetProtocol())
            {
                Protocol->OnAttach(Self);
            }
        };
        asio::post(mExecutor, OnCallback);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SStr TCPClient::GetAddress() const
    {
        return mSocket.remote_endpoint().address().to_v4().to_string();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::Close(Bool Immediately)
    {
        const State State = mState.exchange(State::Closing, std::memory_order_acquire);

        if (State == State::Connected || State == State::Connecting)
        {
            const Bool Flushable = mFlushable.load(std::memory_order_acquire);

            if (Immediately || (Flushable && mEncoder.IsEmpty()))
            {
                DoClose();
            }
            else
            {
                Flush();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::Write(CPtr<const UInt8> Bytes)
    {
        const State State = mState.load(std::memory_order_acquire);

        if (State == State::Connected)
        {
            const CPtr<UInt8> Chunk = mEncoder.Reserve(Bytes.size() + sizeof(Header));

            if (Chunk.empty())
            {
                Log::Error("Client: \"Can't reserve more data in the write buffer\"");

                DoClose();
            }
            else
            {
                * reinterpret_cast<Ptr<Header>>(Chunk.data()) = static_cast<Header>(Bytes.size());

                std::memcpy(Chunk.data() + sizeof(Header), Bytes.data(), Bytes.size());

                if (ConstSPtr<Protocol> Protocol = GetProtocol())
                {
                    Protocol->OnWrite(shared_from_this(), Chunk.subspan(sizeof(Header), Bytes.size()));
                }

                mEncoder.Commit(Chunk.size());
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::Flush()
    {
        const Bool Flushable = mFlushable.exchange(false);

        if (Flushable)
        {
            DoFlush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::DoClose()
    {
        const State State = mState.exchange(State::Closed);
        if (State != State::Closed)
        {
            // shutdown socket properly
            asio::error_code Error;
            mSocket.shutdown(asio::ip::tcp::socket::shutdown_both, Error);
            mSocket.close(Error);

            // reset all buffer(s)
            mEncoder.Reset();
            mDecoder.Reset();
            mFlushable.store(false, std::memory_order_relaxed);

            const auto OnCallback = [Self = shared_from_this()]()
            {
                if (ConstSPtr<Protocol> Protocol = Self->GetProtocol())
                {
                    Protocol->OnDetach(Self);
                }
            };
            asio::post(mExecutor, OnCallback);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::DoRead(Bool Body, UInt32 Quantity)
    {
        const CPtr<UInt8> Chunk = Body ? mDecoder.Reserve(Quantity) : CPtr<UInt8>(reinterpret_cast<Ptr<UInt8>>(& mHeader), Quantity);

        if (Chunk.empty())
        {
            Log::Error("Client: \"Can't reserve more data in the read buffer\"");

            DoClose();
        }
        else
        {
            const auto OnCompletion = [Self = shared_from_this(), Chunk, Body](asio::error_code Error, UInt Transferred)
            {
                Self->WhenRead(Error, Chunk, Body);
            };
            asio::async_read(mSocket, asio::mutable_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::DoFlush()
    {
        if (const CPtr<UInt8> Chunk = mEncoder.Read(); !Chunk.empty())
        {
            const auto OnCompletion = [Self = shared_from_this(), Chunk](asio::error_code Error, UInt Transferred)
            {
                // @TODO: Check if Transferred is the same as chunk
                Self->WhenWrite(Error, Chunk);
            };
            asio::async_write(mSocket, asio::const_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
        else
        {
            mFlushable.store(true, std::memory_order_release);

            if (mState == State::Closing)
            {
                DoClose();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::WhenResolve(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
    {
        if (Error)
        {
            WhenError(Error);
        }
        else
        {
            const asio::ip::tcp::endpoint Endpoint = (* Result);

            const auto OnCompletion = [Self = shared_from_this(), Result = ++Result](const auto Error)
            {
                Self->WhenConnect(std::forward<decltype(Error)>(Error), Result);
            };
            mSocket.async_connect(Endpoint, OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::WhenConnect(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
    {
        if (Error)
        {
            mSocket.close();

            // try to connect to the next endpoint in the list (if available),
            // otherwise notify about it.
            if (Result != asio::ip::tcp::resolver::iterator())
            {
                WhenResolve(std::error_code(), Result);
            }
            else
            {
                WhenError(Error);
            }
        }
        else
        {
            Start();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::WhenError(Ref<const std::error_code> Error)
    {
        if (Error)
        {
            Log::Error("Client: \"{} - {}\"", Error.value(), Error.message());
        }
        DoClose();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::WhenRead(Ref<const std::error_code> Error, CPtr<UInt8> Block, Bool Body)
    {
        if (mState != State::Connected)
        {
            return;
        }

        if (Error)
        {
            WhenError(Error);
        }
        else
        {
            if (Body)
            {
                mDecoder.Commit(Block.size());

                const auto OnCallback = [Self = shared_from_this(), Block]()
                {
                    if (! Block.empty())
                    {
                        if (ConstSPtr<Protocol> Protocol = Self->GetProtocol())
                        {
                            Protocol->OnRead(Self, Block);
                        }
                        Self->mDecoder.Consume(Block);
                    }
                };
                asio::post(mExecutor, OnCallback);
            }

            DoRead(!Body, Body ? sizeof(Header) : * reinterpret_cast<Ptr<Header>>(Block.data()));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void TCPClient::WhenWrite(Ref<const std::error_code> Error, CPtr<UInt8> Block)
    {
        if (mState == State::Closed)
        {
            return;
        }

        if (Error)
        {
            WhenError(Error);
        }
        else
        {
            mEncoder.Consume(Block);

            DoFlush();
        }
    }
}