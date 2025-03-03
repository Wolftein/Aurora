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

#include "Font.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Font::Font(Any<Content::Uri> Key)
        : AbstractResource(Move(Key))
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Font::Load(Any<Metrics> Metrics, Any<Atlas> Atlas, Any<Table<UInt32, Glyph>> Glyphs, Any<Table<UInt64, Real32>> Kerning)
    {
        mMetrics = Move(Metrics);
        mAtlas   = Move(Atlas);
        mGlyphs  = Move(Glyphs);
        mKerning = Move(Kerning);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Font::Calculate(CStr16 Text, Real32 Size, ConstRef<Pivot> Pivot) const
    {
        const Vector2f Measurement = Measure(Text, Size);

        Real32 OffsetX = 0.0f;
        Real32 OffsetY = 0.0f;

        switch (Pivot.GetType())
        {
        case Pivot::Type::LeftTop:
            break;
        case Pivot::Type::LeftMiddle:
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Pivot::Type::LeftBottom:
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Pivot::Type::LeftBaseline:
            OffsetY -= mMetrics.Ascender * Size;
            break;
        case Pivot::Type::CenterTop:
            OffsetX -= Measurement.GetX() * 0.5f;
            break;
        case Pivot::Type::CenterMiddle:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Pivot::Type::CenterBottom:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Pivot::Type::CenterBaseline:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= mMetrics.Ascender * Size;
            break;
        case Pivot::Type::RightTop:
            OffsetX -= Measurement.GetX();
            break;
        case Pivot::Type::RightMiddle:
            OffsetX -= Measurement.GetX();
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Pivot::Type::RightBottom:
            OffsetX -= Measurement.GetX();
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Pivot::Type::RightBaseline:
            OffsetX -= Measurement.GetX();
            OffsetY -= mMetrics.Ascender * Size;
            break;
        }
        return Rectf(OffsetX, OffsetY, OffsetX + Measurement.GetX(), OffsetY + Measurement.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2f Font::Measure(CStr16 Text, Real32 Size) const
    {
        Real32 CurrentX = 0.0f;
        Real32 MaximumX = 0.0f;
        Real32 MaximumY = mMetrics.UnderlineHeight;

        for (UInt Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const UInt Codepoint = Text[Symbol];

            switch (Codepoint)
            {
            case '\r':
                CurrentX = 0.0f;
                break;
            case '\n':
                MaximumY += mMetrics.UnderlineHeight;
                break;
            default:
            {
                const Ptr<const Glyph> Glyph = GetGlyph(Codepoint);
                CurrentX += (GetKerning(Previous, Codepoint) + Glyph->Advance);
                MaximumX = Max(MaximumX, CurrentX);
                break;
            }
            }
            Previous = Codepoint;
        }
        return Vector2f(MaximumX, MaximumY) * Size;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Font::OnCreate(Ref<Subsystem::Context> Context)
    {
        SetMemory(mGlyphs.size() * sizeof(Glyph));

        // Allocates texture for the atlas
        constexpr UInt8 k_DefaultMipmaps = 1;
        constexpr UInt8 k_DefaultSamples = 1;

        const SPtr<Texture> Atlas = NewPtr<Texture>(Content::Uri::Merge(GetKey(), "Atlas"));
        Atlas->Load(
            TextureFormat::RGBA8UIntNorm,
            TextureLayout::Source, mAtlas.Width, mAtlas.Height, k_DefaultMipmaps, k_DefaultSamples, Move(mAtlas.Bytes));

        // Allocates material for the font
        constexpr Sampler k_DefaultSampler = Sampler(TextureEdge::Repeat, TextureEdge::Repeat, TextureFilter::Bilinear);

        mMaterial = NewPtr<Material>(Content::Uri::Merge(GetKey(), "Material"));
        mMaterial->SetOwnership(true);
        mMaterial->SetTexture(TextureSlot::Diffuse, Atlas);
        mMaterial->SetSampler(TextureSlot::Diffuse, k_DefaultSampler);
        mMaterial->SetParameter(0, Vector3f(Atlas->GetWidth(), Atlas->GetHeight(), mMetrics.Distance));
        return mMaterial->Create(Context);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Font::OnDelete(Ref<Subsystem::Context> Context)
    {
        if (mMaterial)
        {
            mMaterial->Delete(Context);
        }
    }
}