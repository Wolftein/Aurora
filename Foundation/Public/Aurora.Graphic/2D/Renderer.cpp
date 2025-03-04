// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
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

    Renderer::Renderer()
        : mInFlightBatch { 0 }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Initialize(ConstSPtr<Graphic::Service> Graphics)
    {
        mGraphics = Graphics;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Begin(Ref<Encoder> Encoder, ConstRef<Camera> Camera)
    {
        const auto SceneAllocationBinding = mGraphics->Allocate(Graphic::Usage::Uniform, CastSpan(Camera.GetWorld()));

        mInFlightEncoder = AddressOf(Encoder);
        mInFlightEncoder->SetUniforms(Encoder::Uniforms::Scene, SceneAllocationBinding);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::SetPipeline(ConstSPtr<Pipeline> Pipeline)
    {
        if (mInFlightPipeline && mInFlightPipeline != Pipeline)
        {
            Flush(false);
        }

        mInFlightPipeline = Pipeline;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::SetMaterial(ConstSPtr<Material> Material)
    {
        if (mInFlightMaterial && mInFlightMaterial != Material)
        {
            Flush(false);
        }

        mInFlightMaterial = Material;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(ConstSPtr<Font> Font, CStr16 Text, UInt16 Size, ConstRef<Matrix4f> Transformation, ConstRef<Pivot> Pivot, Color Tint)
    {
        const Rectf Boundaries = Font->Calculate(Text, Size, Pivot);
        Real32 CurrentX = Boundaries.GetX();
        Real32 CurrentY = Boundaries.GetY();

        for (Char16 Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char16 Codepoint = Text[Symbol];

            switch (Codepoint)
            {
            case '\r':
                CurrentX = Boundaries.GetX();
                break;
            case '\n':
                CurrentY += Font->GetMetrics().UnderlineHeight * Size;
                break;
            default:
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                if (Glyph->PlaneBounds.GetWidth() > 0 && Glyph->PlaneBounds.GetHeight() > 0)
                {
                    const Rectf Destination = (Glyph->PlaneBounds * Size) + Vector2f(CurrentX, CurrentY);
                    Push(Transformation, Destination, Glyph->ImageBounds, Tint.GetValue());
                }
                CurrentX += (Font->GetKerning(Previous, Codepoint) + Glyph->Advance) * Size;
                break;
            }
            Previous = Codepoint;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(ConstRef<Rectf> Rectangle, ConstRef<Rectf> Source, ConstRef<Matrix4f> Transformation, ConstRef<Pivot> Pivot, Color Tint)
    {
        Push(Transformation, Calculate(Pivot, Rectangle), Source, Tint.GetValue());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::End()
    {
        Flush(true);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Push(ConstRef<Matrix4f> Transformation, ConstRef<Rectf> Rectangle, ConstRef<Rectf> Source, UInt32 Color)
    {
        const auto [VertexBufferPointer, VertexBufferBinding]
            = mGraphics->Allocate<PosTexColorLayout>(Graphic::Usage::Vertex, k_VerticesPerQuad);

        VertexBufferPointer[0].Position = Transformation * Vector3f(Rectangle.GetLeft(),  Rectangle.GetBottom(), 0.0f);
        VertexBufferPointer[0].Texture.Set(Source.GetLeft(), Source.GetBottom());
        VertexBufferPointer[0].Color = Color;

        VertexBufferPointer[1].Position = Transformation * Vector3f(Rectangle.GetRight(), Rectangle.GetBottom(), 0.0f);
        VertexBufferPointer[1].Texture.Set(Source.GetRight(), Source.GetBottom());
        VertexBufferPointer[1].Color = Color;

        VertexBufferPointer[2].Position = Transformation * Vector3f(Rectangle.GetRight(), Rectangle.GetTop(),    0.0f);
        VertexBufferPointer[2].Texture.Set(Source.GetRight(), Source.GetTop());
        VertexBufferPointer[2].Color = Color;

        VertexBufferPointer[3].Position = Transformation * Vector3f(Rectangle.GetLeft(),  Rectangle.GetTop(),    0.0f);
        VertexBufferPointer[3].Texture.Set(Source.GetLeft(),  Source.GetTop());
        VertexBufferPointer[3].Color = Color;

        const auto [IndexBufferPointer, IndexBufferBinding] = mGraphics->Allocate<UInt16>(Graphic::Usage::Index, k_IndicesPerQuad);
        const UInt32 IndexBaseValue = mInFlightBatch * 4;
        IndexBufferPointer[0] = IndexBaseValue;
        IndexBufferPointer[1] = IndexBaseValue + 1;
        IndexBufferPointer[2] = IndexBaseValue + 2;
        IndexBufferPointer[3] = IndexBaseValue;
        IndexBufferPointer[4] = IndexBaseValue + 2;
        IndexBufferPointer[5] = IndexBaseValue + 3;

        if (mInFlightBatch == 0)
        {
            mInFlightEncoder->SetVertices(0, VertexBufferBinding);
            mInFlightEncoder->SetIndices(IndexBufferBinding);
        }
        ++mInFlightBatch;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush(Bool Restart)
    {
        if (mInFlightBatch > 0)
        {
            if (CPtr<const UInt8> Parameters = mInFlightMaterial->GetParameters(); !Parameters.empty())
            {
                const auto MaterialBufferBinding = mGraphics->Allocate(Graphic::Usage::Uniform, Parameters);
                mInFlightEncoder->SetUniforms(Encoder::Uniforms::Material, MaterialBufferBinding);
            }

            mInFlightEncoder->SetTexture(0, * mInFlightMaterial->GetTexture(Graphic::TextureSlot::Diffuse));
            mInFlightEncoder->SetSampler(0,   mInFlightMaterial->GetSampler(Graphic::TextureSlot::Diffuse));
            mInFlightEncoder->SetPipeline(* mInFlightPipeline);
            mInFlightEncoder->Draw(mInFlightBatch * k_IndicesPerQuad, 0, 0);

            if (Restart)
            {
                mInFlightEncoder->Reset();
            }
            mInFlightBatch = 0;
        }
    }
}