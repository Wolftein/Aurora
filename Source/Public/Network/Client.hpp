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
#include "Statistics.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class Client : public Channel
    {
    public:

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
        Ref<const Statistics> GetStatistics() const
        {
            return mStatistics;
        }

        // -=(Undocumented)=-
        void Close(Bool Forcibly)
        {
            Flush();
            OnClose(Forcibly);
        }

        // -=(Undocumented)=-
        void Write(CPtr<const UInt08> Bytes)
        {
            OnWrite(Bytes);
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
        void Flush() override
        {
            if (mAccumulator.HasData())
            {
                OnWrite(mAccumulator.GetData());

                mAccumulator.Clear();
            }

            OnFlush();
        }

    private:

        // -=(Undocumented)=-
        virtual void OnClose(Bool Forcibly) = 0;

        // -=(Undocumented)=-
        virtual void OnWrite(CPtr<const UInt08> Bytes) = 0;

        // -=(Undocumented)=-
        virtual void OnFlush() = 0;

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt           mAttachment;
        Statistics     mStatistics;
        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}