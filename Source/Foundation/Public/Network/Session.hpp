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

#include "Packet.hpp"
#include "Protocol.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class Session : public EnableSmartPointer<Session>
    {
    public:

        // -=(Undocumented)=-
        void Attach(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Write(Message && Packet)
        {
            // write message's id
            mAccumulator.WriteInt(Packet.GetID());

            // write message's body
            Packet.Encode(mAccumulator);
        }

        // -=(Undocumented)=-
        void Close(Bool Forcibly)
        {
            OnClose(Forcibly);
        }

        // -=(Undocumented)=-
        void Flush()
        {
            if (mAccumulator.HasData())
            {
                OnFlush(mAccumulator);
            }
        }

    protected:

        // -=(Undocumented)=-
        void OnAttach()
        {
            if (mProtocol)
            {
                mProtocol->OnAttach(shared_from_this());
            }
        }

        // -=(Undocumented)=-
        void OnDetach()
        {
            if (mProtocol)
            {
                mProtocol->OnDetach(shared_from_this());
            }
        }

        // -=(Undocumented)=-
        void OnError(UInt Error, CStr Description)
        {
            if (mProtocol)
            {
                mProtocol->OnError(shared_from_this(), Error, Description);
            }
        }

        // -=(Undocumented)=-
        void OnRead(CPtr<UInt08> Bytes)
        {
            if (mProtocol)
            {
                mProtocol->OnRead(shared_from_this(), Bytes);
            }
        }

        // -=(Undocumented)=-
        void OnWrite(CPtr<UInt08> Bytes)
        {
            if (mProtocol)
            {
                mProtocol->OnWrite(shared_from_this(), Bytes);
            }
        }

    protected:

        // -=(Undocumented)=-
        virtual void OnClose(Bool Immediately) = 0;

        // -=(Undocumented)=-
        virtual void OnFlush(Ref<Writer> Accumulator) = 0;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}