// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Heap final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultVertexCapacity  = 8 * 1024 * 1024;

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultIndexCapacity   = 4 * 1024 * 1024;

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultUniformCapacity = 2 * 1024 * 1024;

    public:

        // -=(Undocumented)=-
        void Create(Ref<class Service> Service);

        // -=(Undocumented)=-
        void Commit(Ref<class Service> Service);

        // -=(Undocumented)=-
        template<typename Format>
        auto Allocate(Usage Type, UInt32 Length, UInt32 Stride = sizeof(Format))
        {
            if (Type == Usage::Uniform)
            {
                Stride = k_Alignment;
                Length = Align(Length, k_Alignment) / k_Alignment;
            }
            return AllocateInFlightBuffer<Format>(mBuffers[CastEnum(Type)], Length, Stride);
        }

    private:

        // -=(Undocumented)=-
        struct InFlightBuffer
        {
            // -=(Undocumented)=-
            Object Handle;

            // -=(Undocumented)=-
            Usage  Type;

            // -=(Undocumented)=-
            UInt32 Length;

            // -=(Undocumented)=-
            Writer Memory;
        };

        // -=(Undocumented)=-
        void CreateInFlightBuffer(Ref<class Service> Service, Ref<InFlightBuffer> Buffer, Usage Type, UInt32 Length);

        // -=(Undocumented)=-
        void UpdateInFlightBuffer(Ref<class Service> Service, Ref<InFlightBuffer> Buffer);

        // -=(Undocumented)=-
        template<typename Format>
        auto AllocateInFlightBuffer(Ref<InFlightBuffer> Buffer, UInt32 Length, UInt32 Stride)
        {
            Ref<Writer> Memory = Buffer.Memory;
            UInt32 Offset      = Align(Memory.GetOffset(), Stride);

            if (const UInt32 Skip = Offset - Memory.GetOffset(); Skip > 0)
            {
                Memory.Reserve<UInt8>(Skip);
            }
            return std::make_tuple(Memory.Reserve<Format>(Length * Stride), Binding(Buffer.Handle, Stride, Offset));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<InFlightBuffer, CountEnum<Usage>()> mBuffers;
    };
}