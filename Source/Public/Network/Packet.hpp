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

#include "Core/Serialization/Binary/Serializable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    template<typename Derived, UInt ID>
    class Packet
    {
    public:

        // -=(Undocumented)=-
        constexpr static inline UInt kID = ID;

    public:

        // -=(Undocumented)=-
        Packet() = default;

        // -=(Undocumented)=-
        virtual ~Packet() = default;

        // -=(Undocumented)=-
        UInt GetID() const
        {
            return kID;
        }

        // -=(Undocumented)=-
        void Encode(Ref<Writer> Archive)
        {
            static_cast<Ptr<Derived>>(this)->OnSerialize(Stream<Writer>(Archive));
        }

        // -=(Undocumented)=-
        void Decode(Ref<Reader> Archive)
        {
            static_cast<Ptr<Derived>>(this)->OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        Writer Dump()
        {
            Writer Output;
            Write(Output, * this);

            return Output;
        }

    public:

        // -=(Undocumented)=-
        static void Write(Ref<Writer> Output, Ref<Packet<Derived, ID>> Packet)
        {
            // Write message's id
            Output.WriteInt(Packet.GetID());

            // Write message's body
            Packet.Encode(Output);
        }
    };
}