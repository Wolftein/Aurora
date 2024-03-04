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
#include "Statistics.hpp"

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
        void Poll()
        {
            OnPoll();
        }

        // -=(Undocumented)=-
        void Flush()
        {
            OnFlush();
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
        Ref<const Statistics> GetStatistics() const
        {
            return mStatistics;
        }

        // -=(Undocumented)=-
        void SetProtocol(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        void Close(Bool Forcibly)
        {
            OnClose(Forcibly);
        }

        // -=(Undocumented)=-
        void Write(CPtr<const UInt08> Bytes, UInt32 Channel, Bool Reliable)
        {
            OnWrite(Bytes, Channel, Reliable);
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Write(Message && Packet, UInt32 Channel, Bool Reliable) // TODO: Implement memory pool for packets
        {
            // write message's id
            mAccumulator.WriteInt(Packet.GetID());

            // write message's body
            Packet.Encode(mAccumulator);

            // push intermediate buffer to channel
            OnWrite(mAccumulator.GetData(), Channel, Reliable);

            // clear
            mAccumulator.Clear();
        }

    private:

        // -=(Undocumented)=-
        virtual void OnPoll() = 0;

        // -=(Undocumented)=-
        virtual void OnFlush() = 0;

        // -=(Undocumented)=-
        virtual void OnClose(Bool Forcibly) = 0;

        // -=(Undocumented)=-
        virtual void OnWrite(CPtr<const UInt08> Bytes, UInt32 Channel, Bool Reliable) = 0;

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt           mAttachment;
        Statistics     mStatistics;
        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}