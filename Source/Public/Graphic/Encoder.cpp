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

#include "Encoder.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Encoder::Encoder(ConstSPtr<Graphic::Service> Graphics, UInt VertexCapacity, UInt IndexCapacity, UInt UniformCapacity)
        : mGraphics { Graphics }
    {
        CreateTransientBuffer(mInFlyBuffers[0], Usage::Vertex, VertexCapacity);
        CreateTransientBuffer(mInFlyBuffers[1], Usage::Index, IndexCapacity);
        CreateTransientBuffer(mInFlyBuffers[2], Usage::Uniform, UniformCapacity);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Encoder::~Encoder()
    {
        DeleteTransientBuffer(mInFlyBuffers[0]);
        DeleteTransientBuffer(mInFlyBuffers[1]);
        DeleteTransientBuffer(mInFlyBuffers[2]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Encoder::Ensure(UInt VerticesInBytes, UInt IndicesInBytes, UInt AmountOfUniforms)
    {
        const Bool FitsVertices
            = mInFlyBuffers[0].Writer + VerticesInBytes <= mInFlyBuffers[0].Capacity;
        const Bool FitsIndices
            = mInFlyBuffers[1].Writer + IndicesInBytes  <= mInFlyBuffers[1].Capacity;
        const Bool FitsUniforms
            = mInFlyBuffers[2].Writer + Align<256>(AmountOfUniforms * sizeof(Vector4f)) <= mInFlyBuffers[2].Capacity;

        if (! FitsVertices || ! FitsIndices || ! FitsUniforms)
        {
            Flush();

            if (! FitsVertices)
            {
                mInFlyBuffers[0].Reader = mInFlyBuffers[0].Writer = mInFlyBuffers[0].Marker = 0;
            }

            if (! FitsIndices)
            {
                mInFlyBuffers[1].Reader = mInFlyBuffers[1].Writer = mInFlyBuffers[1].Marker = 0;
            }

            if (! FitsUniforms)
            {
                mInFlyBuffers[2].Reader = mInFlyBuffers[2].Writer = mInFlyBuffers[2].Marker = 0;

                ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerScene]);
                ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerTechnique]);
            }

            return false;
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::Draw(UInt VerticesFormatStride, UInt IndicesFormatStride)
    {
        // Apply draw parameter(s)
        if (IndicesFormatStride > 0)
        {
            mInFlyBatch.Primitive.Count  = (mInFlyBuffers[1].Writer - mInFlyBuffers[1].Marker) / IndicesFormatStride;
            mInFlyBatch.Primitive.Base   = (mInFlyBuffers[1].Marker - mInFlyBuffers[1].Reader) / IndicesFormatStride;
            mInFlyBatch.Primitive.Offset = (mInFlyBuffers[0].Marker - mInFlyBuffers[0].Reader) / VerticesFormatStride;
        }
        else
        {
            mInFlyBatch.Primitive.Count  = (mInFlyBuffers[0].Writer - mInFlyBuffers[0].Marker) / VerticesFormatStride;
            mInFlyBatch.Primitive.Base   = (mInFlyBuffers[0].Marker - mInFlyBuffers[0].Reader) / VerticesFormatStride;
        }

        // Apply batch's vertices range
        SetVertices(
            mInFlyBuffers[0].ID,
            mInFlyBuffers[0].Reader,
            mInFlyBuffers[0].Writer - mInFlyBuffers[0].Marker,
            VerticesFormatStride);
        mInFlyBuffers[0].Marker = mInFlyBuffers[0].Writer;

        // Apply batch's indices range
        if (IndicesFormatStride > 0)
        {
            SetIndices(
                mInFlyBuffers[1].ID,
                mInFlyBuffers[1].Reader,
                mInFlyBuffers[1].Writer - mInFlyBuffers[1].Marker,
                IndicesFormatStride);
            mInFlyBuffers[1].Marker = mInFlyBuffers[1].Writer;
        }

        // Apply batch's uniforms range
        for (UInt Block = 0; Block < Graphic::k_MaxUniforms; ++Block)
        {
            if (mInFlyRanges[Block].Length > 0)
            {
                SetUniforms(
                    Block,
                    mInFlyBuffers[2].ID,
                    mInFlyRanges[Block].Offset,
                    mInFlyRanges[Block].Length);
            }
        }

        // Add the batch into submission
        mInFlySubmission.emplace_back(mInFlyBatch);
        mInFlyBatch = Submission();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::Flush()
    {
        // Flush all pending transient data to the GPU buffer
        for (Ref<TransientBuffer> Buffer : mInFlyBuffers)
        {
            UpdateTransientBuffer(Buffer);
        }

        // Submit all pending work to the GPU
        mGraphics->Submit(mInFlySubmission);

        // Restart all submission(s)
        mInFlySubmission.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<void> Encoder::AllocateTransientBuffer(Ref<TransientBuffer> Buffer, UInt Size)
    {
        const UInt Offset = (Buffer.Writer + Size > Buffer.Capacity ? 0 : Buffer.Writer);

        if (Offset == 0 && Buffer.Writer > 0)
        {
            static_assert(true, "Not enough space in the transient buffer.");
        }

        Buffer.Writer = Offset + Size;
        return & Buffer.Memory[Offset];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<void> Encoder::AllocateTransientUniforms(Ref<TransientRange> Range, UInt Size)
    {
        const UInt      Length = Align<256>(Size);

        // We need to check if there is space for the uniforms, so that we reset the scene and technique parameter(s)
        if (mInFlyBuffers[2].Writer + Length > mInFlyBuffers[2].Capacity)
        {
            Flush();

            mInFlyBuffers[2].Reader = mInFlyBuffers[2].Writer = mInFlyBuffers[2].Marker = 0;

            ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerScene]);
            ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerTechnique]);
        }

        const Ptr<void> Memory = AllocateTransientBuffer(mInFlyBuffers[2], Length);

        Range.Offset = (mInFlyBuffers[2].Writer - Length) / sizeof(Vector4f);
        Range.Length = Length / sizeof(Vector4f);

        return Memory;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::ReallocateTransientUniforms(Ref<TransientRange> Range)
    {
        const UInt Offset = Range.Offset * sizeof(Vector4f);
        const UInt Length = Range.Length * sizeof(Vector4f);

        if (Length > 0)
        {
            Ptr<void> DataPtr = AllocateTransientUniforms(Range, Length);
            FastCopyMemory(DataPtr, & mInFlyBuffers[2].Memory[Offset], Length);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::CreateTransientBuffer(Ref<TransientBuffer> Buffer, Usage Type, UInt Capacity)
    {
        Buffer.ID       = mGraphics->CreateBuffer<UInt08>(Type, Capacity);
        Buffer.Memory   = NewUniquePtr<UInt08[]>(Capacity);
        Buffer.Capacity = Capacity;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::UpdateTransientBuffer(Ref<TransientBuffer> Buffer)
    {
        const UInt Count = (Buffer.Writer - Buffer.Reader);

        if (Count > 0)
        {
            const Bool Discard = (Buffer.Reader == 0);

            mGraphics->UpdateBuffer(Buffer.ID, Discard, Buffer.Reader, CPtr<UInt08>(& Buffer.Memory[Buffer.Reader], Count));

            Buffer.Reader = Buffer.Writer;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Encoder::DeleteTransientBuffer(Ref<TransientBuffer> Buffer)
    {
        mGraphics->DeleteBuffer(Buffer.ID);
    }
}