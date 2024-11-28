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

#include "Frame.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Frame::Initialize(Object Vertices, Object Indices, Object Uniforms)
    {
        CreateTransientBuffer(mHeap[CastEnum(Usage::Vertex)], Vertices, k_DefaultVertices);
        CreateTransientBuffer(mHeap[CastEnum(Usage::Index)], Indices, k_DefaultIndices);
        CreateTransientBuffer(mHeap[CastEnum(Usage::Uniform)], Uniforms, k_DefaultUniforms);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Frame::OnPreSubmission(Ref<Driver> Driver)
    {
        // Uploads all transient buffers associated with the current GPU frame
        // to ensure data is ready for rendering.
        UpdateTransientBuffer(Driver, mHeap[CastEnum(Usage::Vertex)], Usage::Vertex);
        UpdateTransientBuffer(Driver, mHeap[CastEnum(Usage::Index)], Usage::Index);
        UpdateTransientBuffer(Driver, mHeap[CastEnum(Usage::Uniform)], Usage::Uniform);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Frame::OnPostSubmission(Ref<Driver> Driver)
    {
        // Clear the contents of the encoder to reset its state.
        // This method ensures that any previously encoded data is discarded,
        // making the encoder ready for new data to be processed.
        mEncoder.Clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Frame::CreateTransientBuffer(Ref<TransientBuffer> Buffer, Object ID, UInt32 Length)
    {
        Buffer.ID     = ID;
        Buffer.Length = Length;
        Buffer.Writer.Ensure(Length);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Frame::UpdateTransientBuffer(Ref<Driver> Driver, Ref<TransientBuffer> Buffer, Usage Type)
    {
        Ref<Writer> Writer = Buffer.Writer;
        if (!Writer.HasData())
        {
            return;
        }

        // Ensures that the GPU buffer can accommodate all the data from the CPU buffer,
        // maintaining synchronization.
        if (Writer.GetCapacity() > Buffer.Length)
        {
            Driver.DeleteBuffer(Buffer.ID);
            Driver.CreateBuffer(Buffer.ID, Type, Writer.GetCapacity(), CPtr<const UInt8>());
        }

        // Update the GPU buffer to reflect the new data from the CPU buffer.
        Buffer.Length = Writer.GetCapacity();
        Driver.UpdateBuffer(Buffer.ID, false, 0, Writer.GetData());
        Writer.Clear();
    }
}