// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Network/Client.hpp"
#include "Buffer.hpp"
#include <asio.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class TCPClient final : public EnableSmartPointer<TCPClient>, public Client
    {
    public:

        // -=(Undocumented)=-
        using Reactor  = asio::io_context;

        // -=(Undocumented)=-
        using Socket   = asio::ip::tcp::socket;

        // -=(Undocumented)=-
        using Header   = UInt16;

        // -=(Undocumented)=-
        enum class State
        {
            Connecting,
            Connected,
            Closing,
            Closed,
        };

    public:

        // -=(Undocumented)=-
        explicit TCPClient(Ref<Reactor> Executor, Any<Socket> Channel);

        // -=(Undocumented)=-
        ~TCPClient();

        // -=(Undocumented)=-
        void Connect(CStr Address, UInt16 Port);

        // -=(Undocumented)=-
        void Start();

        // -=(Undocumented)=-
        SStr GetAddress() const override;

        // \see Client::Close
        void Close(Bool Forcibly) override;

        // \see Client::Write
        void Write(CPtr<const UInt8> Bytes) override;

        // \see Client::Flush
        void Flush() override;

    private:

        // -=(Undocumented)=-
        void DoClose();

        // -=(Undocumented)=-
        void DoRead(Bool Body, UInt32 Quantity);

        // -=(Undocumented)=-
        void DoFlush();

        // -=(Undocumented)=-
        void WhenResolve(ConstRef<std::error_code> Error, Ref<asio::ip::tcp::resolver::results_type> Result);

        // -=(Undocumented)=-
        void WhenConnect(ConstRef<std::error_code> Error);

        // -=(Undocumented)=-
        void WhenError(ConstRef<std::error_code> Error);

        // -=(Undocumented)=-
        void WhenRead(ConstRef<std::error_code> Error, CPtr<UInt8> Block, Bool Body);

        // -=(Undocumented)=-
        void WhenWrite(ConstRef<std::error_code> Error, CPtr<UInt8> Block);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<Reactor>  mExecutor;
        Socket        mSocket;
        Buffer        mEncoder;
        Buffer        mDecoder;
        Header        mHeader;
        Atomic<State> mState;
        Atomic<Bool>  mFlushable;
    };
}