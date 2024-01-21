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

#include "Core/COM_Common.hpp"
#include <Graphic/Common.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [export, uuid("40585AA2-0027-41A8-9D94-963E499D255F"), v1_enum]
    typedef enum Graphic_Clear
    {
        eClearColor,
        eClearAuxiliary,
        eClearAll,
    } Graphic_Clear;

    // -=(Undocumented)=-
    [export, uuid("B373A5E4-3BB0-11EE-ADD4-1418C3A8EDB8"), v1_enum]
    typedef enum Graphic_Texture_Edge
    {
        eTextureEdgeClamp,
        eTextureEdgeRepeat,
        eTextureEdgeMirror,
    } Graphic_Texture_Edge;

    // -=(Undocumented)=-
    [export, uuid("B796E764-3BB0-11EE-ADD4-1418C3A8EDB8"), v1_enum]
    typedef enum Graphic_Texture_Filter
    {
        eTextureFilterNearest,
        eTextureFilterBilinear,
        eTextureFilterTrilinear,
    } Graphic_Texture_Filter;

    // -=(Undocumented)=-
    [export, uuid("89B21548-137D-11EE-ADCB-1418C3A8EDB8"), v1_enum]
    typedef enum Graphic_Texture_Format
    {
        eTextureFormatBC1UIntNorm,
        eTextureFormatBC1UIntNorm_sRGB,
        eTextureFormatBC2UIntNorm,
        eTextureFormatBC2UIntNorm_sRGB,
        eTextureFormatBC3UIntNorm,
        eTextureFormatBC3UIntNorm_sRGB,
        eTextureFormatBC4UIntNorm,
        eTextureFormatBC5UIntNorm,
        eTextureFormatBC7UIntNorm,
        eTextureFormatBC7UIntNorm_sRGB,
        eTextureFormatR8SInt,
        eTextureFormatR8SIntNorm,
        eTextureFormatR8UInt,
        eTextureFormatR8UIntNorm,
        eTextureFormatR16SInt,
        eTextureFormatR16SIntNorm,
        eTextureFormatR16UInt,
        eTextureFormatR16UIntNorm,
        eTextureFormatR16Float,
        eTextureFormatR32SInt,
        eTextureFormatR32UInt,
        eTextureFormatR32Float,
        eTextureFormatRG8SInt,
        eTextureFormatRG8SIntNorm,
        eTextureFormatRG8UInt,
        eTextureFormatRG8UIntNorm,
        eTextureFormatRG16SInt,
        eTextureFormatRG16SIntNorm,
        eTextureFormatRG16UInt,
        eTextureFormatRG16UIntNorm,
        eTextureFormatRG16Float,
        eTextureFormatRG32SInt,
        eTextureFormatRG32UInt,
        eTextureFormatRG32Float,
        eTextureFormatRGB32SInt,
        eTextureFormatRGB32UInt,
        eTextureFormatRGB32Float,
        eTextureFormatRGBA8SInt,
        eTextureFormatRGBA8SIntNorm,
        eTextureFormatRGBA8UInt,
        eTextureFormatRGBA8UIntNorm,
        eTextureFormatRGBA8UIntNorm_sRGB,
        eTextureFormatRGBA16SInt,
        eTextureFormatRGBA16SIntNorm,
        eTextureFormatRGBA16UInt,
        eTextureFormatRGBA16UIntNorm,
        eTextureFormatRGBA16Float,
        eTextureFormatRGBA32SInt,
        eTextureFormatRGBA32UInt,
        eTextureFormatRGBA32Float,
        eTextureFormatD16Float,
        eTextureFormatD24Float,
        eTextureFormatD32Float,
        eTextureFormatD16X0UIntNorm,
        eTextureFormatD24X0UIntNorm,
        eTextureFormatD24S8UIntNorm,
        eTextureFormatD32X0UIntNorm,
    } Graphic_Texture_Format;

    // -=(Undocumented)=-
    [export, uuid("8E857E92-137D-11EE-ADCB-1418C3A8EDB8"), v1_enum]
    typedef enum Graphic_Texture_Layout
    {
        eTextureLayoutSource,
        eTextureLayoutDestination,
        eTextureLayoutDual,
    } Graphic_Texture_Layout;
}