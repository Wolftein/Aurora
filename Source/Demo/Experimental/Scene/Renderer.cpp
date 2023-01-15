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
#include <EASTL/sort.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
    {
        mService = Context.GetSubsystem<Graphic::Service>();

        mBuffers[0] = mService->CreateBuffer(true, sizeof(Quad) * k_MaxDrawables, {});

        UPtr<UInt16[]> Memory = eastl::make_unique<UInt16[]>(k_MaxDrawables * 6u);

        for (UInt32 Position = 0u, Index = 0u, max = k_MaxDrawables * 6u; Position < max; Index += 4u)
        {
            Memory[Position++] = static_cast<UInt16>(Index);
            Memory[Position++] = static_cast<UInt16>(Index + 1);
            Memory[Position++] = static_cast<UInt16>(Index + 2);
            Memory[Position++] = static_cast<UInt16>(Index + 1);
            Memory[Position++] = static_cast<UInt16>(Index + 3);
            Memory[Position++] = static_cast<UInt16>(Index + 2);
        }

        mBuffers[1] = mService->CreateBuffer(true, 6 * k_MaxDrawables * sizeof(UInt16), {
            reinterpret_cast<UInt08 *>(Memory.get()), 6 * k_MaxDrawables * sizeof(UInt16)
        });

        mBuffers[2] = mService->CreateBuffer(false, k_MaxUniforms * sizeof(Vector4f), {});
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::~Renderer()
    {

    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Begin(Ref<const Camera> Camera, Real32 Time)
    {
        mScene.ProjectionViewMatrix = Camera.GetView() * Camera.GetProjection();
        mScene.Time = Time;
        // TODO: Set Uniform....
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
        if (mDrawables.full())
        {
            Flush();
        }

        Ref<Drawable> Drawable = mDrawables.push_back();
        Drawable.ID          = Material->GetID(); // TODO (Unique ID for Opaque/Translucid)
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
        if (!mDrawables.empty())
        {
            Flush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush()
    {
        eastl::sort(mDrawablesPtr.begin(), mDrawablesPtr.end(), [](Ptr<const Drawable> Left, Ptr<const Drawable> Right)
        {
            return Left->ID > Right->ID;
        });

        UInt VBOffset = 0, UBOffset = 0;

        Ptr<Quad> Vertices = MapVertexBuffer(mDrawablesPtr.size(), VBOffset);
        UInt VertexOffset = (VBOffset - mDrawablesPtr.size());
        UInt Count        = 0;
        printf("V Offset: %d %d - ", mDrawablesPtr.size(), VertexOffset);

        UInt UniformSize = 0;
        SPtr<const Pipeline> PipelineLast = mDrawablesPtr[0]->Pipeline;
        SPtr<const Material> MaterialLast = mDrawablesPtr[0]->Material;

        for (Ptr<Drawable> Drawable : mDrawablesPtr)
        {
            WriteVertexBuffer(Drawable, Vertices);
            ++Vertices;

            if (MaterialLast != Drawable->Material || PipelineLast != Drawable->Pipeline)
            {
                if (MaterialLast != Drawable->Material)
                {
                    UniformSize += MaterialLast->GetParameters().size();
                    mMaterialsPtr.push_back(MaterialLast.get());
                }

                Ref<Submission> Submission = mSubmissions.push_back();
                Submission.Vertices.Buffer    = mBuffers[0];
                Submission.Vertices.Offset    = 0;
                Submission.Vertices.Length    = 0;
                Submission.Vertices.Stride    = 24;
                Submission.Indices.Buffer     = mBuffers[1];
                Submission.Indices.Offset     = VertexOffset * 6;
                Submission.Indices.Length     = 6 * Count;
                Submission.Indices.Stride     = sizeof(UInt16);
                Submission.Pipeline            = PipelineLast->GetID();
                Submission.Textures[0]         = MaterialLast->GetTexture(0)->GetID();

                VertexOffset += Count;
                Count = 0;

                MaterialLast = Drawable->Material;
                PipelineLast = Drawable->Pipeline;
            }

            ++Count;
        }

        // Add Last
        if (Count > 0)
        {
            UniformSize += MaterialLast->GetParameters().size();
            mMaterialsPtr.push_back(MaterialLast.get());

            Ref<Submission> Submission = mSubmissions.push_back();

            Submission.Vertices.Buffer    = mBuffers[0];
            Submission.Vertices.Offset    = 0;
            Submission.Vertices.Length    = 0;
            Submission.Vertices.Stride    = 24;
            Submission.Indices.Buffer     = mBuffers[1];
            Submission.Indices.Offset     = VertexOffset * 6;
            Submission.Indices.Length     = 6 * Count;
            Submission.Indices.Stride     = sizeof(UInt16);
            Submission.Pipeline            = PipelineLast->GetID();
            Submission.Textures[0]         = MaterialLast->GetTexture(0)->GetID();
        }


        UnmapVertexBuffer();


        Ptr<Vector4f> Uniforms = MapUniformBuffer(16 + UniformSize, UBOffset);

        UInt Index  = 0;
        UInt BaseOffset = (UBOffset - (16 + UniformSize));
        UInt MaterialOffset = BaseOffset + 16;
        printf("U Offset: %d\n", UBOffset);

        * reinterpret_cast<Ptr<Scene>>(Uniforms) = mScene;
        Uniforms += 16;

        for (Ptr<const Material> MaterialPtr : mMaterialsPtr)
        {
            WriteUniformBuffer(MaterialPtr->GetParameters(), Uniforms);

            mSubmissions[Index].Uniforms[0].Buffer  = mBuffers[2];
            mSubmissions[Index].Uniforms[0].Offset  = BaseOffset;
            mSubmissions[Index].Uniforms[0].Length  = 16;

            mSubmissions[Index].Uniforms[1].Buffer  = mBuffers[2];
            mSubmissions[Index].Uniforms[1].Offset  = MaterialOffset;
            mSubmissions[Index].Uniforms[1].Length  = MaterialPtr->GetParameters().size();

            Uniforms += MaterialPtr->GetParameters().size();
            MaterialOffset += (MaterialPtr->GetParameters().size());

            ++Index;
        }

        UnmapUniformBuffer();


        mService->Submit(mSubmissions);

        mDrawables.reset_lose_memory();
        mDrawablesPtr.reset_lose_memory();
        mSubmissions.reset_lose_memory();
        mMaterialsPtr.reset_lose_memory();
    }

    Ptr<Renderer::Quad> Renderer::MapVertexBuffer(UInt Count, Ref<UInt> Offset)
    {
        Ptr<Renderer::Quad> Buff = mService->Map<Renderer::Quad>(mBuffers[0], Offset, Count);

        Offset += Count;

        return Buff;
    }

    void Renderer::WriteVertexBuffer(Ptr<Drawable> Drawable, Ptr<Quad> Buffer)
    {
        Real32 DestinationX1 = Drawable->Destination.GetLeft();
        Real32 DestinationX2 = Drawable->Destination.GetRight();
        Real32 DestinationX3 = Drawable->Destination.GetLeft();
        Real32 DestinationX4 = Drawable->Destination.GetRight();

        Real32 DestinationY1 = Drawable->Destination.GetTop();
        Real32 DestinationY2 = Drawable->Destination.GetTop();
        Real32 DestinationY3 = Drawable->Destination.GetBottom();
        Real32 DestinationY4 = Drawable->Destination.GetBottom();

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

    void Renderer::UnmapVertexBuffer()
    {
        mService->Unmap(mBuffers[0]);
    }

    Ptr<Vector4f> Renderer::MapUniformBuffer(UInt Count, Ref<UInt> Offset)
    {
        Ptr<Vector4f> Buff = mService->Map<Vector4f>(mBuffers[2], true, Offset, Count);

        Offset += Count;

        return Buff;
    }

    void Renderer::WriteUniformBuffer(CPtr<const Vector4f> Uniform, Ptr<Vector4f> Buffer)
    {
        memcpy(Buffer, Uniform.data(), Uniform.size_bytes());
    }

    void Renderer::UnmapUniformBuffer()
    {
        mService->Unmap(mBuffers[2]);
    }


}