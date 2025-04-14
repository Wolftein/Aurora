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

#include "Aurora.Math/Math.hpp"

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
        k_Alignment      = 0x0100,

        // -=(Undocumented)=-
        k_MaxAttachments = 0x0004,

        // -=(Undocumented)=-
        k_MaxAttributes  = 0x000C,

        // -=(Undocumented)=-
        k_MaxBuffers     = 0x1000,

        // -=(Undocumented)=-
        k_MaxFetches     = 0x0008,

        // -=(Undocumented)=-
        k_MaxMaterials   = 0x1000,

        // -=(Undocumented)=-
        k_MaxMipmap      = 0x0010,

        // -=(Undocumented)=-
        k_MaxPasses      = 0x0040,

        // -=(Undocumented)=-
        k_MaxPipelines   = 0x00C0,

        // -=(Undocumented)=-
        k_MaxSamples     = 0x0008,

        // -=(Undocumented)=-
        k_MaxSamplers    = 0x00FF,

        // -=(Undocumented)=-
        k_MaxSlots       = 0x0008,

        // -=(Undocumented)=-
        k_MaxSources     = 0x000C,

        // -=(Undocumented)=-
        k_MaxStages      = 0x0003,

        // -=(Undocumented)=-
        k_MaxTextures    = 0x1000,

        // -=(Undocumented)=-
        k_MaxUniforms    = 0x0004,
    };

    // -=(Undocumented)=-
    enum class Backend : UInt8
    {
        None,
        D3D11,
        GLES3,
    };

    // -=(Undocumented)=-
    enum class BlendColor : UInt8
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
    enum class BlendFactor : UInt8
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
    enum class BlendFunction : UInt8
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    // -=(Undocumented)=-
    enum class Clear : UInt8
    {
        Color,
        Auxiliary,
        All,
    };

    // -=(Undocumented)=-
    enum class Cull : UInt8
    {
        None,
        Back,
        Front,
    };

    // -=(Undocumented)=-
    enum class Language : UInt8
    {
        Version_1,
        Version_2,
        Version_3,
        Version_4,
        Version_5,
        Version_6,
    };

    // -=(Undocumented)=-
    enum class Stage : UInt8
    {
        Vertex,
        Fragment,
        Geometry,
    };

    // -=(Undocumented)=-
    enum class TestAction : UInt8
    {
        Keep,
        Replace,
        Zero,
        Decrement,
        DecrementSaturate,
        Invert,
        Increment,
        IncrementSaturate,
    };

    // -=(Undocumented)=-
    enum class TestCondition : UInt8
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
    enum class TextureEdge : UInt8
    {
        Clamp,
        Repeat,
        Mirror,
    };

    // -=(Undocumented)=-
    enum class TextureFilter : UInt8
    {
        Nearest,
        Bilinear,
        Trilinear,
        Anisotropic,
    };

    // -=(Undocumented)=-
    enum class TextureFormat : UInt8
    {
        BC1UIntNorm,
        BC1UIntNorm_sRGB,
        BC2UIntNorm,
        BC2UIntNorm_sRGB,
        BC3UIntNorm,
        BC3UIntNorm_sRGB,
        BC4UIntNorm,
        BC5UIntNorm,
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
        BGRA8UIntNorm,
        BGRA8UIntNorm_sRGB,
        RGBA16SInt,
        RGBA16SIntNorm,
        RGBA16UInt,
        RGBA16UIntNorm,
        RGBA16Float,
        RGBA32SInt,
        RGBA32UInt,
        RGBA32Float,
        D32Float,
        D16X0UIntNorm,
        D24S8UIntNorm,
        D32S8UIntNorm,
    };

    // -=(Undocumented)=-
    enum class TextureLayout : UInt8
    {
        Source,
        Destination,
        Dual,
    };

    // -=(Undocumented)=-
    enum class TextureSlot : UInt8
    {
        None,
        Diffuse,
        Normal,
        Roughness,
        Metallic,
        Specular,
        Emissive,
        Displacement,
        Occlusion,
        Height,
        Ambient,
        Detail,
        Translucency,
    };

    // -=(Undocumented)=-
    enum class Usage : UInt8
    {
        Vertex,
        Index,
        Uniform,
    };

    // -=(Undocumented)=-
    enum class VertexFormat : UInt8
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
    enum class VertexSemantic : UInt8
    {
        None,
        Position,
        Normal,
        Color,
        Tangent,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
        TexCoord4,
        TexCoord5,
        TexCoord6,
        TexCoord7,
        Custom0,
        Custom1,
        Custom2,
        Custom3,
        Custom4,
        Custom5,
        Custom6,
        Custom7,
    };

    // -=(Undocumented)=-
    enum class VertexTopology : UInt8
    {
        Point,
        Line,
        Triangle,
    };

    // -=(Undocumented)=-
    using  Object = UInt16;

    // -=(Undocumented)=-
    struct Resolution
    {
        // -=(Undocumented)=-
        UInt16 Width;

        // -=(Undocumented)=-
        UInt16 Height;

        // -=(Undocumented)=-
        UInt16 Frequency;
    };

    // -=(Undocumented)=-
    struct Adapter
    {
        // -=(Undocumented)=-
        SStr   Description;

        // -=(Undocumented)=-
        UInt32 DedicatedMemoryInMBs;

        // -=(Undocumented)=-
        UInt32 SystemMemoryInMBs;

        // -=(Undocumented)=-
        UInt32 SharedMemoryInMBs;

        // -=(Undocumented)=-
        Vector<Resolution> Displays;
    };

    // -=(Undocumented)=-
    struct Attachment
    {
        // -=(Undocumented)=-
        Object Texture = 0;

        // -=(Undocumented)=-
        UInt8  Level   = 0;
    };

    // -=(Undocumented)=-
    struct Attribute
    {
        // -=(Undocumented)=-
        VertexSemantic ID      = VertexSemantic::None;

        // -=(Undocumented)=-
        VertexFormat   Format  = VertexFormat::Float32x4;

        // -=(Undocumented)=-
        UInt16         Slot    = 0;

        // -=(Undocumented)=-
        UInt16         Offset  = 0;

        // -=(Undocumented)=-
        UInt16         Divisor = 0;
    };

    // -=(Undocumented)=-
    struct Binding
    {
        // -=(Undocumented)=-
        Object Buffer = 0;

        // -=(Undocumented)=-
        UInt16 Stride = 0;

        // -=(Undocumented)=-
        UInt32 Offset = 0;
    };

    // -=(Undocumented)=-
    struct Capabilities
    {
        // -=(Undocumented)=-
        Backend  Backend  = Backend::None;

        // -=(Undocumented)=-
        Language Language = Language::Version_1;

        // -=(Undocumented)=-
        Bool     Adaptive = false;

        // -=(Undocumented)=-
        UInt8    Samples  = 1;

        // -=(Undocumented)=-
        Vector<Adapter> Adapters;
    };

    // -=(Undocumented)=-
    struct Descriptor
    {
        // -=(Undocumented)=-
        Cull           Cull                = Cull::Back;

        // -=(Undocumented)=-
        Bool           Fill                = true;

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
        UInt8          StencilMask         = 0b11111111;

        // -=(Undocumented)=-
        TestCondition  StencilCondition    = TestCondition::Always;

        // -=(Undocumented)=-
        TestAction     StencilOnFail       = TestAction::Keep;

        // -=(Undocumented)=-
        TestAction     StencilOnDepthFail  = TestAction::Keep;

        // -=(Undocumented)=-
        TestAction     StencilOnDepthPass  = TestAction::Keep;

        // -=(Undocumented)=-
        Attribute      InputLayout[k_MaxAttributes];

        // -=(Undocumented)=-
        VertexTopology InputTopology       = VertexTopology::Triangle;
    };

    // -=(Undocumented)=-
    struct Sampler
    {
        // -=(Undocumented)=-
        TextureEdge   EdgeU  = TextureEdge::Clamp;

        // -=(Undocumented)=-
        TextureEdge   EdgeV  = TextureEdge::Clamp;

        // -=(Undocumented)=-
        TextureFilter Filter = TextureFilter::Nearest;
    };

    // -=(Undocumented)=-
    struct Primitive
    {
        // -=(Undocumented)=-
        UInt32 Count     = 0;

        // -=(Undocumented)=-
        SInt32 Base      = 0;

        // -=(Undocumented)=-
        UInt32 Offset    = 0;

        // -=(Undocumented)=-
        UInt32 Instances = 0;
    };

    // -=(Undocumented)=-
    struct Submission
    {
        // -=(Undocumented)=-
        Rect<UInt16>                  Scissor   = Rect<UInt16>(0, 0, UINT16_MAX, UINT16_MAX);

        // -=(Undocumented)=-
        UInt8                         Stencil   = 0;

        // -=(Undocumented)=-
        Object                        Pipeline  = 0;

        // -=(Undocumented)=-
        Array<Binding, k_MaxFetches>  Vertices;

        // -=(Undocumented)=-
        Binding                       Indices;

        // -=(Undocumented)=-
        Array<Binding, k_MaxUniforms> Uniforms;

        // -=(Undocumented)=-
        Array<Sampler, k_MaxSlots>    Samplers;

        // -=(Undocumented)=-
        Array<Object, k_MaxSlots>     Textures  = { 0 };

        // -=(Undocumented)=-
        Primitive                     Primitive;
    };
}