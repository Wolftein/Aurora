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

        mVertexBuffer = mService->CreateBuffer(true, sizeof(Quad)  * 4 * k_MaxDrawables, {});
        mVertexBufferOffset = 0;

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

        mIndexBuffer = mService->CreateBuffer(true, 6 * k_MaxDrawables * sizeof(UInt16), {
            reinterpret_cast<UInt08 *>(Memory.get()), 6 * k_MaxDrawables * sizeof(UInt16)
        });

        mUniformBuffer = mService->CreateBuffer(false, k_MaxUniforms * sizeof(Vector4f), {});
        mUniformBufferOffset = 0;
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

        Ptr<Quad> Vertices = MapVertexBuffer(mDrawablesPtr.size());
        UInt VertexOffset = (mVertexBufferOffset - mDrawablesPtr.size());
        UInt Count        = 0;

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
                Submission.Vertices.Buffer    = mVertexBuffer;
                Submission.Vertices.Offset    = 0;
                Submission.Vertices.Length    = 0;
                Submission.Vertices.Stride    = 24;
                Submission.Indices.Buffer     = mIndexBuffer;
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

            Submission.Vertices.Buffer    = mVertexBuffer;
            Submission.Vertices.Offset    = 0;
            Submission.Vertices.Length    = 0;
            Submission.Vertices.Stride    = 24;
            Submission.Indices.Buffer     = mIndexBuffer;
            Submission.Indices.Offset     = VertexOffset * 6;
            Submission.Indices.Length     = 6 * Count;
            Submission.Indices.Stride     = sizeof(UInt16);
            Submission.Pipeline            = PipelineLast->GetID();
            Submission.Textures[0]         = MaterialLast->GetTexture(0)->GetID();
        }


        UnmapVertexBuffer();


        Ptr<Vector4f> Uniforms = MapUniformBuffer(16 + UniformSize);

        UInt Index  = 0;
        UInt BaseOffset = (mUniformBufferOffset - (16 + UniformSize));
        UInt MaterialOffset = BaseOffset + 16;

        * reinterpret_cast<Ptr<Scene>>(Uniforms) = mScene;
        Uniforms += 16;

        for (Ptr<const Material> MaterialPtr : mMaterialsPtr)
        {
            WriteUniformBuffer(MaterialPtr->GetParameters(), Uniforms);

            mSubmissions[Index].Uniforms[0].Buffer  = mUniformBuffer;
            mSubmissions[Index].Uniforms[0].Offset  = BaseOffset;
            mSubmissions[Index].Uniforms[0].Length  = 16;

            mSubmissions[Index].Uniforms[1].Buffer  = mUniformBuffer;
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

    Ptr<Renderer::Quad> Renderer::MapVertexBuffer(UInt Count)
    {
        const UInt Offset   = mVertexBufferOffset + Count > k_MaxDrawables ? 0 : mVertexBufferOffset;
        const UInt Size     = Count * sizeof(Quad);
        mVertexBufferOffset = (Offset + Count);

        return static_cast<Ptr<Quad>>(mService->Map(mVertexBuffer, Offset == 0, Offset * sizeof(Quad), Size));
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

        Buffer->V1.X    = DestinationX1;
        Buffer->V1.Y    = DestinationY1;
        Buffer->V1.Z    = Drawable->Depth;
        Buffer->V1.RGBA = Drawable->Color;
        Buffer->V1.U    = Drawable->Source.GetLeft();
        Buffer->V1.V    = Drawable->Source.GetTop();

        Buffer->V2.X    = DestinationX2;
        Buffer->V2.Y    = DestinationY2;
        Buffer->V2.Z    = Drawable->Depth;
        Buffer->V2.RGBA = Drawable->Color;
        Buffer->V2.U    = Drawable->Source.GetRight();
        Buffer->V2.V    = Drawable->Source.GetTop();

        Buffer->V3.X    = DestinationX3;
        Buffer->V3.Y    = DestinationY3;
        Buffer->V3.Z    = Drawable->Depth;
        Buffer->V3.RGBA = Drawable->Color;
        Buffer->V3.U    = Drawable->Source.GetLeft();
        Buffer->V3.V    = Drawable->Source.GetBottom();

        Buffer->V4.X    = DestinationX4;
        Buffer->V4.Y    = DestinationY4;
        Buffer->V4.Z    = Drawable->Depth;
        Buffer->V4.RGBA = Drawable->Color;
        Buffer->V4.U    = Drawable->Source.GetRight();
        Buffer->V4.V    = Drawable->Source.GetBottom();
    }

    void Renderer::UnmapVertexBuffer()
    {
        mService->Unmap(mVertexBuffer);
    }

    Ptr<Vector4f> Renderer::MapUniformBuffer(UInt Count)
    {
        const UInt Offset   = mUniformBufferOffset + Count > k_MaxUniforms ? 0 : mUniformBufferOffset;
        const UInt Size     = Count * sizeof(Vector4f);
        mUniformBufferOffset = (Offset + Count);

        return static_cast<Ptr<Vector4f>>(mService->Map(mUniformBuffer, Offset == 0, Offset * sizeof(Vector4f), Size));
    }

    void Renderer::WriteUniformBuffer(CPtr<const Vector4f> Uniform, Ptr<Vector4f> Buffer)
    {
        memcpy(Buffer, Uniform.data(), Uniform.size_bytes());
    }

    void Renderer::UnmapUniformBuffer()
    {
        mService->Unmap(mUniformBuffer);
    }


}