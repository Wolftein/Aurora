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

    AsioClient::AsioClient(Socket && Channel)
        : mState     { State::Closed },
          mChannel   { Move(Channel) },
          mEncoder   { 65'536 },
          mDecoder   { 65'536 },
          mFlushable { true }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    AsioClient::~AsioClient()
    {
        mProtocol = nullptr;
        Close(true);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::Start()
    {
        mState      = State::Connected;
        mStatistics = { };
        mStatistics.Address = mChannel.remote_endpoint().address().to_v4().to_string();

        mChannel.set_option(asio::ip::tcp::no_delay(true));

        DoRead(false, sizeof(Header));

        if (mProtocol)
        {
            mProtocol->OnAttach(shared_from_this());
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::Connect(CStr Address, UInt16 Port)
    {
        if (mState != State::Closed)
        {
            return;
        }

        mState = State::Connecting;

        const SPtr<asio::ip::tcp::resolver> Resolver
            = NewPtr<asio::ip::tcp::resolver>(mChannel.get_executor());

        const auto OnCompletion = [Self = shared_from_this(), Resolver](const auto Error, auto Result)
        {
            Self->WhenResolve(Error, Result);
        };
        Resolver->async_resolve(Address.data(), std::to_string(Port), OnCompletion);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::OnClose(Bool Immediately)
    {
        if (mState == State::Connected || mState == State::Connecting)
        {
            if (Immediately || (mFlushable && mEncoder.IsEmpty()))
            {
                DoClose();
            }
            else
            {
                Flush();

                mState = State::Closing;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::OnWrite(CPtr<const UInt08> Bytes)
    {
        if (mState == State::Connected)
        {
            const CPtr<UInt08> Chunk = mEncoder.Reserve(Bytes.size() + sizeof(Header));

            if (Chunk.empty())
            {
                mProtocol->OnError(shared_from_this(), ENOBUFS, "Can't reserve more data in the write buffer");

                DoClose();
            }
            else
            {
                * reinterpret_cast<Ptr<Header>>(Chunk.data()) = static_cast<Header>(Bytes.size());

                FastCopyMemory(Chunk.data() + sizeof(Header), Bytes.data(), Bytes.size());

                if (mProtocol)
                {
                    mProtocol->OnWrite(shared_from_this(), Chunk.subspan(sizeof(Header), Bytes.size()));
                }

                mEncoder.Commit(Chunk.size());

                mStatistics.TotalPacketSent   += 1;
                mStatistics.TotalBytesPending += Bytes.size() + sizeof(Header);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::OnFlush()
    {
        if (mState == State::Connected && mFlushable)
        {
            DoFlush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::DoClose()
    {
        if (mState == State::Closed)
        {
            return;
        }

        mState = State::Closed;

        // shutdown socket properly
        asio::error_code Error;
        mChannel.shutdown(asio::ip::tcp::socket::shutdown_both, Error);
        mChannel.close(Error);

        // reset all buffer(s)
        mEncoder.Reset();
        mDecoder.Reset();
        mFlushable = true;

        if (mProtocol)
        {
            mProtocol->OnDetach(shared_from_this());
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::DoRead(Bool Body, UInt32 Quantity)
    {
        const CPtr<UInt08> Chunk = mDecoder.Reserve(Quantity);

        if (Chunk.empty())
        {
            mProtocol->OnError(shared_from_this(), ENOBUFS, "Can't reserve more data in the read buffer");

            DoClose();
        }
        else
        {
            const auto OnCompletion = [Self = shared_from_this(), Body](const auto Error, auto Transferred)
            {
                Self->WhenRead(Error, Transferred, Body);
            };
            asio::async_read(mChannel, asio::mutable_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::DoFlush()
    {
        if (const CPtr<UInt08> Chunk = mEncoder.Read(); !Chunk.empty())
        {
            mFlushable = false;

            const auto OnCompletion = [Self = shared_from_this()](const auto Error, auto Transferred)
            {
                Self->WhenWrite(Error, Transferred);
            };
            asio::async_write(mChannel, asio::const_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
        else
        {
            mFlushable = true;

            if (mState == State::Closing)
            {
                DoClose();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::WhenResolve(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
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
            mChannel.async_connect(Endpoint, OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::WhenConnect(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
    {
        if (Error)
        {
            mChannel.close();

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

    void AsioClient::WhenError(Ref<const std::error_code> Error)
    {
        if (Error)
        {
            if (mProtocol)
            {
                mProtocol->OnError(shared_from_this(), Error.value(), Error.message());
            }
        }
        DoClose();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::WhenRead(Ref<const std::error_code> Error, UInt32 Transferred, Bool Body)
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
            mDecoder.Commit(Transferred);

            mStatistics.TotalBytesReceived += Transferred;

            if (const CPtr<UInt08> Chunk = mDecoder.Read(); !Chunk.empty())
            {
                if (Body)
                {
                    mStatistics.TotalPacketReceived += 1;

                    if (mProtocol)
                    {
                        mProtocol->OnRead(shared_from_this(), Chunk);
                    }
                }

                mDecoder.Consume(Chunk.size());

                DoRead(!Body, !Body ? * reinterpret_cast<Ptr<Header>>(Chunk.data()) : sizeof(Header));
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioClient::WhenWrite(Ref<const std::error_code> Error, UInt32 Transferred)
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
            mStatistics.TotalBytesSent    += Transferred;
            mStatistics.TotalBytesPending -= Transferred;

            mEncoder.Consume(Transferred);

            DoFlush();
        }
    }
}