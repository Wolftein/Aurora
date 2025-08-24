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

#include "Aurora.Base/Base.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Constants used across the graphic module.
    enum
    {
        /// Alignment requirement for GPU uniform buffer offsets.
        kBlockAlignment = 0x0100,

        /// Maximum block size (64 KB) used for uniform buffer pages.
        kBlockSize      = 0x10000,

        /// Identifier for the default display render target.
        kDisplay        = 0x0000,

        /// Maximum number of render target attachments.
        kMaxAttachments = 0x0004,

        /// Maximum number of vertex attributes per vertex layout.
        kMaxAttributes  = 0x000C,

        /// Maximum number of buffer objects supported.
        kMaxBuffers     = 0x1000,

        /// Maximum number of frames that can be processed concurrently.
        kMaxFrames      = 0x0002,

        /// Maximum number of vertex fetch bindings.
        kMaxFetches     = 0x0008,

        /// Maximum number of materials supported.
        kMaxMaterials   = 0x1000,

        /// Maximum number of mipmap levels supported for textures.
        kMaxMipmap      = 0x0010,

        /// Maximum number of render passes supported.
        kMaxPasses      = 0x0040,

        /// Maximum number of pipeline objects supported.
        kMaxPipelines   = 0x00C0,

        /// Maximum number of samplers objects supported.
        kMaxSamplers    = 0x00FF,

        /// Maximum number of multisample supported.
        kMaxSamples     = 0x0008,

        /// Maximum number of texture slots supported per pipeline.
        kMaxSlots       = 0x000C,

        /// Maximum number of shader stages supported per pipeline.
        kMaxStages      = 0x0002,

        /// Maximum number of texture objects supported.
        kMaxTextures    = 0x1000,

        /// Maximum number of uniform buffer fetches supported.
        kMaxUniforms    = 0x0004,
    };

    /// \brief Specifies the memory access type for a resource.
    enum class Access : UInt8
    {
        Device, ///< Accessible only by the device (GPU).
        Host,   ///< Accessible only by the host (CPU).
        Dual,   ///< Accessible by both device and host.
    };

    /// \brief Describes the available graphics backend.
    enum class Backend : UInt8
    {
        None,         ///< No backend selected.
        D3D11,        ///< Direct3D 11 backend.
    };

    /// \brief Specifies color channel masks used in blend operations.
    enum class BlendMask : UInt8
    {
        RGBA = 0b00001111, ///< Red, Green, Blue, Alpha.
        RGB  = 0b00000111, ///< Red, Green, Blue.
        RGA  = 0b00001011, ///< Red, Green, Alpha.
        RBA  = 0b00001101, ///< Red, Blue, Alpha.
        RG   = 0b00000011, ///< Red, Green.
        RB   = 0b00000101, ///< Red, Blue.
        RA   = 0b00001001, ///< Red, Alpha.
        R    = 0b00000001, ///< Red.
        GBA  = 0b00001110, ///< Green, Blue, Alpha.
        GB   = 0b00000110, ///< Green, Blue.
        GA   = 0b00001010, ///< Green, Alpha.
        G    = 0b00000010, ///< Green.
        BA   = 0b00001100, ///< Blue, Alpha.
        B    = 0b00000100, ///< Blue.
        A    = 0b00001000, ///< Alpha.
        None = 0b00000000, ///< No channels.
    };

    /// \brief Specifies how source and destination colors are factored in blending.
    enum class BlendFactor : UInt8
    {
        Zero,              ///< Use 0.0 as blend factor.
        One,               ///< Use 1.0 as blend factor.
        SrcColor,          ///< Use the source color.
        OneMinusSrcColor,  ///< Use 1.0 minus the source color.
        SrcAlpha,          ///< Use the source alpha.
        OneMinusSrcAlpha,  ///< Use 1.0 minus the source alpha.
        DstColor,          ///< Use the destination color.
        OneMinusDstColor,  ///< Use 1.0 minus the destination color.
        DstAlpha,          ///< Use the destination alpha.
        OneMinusDstAlpha,  ///< Use 1.0 minus the destination alpha.
    };

    /// \brief Specifies the blending function used to combine source and destination.
    enum class BlendFunction : UInt8
    {
        Add,             ///< Add source and destination.
        Subtract,        ///< Subtract destination from source.
        ReverseSubtract, ///< Subtract source from destination.
        Min,             ///< Use the minimum of source and destination.
        Max,             ///< Use the maximum of source and destination.
    };

    /// \brief Specifies which parts of a render pass should be cleared.
    enum class Clear : UInt8
    {
        Color,       ///< Clear only the color buffers.
        Auxiliary,   ///< Clear depth and/or stencil buffers.
        All,         ///< Clear color, depth, and stencil buffers.
    };

    /// \brief Specifies face culling behavior in rasterization.
    enum class Cull : UInt8
    {
        None,  ///< Do not cull faces.
        Back,  ///< Cull back-facing triangles.
        Front, ///< Cull front-facing triangles.
    };

    /// \brief Specifies the polygon fill mode used in rasterization.
    enum class Fill : UInt8
    {
        Solid,     ///< Render filled triangles.
        Wireframe, ///< Render triangles as wireframes.
    };

    /// \brief Defines the shader language version.
    enum class Language : UInt8
    {
        V1, ///< Shader Model 1.
        V2, ///< Shader Model 2.
        V3, ///< Shader Model 3.
        V4, ///< Shader Model 4.
        V5, ///< Shader Model 5.
        V6, ///< Shader Model 6.
    };

    /// \brief Defines how vertices are assembled into primitives.
    enum class Primitive : UInt8
    {
        LineList,       ///< Independent lines (2 vertices per line).
        LineStrip,      ///< Connected lines (1 vertex per segment after the first).
        TriangleList,   ///< Independent triangles (3 vertices per triangle).
        TriangleStrip,  ///< Connected triangles (1 triangle per vertex after the first two).
    };

    /// \brief Specifies the number of samples for multisampling.
    enum class Samples : UInt8
    {
        X1,  ///< Single sample (no multisampling).
        X2,  ///< 2x multisampling.
        X4,  ///< 4x multisampling.
        X8,  ///< 8x multisampling.
        X16, ///< 16x multisampling.
    };

    /// \brief Identifies the programmable shader stage.
    enum class Stage : UInt8
    {
        Vertex,   ///< Vertex shader stage.
        Fragment, ///< Fragment (pixel) shader stage.
    };

    /// \brief Defines the action to perform when a test (e.g., stencil) passes or fails.
    enum class TestAction : UInt8
    {
        Keep,              ///< Keep the current value.
        Replace,           ///< Replace with the reference value.
        Zero,              ///< Set to zero.
        Decrement,         ///< Decrement the value, wrapping if necessary.
        DecrementSaturate, ///< Decrement the value, but clamp to zero.
        Invert,            ///< Bitwise invert the current value.
        Increment,         ///< Increment the value, wrapping if necessary.
        IncrementSaturate, ///< Increment the value, but clamp to the maximum.
    };

    /// \brief Defines the comparison condition used in depth or stencil tests.
    enum class TestCondition : UInt8
    {
        Always,       ///< Always passes the test.
        Never,        ///< Never passes the test.
        Greater,      ///< Passes if source > destination.
        GreaterEqual, ///< Passes if source >= destination.
        Equal,        ///< Passes if source == destination.
        NotEqual,     ///< Passes if source != destination.
        Less,         ///< Passes if source < destination.
        LessEqual,    ///< Passes if source <= destination.
    };

    /// \brief Defines how texture coordinates outside the [0,1] range are handled.
    enum class TextureEdge : UInt8
    {
        Clamp,  ///< Clamp to edge; coordinates are clamped to the nearest valid texel.
        Repeat, ///< Repeat texture; coordinates wrap around.
        Mirror, ///< Mirror texture at every integer boundary.
    };

    /// \brief Specifies the sampling method used when accessing textures.
    enum class TextureFilter : UInt8
    {
        MinMagMipPoint,              ///< Point sampling for minification, magnification, and mipmapping.
        MinMagPointMipLinear,        ///< Point sampling for min/mag, linear interpolation between mip levels.
        MinPointMagLinearMipPoint,   ///< Point sampling for minification, linear for magnification, point for mipmaps.
        MinPointMagMipLinear,        ///< Point sampling for minification, linear for magnification, linear mip interpolation.
        MinLinearMagMipPoint,        ///< Linear minification, point magnification, point mipmaps.
        MinLinearMagPointMipLinear,  ///< Linear minification, point magnification, linear mip interpolation.
        MinMagLinearMipPoint,        ///< Linear minification and magnification, point mipmaps.
        MinMagMipLinear,             ///< Linear minification, magnification, and mipmapping.
        Anisotropic2x,               ///< Anisotropic filtering with max 2x samples.
        Anisotropic4x,               ///< Anisotropic filtering with max 4x samples.
        Anisotropic8x,               ///< Anisotropic filtering with max 8x samples.
        Anisotropic16x               ///< Anisotropic filtering with max 16x samples.
    };

    /// \brief Describes the format of texture data.
    enum class TextureFormat : UInt8
    {
        BC1UIntNorm,           ///< Block-compressed RGB (DXT1), unsigned normalized.
        BC1UIntNorm_sRGB,      ///< Block-compressed RGB (DXT1), sRGB gamma-corrected.
        BC2UIntNorm,           ///< Block-compressed RGBA (DXT3), unsigned normalized.
        BC2UIntNorm_sRGB,      ///< Block-compressed RGBA (DXT3), sRGB gamma-corrected.
        BC3UIntNorm,           ///< Block-compressed RGBA (DXT5), unsigned normalized.
        BC3UIntNorm_sRGB,      ///< Block-compressed RGBA (DXT5), sRGB gamma-corrected.
        BC4UIntNorm,           ///< Block-compressed single-channel (red), unsigned normalized.
        BC5UIntNorm,           ///< Block-compressed two-channel (red-green), unsigned normalized.
        R8SInt,                ///< 8-bit signed integer.
        R8SIntNorm,            ///< 8-bit signed normalized.
        R8UInt,                ///< 8-bit unsigned integer.
        R8UIntNorm,            ///< 8-bit unsigned normalized.
        R16SInt,               ///< 16-bit signed integer.
        R16SIntNorm,           ///< 16-bit signed normalized.
        R16UInt,               ///< 16-bit unsigned integer.
        R16UIntNorm,           ///< 16-bit unsigned normalized.
        R16Float,              ///< 16-bit floating-point.
        R32SInt,               ///< 32-bit signed integer.
        R32UInt,               ///< 32-bit unsigned integer.
        R32Float,              ///< 32-bit floating-point.
        RG8SInt,               ///< 2×8-bit signed integers.
        RG8SIntNorm,           ///< 2×8-bit signed normalized.
        RG8UInt,               ///< 2×8-bit unsigned integers.
        RG8UIntNorm,           ///< 2×8-bit unsigned normalized.
        RG16SInt,              ///< 2×16-bit signed integers.
        RG16SIntNorm,          ///< 2×16-bit signed normalized.
        RG16UInt,              ///< 2×16-bit unsigned integers.
        RG16UIntNorm,          ///< 2×16-bit unsigned normalized.
        RG16Float,             ///< 2×16-bit floating-point.
        RG32SInt,              ///< 2×32-bit signed integers.
        RG32UInt,              ///< 2×32-bit unsigned integers.
        RG32Float,             ///< 2×32-bit floating-point.
        RGB32SInt,             ///< 3×32-bit signed integers.
        RGB32UInt,             ///< 3×32-bit unsigned integers.
        RGB32Float,            ///< 3×32-bit floating-point.
        RGBA8SInt,             ///< 4×8-bit signed integers.
        RGBA8SIntNorm,         ///< 4×8-bit signed normalized.
        RGBA8UInt,             ///< 4×8-bit unsigned integers.
        RGBA8UIntNorm,         ///< 4×8-bit unsigned normalized.
        RGBA8UIntNorm_sRGB,    ///< 4×8-bit unsigned normalized with sRGB correction.
        BGRA8UIntNorm,         ///< 4×8-bit unsigned normalized in BGRA order.
        BGRA8UIntNorm_sRGB,    ///< 4×8-bit unsigned normalized in BGRA with sRGB.
        RGBA16SInt,            ///< 4×16-bit signed integers.
        RGBA16SIntNorm,        ///< 4×16-bit signed normalized.
        RGBA16UInt,            ///< 4×16-bit unsigned integers.
        RGBA16UIntNorm,        ///< 4×16-bit unsigned normalized.
        RGBA16F,               ///< 4×16-bit floating-point.
        RGBA32SInt,            ///< 4×32-bit signed integers.
        RGBA32UInt,            ///< 4×32-bit unsigned integers.
        RGBA32Float,           ///< 4×32-bit floating-point.
        D16Float,              ///< 16-bit floating-point depth.
        D32Float,              ///< 32-bit floating-point depth.
        D24S8UIntNorm,         ///< 24-bit floating-point depth + 8-bit stencil.
        D32S8UIntNorm,         ///< 32-bit floating-point depth + 8-bit stencil.
    };

    /// \brief Describes how a texture is used within a rendering or compute pass.
    enum class TextureLayout : UInt8
    {
        Source,   ///< Used as input (e.g., sampled in a shader).
        Target,   ///< Used as output (e.g., render or compute target).
        Both,     ///< Used as both input and output in the same pass.
    };

    /// \brief Describes logical texture semantic used in materials and pipelines.
    enum class TextureSemantic : UInt8
    {
        Diffuse,      ///< Base color or albedo map.
        Normal,       ///< Tangent-space normal map.
        Roughness,    ///< Surface roughness map.
        Metallic,     ///< Metalness map.
        Specular,     ///< Specular intensity map.
        Emissive,     ///< Self-illumination map.
        Displacement, ///< Displacement map.
        Occlusion,    ///< Ambient occlusion map.
        Height,       ///< Height map (alternative to displacement).
        Ambient,      ///< Indirect lighting texture (rare).
        Detail,       ///< Secondary detail texture (e.g., overlays, wear).
        Translucency, ///< Translucency or subsurface scattering mask.
        None,         ///< No semantic assigned.
    };

    /// \brief Specifies the intended purpose of a buffer.
    enum class Usage : UInt8
    {
        Vertex,  ///< Used as a vertex buffer.
        Index,   ///< Used as an index buffer.
        Uniform, ///< Used as an uniform buffer.
    };

    /// \brief Specifies the format of a vertex attribute.
    enum class VertexFormat : UInt8
    {
        Float16x2,       ///< Two 16-bit float point.
        Float16x4,       ///< Four 16-bit floating-point.
        Float32x1,       ///< One 32-bit floating-point.
        Float32x2,       ///< Two 32-bit floating-point.
        Float32x3,       ///< Three 32-bit floating-point.
        Float32x4,       ///< Four 32-bit floating-point.
        SInt8x4,         ///< Four 8-bit signed integers.
        SIntNorm8x4,     ///< Four normalized 8-bit signed integers.
        UInt8x4,         ///< Four 8-bit unsigned integers.
        UIntNorm8x4,     ///< Four normalized 8-bit unsigned integers.
        SInt16x2,        ///< Two 16-bit signed integers.
        SIntNorm16x2,    ///< Two normalized 16-bit signed integers.
        UInt16x2,        ///< Two 16-bit unsigned integers.
        UIntNorm16x2,    ///< Two normalized 16-bit unsigned integers.
        SInt16x4,        ///< Four 16-bit signed integers.
        SIntNorm16x4,    ///< Four normalized 16-bit signed integers.
        UInt16x4,        ///< Four 16-bit unsigned integers.
        UIntNorm16x4,    ///< Four normalized 16-bit unsigned integers.
    };

    /// \brief Identifies the semantic meaning of a vertex attribute.
    enum class VertexSemantic : UInt8
    {
        Position,      ///< Position of the vertex.
        Normal,        ///< Normal vector.
        Tangent,       ///< Tangent vector.
        Color,         ///< Vertex color.
        TexCoord0,     ///< First texture coordinate set.
        TexCoord1,     ///< Second texture coordinate set.
        TexCoord2,     ///< Third texture coordinate set.
        TexCoord3,     ///< Fourth texture coordinate set.
        BlendIndices,  ///< Bone indices for skinning.
        BlendWeights,  ///< Bone weights for skinning.
        Custom0,       ///< Custom semantic 0.
        Custom1,       ///< Custom semantic 1.
        Custom2,       ///< Custom semantic 2.
        Custom3,       ///< Custom semantic 3.
        Custom4,       ///< Custom semantic 4.
        Custom5,       ///< Custom semantic 5.
        Custom6,       ///< Custom semantic 6.
        Custom7,       ///< Custom semantic 7.
        None,          ///< No semantic assigned.
    };

    /// \brief Identifier for GPU-bound objects (e.g., textures, buffers, shaders).
    using Object = UInt16;

    /// \brief Represents a display resolution mode.
    struct Resolution final
    {
        /// The width of the display mode in pixels.
        UInt16 Width;

        /// The height of the display mode in pixels.
        UInt16 Height;

        /// The refresh rate of the display mode in hertz (Hz).
        UInt16 Frequency;
    };

    /// \brief Describes a graphics adapter (GPU).
    struct Adapter final
    {
        /// The human-readable name of the graphics adapter.
        Text   Description;

        /// The dedicated video memory in megabytes (MB).
        UInt32 DedicatedMemoryInMBs;

        /// The system memory shared with the GPU in megabytes (MB).
        UInt32 SystemMemoryInMBs;

        /// The unified memory shared between CPU and GPU in megabytes (MB).
        UInt32 SharedMemoryInMBs;

        /// The list of supported display resolutions.
        Vector<Resolution> Resolutions;
    };

    /// \brief Represents a texture attachment in a render pass.
    struct Attachment final
    {
        /// \brief Default constructor for an attachment.
        AURORA_INLINE constexpr Attachment() = default;

        /// \brief Constructs a single-texture attachment for non-MSAA rendering.
        ///
        /// \param Texture The render target texture (must be non-multisample).
        /// \param Level   The mipmap level to render into (default = 0 for base level).
        AURORA_INLINE constexpr Attachment(Object Texture, UInt8 Level = 0)
            : Target      { Texture },
              TargetLevel { Level }
        {
        }

        /// \brief Constructs an attachment with a multisample source texture and a resolve target.
        ///
        /// \param Source      The multisample render target texture (must have sample count > 1).
        /// \param SourceLevel The mipmap level of the source texture to render into.
        /// \param Target      The non-multisample texture that will receive the resolved output.
        /// \param TargetLevel The mipmap level of the target texture for the resolve operation.
        AURORA_INLINE constexpr Attachment(Object Source, UInt8 SourceLevel, Object Target, UInt8 TargetLevel)
            : Source      { Source },
              SourceLevel { SourceLevel },
              Target      { Target },
              TargetLevel { TargetLevel }
        {
        }

        /// Multisample source texture for MSAA rendering (0 when MSAA is disabled).
        Object Source      = 0;

        /// Mipmap level of the Source texture to render into.
        UInt8  SourceLevel = 0;

        /// Final output texture that receives rendering results.
        Object Target      = 0;

        /// Mipmap level of the Target texture to render into.
        UInt8  TargetLevel = 0;
    };

    /// \brief Defines a vertex attribute within a vertex buffer layout.
    struct Attribute final
    {
        /// \brief Default-initialized attribute with no semantic and default format.
        AURORA_INLINE constexpr Attribute() = default;

        /// \brief Initializes a vertex attribute with specified properties.
        ///
        /// \param Semantic  Semantic meaning of the attribute.
        /// \param Format    Data format of the attribute.
        /// \param Offset    Byte offset from the start of the vertex to this attribute.
        AURORA_INLINE constexpr Attribute(VertexSemantic Semantic, VertexFormat Format, UInt16 Offset)
            : Semantic { Semantic },
              Format   { Format },
              Offset   { Offset },
              Stream   { 0 },
              Divisor  { 0 }
        {
        }

        /// \brief Initializes a vertex attribute with specified properties.
        ///
        /// \param Semantic Semantic meaning of the attribute.
        /// \param Format   Data format of the attribute.
        /// \param Offset   Byte offset from the start of the vertex to this attribute.
        /// \param Stream   Index of the vertex buffer stream that provides this attribute.
        /// \param Divisor  Rate at which the attribute advances during instanced rendering.
        AURORA_INLINE constexpr Attribute(VertexSemantic Semantic, VertexFormat Format, UInt16 Offset, UInt16 Stream, UInt16 Divisor)
            : Semantic { Semantic },
              Format   { Format },
              Offset   { Offset },
              Stream   { Stream },
              Divisor  { Divisor }
        {
        }

            /// Semantic meaning of the attribute.
            VertexSemantic Semantic = VertexSemantic::None;

            /// Data format of the attribute.
            VertexFormat   Format   = VertexFormat::Float32x4;

            /// Byte offset from the start of the vertex to this attribute.
            UInt16         Offset   = 0;

            /// Index of the vertex buffer stream that provides this attribute.
            UInt16         Stream   = 0;

            /// Rate at which the attribute advances during instanced rendering.
            UInt16         Divisor  = 0;
    };

    /// \brief Describes the graphics capabilities of the current system and backend.
    struct Capabilities final
    {
        /// The current graphics backend in use.
        Backend  Backend  = Backend::None;

        /// The current graphics API version.
        Language Language = Language::V1;

        /// The current multisampling level.
        Samples  Samples  = Samples::X1;

        /// The list of available graphics adapters.
        Vector<Adapter> Adapters;
    };

    /// \brief Describes the fixed-function GPU state used by a rendering pipeline.
    struct Descriptor final
    {
        /// Face culling mode (e.g., back-face or front-face culling).
        Cull           Cull                  = Cull::Back;

        /// Polygon fill mode (solid or wireframe).
        Fill           Fill                  = Fill::Solid;

        /// Write mask controlling which color channels are affected.
        BlendMask      BlendMask             = BlendMask::RGBA;

        /// Source blend factor for color channels.
        BlendFactor    BlendSrcColor         = BlendFactor::One;

        /// Destination blend factor for the color channels.
        BlendFactor    BlendDstColor         = BlendFactor::Zero;

        /// Blend operation for color channels.
        BlendFunction  BlendEquationColor    = BlendFunction::Add;

        /// Source blend factor for the alpha channel.
        BlendFactor    BlendSrcAlpha         = BlendFactor::One;

        /// Destination blend factor for the alpha channel.
        BlendFactor    BlendDstAlpha         = BlendFactor::Zero;

        /// Blend function for the alpha channel.
        BlendFunction  BlendEquationAlpha    = BlendFunction::Add;

        /// Enables or disables depth clipping.
        Bool           DepthClip             = true;

        /// Enables or disables writing to the depth buffer.
        Bool           DepthMask             = true;

        /// Depth comparison function used for depth testing.
        TestCondition  DepthTest             = TestCondition::LessEqual;

        /// Constant offset added to fragment depth.
        Real32         DepthBias             = 0.0f;

        /// Multiplier applied to fragment slope for depth biasing.
        Real32         DepthBiasSlope        = 0.0f;

        /// Maximum value applied to depth bias (clamping).
        Real32         DepthBiasClamp        = 0.0f;

        /// Bitmask selecting readable bits of the stencil buffer.
        UInt8          StencilReadMask       = 0xFF;

        /// Bitmask selecting writable bits of the stencil buffer.
        UInt8          StencilWriteMask      = 0xFF;

        /// Stencil comparison function for front-facing geometry.
        TestCondition  StencilFrontTest      = TestCondition::Always;

        /// Action when stencil test fails (front-facing).
        TestAction     StencilFrontFail      = TestAction::Keep;

        /// Action when stencil passes but depth test fails (front-facing).
        TestAction     StencilFrontDepthFail = TestAction::Keep;

        /// Action when both stencil and depth tests pass (front-facing).
        TestAction     StencilFrontDepthPass = TestAction::Keep;

        /// Stencil comparison function for back-facing geometry.
        TestCondition  StencilBackTest       = TestCondition::Always;

        /// Action when stencil test fails (back-facing).
        TestAction     StencilBackFail       = TestAction::Keep;

        /// Action when stencil passes but depth test fails (back-facing).
        TestAction     StencilBackDepthFail  = TestAction::Keep;

        /// Action when both stencil and depth tests pass (back-facing).
        TestAction     StencilBackDepthPass  = TestAction::Keep;

        /// Attributes describing the input layout for vertex data.
        Vector<Attribute, kMaxAttributes> InputAttributes;

        /// Primitive topology used for vertex interpretation.
        Primitive      InputPrimitive        = Primitive::TriangleList;
    };

    /// \brief Encapsulates parameters for a draw call, supporting both indexed and non-indexed rendering.
    struct Draw final
    {
        /// \brief Default constructor creates an empty draw command.
        AURORA_INLINE constexpr Draw() = default;

        /// \brief Constructs a primitive with specified parameters.
        /// 
        /// \param Count     The number of vertices or indices to draw.
        /// \param Base      The base vertex index for indexed draws, or first vertex index for non-indexed draws.
        /// \param Offset    The offset into the index buffer or vertex buffer.
        /// \param Instances Optional number of instances to draw (for instanced rendering).
        AURORA_INLINE constexpr Draw(UInt32 Count, SInt32 Base, UInt32 Offset, UInt32 Instances = 1)
            : Count     { Count },
              Base      { Base },
              Offset    { Offset },
              Instances { Instances }
        {
        }

        /// Number of vertices or indices to draw.
        UInt32 Count     = 0;

        /// Base vertex index offset for indexed draws, or first vertex index for non-indexed draws.
        SInt32 Base      = 0;

        /// Offset into the index buffer or vertex buffer.
        UInt32 Offset    = 0;

        /// Number of instances to draw (for instanced rendering).
        UInt32 Instances = 1;
    };

    /// \brief Describes how a texture is sampled in a shader.
    struct Sampler final
    {
        /// \brief Default-initialized sampler with clamp wrap mode and nearest filtering.
        AURORA_INLINE constexpr Sampler() = default;

        /// \brief Initializes a sampler with specified wrap modes and filter.
        /// 
        /// \param WrapModeU Texture wrap behavior for the U (horizontal) coordinate.
        /// \param WrapModeV Texture wrap behavior for the V (vertical) coordinate.
        /// \param Filter    Filtering method used when sampling the texture.
        AURORA_INLINE constexpr Sampler(TextureEdge WrapModeU, TextureEdge WrapModeV, TextureFilter Filter)
            : WrapModeU { WrapModeU },
              WrapModeV { WrapModeV },
              Filter    { Filter }
        {
        }

        /// Texture wrap behavior for the U (horizontal) coordinate.
        TextureEdge   WrapModeU = TextureEdge::Clamp;

        /// Texture wrap behavior for the V (vertical) coordinate.
        TextureEdge   WrapModeV = TextureEdge::Clamp;

        /// Filtering method used when sampling the texture.
        TextureFilter Filter    = TextureFilter::MinMagMipPoint;
    };

    /// \brief Defines a rectangular scissor region for pixel clipping during rendering.
    struct Scissor final
    {
        /// \brief Default-initialized scissor region covering the entire render target.
        AURORA_INLINE constexpr Scissor() = default;

        /// \brief Initializes a scissor region with specified position and size.
        /// 
        /// \param X      X coordinate of the top-left corner, in pixels.
        /// \param Y      Y coordinate of the top-left corner, in pixels.
        /// \param Width  Width of the scissor region, in pixels.
        /// \param Height Height of the scissor region, in pixels.
        AURORA_INLINE constexpr Scissor(UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height)
            : X      { X },
              Y      { Y },
              Width  { Width },
              Height { Height }
        {
        }

        /// X coordinate of the top-left corner.
        UInt16 X      = 0;

        /// Y coordinate of the top-left corner.
        UInt16 Y      = 0;

        /// Width of the scissor region in pixels.
        UInt16 Width  = UINT16_MAX;

        /// Height of the scissor region in pixels.
        UInt16 Height = UINT16_MAX;
    };

    /// \brief Describes a buffer stream used for vertex, index, or uniform input.
    struct Stream final
    {
        /// \brief Default-initialized stream with no buffer and zero stride/offset.
        AURORA_INLINE constexpr Stream() = default;

        /// \brief Initializes a stream with a buffer, stride, and offset.
        /// 
        /// \param Buffer Handle to the GPU buffer object.
        /// \param Stride Size in bytes of each element in the buffer.
        /// \param Offset Byte offset from the start of the buffer to the first element (default
        AURORA_INLINE constexpr Stream(Object Buffer, UInt16 Stride, UInt32 Offset)
            : Buffer { Buffer },
              Stride { Stride },
              Offset { Offset }
        {
        }

        /// Handle to the bound GPU buffer.
        Object Buffer = 0;

        /// Size in bytes of each bound element.
        UInt16 Stride = 0;

        /// Byte offset from the start of the buffer to the first element.
        UInt32 Offset = 0;
    };

    /// \brief Defines a rectangular viewport for transforming normalized device coordinates.
    struct Viewport final
    {
        /// \brief Constructs a viewport with default zero values.
        AURORA_INLINE constexpr Viewport() = default;

        /// \brief Constructs a viewport with position and size, using a default depth range of [0, 1].
        /// 
        /// \param X      X coordinate of the top-left corner, in pixels.
        /// \param Y      Y coordinate of the top-left corner, in pixels.
        /// \param Width  Width of the viewport, in pixels.
        /// \param Height Height of the viewport, in pixels.
        AURORA_INLINE constexpr Viewport(Real32 X, Real32 Y, Real32 Width, Real32 Height)
            : X      { X },
              Y      { Y },
              Width  { Width },
              Height { Height }
        {
        }

        /// \brief Constructs a viewport with position, size, and depth range.
        /// 
        /// \param X        X coordinate of the top-left corner, in pixels.
        /// \param Y        Y coordinate of the top-left corner, in pixels.
        /// \param Width    Width of the viewport, in pixels.
        /// \param Height   Height of the viewport, in pixels.
        /// \param MinDepth Minimum depth value mapped from normalized depth.
        /// \param MaxDepth Maximum depth value mapped from normalized depth.
        AURORA_INLINE Viewport(Real32 X, Real32 Y, Real32 Width, Real32 Height, Real32 MinDepth, Real32 MaxDepth)
            : X        { X },
              Y        { Y },
              Width    { Width },
              Height   { Height },
              MinDepth { MinDepth },
              MaxDepth { MaxDepth }
        {
        }

        /// X coordinate of the top-left corner in screen space.
        Real32 X        = 0.0f;

        /// Y coordinate of the top-left corner in screen space.
        Real32 Y        = 0.0f;

        /// Width of the viewport in pixels.
        Real32 Width    = 0.0f;

        /// Height of the viewport in pixels.
        Real32 Height   = 0.0f;

        /// Minimum depth value for depth range mapping.
        Real32 MinDepth = 0.0f;

        /// Maximum depth value for depth range mapping.
        Real32 MaxDepth = 1.0f;
    };

    /// \brief Defines a submission command to be executed by the GPU.
    struct Submission final
    {
        /// TODO: Update Flags / Hash

        /// Scissor rectangle that defines the renderable area.
        Scissor                      Scissor;

        /// Stencil reference value for stencil tests.
        UInt8                        Stencil  = 0;

        /// Pipeline state object used for rendering.
        Object                       Pipeline = 0;

        /// Collection of uniform buffer bindings for the pipeline.
        Array<Stream,  kMaxUniforms> Uniforms;          // TODO: Sparse Collection (Binding)

        /// Collection of sampler bindings for texture sampling.
        Array<Sampler, kMaxSlots>    Samplers;          // TODO: Sparse Collection (Binding)

        /// Collection of texture bindings for the pipeline.
        Array<Object,  kMaxSlots>    Textures { };      // TODO: Sparse Collection (Binding)

        /// Collection of vertex buffer bindings for geometry input.
        Array<Stream,  kMaxFetches>  Vertices;          // TODO: Sparse Collection (Binding)

        /// Index buffer binding used for indexed rendering.
        Stream                       Indices;

        /// Draw command that defines the primitives to render.
        Draw                         Command;
    };
}