// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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

    Encoder::Encoder(ConstSPtr<Service> Graphics)
        : mGraphics { Graphics }
    {
        // TODO: Configurable?
        CreateTransientBuffer(mInFlyBuffers[0], Usage::Vertices, 64 * 1024 * 1024); // 64mb
        CreateTransientBuffer(mInFlyBuffers[1], Usage::Indices,   4 * 1024 * 1024); // 4mb
        CreateTransientBuffer(mInFlyBuffers[2], Usage::Uniform,  16 * 1024 * 1024); // 16mb
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

    void Encoder::Draw(UInt VertexStrideBytes, UInt IndexStrideBytes)
    {
        // Apply draw parameter(s)
        mInFlyBatch.Primitive.Count  = (mInFlyBuffers[1].Writer - mInFlyBuffers[1].Marker) / IndexStrideBytes;
        mInFlyBatch.Primitive.Base   = (mInFlyBuffers[1].Marker - mInFlyBuffers[1].Reader) / IndexStrideBytes;
        mInFlyBatch.Primitive.Offset = (mInFlyBuffers[0].Marker - mInFlyBuffers[0].Reader) / VertexStrideBytes;

        // Apply batch's vertices range
        SetVertices(
            mInFlyBuffers[0].ID,
            mInFlyBuffers[0].Reader,
            mInFlyBuffers[0].Writer - mInFlyBuffers[0].Marker,
            VertexStrideBytes);
        mInFlyBuffers[0].Marker = mInFlyBuffers[0].Writer;

        // Apply batch's indices range
        SetIndices(
            mInFlyBuffers[1].ID,
            mInFlyBuffers[1].Reader,
            mInFlyBuffers[1].Writer - mInFlyBuffers[1].Marker,
            IndexStrideBytes);
        mInFlyBuffers[1].Marker = mInFlyBuffers[1].Writer;

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