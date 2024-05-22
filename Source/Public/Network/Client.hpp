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

#include "Channel.hpp"
#include "Packet.hpp"
#include "Protocol.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class Client
    {
    public:

        // -=(Undocumented)=-
        Client()
            : mAttachment { 0 }
        {
        }

        // -=(Undocumented)=-
        virtual ~Client() = default;

        // -=(Undocumented)=-
        void SetProtocol(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        void SetAttachment(UInt Attachment)
        {
            mAttachment = Attachment;
        }

        // -=(Undocumented)=-
        UInt GetAttachment() const
        {
            return mAttachment;
        }

        // -=(Undocumented)=-
        void Close(Bool Forcibly)
        {
            OnClose(Forcibly);
        }

        // -=(Undocumented)=-
        void Append(CPtr<const UInt08> Bytes)
        {
			mAccumulator.Write(Bytes.data(), Bytes.size());
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Append(Message && Packet)
        {
            // write message's id
            mAccumulator.WriteInt(Packet.GetID());

            // write message's body
            Packet.Encode(mAccumulator);
        }

        // -=(Undocumented)=-
        void Flush(Channel Mode)
        {
            if (mAccumulator.HasData())
            {
                OnWrite(mAccumulator.GetData(), Mode);

                mAccumulator.Clear();
            }
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Write(Message && Packet, Channel Mode = Channel::Reliable)
        {
            this->template Append<Message>(Packet);
            Flush(Mode);
        }

    private:

        // -=(Undocumented)=-
        virtual void OnClose(Bool Forcibly) = 0;

        // -=(Undocumented)=-
        virtual void OnWrite(CPtr<const UInt08> Bytes, Channel Mode) = 0;

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt           mAttachment;
        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}