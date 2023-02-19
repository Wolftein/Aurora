// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2020 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Renderer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
        : mService { Context.GetSubsystem<Graphic::Service>() }
    {
        CreateResources();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::~Renderer()
    {
        DeleteResources();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Begin(Ref<const Camera> Camera, Real32 Time)
    {
        mSceneData.Camera = Camera.GetView() * Camera.GetProjection();
        mSceneData.Time   = Time;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(
        Ref<const Rectf> Destination,
        Ref<const Rectf> Source,
        Real32 Depth,
        Real32 Angle,
        UInt32 Color,
        Ref<const SPtr<Pipeline>> Pipeline,
        Ref<const SPtr<Material>> Material)
    {
        if (mDrawablesRef.full())
        {
            Flush();
        }

        Ref<Drawable> Drawable = mDrawablesRef.push_back();
        Drawable.ID          = Material->GetID(); // TODO (Unique ID for Opaque/Translucid and Depth)
        Drawable.Destination = Destination;
        Drawable.Source      = Source;
        Drawable.Depth       = Depth;
        Drawable.Angle       = Angle;
        Drawable.Color       = Color;
        Drawable.Pipeline    = Pipeline;
        Drawable.Material    = Material;

        mDrawablesPtr.push_back(& Drawable);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::End()
    {
        if (!mDrawablesRef.empty())
        {
            Flush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush()
    {
        // Sort all sprites back to front and by material/pipeline
        eastl::sort(mDrawablesPtr.begin(), mDrawablesPtr.end(), [](Ptr<const Drawable> Left, Ptr<const Drawable> Right)
        {
            return Left->ID > Right->ID;
        });

        // Writes all drawables into the vertex buffer
        UInt UniformSize = 0;
        UInt IndexBufferOffset = 0;
        UInt ArrayBufferOffset;
        UInt ArrayBufferBatch = 0;
        Ptr<VertexShaderGeometry> ArrayBlockPtr
            = mService->Map<VertexShaderGeometry>(mArrayBuffer, ArrayBufferOffset, mDrawablesPtr.size());

        IndexBufferOffset = ArrayBufferOffset;

        for (UInt Index = 0, Max = mDrawablesPtr.size() - 1; Index <= Max; ++Index)
        {
            const Ptr<const Drawable> ThisDrawable = mDrawablesPtr[Index];
            const Ptr<const Drawable> NextDrawable = (Index == Max ? ThisDrawable : mDrawablesPtr[Index + 1]);

            WriteGeometry(ThisDrawable, ArrayBlockPtr);
            ++ArrayBlockPtr;
            ++ArrayBufferBatch;

            const Bool HasMaterialChanged = (ThisDrawable->Material != NextDrawable->Material || ThisDrawable == NextDrawable);
            const Bool HasPipelineChanged = (ThisDrawable->Pipeline != NextDrawable->Pipeline);

            if (HasMaterialChanged || HasPipelineChanged)
            {
                if (HasMaterialChanged)
                {
                    UniformSize += ThisDrawable->Material->GetParameters().size();
                    mMaterialsPtr.push_back(ThisDrawable->Material.get());
                }

                Ref<Submission> Submission = mSubmissions.push_back();
                Submission.Vertices.Buffer = mArrayBuffer;
                Submission.Vertices.Stride = sizeof(VertexShaderLayout);

                Submission.Indices.Buffer = mIndexBuffer;
                Submission.Indices.Length = k_IndicesPerQuad * ArrayBufferBatch;
                Submission.Indices.Offset = IndexBufferOffset * k_IndicesPerQuad;
                Submission.Indices.Stride = sizeof(UInt16);

                Submission.Pipeline = ThisDrawable->Pipeline->GetID();

                for (SInt SourceIndex = 0; SourceIndex < k_MaxSources; ++SourceIndex)
                {
                    if (const SPtr<const Texture> Texture = ThisDrawable->Material->GetTexture(SourceIndex))
                    {
                        Submission.Textures[SourceIndex] = Texture->GetID();
                        Submission.Samplers[SourceIndex] = mSampler;
                    }
                }

                IndexBufferOffset += ArrayBufferBatch;
                ArrayBufferBatch  = 0;
            }
        }

        mService->Unmap(mArrayBuffer);

        {
            UInt UniformBufferSize = k_SceneSize + UniformSize;
            UInt UniformBufferOffset;
            UInt Index = 0;
            Ptr<Vector4f> UniformBlockPtr = mService->Map<Vector4f>(mSceneBuffer, UniformBufferOffset, UniformBufferSize);

            UInt BaseUniformOffset = UniformBufferOffset;
            UInt ElseUniformOffset = (BaseUniformOffset + k_SceneSize);

            * reinterpret_cast<Ptr<VertexShaderData>>(UniformBlockPtr) = mSceneData;
            UniformBlockPtr += k_SceneSize;


            for (Ptr<const Material> MaterialPtr : mMaterialsPtr)
            {
                const CPtr<const Vector4f> Parameters = MaterialPtr->GetParameters();

                mSubmissions[Index].Uniforms[0].Buffer  = mSceneBuffer;
                mSubmissions[Index].Uniforms[0].Offset  = BaseUniformOffset;
                mSubmissions[Index].Uniforms[0].Length  = k_SceneSize;

                if (! Parameters.empty())
                {
                    memcpy(UniformBlockPtr, Parameters.data(), Parameters.size_bytes());

                    mSubmissions[Index].Uniforms[1].Buffer  = mSceneBuffer;
                    mSubmissions[Index].Uniforms[1].Offset  = ElseUniformOffset;
                    mSubmissions[Index].Uniforms[1].Length  = Parameters.size();

                    UniformBlockPtr   += Parameters.size();
                    ElseUniformOffset += Parameters.size();
                }

                ++Index;
            }

            mService->Unmap(mSceneBuffer);
        }

        // Submit all command(s) to the gpu
        mService->Submit(mSubmissions);

        // Reset all stack(s) back to original states
        mSubmissions.reset_lose_memory();
        mDrawablesRef.reset_lose_memory();
        mDrawablesPtr.reset_lose_memory();
        mMaterialsPtr.reset_lose_memory();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::CreateResources()
    {
        // Create array buffer
        mArrayBuffer = mService->CreateBuffer<VertexShaderGeometry>(true, k_MaxDrawables);

        // Create index buffer
        UInt           IndexSize   = k_MaxDrawables * k_IndicesPerQuad;
        UPtr<UInt16[]> IndexMemory = NewUniquePtr<UInt16[]>(IndexSize);

        for (UInt Position = 0, Index = 0, Max = k_MaxDrawables * k_IndicesPerQuad; Position < Max; Index += 4)
        {
            IndexMemory[Position++] = Index;
            IndexMemory[Position++] = Index + 1;
            IndexMemory[Position++] = Index + 2;
            IndexMemory[Position++] = Index + 1;
            IndexMemory[Position++] = Index + 3;
            IndexMemory[Position++] = Index + 2;
        }

        mIndexBuffer = mService->CreateBuffer<UInt16>(true, IndexSize, {
            reinterpret_cast<Ptr<UInt16>>(IndexMemory.get()), IndexSize
        });

        // Create scene buffer
        mSceneBuffer = mService->CreateBuffer<Vector4f>(false, k_MaxUniforms);

        // Create samplers
        mSampler = mService->CreateSampler(TextureEdge::Clamp, TextureEdge::Clamp, TextureFilter::Trilinear);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DeleteResources()
    {
        mService->DeleteBuffer(mArrayBuffer);
        mService->DeleteBuffer(mIndexBuffer);
        mService->DeleteBuffer(mSceneBuffer);

        mService->DeleteSampler(mSampler);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::WriteGeometry(Ptr<const Drawable> Drawable, Ptr<VertexShaderGeometry> Buffer)
    {
        const Real32 DestinationX1 = Drawable->Destination.GetLeft();
        const Real32 DestinationX2 = Drawable->Destination.GetRight();
        const Real32 DestinationX3 = Drawable->Destination.GetLeft();
        const Real32 DestinationX4 = Drawable->Destination.GetRight();

        const Real32 DestinationY1 = Drawable->Destination.GetTop();
        const Real32 DestinationY2 = Drawable->Destination.GetTop();
        const Real32 DestinationY3 = Drawable->Destination.GetBottom();
        const Real32 DestinationY4 = Drawable->Destination.GetBottom();

        Buffer->V1.Position.Set(DestinationX1, DestinationY1, Drawable->Depth);
        Buffer->V1.Color = Drawable->Color;
        Buffer->V1.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetTop());

        Buffer->V2.Position.Set(DestinationX2, DestinationY2, Drawable->Depth);
        Buffer->V2.Color = Drawable->Color;
        Buffer->V2.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetTop());

        Buffer->V3.Position.Set(DestinationX3, DestinationY3, Drawable->Depth);
        Buffer->V3.Color = Drawable->Color;
        Buffer->V3.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetBottom());

        Buffer->V4.Position.Set(DestinationX4, DestinationY4, Drawable->Depth);
        Buffer->V4.Color = Drawable->Color;
        Buffer->V4.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetBottom());
    }
}