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

#include "Loader.hpp"
#include "Aurora.Content/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool ArteryFontLoader::Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data)
    {
        UInt32 CurrentOffset = 0;

#define ARTERY_FONT_DECODE_ALIGN()                                                                    \
        if (CurrentOffset & 0x03u) CurrentOffset += (0x04u - (CurrentOffset & 0x03u));

#define ARTERY_FONT_DECODE_READ(Target, Size)                                                         \
        Target = * reinterpret_cast<Ptr<decltype(Target)>>(Data.GetSpan<UInt8>(CurrentOffset).data());  \
        CurrentOffset += Size;

#define ARTERY_FONT_DECODE_READ_PTR(Target, Size)                                                     \
        Target = reinterpret_cast<decltype(Target)>(Data.GetSpan<UInt8>(CurrentOffset).data());         \
        CurrentOffset += Size;

#define ARTERY_FONT_DECODE_READ_STRING(Str, Size)                  \
        if (Size > 0)                                                \
        {                                                            \
            Ptr<Char> Characters;                                    \
            ARTERY_FONT_DECODE_READ_PTR(Characters, Size);         \
            ARTERY_FONT_DECODE_ALIGN();                            \
        }

        // Font Data
        Graphic::Font::Metrics              FontMetrics;
        Table<UInt32, Graphic::Font::Glyph> FontGlyphs;
        Table<UInt64, Real32>               FontKerning;

        // Read header
        ArteryFontHeader FontHeader;
        ARTERY_FONT_DECODE_READ(FontHeader, sizeof(ArteryFontHeader));

        if (memcmp(FontHeader.Tag, "ARTERY/FONT\0\0\0\0\0", sizeof(FontHeader.Tag)))
        {
            LOG_WARNING("Artery Font: Invalid header detected.");
            return false;
        }
        if (FontHeader.Magic != 0x4D276A5C || FontHeader.RealType != 0x14)
        {
            LOG_WARNING("Artery Font: Non 32-bit float font.");
            return false;
        }

        ARTERY_FONT_DECODE_READ_STRING(nullptr /* Metadata */, FontHeader.MetadataLength);

        if (FontHeader.VariantCount > 1) // TODO: Support more than 1?
        {
            LOG_WARNING("Artery Font: Invalid font with more than 1 font-face.");
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

                FontGlyphs.try_emplace(Glyph.Codepoint, Graphic::Font::Glyph {
                    Rect(Glyph.PlaneBounds.Left, Glyph.PlaneBounds.Top, Glyph.PlaneBounds.Right, Glyph.PlaneBounds.Bottom),
                    Rect(Glyph.ImageBounds.Left, Glyph.ImageBounds.Top, Glyph.ImageBounds.Right, Glyph.ImageBounds.Bottom),
                    Glyph.Advance.Horizontal
                });
            }

            Ptr<ArteryKernPair> KernPairs;
            ARTERY_FONT_DECODE_READ_PTR(KernPairs, sizeof(ArteryKernPair) * VariantHeader.KernPairCount);

            FontKerning.reserve(VariantHeader.KernPairCount);
            for (SInt Element = 0; Element < VariantHeader.KernPairCount; ++Element)
            {
                Ref<ArteryKernPair> Pair = KernPairs[Element];

                const UInt64 Key = static_cast<UInt64>(Pair.Codepoint1) << 32 | Pair.Codepoint2;
                FontKerning.try_emplace(Key, Pair.Advance.Horizontal);
            }

            FontMetrics.Size               = VariantHeader.Metrics[0];
            FontMetrics.Distance           = VariantHeader.Metrics[1];
            FontMetrics.Ascender           = VariantHeader.Metrics[3];
            FontMetrics.Descender          = VariantHeader.Metrics[4];
            FontMetrics.UnderlineOffset    = VariantHeader.Metrics[6];
            FontMetrics.UnderlineSize      = VariantHeader.Metrics[5];
            FontMetrics.UnderlineThickness = VariantHeader.Metrics[7];
        }

        // Read images
        Blob   FontAtlasBytes;
        UInt32 FontAtlasWidth;
        UInt32 FontAtlasHeight;

        for (SInt Index = 0; Index < FontHeader.ImageCount; ++Index)
        {
            ArteryImageHeader ImageHeader;
            ARTERY_FONT_DECODE_READ(ImageHeader, sizeof(ArteryImageHeader));
            ARTERY_FONT_DECODE_READ_STRING(nullptr /* ImageMetadata */, ImageHeader.MetadataLength);

            Ptr<UInt8> ImageData;
            ARTERY_FONT_DECODE_READ_PTR(ImageData, ImageHeader.DataLength);
            ARTERY_FONT_DECODE_ALIGN();

            if (ImageHeader.Encoding != 0x01)
            {
                LOG_WARNING("Artery Font: Invalid font image.");
                return false; // Only support binary mode & top down orientation
            }

            FontAtlasBytes = Blob(ImageHeader.DataLength);
            FontAtlasBytes.Copy(ImageData, ImageHeader.DataLength);

            FontAtlasWidth  = ImageHeader.Width;
            FontAtlasHeight = ImageHeader.Height;
        }

        // Creates the texture atlas for the font.
        const Tracker<Graphic::Texture> Atlas = Tracker<Graphic::Texture>::Create(
            Content::Uri::Merge(Scope.GetResource()->GetKey(), "Atlas"));
        Atlas->Load(Graphic::TextureFormat::RGBA8UIntNorm, FontAtlasWidth, FontAtlasHeight, 1, Move(FontAtlasBytes));
        Atlas->SetPolicy(Content::Resource::Policy::Exclusive);

        // Creates the material for the font.
        constexpr Graphic::Sampler kDefaultSample(
            Graphic::TextureEdge::Clamp,
            Graphic::TextureEdge::Clamp,
            Graphic::TextureFilter::MinMagLinearMipPoint);

        Tracker<Graphic::Material> Material = Tracker<Graphic::Material>::Create(
            Content::Uri::Merge(Scope.GetResource()->GetKey(), "Material"));
        Material->SetPolicy(Content::Resource::Policy::Exclusive);
        Material->SetKind(Graphic::Material::Kind::Alpha);
        Material->SetTexture(Graphic::TextureSemantic::Diffuse, Atlas);
        Material->SetSampler(Graphic::TextureSemantic::Diffuse, kDefaultSample);
        Material->SetParameter(0, Vector2(FontMetrics.Distance / FontAtlasWidth, FontMetrics.Distance / FontAtlasHeight));

        // Normalize Glyphs coordinates to use Y-TopLeft instead of Y-BottomLeft
        const Vector2 AtlasCoordNormalized(1.0f / FontAtlasWidth, 1.0f / FontAtlasHeight);

        for (auto & Iterator : FontGlyphs)
        {
            Ref<Graphic::Font::Glyph> Element = Iterator.second;

            const Real32 MinY = FontMetrics.Ascender - Element.LocalBounds.GetTop();
            const Real32 MaxY = MinY + Element.LocalBounds.GetTop() - Element.LocalBounds.GetBottom();

            Element.AtlasBounds *= AtlasCoordNormalized;
            Element.LocalBounds.SetTop(MinY);
            Element.LocalBounds.SetBottom(MaxY);
        }

        // Read appendices?
        // Read footer?

        const Tracker<Graphic::Font> Asset = Tracker<Graphic::Font>::Cast(Scope.GetResource());
        Asset->Load(Move(FontMetrics), Move(FontGlyphs), Move(FontKerning), Material);
        return true;
    }
}