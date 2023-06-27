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

#include "Loader.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool ArteryFontLoader::Load(Ref<Chunk> Data, Ref<const SPtr<Scene::Font>> Asset)
    {
        UInt32 CurrentOffset = 0;

#define ARTERY_FONT_DECODE_ALIGN()                                                                      \
        if (CurrentOffset & 0x03u) CurrentOffset += (0x04u - (CurrentOffset & 0x03u));

#define ARTERY_FONT_DECODE_READ(Target, Size)                                                           \
        Target = * reinterpret_cast<Ptr<decltype(Target)>>(Data.GetSpan<UInt08>(CurrentOffset).data()); \
        CurrentOffset += Size;

#define ARTERY_FONT_DECODE_READ_PTR(Target, Size)                                                       \
        Target = reinterpret_cast<decltype(Target)>(Data.GetSpan<UInt08>(CurrentOffset).data());        \
        CurrentOffset += Size;

#define ARTERY_FONT_DECODE_READ_STRING(Str, Size)                  \
        if (Size > 0)                                              \
        {                                                          \
            Ptr<Char> Characters;                                  \
            ARTERY_FONT_DECODE_READ_PTR(Characters, Size);         \
            ARTERY_FONT_DECODE_ALIGN();                            \
        }

        // Font Data
        Scene::Font::Metrics              FontMetrics;
        Scene::Font::Atlas                FontAtlas;
        Table<UInt32, Scene::Font::Glyph> FontGlyphs;
        Table<UInt64, Real32>             FontKerning;

        // Read header
        ArteryFontHeader FontHeader;
        ARTERY_FONT_DECODE_READ(FontHeader, sizeof(ArteryFontHeader));

        if (memcmp(FontHeader.Tag, "ARTERY/FONT\0\0\0\0\0", sizeof(FontHeader.Tag)))
        {
            return false;
        }
        if (FontHeader.Magic != 0x4D276A5C || FontHeader.RealType != 0x14)
        {
            return false; // Only 32-bit float is supported
        }

        ARTERY_FONT_DECODE_READ_STRING(nullptr /* Metadata */, FontHeader.MetadataLength);

        if (FontHeader.VariantCount > 1) // TODO: Support more than 1?
        {
            return false;
        }

        // Read variants
        for (SInt Index = 0; Index < FontHeader.VariantCount; ++Index)
        {
            ArteryFontVariantHeader VariantHeader;
            ARTERY_FONT_DECODE_READ(VariantHeader, sizeof(ArteryFontVariantHeader));
            ARTERY_FONT_DECODE_READ_STRING(nullptr /* VariantName */, VariantHeader.NameLength);
            ARTERY_FONT_DECODE_READ_STRING(nullptr /* VariantMetadata */, VariantHeader.MetadataLength);

            Ptr<ArteryGlyph>    Glyphs;
            ARTERY_FONT_DECODE_READ_PTR(Glyphs, sizeof(ArteryGlyph) * VariantHeader.GlyphCount);

            FontGlyphs.reserve(VariantHeader.GlyphCount);
            for (SInt Element = 0; Element < VariantHeader.GlyphCount; ++Element)
            {
                Ref<ArteryGlyph> Glyph = Glyphs[Element];

                FontGlyphs.try_emplace(Glyph.Codepoint, Scene::Font::Glyph {
                    Glyph.Advance.Horizontal,
                    Rectf(Glyph.PlaneBounds.Left, Glyph.PlaneBounds.Top, Glyph.PlaneBounds.Right, Glyph.PlaneBounds.Bottom),
                    Rectf(Glyph.ImageBounds.Left, Glyph.ImageBounds.Top, Glyph.ImageBounds.Right, Glyph.ImageBounds.Bottom)
                });
            }

            Ptr<ArteryKernPair> KernPairs;
            ARTERY_FONT_DECODE_READ_PTR(KernPairs, sizeof(ArteryKernPair) * VariantHeader.KernPairCount);

            FontKerning.reserve(VariantHeader.KernPairCount);
            for (SInt Element = 0; Element < VariantHeader.KernPairCount; ++Element)
            {
                Ref<ArteryKernPair> KernPair = KernPairs[Element];

                FontKerning.try_emplace(
                    static_cast<UInt64>(KernPair.Codepoint1) << 32 | KernPair.Codepoint2, KernPair.Advance.Horizontal);
            }

            FontMetrics.Ascender           = VariantHeader.Metrics[3];
            FontMetrics.Descender          = VariantHeader.Metrics[4];
            FontMetrics.UnderlineHeight    = VariantHeader.Metrics[5];
            FontMetrics.UnderlineY         = VariantHeader.Metrics[6];
            FontMetrics.UnderlineThickness = VariantHeader.Metrics[7];
        }

        // Read images
        for (SInt Index = 0; Index < FontHeader.ImageCount; ++Index)
        {
            ArteryImageHeader ImageHeader;
            ARTERY_FONT_DECODE_READ(ImageHeader, sizeof(ArteryImageHeader));
            ARTERY_FONT_DECODE_READ_STRING(nullptr /* ImageMetadata */, ImageHeader.MetadataLength);

            Ptr<UInt08> ImageData;
            ARTERY_FONT_DECODE_READ_PTR(ImageData, ImageHeader.DataLength);
            ARTERY_FONT_DECODE_ALIGN();

            if (ImageHeader.Encoding != 0x01)
            {
                return false; // Only support binary mode & top down orientation
            }

            FontAtlas.Width  = ImageHeader.Width;
            FontAtlas.Height = ImageHeader.Height;

            FontAtlas.Data   = Chunk(ImageHeader.DataLength);
            memcpy(FontAtlas.Data.GetData(), ImageData, ImageHeader.DataLength);
        }

        // Normalize Glyphs coordinates to use Y-TopLeft instead of Y-BottomLeft
        const Vector2f AtlasCoordNormalized(1.0f / FontAtlas.Width, 1.0f / FontAtlas.Height);

        for (auto & Iterator : FontGlyphs)
        {
            Ref<Scene::Font::Glyph> Element = Iterator.second;

            const Real32 MinY = FontMetrics.Ascender - Element.PlaneBounds.GetTop();
            const Real32 MaxY = MinY + Element.PlaneBounds.GetTop() - Element.PlaneBounds.GetBottom();

            Element.ImageBounds *= AtlasCoordNormalized;
            Element.PlaneBounds.SetTop(MinY);
            Element.PlaneBounds.SetBottom(MaxY);
        }

        // Read appendices?
        // Read footer?

        Asset->Load(FontMetrics, FontAtlas, FontGlyphs, FontKerning);

        return true;
    }
}