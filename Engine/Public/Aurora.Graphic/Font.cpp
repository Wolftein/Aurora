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

    Font::Font(AnyRef<Content::Uri> Key)
        : AbstractResource(Move(Key))
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Font::Load(AnyRef<Metrics> Metrics, AnyRef<Table<UInt32, Glyph>> Registry, AnyRef<Table<UInt64, Real32>> Kerning, ConstTracker<Material> Material)
    {
        mMetrics  = Move(Metrics);
        mRegistry = Move(Registry);
        mKerning  = Move(Kerning);
        mMaterial = Material;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2 Font::Measure(ConstText Text, Real32 Size, Real32 Spacing) const
    {
        Real32 CurrentX = 0.0f;
        Real32 MaximumX = 0.0f;
        Real32 MaximumY = (mMetrics.Ascender - mMetrics.Descender);

        for (UInt Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const UInt32 Codepoint = Text[Symbol];

            switch (Codepoint)
            {
                case '\r':
                    CurrentX = 0.0f;
                    break;
                case '\n':
                    MaximumY += (mMetrics.Ascender - mMetrics.Descender);
                    break;
                default:
                {
                    if (const ConstPtr<Glyph> Glyph = GetGlyph(Codepoint); Glyph)
                    {
                        CurrentX += (GetKerning(Previous, Codepoint) + Glyph->Advance + Spacing);
                        MaximumX = Max(MaximumX, CurrentX);
                    }
                    break;
                }
            }
            Previous = Codepoint;
        }
        return Vector2(MaximumX, MaximumY) * Size;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rect Font::Layout(ConstText Word, Real32 Size, Pivot Alignment, Real32 Spacing) const
    {
        const Vector2 Measurement = Measure(Word, Size, Spacing);

        Real32 OffsetX = 0.0f;
        Real32 OffsetY = 0.0f;

        switch (Alignment)
        {
            case Pivot::LeftTop:
                break;
            case Pivot::LeftMiddle:
                OffsetY -= mMetrics.Ascender * 0.5f * Size;
                break;
            case Pivot::LeftBottom:
                OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
                break;
            case Pivot::CenterTop:
                OffsetX -= Measurement.GetX() * 0.5f;
                break;
            case Pivot::CenterMiddle:
                OffsetX -= Measurement.GetX() * 0.5f;
                OffsetY -= mMetrics.Ascender * 0.5f * Size;
                break;
            case Pivot::CenterBottom:
                OffsetX -= Measurement.GetX() * 0.5f;
                OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
                break;
            case Pivot::RightTop:
                OffsetX -= Measurement.GetX();
                break;
            case Pivot::RightMiddle:
                OffsetX -= Measurement.GetX();
                OffsetY -= mMetrics.Ascender * 0.5f * Size;
                break;
            case Pivot::RightBottom:
                OffsetX -= Measurement.GetX();
                OffsetY -= (mMetrics.Ascender - mMetrics.Descender) * Size;
                break;
        }
        return Rect(OffsetX, OffsetY, OffsetX + Measurement.GetX(), OffsetY + Measurement.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Font::OnCreate(Ref<Service::Host> Host)
    {
        if (mMaterial && mMaterial->HasPolicy(Content::Resource::Policy::Exclusive))
        {
            return mMaterial->Create(Host);
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Font::OnDelete(Ref<Service::Host> Host)
    {
        if (mMaterial && mMaterial->HasPolicy(Content::Resource::Policy::Exclusive))
        {
            mMaterial->Delete(Host);
        }
    }
}