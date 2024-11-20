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
        void SetProtocol(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        ConstSPtr<Protocol> GetProtocol() const
        {
            return mProtocol;
        }

        // -=(Undocumented)=-
        virtual SStr GetAddress() const = 0;

        // -=(Undocumented)=-
        virtual void Close(Bool Forcibly) = 0;

        // -=(Undocumented)=-
        virtual void Write(CPtr<const UInt8> Bytes) = 0;

        // -=(Undocumented)=-
        template<typename Packet>
        void Write(Any<Packet> Message)
        {
            // Write message into a buffer
            mAccumulator.Clear();
            mAccumulator.WriteInt(Message.GetID());
            mAccumulator.WriteObject(Message);

            // Write buffer into connection
            Write(CPtr<const UInt8>(mAccumulator.GetData()));
        }

        // -=(Undocumented)=-
        virtual void Flush() = 0;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt           mAttachment;
        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}