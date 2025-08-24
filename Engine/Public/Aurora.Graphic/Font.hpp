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
#include "Aurora.Math/Geometry/Rect.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Represents a font resource used for text rendering.
    class Font final : public Content::AbstractResource<Font>
    {
        friend class AbstractResource;

    public:

        /// \brief Describes an individual glyph in the font.
        struct Glyph final
        {
            /// Bounds of the glyph in font units (relative to baseline).
            Rect   LocalBounds;

            /// Bounds of the glyph in texture coordinates (within the font atlas).
            Rect   AtlasBounds;

            /// Advance width (pen movement in X after drawing this glyph).
            Real32 Advance;
        };

        /// \brief Metrics describing the font typeface.
        struct Metrics final
        {
            /// Nominal font size used when generating glyphs.
            Real32 Size;

            /// Signed distance field spread value.
            Real32 Distance;

            /// Distance from baseline to top of typical ascenders.
            Real32 Ascender;

            /// Distance from baseline to bottom of typical descenders.
            Real32 Descender;

            /// Vertical offset from baseline to top of underline.
            Real32 UnderlineOffset;

            /// Total vertical space occupied by the underline.
            Real32 UnderlineSize;

            /// Visual thickness of the underline stroke.
            Real32 UnderlineThickness;
        };

    public:

        /// \brief Constructs a font resource with the given content key.
        ///
        /// \param Key The URI key identifying this font resource.
        explicit Font(AnyRef<Content::Uri> Key);

        /// \brief Loads the font data.
        ///
        /// \param Metrics  Information about the font’s sizing, spacing, and alignment.
        /// \param Registry Table of glyphs indexed by UTF-32 codepoints.
        /// \param Kerning  Table of kerning adjustments indexed by character pairs.
        /// \param Material The material resource used for rendering the font.
        void Load(AnyRef<Metrics> Metrics, AnyRef<Table<UInt32, Glyph>> Registry, AnyRef<Table<UInt64, Real32>> Kerning, ConstTracker<Material> Material);

        /// \brief Gets the metrics that describe the overall properties of the font.
        ///
        /// \return The metrics that describe this font.
        AURORA_INLINE ConstRef<Metrics> GetMetrics() const
        {
            return mMetrics;
        }

        /// \brief Retrieves the glyph data for a specific codepoint.
        ///
        /// \param Codepoint The UTF-32 codepoint to look up.
        /// \param Fallback  The fallback codepoint to use if the primary glyph is missing.
        /// \return Pointer to the glyph if found, nullptr if neither the requested glyph nor fallback glyph exists.
        AURORA_INLINE ConstPtr<Glyph> GetGlyph(UInt32 Codepoint, UInt32 Fallback = '?') const
        {
            if (const auto Iterator = mRegistry.find(Codepoint); Iterator != mRegistry.end())
            {
                return &Iterator->second;
            }
            return Fallback != 0 && Codepoint != Fallback ? GetGlyph(Fallback, 0) : nullptr;
        }

        /// \brief Retrieves the kerning adjustment between two characters.
        ///
        /// \param First  The character on the left side of the pair.
        /// \param Second The character on the right side of the pair.
        /// \return The recommended spacing adjustment in font units.
        AURORA_INLINE Real32 GetKerning(UInt32 First, UInt32 Second) const
        {
            const UInt64 Key = static_cast<UInt64>(First) << 32 | Second;

            if (const auto Iterator = mKerning.find(Key); Iterator != mKerning.end())
            {
                return Iterator->second;
            }
            return 0.0f;
        }

        /// \brief Gets the material resource used for rendering the font.
        ///
        /// \return The material of the font.
        AURORA_INLINE ConstTracker<Material> GetMaterial() const
        {
            return mMaterial;
        }

        /// \brief Computes the dimensions of text at the specified font size.
        ///
        /// \param Word    The text string encoded in UTF-8.
        /// \param Size    The size at which the font is evaluated.
        /// \param Spacing Additional spacing applied between characters.
        /// \return The dimensions of the text in pixels.
        Vector2 Measure(ConstText Word, Real32 Size, Real32 Spacing = 0.0f) const;

        /// \brief Calculates the bounding rectangle for a text string at the specified font size.
        ///
        /// \param Word      The text string encoded in UTF-8.
        /// \param Size      The size at which the font is evaluated.
        /// \param Alignment The pivot point used to align the rectangle.
        /// \param Spacing   Additional spacing applied between characters, in pixels.
        /// \return The bounding rectangle of the text in pixels, positioned according to the alignment.
        Rect Layout(ConstText Word, Real32 Size, Pivot Alignment, Real32 Spacing = 0.0f) const;

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Metrics               mMetrics;
        Table<UInt32, Glyph>  mRegistry;
        Table<UInt64, Real32> mKerning;
        Tracker<Material>     mMaterial;
    };
}