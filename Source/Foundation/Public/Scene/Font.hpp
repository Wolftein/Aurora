// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Graphic/Material.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class Font final : public Content::AbstractResource<Hash("Font")>
    {
    public:

        // -=(Undocumented)=-
        struct Atlas
        {
            // -=(Undocumented)=-
            Chunk Data;

            // -=(Undocumented)=-
            UInt32 Width;

            // -=(Undocumented)=-
            UInt32 Height;
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
        Font(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Ref<Metrics> Metrics, Ref<Atlas> Image, Ref<Table<UInt32, Glyph>> Glyphs, Ref<Table<UInt64, Real32>> Kerning);

        // -=(Undocumented)=-
        Ref<const Metrics> GetMetrics() const
        {
            return mMetrics;
        }

        // -=(Undocumented)=-
        Ptr<const Glyph> GetGlyph(UInt32 Unicode) const
        {
            const auto Iterator = mGlyphs.find(Unicode);
            return (Iterator == mGlyphs.end() ? nullptr : & Iterator->second);
        }

        // -=(Undocumented)=-
        Real32 GetKerning(UInt32 First, UInt32 Second) const
        {
            const auto Iterator = mKerning.find(static_cast<UInt64>(First) << 32 | Second);
            return (Iterator == mKerning.end() ? 0.0f : Iterator->second);
        }

        // -=(Undocumented)=-
        SPtr<const Graphic::Material> GetMaterial() const
        {
            return mMaterial;
        }

    public:

        // \see Resource::OnLoad(Ref<Subsystem::Context>)
        Bool OnLoad(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnUnload(Ref<Subsystem::Context>)
        void OnUnload(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Metrics                 mMetrics;
        Atlas                   mImage;
        Table<UInt32, Glyph>    mGlyphs;
        Table<UInt64, Real32>   mKerning;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Graphic::Material> mMaterial;
    };
}