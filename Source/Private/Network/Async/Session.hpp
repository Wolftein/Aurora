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

#include "Core/Memory/Buffer.hpp"
#include "Network/Session.hpp"
#include <asio.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class AsyncSession : public EnableSmartPointer<AsyncSession>, public virtual Session
    {
    public:

        // -=(Undocumented)=-
        using Socket = asio::ip::tcp::socket;

        // -=(Undocumented)=-
        using Header = UInt16;

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
        explicit AsyncSession(Socket && Channel);

        // -=(Undocumented)=-
        void Start();

        // -=(Undocumented)=-
        void Connect(CStr Address, CStr Service);

    protected:

        // \see Session::OnClose
        void OnClose(Bool Immediately) override;

        // \see Session::OnFlush
        void OnFlush(Ref<Writer> Accumulator) override;

    private:

        // -=(Undocumented)=-
        void DoClose();

        // -=(Undocumented)=-
        void DoRead(Bool Body, UInt32 Quantity);

        // -=(Undocumented)=-
        void DoFlush();

        // -=(Undocumented)=-
        void WhenResolve(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result);

        // -=(Undocumented)=-
        void WhenConnect(Ref<const std::error_code> Error, asio::ip::tcp::resolver::iterator Result);

        // -=(Undocumented)=-
        void WhenError(Ref<const std::error_code> Error);

        // -=(Undocumented)=-
        void WhenRead(Ref<const std::error_code> Error, UInt32 Transferred, Bool Body);

        // -=(Undocumented)=-
        void WhenWrite(Ref<const std::error_code> Error, UInt32 Transferred);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        State          mState;
        Socket         mChannel;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Buffer         mEncoder;
        Buffer         mDecoder;
    };
}