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

#include "Image.hpp"
#include "Material.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Font final : public Content::AbstractResource<Hash("Font")>
    {
    public:

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
        Font(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Ref<Metrics> Metrics, Ref<Image> Atlas, Ref<Table<UInt32, Glyph>> Glyphs, Ref<Table<UInt64, Real32>> Kerning);

        // -=(Undocumented)=-
        Ref<const Metrics> GetMetrics() const
        {
            return mMetrics;
        }

        // -=(Undocumented)=-
        Ptr<const Glyph> GetGlyph(UInt32 Unicode, UInt32 Fallback = '?') const
        {
            const auto Iterator = mGlyphs.find(Unicode);

            if (Iterator != mGlyphs.end())
            {
                return & Iterator->second;
            }
            else
            {
                return (Fallback != 0 ? GetGlyph(Fallback) : nullptr);
            }
        }

        // -=(Undocumented)=-
        Real32 GetKerning(UInt32 First, UInt32 Second) const
        {
            const auto Iterator = mKerning.find(static_cast<UInt64>(First) << 32 | Second);
            return (Iterator == mKerning.end() ? 0.0f : Iterator->second);
        }

        // -=(Undocumented)=-
        SPtr<Graphic::Material> GetMaterial() const
        {
            return mMaterial;
        }

    protected:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDispose(Ref<Subsystem::Context>)
        void OnDispose(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Metrics               mMetrics;
        Table<UInt32, Glyph>  mGlyphs;
        Table<UInt64, Real32> mKerning;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Image                 mAtlas;       // TODO: Support multiple page atlas
        SPtr<Material>        mMaterial;
    };
}