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

#include "Aurora.Graphic/Service.hpp"

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
        Heap(ConstSPtr<Service> Graphics);

        // -=(Undocumented)=-
        ~Heap();

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

        // -=(Undocumented)=-
        template<typename Format>
        auto Allocate(Usage Type, CPtr<const Format> Data)
        {
            const auto [Pointer, Description] = Allocate<Format>(Type, Data.size());
            std::memcpy(Pointer, Data.data(), Data.size_bytes());
            return Description;
        }

        // -=(Undocumented)=-
        template<typename Format>
        Bool Fits(Usage Type, UInt32 Length, UInt32 Stride = sizeof(Format)) const
        {
            if (Type == Usage::Uniform)
            {
                Stride = k_Alignment;
                Length = Align(Length, k_Alignment) / k_Alignment;
            }

            ConstRef<InFlightBuffer> Buffer = mBuffers[CastEnum(Type)];
            const UInt32 Size   = Length * Stride;
            const UInt32 Offset = Align(Buffer.Writer, Stride);

            if (Buffer.Marker != 0)
            {
                return Offset + Size <= Buffer.Reader;
            }
            else
            {
                return Offset + Size <= Buffer.Length || Size <= Buffer.Reader;
            }
        }

        // -=(Undocumented)=-
        void Reallocate(Ref<Binding> Description);  // TODO: Hacky, needs to thing another way!

        // -=(Undocumented)=-
        void Commit(Bool Copy);

    private:

        // -=(Undocumented)=-
        struct InFlightBuffer
        {
            // -=(Undocumented)=-
            Object        Handle;

            // -=(Undocumented)=-
            UPtr<UInt8[]> Memory;

            // -=(Undocumented)=-
            UInt32        Length;

            // -=(Undocumented)=-
            UInt32        Reader;

            // -=(Undocumented)=-
            UInt32        Writer;

            // -=(Undocumented)=-
            UInt32        Marker;
        };

        // -=(Undocumented)=-
        void CreateInFlightBuffer(Ref<InFlightBuffer> Buffer, Usage Type, UInt32 Length);

        // -=(Undocumented)=-
        void UpdateInFlightBuffer(Ref<InFlightBuffer> Buffer, Bool Copy);

        // -=(Undocumented)=-
        void DeleteInFlightBuffer(Ref<InFlightBuffer> Buffer);

        // -=(Undocumented)=-
        template<typename Format>
        auto AllocateInFlightBuffer(Ref<InFlightBuffer> Buffer, UInt32 Length, UInt32 Stride)
        {
            const UInt32 Size = Length * Stride;
            UInt32 Offset = Align(Buffer.Writer, Stride);

            if (Offset + Size > Buffer.Length)
            {
                Buffer.Marker = Buffer.Writer;
                Offset        = 0;
            }

            Buffer.Writer = Offset + Size;
            return std::make_tuple(
                    reinterpret_cast<Ptr<Format>>(AddressOf(Buffer.Memory[Offset])), Binding(Buffer.Handle, Stride, Offset));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Service>                             mGraphics;
        Array<InFlightBuffer, CountEnum<Usage>()> mBuffers;
    };
}