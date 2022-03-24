// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Core/Core.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    enum
    {
        // -=(Undocumented)=-
        k_Default        = 0,

        // -=(Undocumented)=-
        k_MaxAttachments = 0x0004,

        // -=(Undocumented)=-
        k_MaxAttributes  = 0x0008,

        // -=(Undocumented)=-
        k_MaxBuffers     = 0x0080,

        // -=(Undocumented)=-
        k_MaxMipmap      = 0x0010,

        // -=(Undocumented)=-
        k_MaxPasses      = 0x0040,

        // -=(Undocumented)=-
        k_MaxPipelines   = 0x00C0,

        // -=(Undocumented)=-
        k_MaxSamplers    = 0x0040,

        // -=(Undocumented)=-
        k_MaxSources     = 0x0004,

        // -=(Undocumented)=-
        k_MaxTextures    = 0x0800,

        // -=(Undocumented)=-
        k_MaxUniforms    = 0x0004,
    };

    // -=(Undocumented)=-
    enum class Backend
    {
        None,
        Direct3D11,
    };

    // -=(Undocumented)=-
    enum class BlendColor
    {
        RGBA = 0b00001111,
        RGB  = 0b00000111,
        RGA  = 0b00001011,
        RBA  = 0b00001101,
        RG   = 0b00000011,
        RB   = 0b00000101,
        RA   = 0b00001001,
        R    = 0b00000001,
        GBA  = 0b00001110,
        GB   = 0b00000110,
        GA   = 0b00001010,
        G    = 0b00000010,
        BA   = 0b00001100,
        B    = 0b00000100,
        A    = 0b00001000,
        None = 0b00000000,
    };

    // -=(Undocumented)=-
    enum class BlendFactor
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstColor,
        OneMinusDstColor,
        DstAlpha,
        OneMinusDstAlpha,
    };

    // -=(Undocumented)=-
    enum class BlendFunction
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    // -=(Undocumented)=-
    enum class Clear
    {
        Color,
        Auxiliary,
        All,
    };

    // -=(Undocumented)=-
    enum class TestAction
    {
        Keep,
        Replace,
        Zero,
        Decrement,
        DecrementWrap,
        Invert,
        Increment,
        IncrementWrap,
    };

    // -=(Undocumented)=-
    enum class TestCondition
    {
        Always,
        Never,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,
        Less,
        LessEqual,
    };

    // -=(Undocumented)=-
    enum class TextureEdge
    {
        Clamp,
        Repeat,
        Mirror,
    };

    // -=(Undocumented)=-
    enum class TextureFilter
    {
        Nearest,
        Bilinear,
        Trilinear,
    };

    // -=(Undocumented)=-
    enum class TextureFormat
    {
        BC1UIntNorm,
        BC1UIntNorm_sRGB,
        BC2UIntNorm,
        BC2UIntNorm_sRGB,
        BC3UIntNorm,
        BC3UIntNorm_sRGB,
        BC4UIntNorm,
        BC5UIntNorm,
        BC7UIntNorm,
        BC7UIntNorm_sRGB,
        R8SInt,
        R8SIntNorm,
        R8UInt,
        R8UIntNorm,
        R16SInt,
        R16SIntNorm,
        R16UInt,
        R16UIntNorm,
        R16Float,
        R32SInt,
        R32UInt,
        R32Float,
        RG8SInt,
        RG8SIntNorm,
        RG8UInt,
        RG8UIntNorm,
        RG16SInt,
        RG16SIntNorm,
        RG16UInt,
        RG16UIntNorm,
        RG16Float,
        RG32SInt,
        RG32UInt,
        RG32Float,
        RGB32SInt,
        RGB32UInt,
        RGB32Float,
        RGBA8SInt,
        RGBA8SIntNorm,
        RGBA8UInt,
        RGBA8UIntNorm,
        RGBA8UIntNorm_sRGB,
        RGBA16SInt,
        RGBA16SIntNorm,
        RGBA16UInt,
        RGBA16UIntNorm,
        RGBA16Float,
        RGBA32SInt,
        RGBA32UInt,
        RGBA32Float,
        D16Float,
        D24Float,
        D32Float,
        D16X0UIntNorm,
        D24X0UIntNorm,
        D24S8UIntNorm,
        D32X0UIntNorm,
    };

    // -=(Undocumented)=-
    enum class TextureLayout
    {
        Source,
        Destination,
        Dual,
    };

    // -=(Undocumented)=-
    enum class VertexFormat
    {
        Float16x2,
        Float16x4,
        Float32x1,
        Float32x2,
        Float32x3,
        Float32x4,
        SInt8x4,
        SIntNorm8x4,
        UInt8x4,
        UIntNorm8x4,
        SInt16x2,
        SIntNorm16x2,
        UInt16x2,
        UIntNorm16x2,
        SInt16x4,
        SIntNorm16x4,
        UInt16x4,
        UIntNorm16x4,
    };

    // -=(Undocumented)=-
    enum class VertexTopology
    {
        Point,
        Line,
        Triangle,
    };

    // -=(Undocumented)=-
    struct Attribute
    {
        // -=(Undocumented)=-
        SInt32       Slot   = -1;

        // -=(Undocumented)=-
        SInt32       Offset = -1;

        // -=(Undocumented)=-
        VertexFormat Format = VertexFormat::Float32x4;
    };

    // -=(Undocumented)=-
    struct Binding
    {
        // -=(Undocumented)=-
        UInt Buffer = 0;

        // -=(Undocumented)=-
        UInt Length = 0;

        // -=(Undocumented)=-
        UInt Offset = 0;

        // -=(Undocumented)=-
        UInt Stride = 0;
    };

    // -=(Undocumented)=-
    struct Color
    {
        Real32 R = 0.0f, G = 0.0f, B = 0.0f, A = 1.0f;
    };

    // -=(Undocumented)=-
    struct Descriptor
    {
        // -=(Undocumented)=-
        BlendColor     BlendMask           = BlendColor::RGBA;

        // -=(Undocumented)=-
        BlendFactor    BlendColorSrcFactor = BlendFactor::One;

        // -=(Undocumented)=-
        BlendFactor    BlendColorDstFactor = BlendFactor::Zero;

        // -=(Undocumented)=-
        BlendFunction  BlendColorEquation  = BlendFunction::Add;

        // -=(Undocumented)=-
        BlendFactor    BlendAlphaSrcFactor = BlendFactor::One;

        // -=(Undocumented)=-
        BlendFactor    BlendAlphaDstFactor = BlendFactor::Zero;

        // -=(Undocumented)=-
        BlendFunction  BlendAlphaEquation  = BlendFunction::Add;

        // -=(Undocumented)=-
        Bool           DepthMask           = true;

        // -=(Undocumented)=-
        TestCondition  DepthCondition      = TestCondition::LessEqual;

        // -=(Undocumented)=-
        UInt08         StencilMask         = 0b11111111;

        // -=(Undocumented)=-
        TestCondition  StencilCondition    = TestCondition::Always;

        // -=(Undocumented)=-
        TestAction     StencilOnFail       = TestAction::Keep;

        // -=(Undocumented)=-
        TestAction     StencilOnZFail      = TestAction::Keep;

        // -=(Undocumented)=-
        TestAction     StencilOnZPass      = TestAction::Keep;

        // -=(Undocumented)=-
        Attribute      InputLayout[k_MaxAttributes];

        // -=(Undocumented)=-
        VertexTopology InputTopology       = VertexTopology::Triangle;
    };

    // -=(Undocumented)=-
    struct Rect
    {
        UInt16 Left = 0, Top = 0, Right = 0, Bottom = 0;
    };

    // -=(Undocumented)=-
    struct Submission
    {
        // -=(Undocumented)=-
        Binding Vertices;

        // -=(Undocumented)=-
        Binding Indices;

        // -=(Undocumented)=-
        Binding Uniforms[k_MaxUniforms];

        // -=(Undocumented)=-
        Rect    Scissor;

        // -=(Undocumented)=-
        UInt    Stencil                = 0;

        // -=(Undocumented)=-
        UInt    Pipeline               = 0;

        // -=(Undocumented)=-
        UInt    Samplers[k_MaxSources] = { 0 };

        // -=(Undocumented)=-
        UInt    Textures[k_MaxSources] = { 0 };
    };
}