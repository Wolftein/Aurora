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
        virtual ~Client() = default;

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
        template<typename Type>
        void Write(CPtr<const Type> Bytes)
        {
            mAccumulator.Write(Bytes.data(), Bytes.size_bytes());
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

        // -=(Undocumented)=-
        Ref<const Statistics> GetStatistics() const
        {
            return mStatistics;
        }

    protected:

        // -=(Undocumented)=-
        virtual void OnClose(Bool Immediately) = 0;

        // -=(Undocumented)=-
        virtual void OnFlush(Ref<Writer> Accumulator) = 0;

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Statistics     mStatistics;
        SPtr<Protocol> mProtocol;
        Writer         mAccumulator;
    };
}