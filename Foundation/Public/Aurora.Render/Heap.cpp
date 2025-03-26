// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Heap.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Heap::Heap(ConstSPtr<Service> Graphics)
        : mGraphics { Graphics }
    {
        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Vertex)], Usage::Vertex, k_DefaultVertexCapacity);
        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Index)], Usage::Index, k_DefaultIndexCapacity);
        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Uniform)], Usage::Uniform, k_DefaultUniformCapacity);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Heap::~Heap()
    {
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Vertex)]);
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Index)]);
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Uniform)]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::Reallocate(Ref<Binding> Description)
    {
        CPtr<const UInt8> Memory(
                AddressOf(mBuffers[CastEnum(Usage::Uniform)].Memory[Description.Offset]), Description.Stride);
        Description = Allocate(Usage::Uniform, Memory);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::Commit(Bool Copy)
    {
        UpdateInFlightBuffer(mBuffers[CastEnum(Usage::Vertex)], Copy);
        UpdateInFlightBuffer(mBuffers[CastEnum(Usage::Index)], Copy);
        UpdateInFlightBuffer(mBuffers[CastEnum(Usage::Uniform)], Copy);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::CreateInFlightBuffer(Ref<InFlightBuffer> Buffer, Usage Type, UInt32 Length)
    {
        Buffer.Handle = mGraphics->CreateBuffer(Type, Length);
        Buffer.Memory = NewUniquePtr<UInt8[]>(Length);
        Buffer.Length = Length;
        Buffer.Reader = 0;
        Buffer.Writer = 0;
        Buffer.Marker = 0;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::UpdateInFlightBuffer(Ref<InFlightBuffer> Buffer, Bool Copy)
    {
        const UInt32 Length = (Buffer.Marker ? Buffer.Marker - Buffer.Reader : Buffer.Writer - Buffer.Reader);
        if (Length > 0)
        {
            Data Memory;

            if (Copy)
            {
                Memory = Data(Length);
                Memory.Copy(AddressOf(Buffer.Memory[Buffer.Reader]), Length);
            }
            else
            {
                Memory = Data(AddressOf(Buffer.Memory[Buffer.Reader]), Length, Data::EMPTY_DELETER);
            }

            mGraphics->UpdateBuffer(Buffer.Handle, Copy, Buffer.Reader, Move(Memory));
        }

        if (Buffer.Marker)
        {
            Buffer.Marker = 0;
            Buffer.Reader = 0;
            UpdateInFlightBuffer(Buffer, Copy);
        }
        else
        {
            Buffer.Reader = Buffer.Writer;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::DeleteInFlightBuffer(Ref<InFlightBuffer> Buffer)
    {
        mGraphics->DeleteBuffer(Buffer.Handle);
    }
}