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

    Font::Font(Ref<const Content::Uri> Key)
        : AbstractResource(Key)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Font::Load(Metrics&& Metrics, Atlas&& Atlas, Table<UInt32, Glyph>&& Glyphs, Table<UInt64, Real32>&& Kerning)
    {
        mMetrics = Move(Metrics);
        mAtlas   = Move(Atlas);
        mGlyphs  = Move(Glyphs);
        mKerning = Move(Kerning);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Font::Measure(CStr16 Text, Real32 Scale, Alignment Alignment)
    {
        Real32 CurrentX    = 0.0f;
        Real32 CurrentY    = 0.0f;
        Real32 BoundariesX = 0.0f;
        Real32 BoundariesY = 0.0f;

        for (UInt Letter = 0; Letter < Text.size(); ++Letter)
        {
            const UInt Character = Text[Letter];

            switch (Character)
            {
                case '\r':
                    if (CurrentX > BoundariesX)
                    {
                        BoundariesX = CurrentX;
                    }
                    CurrentX = 0.0f;
                    break;
                case '\n':
                    CurrentY += Scale * mMetrics.UnderlineHeight;
                    break;
                default:
                {
                    const Ptr<const Glyph> Glyph = GetGlyph(Character);

                    const Real32 Kerning = (Letter < Text.size() - 1) ? GetKerning(Character, Text[Letter + 1]) : 0.0f;
                    CurrentX += Scale * (Glyph->Advance + Kerning);
                    break;
                }
            }
        }

        if (CurrentX > BoundariesX)
        {
            BoundariesX = CurrentX;
        }

        if (CurrentY > BoundariesY)
        {
            BoundariesY = CurrentY;
        }

        Real32 OffsetX = 0.0f;
        Real32 OffsetY = 0.0f;

        switch (Alignment)
        {
        case Alignment::LeftTop:
            OffsetY = mMetrics.Descender * Scale;
            break;
        case Alignment::LeftMiddle:
            OffsetY = - (mMetrics.Ascender + mMetrics.Descender) / 2.0f * Scale;
            break;
        case Alignment::LeftBottom:
            OffsetY = mMetrics.Ascender * Scale;
            break;
        case Alignment::LeftBaseline:
            break;
        case Alignment::CenterTop:
            OffsetX = - BoundariesX * 0.5f;
            OffsetY = mMetrics.Descender * Scale;
            break;
        case Alignment::CenterMiddle:
            OffsetX = - BoundariesX * 0.5f;
            OffsetY = - (mMetrics.Ascender + mMetrics.Descender) / 2.0f * Scale;
            break;
        case Alignment::CenterBottom:
            OffsetX = - BoundariesX * 0.5f;
            OffsetY = mMetrics.Ascender * Scale;
            break;
        case Alignment::CenterBaseline:
            OffsetX = - BoundariesX * 0.5f;
            break;
        case Alignment::RightTop:
            OffsetX = - BoundariesX;
            OffsetY = mMetrics.Descender * Scale;
            break;
        case Alignment::RightMiddle:
            OffsetX = - BoundariesX;
            OffsetY = - (mMetrics.Ascender + mMetrics.Descender) / 2.0f * Scale;
            break;
        case Alignment::RightBottom:
            OffsetX = - BoundariesX;
            OffsetY = mMetrics.Ascender * Scale;
            break;
        case Alignment::RightBaseline:
            OffsetX = - BoundariesX;
            break;
        }
        return Rectf(OffsetX, OffsetY, OffsetX + BoundariesX, OffsetY + BoundariesY);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Font::OnCreate(Ref<Subsystem::Context> Context)
    {
        Bool Success;

        // Allocates texture for the atlas
        const SPtr<Texture> Atlas = NewPtr<Texture>("_Private");
        Atlas->Load(
            Graphic::TextureFormat::RGBA8UIntNorm,
            Graphic::TextureLayout::Source, mAtlas.Width, mAtlas.Height, 1, Move(mAtlas.Bytes));
        Success = Atlas->Create(Context);

        // Allocates material for the font
        mMaterial = NewPtr<Graphic::Material>("_Private");
        mMaterial->SetResidence(true);
        mMaterial->SetTexture(0, Atlas);
        mMaterial->SetSampler(0, Sampler { TextureEdge::Repeat, TextureEdge::Repeat, TextureFilter::Bilinear });
        mMaterial->SetParameter(0, Vector3f(Atlas->GetWidth(), Atlas->GetHeight(), mMetrics.Distance));

        Success = Success && mMaterial->Create(Context);

        SetMemory(mGlyphs.size() * sizeof(Glyph));
        return Success;
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