// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Content/Loader.hpp"
#include "Aurora.Renderer/Font.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    class ArteryFontLoader final : public AbstractLoader<ArteryFontLoader, Graphic::Font>
    {
    public:

        // \see Loader::GetExtensions
        List<CStr> GetExtensions() const override
        {
            static List<CStr> EXTENSION_LIST = { "arfont" };
            return EXTENSION_LIST;
        }

        // \see AbstractLoader::Load
        Bool OnLoad(Ref<class Service> Service, Data&& File, Ref<Graphic::Font> Asset);

    private:

        // -=(Undocumented)=-
        struct ArteryFontHeader
        {
            Char   Tag[0x10];
            UInt32 Magic;
            UInt32 Version;
            UInt32 Flags;
            UInt32 RealType;
            UInt32 Reserved[4];
            UInt32 MetadataFormat;
            UInt32 MetadataLength;
            UInt32 VariantCount;
            UInt32 VariantsLength;
            UInt32 ImageCount;
            UInt32 ImagesLength;
            UInt32 AppendixCount;
            UInt32 AppendicesLength;
            UInt32 Reserved2[8];
        };

        // -=(Undocumented)=-
        struct ArteryFontFooter
        {
            UInt32 Salt;
            UInt32 Magic;
            UInt32 Reserved[4];
            UInt32 Length;
            UInt32 Checksum;
        };

        // -=(Undocumented)=-
        struct ArteryFontVariantHeader
        {
            UInt32 Flags;
            UInt32 Weight;
            UInt32 CodepointType;
            UInt32 ImageType;
            UInt32 FallbackVariant;
            UInt32 FallbackGlyph;
            UInt32 Reserved[6];
            Real32 Metrics[32];
            UInt32 NameLength;
            UInt32 MetadataLength;
            UInt32 GlyphCount;
            UInt32 KernPairCount;
        };

        // -=(Undocumented)=-
        struct ArteryImageHeader
        {
            UInt32 Flags;
            UInt32 Encoding;
            UInt32 Width;
            UInt32 Height;
            UInt32 Channels;
            UInt32 PixelFormat;
            UInt32 ImageType;
            UInt32 RowLength;
            SInt32 Orientation;
            UInt32 ChildImages;
            UInt32 TextureFlags;
            UInt32 Reserved[3];
            UInt32 MetadataLength;
            UInt32 DataLength;
        };

        // -=(Undocumented)=-
        struct ArteryGlyph
        {
            UInt32 Codepoint;
            UInt32 Image;

            struct {
                Real32 Left, Bottom, Right, Top;
            } PlaneBounds, ImageBounds;

            struct {
                Real32 Horizontal, Vertical;
            } Advance;
        };

        // -=(Undocumented)=-
        struct ArteryKernPair
        {
            UInt32  Codepoint1, Codepoint2;

            struct {
                Real32 Horizontal, Vertical;
            } Advance;
        };
    };
}