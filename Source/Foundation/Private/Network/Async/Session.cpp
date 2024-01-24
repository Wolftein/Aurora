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

#include "Session.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    AsyncSession::AsyncSession(Socket && Channel)
        : mState   { State::Closed },
          mChannel { Move(Channel) },
          mEncoder { 65'536 },
          mDecoder { 65'536 }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::Start()
    {
        mState = State::Connected;

        mChannel.set_option(asio::ip::tcp::no_delay(true));

        DoRead(false, sizeof(Header));

        OnAttach();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::Connect(CStr Address, CStr Service)
    {
        if (mState != State::Closed)
        {
            return;
        }

        mState = State::Connecting;

        const SPtr<asio::ip::tcp::resolver> Resolver
            = NewPtr<asio::ip::tcp::resolver>(mChannel.get_executor());

        const auto OnCompletion = [Self = shared_from_this(), Resolver](const auto Error, auto Result) {
            CastPtr<AsyncSession>(Self)->WhenResolve(Error, Result);
        };
        Resolver->async_resolve(Address.data(), Service.data(), OnCompletion);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::OnClose(Bool Immediately)
    {
        if (Immediately || mEncoder.IsEmpty())
        {
            DoClose();
        }
        else
        {
            DoFlush();

            mState = State::Closing;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::OnFlush(Ref<Writer> Accumulator)
    {
        if (mState == State::Connected)
        {
            const CPtr<UInt08> Chunk = mEncoder.Reserve(mAccumulator.GetOffset() + sizeof(Header));

            if (Chunk.empty())
            {
                DoClose();
            }
            else
            {
                const CPtr<const UInt08> Block = mAccumulator.GetData();

                * reinterpret_cast<Ptr<Header>>(Chunk.data()) = static_cast<Header>(Block.size());

                std::copy(Block.begin(), Block.begin() + Block.size(), Chunk.data() + sizeof(Header));

                OnWrite(Chunk.subspan(sizeof(Header), mAccumulator.GetOffset()));

                mEncoder.Commit(Chunk.size());

                DoFlush();
            }

            mAccumulator.Clear();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::DoClose()
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
        mAccumulator.Clear();
        mEncoder.Reset();
        mDecoder.Reset();

        OnDetach();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::DoRead(Bool Body, UInt32 Quantity)
    {
        const CPtr<UInt08> Chunk = mDecoder.Reserve(Quantity);

        if (Chunk.empty())
        {
            DoClose();
        }
        else
        {
            const auto OnCompletion = [Self = shared_from_this(), Body](const auto Error, auto Transferred) {
                CastPtr<AsyncSession>(Self)->WhenRead(Error, Transferred, Body);
            };
            asio::async_read(mChannel, asio::mutable_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::DoFlush()
    {
        if (const CPtr<UInt08> Chunk = mEncoder.Read(); !Chunk.empty())
        {
            const auto OnCompletion = [Self = shared_from_this()](const auto Error, auto Transferred) {
                CastPtr<AsyncSession>(Self)->WhenWrite(Error, Transferred);
            };
            asio::async_write(mChannel, asio::const_buffer(Chunk.data(), Chunk.size()), OnCompletion);
        }
        else
        {
            if (mState == State::Closing)
            {
                DoClose();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::WhenResolve(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
    {
        if (Error)
        {
            WhenError(Error);
        }
        else
        {
            const asio::ip::tcp::endpoint Endpoint = (* Result);

            const auto OnCompletion = [Self = shared_from_this(), Result = ++Result](const auto Error) {
                CastPtr<AsyncSession>(Self)->WhenConnect(eastl::forward<decltype(Error)>(Error), Result);
            };
            mChannel.async_connect(Endpoint, OnCompletion);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::WhenConnect(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result)
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

    void AsyncSession::WhenError(Ref<const std::error_code> Error)
    {
        if (Error)
        {
            OnError(Error.value(), Error.message());
        }
        DoClose();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::WhenRead(Ref<const std::error_code> Error, UInt32 Transferred, Bool Body)
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

            if (const CPtr<UInt08> Chunk = mDecoder.Read(); !Chunk.empty())
            {
                if (Body)
                {
                    OnRead(Chunk);
                }

                mDecoder.Consume(Chunk.size());

                DoRead(!Body, !Body ? * reinterpret_cast<Ptr<Header>>(Chunk.data()) : sizeof(Header));
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsyncSession::WhenWrite(Ref<const std::error_code> Error, UInt32 Transferred)
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
            mEncoder.Consume(Transferred);

            DoFlush();
        }
    }
}