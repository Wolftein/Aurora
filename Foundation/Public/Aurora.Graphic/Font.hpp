// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Material.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Font final : public Content::AbstractResource<Font>
    {
        friend class AbstractResource;

    public:

        // -=(Undocumented)=-
        struct Atlas
        {
            // -=(Undocumented)=-
            UInt32 Width;

            // -=(Undocumented)=-
            UInt32 Height;

            // -=(Undocumented)=-
            Data   Bytes;
        };

        // -=(Undocumented)=-
        struct Glyph
        {
            // -=(Undocumented)=-
            Real32 Advance;

            // -=(Undocumented)=-
            Rectf  PlaneBounds;

            // -=(Undocumented)=-
            Rectf  ImageBounds;
        };

        // -=(Undocumented)=-
        struct Metrics
        {
            // -=(Undocumented)=-
            Real32 Size;

            // -=(Undocumented)=-
            Real32 Distance;

            // -=(Undocumented)=-
            Real32 Ascender;

            // -=(Undocumented)=-
            Real32 Descender;

            // -=(Undocumented)=-
            Real32 UnderlineY;

            // -=(Undocumented)=-
            Real32 UnderlineHeight;

            // -=(Undocumented)=-
            Real32 UnderlineThickness;
        };

    public:

        // -=(Undocumented)=-
        explicit Font(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Any<Metrics> Metrics, Any<Atlas> Atlas, Any<Table<UInt32, Glyph>> Glyphs, Any<Table<UInt64, Real32>> Kerning);

        // -=(Undocumented)=-
        Rectf Calculate(CStr16 Text, Real32 Size, Pivot Pivot) const;

        // -=(Undocumented)=-
        Vector2f Measure(CStr16 Text, Real32 Size) const;

        // -=(Undocumented)=-
        ConstRef<Metrics> GetMetrics() const
        {
            return mMetrics;
        }

        // -=(Undocumented)=-
        ConstPtr<Glyph> GetGlyph(UInt32 Unicode, UInt32 Fallback = '?') const
        {
            if (const auto Iterator = mGlyphs.find(Unicode); Iterator != mGlyphs.end())
            {
                return AddressOf(Iterator->second);
            }
            return Fallback != 0 && Unicode != Fallback ? GetGlyph(Fallback, 0) : nullptr;
        }

        // -=(Undocumented)=-
        Real32 GetKerning(UInt32 First, UInt32 Second) const
        {
            const auto Iterator = mKerning.find(static_cast<UInt64>(First) << 32 | Second);
            return Iterator == mKerning.end() ? 0.0f : Iterator->second;
        }

        // -=(Undocumented)=-
        ConstSPtr<Material> GetMaterial() const
        {
            return mMaterial;
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Metrics               mMetrics;
        Table<UInt32, Glyph>  mGlyphs;
        Table<UInt64, Real32> mKerning;
        Atlas                 mAtlas;       // TODO: Support multiple page atlas
        SPtr<Material>        mMaterial;
    };
}