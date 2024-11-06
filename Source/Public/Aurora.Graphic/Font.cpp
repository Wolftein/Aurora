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

    Vector2f Font::Measure(CStr16 Text, Real32 Size) const
    {
        const Real32 Scale = Size / mMetrics.Size;

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
                    MaximumY += mMetrics.UnderlineHeight * Scale;
                    break;
                default:
                {
                    const Ptr<const Glyph> Glyph = GetGlyph(Codepoint);
                    CurrentX += GetKerning(Previous, Codepoint) * Scale + Glyph->Advance * Scale;
                    MaximumX = Max(MaximumX, CurrentX);
                    break;
                }
            }
            Previous = Codepoint;
        }
        return Vector2f(MaximumX, MaximumY) * mMetrics.Size;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Font::Calculate(CStr16 Text, Real32 Size, Ref<const Vector2f> Position, Alignment Alignment) const
    {
        const Vector2f Measurement = Measure(Text, Size);

        Real32 OffsetX = Position.GetX();
        Real32 OffsetY = Position.GetY();

        switch (Alignment)
        {
        case Alignment::LeftTop:
            break;
        case Alignment::LeftMiddle:
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Alignment::LeftBottom:
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Alignment::LeftBaseline:
            OffsetY -= mMetrics.Ascender * Size;
            break;
        case Alignment::CenterTop:
            OffsetX -= Measurement.GetX() * 0.5f;
            break;
        case Alignment::CenterMiddle:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Alignment::CenterBottom:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Alignment::CenterBaseline:
            OffsetX -= Measurement.GetX() * 0.5f;
            OffsetY -= mMetrics.Ascender * Size;
            break;
        case Alignment::RightTop:
            OffsetX -= Measurement.GetX();
            break;
        case Alignment::RightMiddle:
            OffsetX -= Measurement.GetX();
            OffsetY -= mMetrics.Ascender * 0.5f * Size;
            break;
        case Alignment::RightBottom:
            OffsetX -= Measurement.GetX();
            OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
            break;
        case Alignment::RightBaseline:
            OffsetX -= Measurement.GetX();
            OffsetY -= mMetrics.Ascender * Size;
            break;
        }
        return Rectf(OffsetX, OffsetY, OffsetX + Measurement.GetX(), OffsetY + Measurement.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Font::OnCreate(Ref<Subsystem::Context> Context)
    {
        SetMemory(mGlyphs.size() * sizeof(Glyph));

        // Allocates texture for the atlas
        const SPtr<Texture> Atlas = NewPtr<Texture>("_Private");
        Atlas->Load(
            TextureFormat::RGBA8UIntNorm,
            TextureLayout::Source, mAtlas.Width, mAtlas.Height, 1, Move(mAtlas.Bytes));

        // Allocates material for the font
        mMaterial = NewPtr<Material>("_Private");
        mMaterial->SetResidence(true);
        mMaterial->SetTexture(0, Atlas);
        mMaterial->SetSampler(0, Sampler { TextureEdge::Repeat, TextureEdge::Repeat, TextureFilter::Bilinear });
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