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

    void Heap::Initialize(ConstSPtr<Service> Service)
    {
        mService = Service;

        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Vertex)], Usage::Vertex, k_DefaultVertexCapacity);
        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Index)], Usage::Index, k_DefaultIndexCapacity);
        CreateInFlightBuffer(mBuffers[CastEnum(Usage::Uniform)], Usage::Uniform, k_DefaultUniformCapacity);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::Dispose()
    {
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Vertex)]);
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Index)]);
        DeleteInFlightBuffer(mBuffers[CastEnum(Usage::Uniform)]);
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

    void Heap::Reallocate(Ref<Binding> Description)
    {
        CPtr<const UInt8> Memory(
                AddressOf(mBuffers[CastEnum(Usage::Uniform)].Memory[Description.Offset]), Description.Stride);
        Description = Allocate(Usage::Uniform, Memory);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::CreateInFlightBuffer(Ref<InFlightBuffer> Buffer, Usage Type, UInt32 Length)
    {
        Buffer.Handle = mService->CreateBuffer(Type, Length);
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
        if (Buffer.Marker > 0 && Buffer.Reader == Buffer.Marker)
        {
            Buffer.Reader = 0;
            Buffer.Marker = 0;
        }

        const UInt32 Count = (Buffer.Marker > 0 ? Buffer.Marker - Buffer.Reader : Buffer.Writer - Buffer.Reader);

        if (Count > 0)
        {
            Data Memory;

            if (Copy)
            {
                Memory = Data(Count);
                Memory.Copy(AddressOf(Buffer.Memory[Buffer.Reader]), Count);
            }
            else
            {
                Memory = Data(AddressOf(Buffer.Memory[Buffer.Reader]), Count, Data::EMPTY_DELETER);
            }

            mService->UpdateBuffer(Buffer.Handle, (Buffer.Reader == 0), Buffer.Reader, Move(Memory));

            Buffer.Reader = Buffer.Writer;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Heap::DeleteInFlightBuffer(Ref<InFlightBuffer> Buffer)
    {
        if (Buffer.Handle)
        {
            mService->DeleteBuffer(Buffer.Handle);
        }
    }
}