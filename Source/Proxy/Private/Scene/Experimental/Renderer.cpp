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

#include "Renderer.hpp"
#include <Content/Service.hpp>
#include <windows.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    UInt UTF8Length(const char* p)
    {
        size_t n = 0;

        while(*p)
        {
            if((*p & 0xc0) != 0x80) // If this is a leading char...
                ++n;
            ++p;
        }

        return n;
    }

    UInt UTF8ReadChar(const char* p, const char** ppEnd)
    {
        char16_t        c = 0;
        const char*  pCurrent;
        uint8_t         cChar0((uint8_t)*p), cChar1, cChar2, cChar3;

        if(cChar0 < 0x80)
        {
            c = cChar0;
            pCurrent = p + 1;
        }
        else
        {

            if((cChar0 & 0xE0) == 0xC0)
            {
                c = (char16_t)((cChar0 & 0x1F) << 6);

                cChar1 = static_cast<uint8_t>(p[1]);
                c |= cChar1 & 0x3F;


                pCurrent = p + 2;
            }
            else if((cChar0 & 0xF0) == 0xE0)
            {
                c = (char16_t)((cChar0 & 0xF) << 12);

                cChar1 = static_cast<uint8_t>(p[1]);
                c |= (cChar1 & 0x3F) << 6;

                cChar2 = static_cast<uint8_t>(p[2]);
                c |= cChar2 & 0x3F;


                pCurrent = p + 3;
            }
            else
            {
                c = (char16_t)((cChar0 & 0x7) << 18);

                cChar1 = static_cast<uint8_t>(p[1]);
                c |= (char16_t)((cChar1 & 0x3F) << 12);

                cChar2 = static_cast<uint8_t>(p[2]);
                c |= (cChar2 & 0x3F) << 6;

                cChar3 = static_cast<uint8_t>(p[3]);
                c |= cChar3 & 0x3F;


                pCurrent = p + 4;
            }
        }

        if(ppEnd)
            *ppEnd = pCurrent;

        return c;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
        : mGraphics { Context.GetSubsystem<Graphic::Service>() }
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

    void Renderer::Begin(Ref<const Graphic::Camera> Camera)
    {
        mSceneData.Camera = Camera.GetView() * Camera.GetProjection();
        mSceneParameters.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(Rectf Destination, Rectf Source, Real32 Depth, Real32 Angle, Color Tint, Order Order,
                    Ref<const SPtr<const Graphic::Pipeline>> Pipeline,
                    Ref<const SPtr<const Graphic::Material>> Material)
    {
        if (!Pipeline->HasLoaded() || !Material->HasLoaded())
        {
            return;
        }

        if (mDrawablesRef.full())
        {
            Flush();
        }


        DrawableID zKey;

        if (Order == Order::Transparent)
        {
            zKey.Transparent.bType     = 0;
            zKey.Transparent.bMaterial = Material->GetID();
            zKey.Transparent.bTechnique = Pipeline->GetID();
            zKey.Transparent.bDepth = * (UInt32 *) (& Depth);
        }
        else
        {
            const auto zDepthInv = (1.0f + Depth);

            zKey.Opaque.bType     = 1;
            zKey.Opaque.bMaterial = Material->GetID();
            zKey.Opaque.bTechnique = Pipeline->GetID();
            zKey.Opaque.bDepth = * (UInt32 *) (& zDepthInv);
        }

        Ref<Drawable> Drawable = mDrawablesRef.push_back();
        Drawable.ID          = zKey;
        Drawable.Destination = Destination;
        Drawable.Source      = Source;
        Drawable.Depth       = Depth;
        Drawable.Angle       = (Angle * 3.141592654f) / 180.0f;
        Drawable.Color       = Tint.AsPacked();
        Drawable.Pipeline    = Pipeline;
        Drawable.Material    = Material;


        mDrawablesPtr.push_back(& Drawable);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawFont(SPtr<const Graphic::Font> Font, CStr Text, Alignment Alignment, Vector2f Position, Real32 Depth, Real32 Scale, Color Tint) // TODO: Remove per quad
    {
        const auto   UTF8StringLength = UTF8Length(Text.data());

        if (!Font->HasLoaded())
        {
            return;
        }

        Ref<const Graphic::Font::Metrics> Metrics = Font->GetMetrics();

        // Align horizontally
        if (Alignment & FONS_ALIGN_LEFT)
        {
            // Empty
        }
        else
        {
            Real32 BoundariesX = 0;

            const char * UTF8String = Text.data();
            for (UInt Index = 0; Index < UTF8StringLength; ++Index)
            {
                const UInt Character = UTF8ReadChar(UTF8String, & UTF8String);
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                const Real32 Kerning = (Index < UTF8StringLength - 1) ? Font->GetKerning(Character, UTF8ReadChar(UTF8String, nullptr)) : 0.0f;
                BoundariesX += Scale * (Glyph->Advance + Kerning);
            }

            if (Alignment & FONS_ALIGN_RIGHT)
            {
                Position.SetX(Position.GetX() - BoundariesX);
            }
            else
            {
                Position.SetX(Position.GetX() - BoundariesX * 0.5f);
            }
        }

        // Align vertically.
        if (Alignment & FONS_ALIGN_TOP)
        {
            Position.SetY(Position.GetY() + (Metrics.Descender * Scale));
        }
        else if (Alignment & FONS_ALIGN_MIDDLE)
        {
            Position.SetY(Position.GetY() - (Metrics.Ascender + Metrics.Descender) / 2.0f * Scale);
        }
        else if (Alignment & FONS_ALIGN_BOTTOM)
        {
            Position.SetY(Position.GetY() + (Metrics.Ascender * Scale));
        }

        Real32 CurrentX = Position.GetX();
        Real32 CurrentY = Position.GetY();

        const char * UTF8String = Text.data();
        for (UInt Index = 0; Index < UTF8StringLength; ++Index)
        {
            UInt Character = UTF8ReadChar(UTF8String, & UTF8String);

            switch (Character)
            {
            case ' ' :
            {
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                if (Index < UTF8StringLength - 1)
                {
                    const Real32 Kerning = Font->GetKerning(Character, UTF8ReadChar(UTF8String, nullptr));
                    CurrentX += Scale * (Glyph->Advance + Kerning);
                }
            }
                break;
            case '\r':
                break;
            case '\n':
                CurrentX  = Position.GetX();
                CurrentY += Scale * Font->GetMetrics().UnderlineHeight;
                break;
            default:
            {
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                Rectf PositionRect = (Glyph->PlaneBounds * Scale) + Vector2f(CurrentX, CurrentY);

                Draw(PositionRect, Glyph->ImageBounds, Depth, 0.0f, Tint, Scene::Renderer::Order::Opaque, mFontPipeline, Font->GetMaterial());

                if (Index < UTF8StringLength - 1)
                {
                    const Real32 Kerning = Font->GetKerning(Character, UTF8ReadChar(UTF8String, nullptr));
                    CurrentX += Scale * (Glyph->Advance + Kerning);
                }
            }
                break;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush() // TODO: Clean version
    {
        // Sort all sprites back to front and by material/pipeline
        eastl::sort(mDrawablesPtr.begin(), mDrawablesPtr.end(), [](Ptr<const Drawable> Left, Ptr<const Drawable> Right)
        {
            return Left->ID.Key > Right->ID.Key;
        });

        // Fill geometry's submission
        UInt UniformSize = 0;
        UInt IndexBufferOffset = 0;
        UInt ArrayBufferOffset;
        UInt ArrayBufferBatch = 0;
        Ptr<VertexShaderGeometry> ArrayBlockPtr
            = mGraphics->Map<VertexShaderGeometry>(mArrayBuffer, ArrayBufferOffset, mDrawablesPtr.size());

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
                    mMaterialsPtr.push_back((Graphic::Material *) ThisDrawable->Material.get());
                }

                Ref<Graphic::Submission> Submission = mSubmissions.push_back();
                Submission.Vertices.Buffer = mArrayBuffer;
                Submission.Vertices.Stride = sizeof(VertexShaderLayout);
                Submission.Indices.Buffer = mIndexBuffer;
                Submission.Indices.Length = k_IndicesPerQuad * ArrayBufferBatch;
                Submission.Indices.Offset = k_IndicesPerQuad * IndexBufferOffset;
                Submission.Indices.Stride = sizeof(UInt16);

                Submission.Pipeline = ThisDrawable->Pipeline->GetID();

                for (SInt SourceIndex = 0; SourceIndex < Graphic::k_MaxSources; ++SourceIndex)
                {
                    if (const SPtr<const Graphic::Texture> Texture = ThisDrawable->Material->GetTexture(SourceIndex))
                    {
                        Submission.Textures[SourceIndex] = Texture->GetID();
                    }
                    Submission.Samplers[SourceIndex] = ThisDrawable->Material->GetSampler(SourceIndex);
                }

                IndexBufferOffset += ArrayBufferBatch;
                ArrayBufferBatch  = 0;
            }
        }

        mGraphics->Unmap(mArrayBuffer);

        {
            UInt UniformBufferSize = k_SceneSize + UniformSize + mSceneParameters.size();
            UInt UniformBufferOffset;
            UInt Index = 0;
            Ptr<Vector4f> UniformBlockPtr = mGraphics->Map<Vector4f>(mSceneBuffer, UniformBufferOffset, UniformBufferSize);

            UInt BaseUniformOffset = UniformBufferOffset;
            UInt OtherUniformOffset = (BaseUniformOffset + k_SceneSize);
            UInt ElseUniformOffset = (OtherUniformOffset + mSceneParameters.size());

            * reinterpret_cast<Ptr<VertexShaderData>>(UniformBlockPtr) = mSceneData;
            UniformBlockPtr += k_SceneSize;

            if (!mSceneParameters.empty())
            {
                FastCopyMemory(UniformBlockPtr, mSceneParameters.data(), sizeof(Vector4f) * mSceneParameters.size());
                UniformBlockPtr += mSceneParameters.size();
            }

            for (Ptr<const Graphic::Material> MaterialPtr : mMaterialsPtr)
            {
                const CPtr<const Vector4f> Parameters = MaterialPtr->GetParameters();

                mSubmissions[Index].Uniforms[0].Buffer  = mSceneBuffer;
                mSubmissions[Index].Uniforms[0].Offset  = BaseUniformOffset;
                mSubmissions[Index].Uniforms[0].Length  = k_SceneSize;

                if (! mSceneParameters.empty())
                {
                    mSubmissions[Index].Uniforms[1].Buffer  = mSceneBuffer;
                    mSubmissions[Index].Uniforms[1].Offset  = OtherUniformOffset;
                    mSubmissions[Index].Uniforms[1].Length  = mSceneParameters.size();
                }

                if (! Parameters.empty())
                {
                    FastCopyMemory(UniformBlockPtr, Parameters.data(), Parameters.size_bytes());

                    mSubmissions[Index].Uniforms[2].Buffer  = mSceneBuffer;
                    mSubmissions[Index].Uniforms[2].Offset  = ElseUniformOffset;
                    mSubmissions[Index].Uniforms[2].Length  = Parameters.size();

                    UniformBlockPtr   += Parameters.size();
                    ElseUniformOffset += Parameters.size();
                }

                ++Index;
            }

            mGraphics->Unmap(mSceneBuffer);
        }

        // Submit all command(s) to the gpu
        mGraphics->Submit(mSubmissions);

        // Reset all stack(s) back to original states
        mSubmissions.clear();
        mDrawablesRef.clear();
        mDrawablesPtr.clear();
        mMaterialsPtr.clear();
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

    void Renderer::CreateResources()
    {
        // Create array buffer
        mArrayBuffer = mGraphics->CreateBuffer<VertexShaderGeometry>(Graphic::Usage::Vertices, k_MaxDrawables);

        // Create index buffer
        UInt           IndexLength = k_MaxDrawables * k_IndicesPerQuad;
        UPtr<UInt16[]> IndexMemory = NewUniquePtr<UInt16[]>(IndexLength);

        for (UInt Position = 0, Index = 0; Position < IndexLength; Index += 4)
        {
            IndexMemory[Position++] = Index;
            IndexMemory[Position++] = Index + 1;
            IndexMemory[Position++] = Index + 2;
            IndexMemory[Position++] = Index + 1;
            IndexMemory[Position++] = Index + 3;
            IndexMemory[Position++] = Index + 2;
        }

        mIndexBuffer = mGraphics->CreateBuffer(Graphic::Usage::Indices, IndexLength, CPtr<UInt16>(IndexMemory.get(), IndexLength));

        // Create scene buffer
        mSceneBuffer = mGraphics->CreateBuffer<Vector4f>(Graphic::Usage::Uniform, k_MaxUniforms);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DeleteResources()
    {
        mGraphics->DeleteBuffer(mArrayBuffer);
        mGraphics->DeleteBuffer(mIndexBuffer);
        mGraphics->DeleteBuffer(mSceneBuffer);
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

        if (Drawable->Angle != 0.0f)
        {
            const Vector3f    Position(Drawable->Destination.GetX(), Drawable->Destination.GetY(), Drawable->Depth);
            const Vector3f    Size(Drawable->Destination.GetWidth(), Drawable->Destination.GetHeight(), 1.0f);
            const Quaternionf Rotation = Quaternionf::FromAngles(Drawable->Angle, Vector3f(0.0f, 0.0f, 1.0f));

            const Matrix4f Transformation = Matrix4f::CreateTransform(Position, Rotation, Size);

            Buffer->V1.Position = Transformation * Vector3f(-0.5f, -0.5f, 0.0f);
            Buffer->V2.Position = Transformation * Vector3f( 0.5f, -0.5f, 0.0f);
            Buffer->V3.Position = Transformation * Vector3f(-0.5f,  0.5f, 0.0f);
            Buffer->V4.Position = Transformation * Vector3f( 0.5f,  0.5f, 0.0f);
        }
        else
        {
            Buffer->V1.Position.Set(DestinationX1, DestinationY1, Drawable->Depth);
            Buffer->V2.Position.Set(DestinationX2, DestinationY2, Drawable->Depth);
            Buffer->V3.Position.Set(DestinationX3, DestinationY3, Drawable->Depth);
            Buffer->V4.Position.Set(DestinationX4, DestinationY4, Drawable->Depth);
        }

        Buffer->V1.Color = Drawable->Color;
        Buffer->V1.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetTop());

        Buffer->V2.Color = Drawable->Color;
        Buffer->V2.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetTop());

        Buffer->V3.Color = Drawable->Color;
        Buffer->V3.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetBottom());

        Buffer->V4.Color = Drawable->Color;
        Buffer->V4.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetBottom());
    }


}