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

#include "Driver.hpp"
#include "Encoder.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Frame final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultVertices = 4 * 1024 * 1024;

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultIndices  = 2 * 1024 * 1024;

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultUniforms = 1 * 1024 * 1024;

        // -=(Undocumented)=-
        template<typename Format>
        struct Allocation
        {
            // -=(Undocumented)=-
            Ptr<Format> Pointer;

            // -=(Undocumented)=-
            Binding     Binding;
        };

    public:

        // -=(Undocumented)=-
        void Initialize(Object Vertices, Object Indices, Object Uniforms);

        // -=(Undocumented)=-
        Ref<Encoder> GetEncoder()
        {
            return mEncoder;
        }

        // -=(Undocumented)=-
        template<typename Format>
        Allocation<Format> Allocate(Usage Type, UInt32 Length, UInt32 Stride = sizeof(Format))
        {
            // This is essential for uniform buffer allocations to maintain alignment requirements.
            if (Type == Usage::Uniform)
            {
                Stride = k_Alignment;
                Length = Align(Length, k_Alignment) / k_Alignment;
            }

            Ref<TransientBuffer> Buffer = mHeap[CastEnum(Type)];

            Ref<Writer> Writer = Buffer.Writer;
            UInt32 Offset      = Align(Writer.GetOffset(), Stride);

            if (const UInt32 Skip = Offset - Writer.GetOffset(); Skip > 0)
            {
                Writer.Reserve<UInt8>(Skip);
            }
            return Allocation<Format>(Writer.Reserve<Format>(Length * Stride), Binding(Buffer.ID, Stride, Offset));
        }

        // -=(Undocumented)=-
        void OnPreSubmission(Ref<Driver> Driver);

        // -=(Undocumented)=-
        void OnPostSubmission(Ref<Driver> Driver);

    private:

        // -=(Undocumented)=-
        struct TransientBuffer
        {
            // -=(Undocumented)=-
            Object ID;

            // -=(Undocumented)=-
            UInt32 Length;

            // -=(Undocumented)=-
            Writer Writer;
        };

        // -=(Undocumented)=-
        using Heap = Array<TransientBuffer, CountEnum<Usage>()>;

        // -=(Undocumented)=-
        void CreateTransientBuffer(Ref<TransientBuffer> Buffer, Object ID, UInt32 Length);

        // -=(Undocumented)=-
        void UpdateTransientBuffer(Ref<Driver> Driver, Ref<TransientBuffer> Buffer, Usage Type);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Encoder mEncoder;
        Heap    mHeap;
    };
}