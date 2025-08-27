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

#include "D3D11Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto CheckIfFail(HRESULT Result)
    {
        if (FAILED(Result))
        {
            constexpr UINT Flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

            TCHAR Message[1024];
            ::FormatMessage(Flags, nullptr, Result, 0, Message, sizeof(Message), nullptr);

            LOG_CRITICAL("D3D11Driver: {}", Message);
            return false;
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Access Value)
    {
        constexpr static Array kMapping = {
            D3D11_USAGE_IMMUTABLE,           // Access::Device
            D3D11_USAGE_STAGING,             // Access::Host
            D3D11_USAGE_DEFAULT,             // Access::Dual
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(BlendMask Value)
    {
        return static_cast<D3D11_COLOR_WRITE_ENABLE>(Value);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(BlendFactor Value)
    {
        static constexpr Array kMapping = {
            D3D11_BLEND_ZERO,                       // BlendFactor::Zero
            D3D11_BLEND_ONE,                        // BlendFactor::One
            D3D11_BLEND_SRC_COLOR,                  // BlendFactor::SrcColor
            D3D11_BLEND_INV_SRC_COLOR,              // BlendFactor::OneMinusSrcColor
            D3D11_BLEND_SRC_ALPHA,                  // BlendFactor::SrcAlpha
            D3D11_BLEND_INV_SRC_ALPHA,              // BlendFactor::OneMinusSrcAlpha
            D3D11_BLEND_DEST_COLOR,                 // BlendFactor::DstColor
            D3D11_BLEND_INV_DEST_COLOR,             // BlendFactor::OneMinusDstColor
            D3D11_BLEND_DEST_ALPHA,                 // BlendFactor::DstAlpha
            D3D11_BLEND_INV_DEST_ALPHA              // BlendFactor::OneMinusDstAlpha
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(BlendFunction Value)
    {
        constexpr static Array kMapping = {
            D3D11_BLEND_OP_ADD,                     // BlendFunction::Add
            D3D11_BLEND_OP_SUBTRACT,                // BlendFunction::Subtract
            D3D11_BLEND_OP_REV_SUBTRACT,            // BlendFunction::ReverseSubtract
            D3D11_BLEND_OP_MIN,                     // BlendFunction::Min
            D3D11_BLEND_OP_MAX                      // BlendFunction::Max
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Cull Value)
    {
        constexpr static Array kMapping = {
            D3D11_CULL_NONE,                // Cull::None
            D3D11_CULL_BACK,                // Cull::Back
            D3D11_CULL_FRONT,               // Cull::Front
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Fill Value)
    {
        constexpr static Array kMapping = {
            D3D11_FILL_SOLID,               // Fill::Solid
            D3D11_FILL_WIREFRAME,           // Fill::Wireframe
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Primitive Value)
    {
        constexpr static Array kMapping = {
            D3D11_PRIMITIVE_TOPOLOGY_LINELIST,         // Primitive::LineList
            D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,        // Primitive::LineStrip
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,     // Primitive::TriangleList
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,    // Primitive::TriangleStrip
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Samples Value)
    {
        constexpr static Array kMapping = {
            1,        // Samples::X1
            2,        // Samples::X2
            4,        // Samples::X4
            8,        // Samples::X8
            16,       // Samples::X16
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(TestAction Value)
    {
        constexpr static Array kMapping = {
            D3D11_STENCIL_OP_KEEP,                  // TestAction::Keep
            D3D11_STENCIL_OP_REPLACE,               // TestAction::Replace
            D3D11_STENCIL_OP_ZERO,                  // TestAction::Zero
            D3D11_STENCIL_OP_DECR,                  // TestAction::Decrease
            D3D11_STENCIL_OP_DECR_SAT,              // TestAction::DecrementSaturate
            D3D11_STENCIL_OP_INVERT,                // TestAction::Invert
            D3D11_STENCIL_OP_INCR,                  // TestAction::Increase
            D3D11_STENCIL_OP_INCR_SAT               // TestAction::IncreaseSaturate
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(TestCondition Value)
    {
        constexpr static Array kMapping = {
            D3D11_COMPARISON_ALWAYS,                // TestCondition::Always
            D3D11_COMPARISON_NEVER,                 // TestCondition::Never
            D3D11_COMPARISON_GREATER,               // TestCondition::Greater
            D3D11_COMPARISON_GREATER_EQUAL,         // TestCondition::GreaterEqual
            D3D11_COMPARISON_EQUAL,                 // TestCondition::Equal
            D3D11_COMPARISON_NOT_EQUAL,             // TestCondition::NotEqual
            D3D11_COMPARISON_LESS,                  // TestCondition::Less
            D3D11_COMPARISON_LESS_EQUAL             // TestCondition::LessEqual
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(TextureEdge Value)
    {
        constexpr static Array kMapping = {
            D3D11_TEXTURE_ADDRESS_CLAMP,            // TextureEdge::Clamp
            D3D11_TEXTURE_ADDRESS_WRAP,             // TextureEdge::Repeat
            D3D11_TEXTURE_ADDRESS_MIRROR            // TextureEdge::Mirror
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(TextureFilter Value)
    {
        constexpr static Array kMapping = {
            D3D11_FILTER_MIN_MAG_MIP_POINT,                 // TextureFilter::MinMagMipPoint
            D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,          // TextureFilter::MinMagPointMipLinear
            D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,    // TextureFilter::MinPointMagLinearMipPoint
            D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,          // TextureFilter::MinPointMagMipLinear
            D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,          // TextureFilter::MinLinearMagMipPoint
            D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,   // TextureFilter::MinLinearMagPointMipLinear
            D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,          // TextureFilter::MinMagLinearMipPoint
            D3D11_FILTER_MIN_MAG_MIP_LINEAR,                // TextureFilter::MinMagMipLinear
            D3D11_FILTER_ANISOTROPIC,                       // TextureFilter::Anisotropic2x
            D3D11_FILTER_ANISOTROPIC,                       // TextureFilter::Anisotropic4x
            D3D11_FILTER_ANISOTROPIC,                       // TextureFilter::Anisotropic8x
            D3D11_FILTER_ANISOTROPIC,                       // TextureFilter::Anisotropic16x
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    template<UInt Data>
    auto As(TextureFormat Value)
    {
        // FORMAT                                   // SRV                                 // DSV
        constexpr static Tuple<DXGI_FORMAT, DXGI_FORMAT, DXGI_FORMAT> kMapping[] = {
            { DXGI_FORMAT_BC1_UNORM,            DXGI_FORMAT_BC1_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC1UIntNorm
            { DXGI_FORMAT_BC1_UNORM_SRGB,       DXGI_FORMAT_BC1_UNORM_SRGB,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC1UIntNorm_sRGB
            { DXGI_FORMAT_BC2_UNORM,            DXGI_FORMAT_BC2_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC2UIntNorm
            { DXGI_FORMAT_BC2_UNORM_SRGB,       DXGI_FORMAT_BC2_UNORM_SRGB,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC2UIntNorm_sRGB
            { DXGI_FORMAT_BC3_UNORM,            DXGI_FORMAT_BC3_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC3UIntNorm
            { DXGI_FORMAT_BC3_UNORM_SRGB,       DXGI_FORMAT_BC3_UNORM_SRGB,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC3UIntNorm_sRGB
            { DXGI_FORMAT_BC4_UNORM,            DXGI_FORMAT_BC4_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC4UIntNorm
            { DXGI_FORMAT_BC5_UNORM,            DXGI_FORMAT_BC5_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BC5UIntNorm
            { DXGI_FORMAT_R8_SINT,              DXGI_FORMAT_R8_SINT,                    DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R8SInt
            { DXGI_FORMAT_R8_SNORM,             DXGI_FORMAT_R8_SNORM,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R8SIntNorm
            { DXGI_FORMAT_R8_UINT,              DXGI_FORMAT_R8_UINT,                    DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R8UInt
            { DXGI_FORMAT_R8_UNORM,             DXGI_FORMAT_R8_UNORM,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R8UIntNorm
            { DXGI_FORMAT_R16_SINT,             DXGI_FORMAT_R16_SINT,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R16SInt
            { DXGI_FORMAT_R16_SNORM,            DXGI_FORMAT_R16_SNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R16SIntNorm
            { DXGI_FORMAT_R16_UINT,             DXGI_FORMAT_R16_UINT,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R16UInt
            { DXGI_FORMAT_R16_UNORM,            DXGI_FORMAT_R16_UNORM,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R16UIntNorm
            { DXGI_FORMAT_R16_FLOAT,            DXGI_FORMAT_R16_FLOAT,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R16Float
            { DXGI_FORMAT_R32_SINT,             DXGI_FORMAT_R32_SINT,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R32SInt
            { DXGI_FORMAT_R32_UINT,             DXGI_FORMAT_R32_UINT,                   DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R32UInt
            { DXGI_FORMAT_R32_FLOAT,            DXGI_FORMAT_R32_FLOAT,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::R32Float
            { DXGI_FORMAT_R8G8_SINT,            DXGI_FORMAT_R8G8_SINT,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG8SInt
            { DXGI_FORMAT_R8G8_SNORM,           DXGI_FORMAT_R8G8_SNORM,                 DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG8SIntNorm
            { DXGI_FORMAT_R8G8_UINT,            DXGI_FORMAT_R8G8_UINT,                  DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG8UInt
            { DXGI_FORMAT_R8G8_UNORM,           DXGI_FORMAT_R8G8_UNORM,                 DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG8UIntNorm
            { DXGI_FORMAT_R16G16_SINT,          DXGI_FORMAT_R16G16_SINT,                DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG16SInt
            { DXGI_FORMAT_R16G16_SNORM,         DXGI_FORMAT_R16G16_SNORM,               DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG16SIntNorm
            { DXGI_FORMAT_R16G16_UINT,          DXGI_FORMAT_R16G16_UINT,                DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG16UInt
            { DXGI_FORMAT_R16G16_UNORM,         DXGI_FORMAT_R16G16_UNORM,               DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG16UIntNorm
            { DXGI_FORMAT_R16G16_FLOAT,         DXGI_FORMAT_R16G16_FLOAT,               DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG16Float
            { DXGI_FORMAT_R32G32_SINT,          DXGI_FORMAT_R32G32_SINT,                DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG32SInt
            { DXGI_FORMAT_R32G32_UINT,          DXGI_FORMAT_R32G32_UINT,                DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG32UInt
            { DXGI_FORMAT_R32G32_FLOAT,         DXGI_FORMAT_R32G32_FLOAT,               DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RG32Float
            { DXGI_FORMAT_R32G32B32_SINT,       DXGI_FORMAT_R32G32B32_SINT,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGB32SInt
            { DXGI_FORMAT_R32G32B32_UINT,       DXGI_FORMAT_R32G32B32_UINT,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGB32UInt
            { DXGI_FORMAT_R32G32B32_FLOAT,      DXGI_FORMAT_R32G32B32_FLOAT,            DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGB32Float
            { DXGI_FORMAT_R8G8B8A8_SINT,        DXGI_FORMAT_R8G8B8A8_SINT,              DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA8SInt
            { DXGI_FORMAT_R8G8B8A8_SNORM,       DXGI_FORMAT_R8G8B8A8_SNORM,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA8SIntNorm
            { DXGI_FORMAT_R8G8B8A8_UINT,        DXGI_FORMAT_R8G8B8A8_UINT,              DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA8UInt
            { DXGI_FORMAT_R8G8B8A8_UNORM,       DXGI_FORMAT_R8G8B8A8_UNORM,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA8UIntNorm
            { DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,        DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA8UIntNorm_sRGB
            { DXGI_FORMAT_B8G8R8A8_UNORM,       DXGI_FORMAT_B8G8R8A8_UNORM,             DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BGRA8UIntNorm
            { DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,        DXGI_FORMAT_UNKNOWN              }, // TextureFormat::BGRA8UIntNorm_sRGB
            { DXGI_FORMAT_R16G16B16A16_SINT,    DXGI_FORMAT_R16G16B16A16_SINT,          DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA16SInt
            { DXGI_FORMAT_R16G16B16A16_SNORM,   DXGI_FORMAT_R16G16B16A16_SNORM,         DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA16SIntNorm
            { DXGI_FORMAT_R16G16B16A16_UINT,    DXGI_FORMAT_R16G16B16A16_UINT,          DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA16UInt
            { DXGI_FORMAT_R16G16B16A16_UNORM,   DXGI_FORMAT_R16G16B16A16_UNORM,         DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA16UIntNorm
            { DXGI_FORMAT_R16G16B16A16_FLOAT,   DXGI_FORMAT_R16G16B16A16_FLOAT,         DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA16Float
            { DXGI_FORMAT_R32G32B32A32_SINT,    DXGI_FORMAT_R32G32B32A32_SINT,          DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA32SInt
            { DXGI_FORMAT_R32G32B32A32_UINT,    DXGI_FORMAT_R32G32B32A32_UINT,          DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA32UInt
            { DXGI_FORMAT_R32G32B32A32_FLOAT,   DXGI_FORMAT_R32G32B32A32_FLOAT,         DXGI_FORMAT_UNKNOWN              }, // TextureFormat::RGBA32Float
            { DXGI_FORMAT_R16_TYPELESS,         DXGI_FORMAT_R16_UNORM,                  DXGI_FORMAT_D16_UNORM            }, // TextureFormat::D16
            { DXGI_FORMAT_R32_TYPELESS,         DXGI_FORMAT_R32_FLOAT,                  DXGI_FORMAT_D32_FLOAT            }, // TextureFormat::D32
            { DXGI_FORMAT_R24G8_TYPELESS,       DXGI_FORMAT_R24_UNORM_X8_TYPELESS,      DXGI_FORMAT_D24_UNORM_S8_UINT    }, // TextureFormat::D24S8UIntNorm
            { DXGI_FORMAT_R32G8X24_TYPELESS,    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,   DXGI_FORMAT_D32_FLOAT_S8X24_UINT }, // TextureFormat::D32S8UIntNorm
        };
        return Fetch<Data>(kMapping[Enum::Cast(Value)]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(VertexFormat Value)
    {
        constexpr static Array kMapping = {
            DXGI_FORMAT_R16G16_FLOAT,           // VertexFormat::Float16x2
            DXGI_FORMAT_R16G16B16A16_FLOAT,     // VertexFormat::Float16x4
            DXGI_FORMAT_R32_FLOAT,              // VertexFormat::Float32x1
            DXGI_FORMAT_R32G32_FLOAT,           // VertexFormat::Float32x2
            DXGI_FORMAT_R32G32B32_FLOAT,        // VertexFormat::Float32x3
            DXGI_FORMAT_R32G32B32A32_FLOAT,     // VertexFormat::Float32x4
            DXGI_FORMAT_R8G8B8A8_SINT,          // VertexFormat::SInt8x4
            DXGI_FORMAT_R8G8B8A8_SNORM,         // VertexFormat::SIntNorm8x4
            DXGI_FORMAT_R8G8B8A8_UINT,          // VertexFormat::UInt8x4
            DXGI_FORMAT_R8G8B8A8_UNORM,         // VertexFormat::UIntNorm8x4
            DXGI_FORMAT_R16G16_SINT,            // VertexFormat::SInt16x2
            DXGI_FORMAT_R16G16_SNORM,           // VertexFormat::SIntNorm16x2
            DXGI_FORMAT_R16G16_UINT,            // VertexFormat::UInt16x2
            DXGI_FORMAT_R16G16_UNORM,           // VertexFormat::UIntNorm16x2
            DXGI_FORMAT_R16G16B16A16_SINT,      // VertexFormat::SInt16x4
            DXGI_FORMAT_R16G16B16A16_SNORM,     // VertexFormat::SIntNorm16x4
            DXGI_FORMAT_R16G16B16A16_UINT,      // VertexFormat::UInt16x4
            DXGI_FORMAT_R16G16B16A16_UNORM,     // VertexFormat::UIntNorm16x4
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto As(Usage Value)
    {
        constexpr static Array kMapping = {
            D3D11_BIND_VERTEX_BUFFER,          // Usage::Vertex
            D3D11_BIND_INDEX_BUFFER,           // Usage::Index
            D3D11_BIND_CONSTANT_BUFFER,        // Usage::Uniform
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    template<UInt Data>
    auto As(VertexSemantic Value)
    {
        constexpr static Tuple<ConstPtr<Char>, UInt32> kMapping[] = {
            { "POSITION",     0 },     // VertexSemantic::Position
            { "NORMAL",       0 },     // VertexSemantic::Normal
            { "TANGENT",      0 },     // VertexSemantic::Tangent
            { "COLOR",        0 },     // VertexSemantic::Color
            { "TEXCOORD",     0 },     // VertexSemantic::TexCoord0
            { "TEXCOORD",     1 },     // VertexSemantic::TexCoord1
            { "TEXCOORD",     2 },     // VertexSemantic::TexCoord2
            { "TEXCOORD",     3 },     // VertexSemantic::TexCoord3
            { "BLENDINDICES", 0 },     // VertexSemantic::BlendIndices
            { "BLENDWEIGHT",  0 },     // VertexSemantic::BlendWeights
            { "CUSTOM",       0 },     // VertexSemantic::Custom0
            { "CUSTOM",       1 },     // VertexSemantic::Custom1
            { "CUSTOM",       2 },     // VertexSemantic::Custom2
            { "CUSTOM",       3 },     // VertexSemantic::Custom3
            { "CUSTOM",       4 },     // VertexSemantic::Custom4
            { "CUSTOM",       5 },     // VertexSemantic::Custom5
            { "CUSTOM",       6 },     // VertexSemantic::Custom6
            { "CUSTOM",       7 },     // VertexSemantic::Custom7
            { "NONE",         0 },     // VertexSemantic::None
        };
        return Fetch<Data>(kMapping[Enum::Cast(Value)]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto Fill(ConstPtr<UInt8> Data, UInt8 Layer, UInt16 Width, UInt16 Height, TextureFormat Layout)
    {
        constexpr static UInt8 kMapping[] = {
            4,      // TextureFormat::BC1UIntNorm
            4,      // TextureFormat::BC1UIntNorm_sRGB
            8,      // TextureFormat::BC2UIntNorm
            8,      // TextureFormat::BC2UIntNorm_sRGB
            8,      // TextureFormat::BC3UIntNorm
            8,      // TextureFormat::BC3UIntNorm_sRGB
            4,      // TextureFormat::BC4UIntNorm
            8,      // TextureFormat::BC5UIntNorm
            8,      // TextureFormat::BC7UIntNorm
            8,      // TextureFormat::BC7UIntNorm_sRGB
            8,      // TextureFormat::R8SInt
            8,      // TextureFormat::R8SIntNorm
            8,      // TextureFormat::R8UInt
            8,      // TextureFormat::R8UIntNorm
            16,     // TextureFormat::R16SInt
            16,     // TextureFormat::R16SIntNorm
            16,     // TextureFormat::R16UInt
            16,     // TextureFormat::R16UIntNorm
            16,     // TextureFormat::R16Float
            32,     // TextureFormat::R32SInt
            32,     // TextureFormat::R32UInt
            32,     // TextureFormat::R32Float
            16,     // TextureFormat::RG8SInt
            16,     // TextureFormat::RG8SIntNorm
            16,     // TextureFormat::RG8UInt
            16,     // TextureFormat::RG8UIntNorm
            32,     // TextureFormat::RG16SInt
            32,     // TextureFormat::RG16SIntNorm
            32,     // TextureFormat::RG16UInt
            32,     // TextureFormat::RG16UIntNorm
            32,     // TextureFormat::RG16Float
            64,     // TextureFormat::RG32SInt
            64,     // TextureFormat::RG32UInt
            64,     // TextureFormat::RG32Float
            96,     // TextureFormat::RGB32SInt
            96,     // TextureFormat::RGB32UInt
            96,     // TextureFormat::RGB32Float
            32,     // TextureFormat::RGBA8SInt
            32,     // TextureFormat::RGBA8SIntNorm
            32,     // TextureFormat::RGBA8UInt
            32,     // TextureFormat::RGBA8UIntNorm
            32,     // TextureFormat::RGBA8UIntNorm_sRGB
            32,     // TextureFormat::BGRA8UIntNorm
            32,     // TextureFormat::BGRA8UIntNorm_sRGB
            64,     // TextureFormat::RGBA16SInt
            64,     // TextureFormat::RGBA16SIntNorm
            64,     // TextureFormat::RGBA16UInt
            64,     // TextureFormat::RGBA16UIntNorm
            64,     // TextureFormat::RGBA16Float
            128,    // TextureFormat::RGBA32SInt
            128,    // TextureFormat::RGBA32UInt
            128,    // TextureFormat::RGBA32Float
            16,     // TextureFormat::D16Float
            32,     // TextureFormat::D32Float
            32,     // TextureFormat::D24S8UIntNorm
            64,     // TextureFormat::D32S8UIntNorm
        };

        const UInt32 Depth = kMapping[Enum::Cast(Layout)];

        if (Data)
        {
            static D3D11_SUBRESOURCE_DATA Content[kMaxMipmap];

            for (UInt32 Level = 0; Level < Layer; ++Level)
            {
                Content[Level].pSysMem          = Data;
                Content[Level].SysMemPitch      = Width * (Depth / 8);
                Content[Level].SysMemSlicePitch = 0;

                Data  += Width * Depth * Height;
                Width  = Max(1, Width  >> 1);
                Height = Max(1, Height >> 1);
            }
            return Content;
        }
        return static_cast<Ptr<D3D11_SUBRESOURCE_DATA>>(nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool D3D11Driver::Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, Samples Samples)
    {
        decltype(& D3D11CreateDevice)  D3D11CreateDevicePtr = nullptr;
        decltype(& CreateDXGIFactory1) CreateDXGIFactoryPtr = nullptr;

        if (const auto Dll = ::LoadLibraryEx("D3D11.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32); Dll != nullptr)
        {
            D3D11CreateDevicePtr
                = reinterpret_cast<decltype(&D3D11CreateDevice)>(GetProcAddress(Dll, "D3D11CreateDevice"));
        }
        if (const auto Dll = ::LoadLibraryEx("DXGI.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32); Dll != nullptr)
        {
            CreateDXGIFactoryPtr
                = reinterpret_cast<decltype(&CreateDXGIFactory1)>(GetProcAddress(Dll, "CreateDXGIFactory1"));
        }

        Bool Successful = D3D11CreateDevicePtr && CreateDXGIFactoryPtr;

        if (Successful)
        {
            ComPtr<ID3D11Device>        Device;
            ComPtr<ID3D11DeviceContext> DeviceImmediate;

            UInt Flags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

            constexpr D3D_FEATURE_LEVEL Direct3DFeatureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };

            // Tries Direct3D 11.1
            HRESULT Result = D3D11CreateDevicePtr(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                Flags,
                Direct3DFeatureLevels,
                _countof(Direct3DFeatureLevels) - 3, // 11.1 ... 10.0
                D3D11_SDK_VERSION,
                Device.GetAddressOf(),
                nullptr,
                DeviceImmediate.GetAddressOf());

            // Tries Direct3D 10.0 and below.
            if (FAILED(Result))
            {
                Result = D3D11CreateDevicePtr(
                    nullptr,
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,
                    Flags,
                    Direct3DFeatureLevels + 1,
                    _countof(Direct3DFeatureLevels) - 1, // 11.0 ... 9.1
                    D3D11_SDK_VERSION,
                    Device.GetAddressOf(),
                    nullptr,
                    DeviceImmediate.GetAddressOf());
            }

            // Tries Direct3D WARP mode.
            if (FAILED(Result))
            {
                Result = D3D11CreateDevicePtr(
                    nullptr,
                    D3D_DRIVER_TYPE_WARP,
                    nullptr,
                    Flags,
                    Direct3DFeatureLevels,
                    _countof(Direct3DFeatureLevels),
                    D3D11_SDK_VERSION,
                    Device.GetAddressOf(),
                    nullptr,
                    DeviceImmediate.GetAddressOf());

                if (SUCCEEDED(Result))
                {
                    LOG_INFO("D3D11Driver: Enabling software mode (WARP)");
                }
            }

            Successful = CheckIfFail(Result);

            if (Successful)
            {
                Successful = CheckIfFail(Device.As<ID3D11Device1>(& mDevice));
                Successful = Successful && CheckIfFail(DeviceImmediate.As<ID3D11DeviceContext1>(& mDeviceImmediate));
                Successful = Successful && CheckIfFail(CreateDXGIFactoryPtr(IID_PPV_ARGS(& mDeviceFactory)));

                if (Successful)
                {
                    LoadAdapters();
                    LoadCapabilities();
                    LoadStates();

                    if (Swapchain)
                    {
                        const Ptr<void> Display = SDL_GetPointerProperty(
                            SDL_GetWindowProperties(Swapchain), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                        CreateSwapchain(mPasses[0], reinterpret_cast<UInt>(Display), Width, Height, As(Samples));
                    }
                }
            }
        }

        return Successful;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Reset(UInt16 Width, UInt16 Height, Samples Samples)
    {
        if (mDeviceImmediate)
        {
            mDeviceImmediate->ClearState();
        }

        /// Deletes the current display pass to release associated render targets.
        DeletePass(kDisplay);

        /// Resizes the swap chain buffers with the new resolution and format.
        const UINT Flags = mProperties.Tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
        CheckIfFail(mDisplay->ResizeBuffers(0, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, Flags));

        /// Recreates swap chain resources, including color and depth-stencil attachments.
        CreateSwapchainResources(mPasses[0], Width, Height, As(Samples));

        /// Restores GPU pipeline states to their default configuration.
        LoadStates();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    ConstRef<Capabilities> D3D11Driver::GetCapabilities() const
    {
        return mCapabilities;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateBuffer(Object ID, Access Access, Usage Usage, UInt32 Length, ConstSpan<Byte> Data)
    {
        const D3D11_USAGE       Kind       = As(Access);
        const D3D11_BIND_FLAG   Bind       = As(Usage);
        const D3D11_BUFFER_DESC Descriptor = CD3D11_BUFFER_DESC(
            Bind == D3D11_BIND_CONSTANT_BUFFER ? Align<kBlockAlignment>(Length) : Length,
            Kind != D3D11_USAGE_STAGING ? Bind : 0,
            Kind,
            Kind == D3D11_USAGE_STAGING ? D3D11_CPU_ACCESS_WRITE : 0);

        D3D11_SUBRESOURCE_DATA Content {
            Data.data(),
            static_cast<UINT>(Data.size_bytes())
        };

        const Ptr<D3D11_SUBRESOURCE_DATA> Memory = (Data.empty() ? nullptr : &Content);
        CheckIfFail(mDevice->CreateBuffer(&Descriptor, Memory, mBuffers[ID].Resource.GetAddressOf()));
        mBuffers[ID].Kind = Kind;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UpdateBuffer(Object ID, UInt32 Offset, Bool Invalidate, ConstSpan<Byte> Data)
    {
        const D3D11_BOX Destination  = CD3D11_BOX(Offset, 0, 0, Offset + Data.size(), 1, 1);
        const D3D11_COPY_FLAGS Flags = (Invalidate ? D3D11_COPY_DISCARD : D3D11_COPY_NO_OVERWRITE);
        mDeviceImmediate->UpdateSubresource1(mBuffers[ID].Resource.Get(), 0, &Destination, Data.data(), 0, 0, Flags);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteBuffer(Object ID)
    {
        mBuffers[ID].~D3D11Buffer();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ResizeBuffer(Object ID, UInt32 Size)
    {
        // Query the existing buffer description to preserve all current settings.
        D3D11_BUFFER_DESC Descriptor;
        mBuffers[ID].Resource->GetDesc(&Descriptor);

        // Update the buffer size while leaving other properties unchanged.
        Descriptor.ByteWidth = Size;

        // Recreate the buffer with the updated description.
        CheckIfFail(mDevice->CreateBuffer(&Descriptor, nullptr, mBuffers[ID].Resource.ReleaseAndGetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size)
    {
        const D3D11_BOX Offset       = CD3D11_BOX(SrcOffset, 0, 0, SrcOffset + Size, 1, 1);
        const D3D11_COPY_FLAGS Flags = D3D11_COPY_NO_OVERWRITE;

        const Ptr<ID3D11Buffer> Target = mBuffers[DstBuffer].Resource.Get();
        const Ptr<ID3D11Buffer> Source = mBuffers[SrcBuffer].Resource.Get();
        mDeviceImmediate->CopySubresourceRegion1(Target, 0, DstOffset, 0, 0, Source, 0, &Offset, Flags);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Blob D3D11Driver::ReadBuffer(Object ID, UInt32 Offset, UInt32 Size)
    {
        return { }; // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<Byte> D3D11Driver::MapBuffer(Object ID, UInt32 Offset, UInt32 Size, Bool Invalidate)
    {
        const D3D11_MAP          Mode = (mBuffers[ID].Kind != D3D11_USAGE_STAGING
            ? (Invalidate ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE)
            : D3D11_MAP_WRITE);
        D3D11_MAPPED_SUBRESOURCE Memory;

        if (CheckIfFail(mDeviceImmediate->Map(mBuffers[ID].Resource.Get(), 0, Mode, 0, &Memory)))
        {
            return static_cast<Ptr<Byte>>(Memory.pData) + Offset;
        }
        return nullptr;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UnmapBuffer(Object ID)
    {
        mDeviceImmediate->Unmap(mBuffers[ID].Resource.Get(), 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreatePass(Object ID, ConstSpan<Attachment> Colors, ConstRef<Attachment> Auxiliary)
    {
        Ref<D3D11Pass> Pass = mPasses[ID];

        // Configure and create render target views (RTVs) for each color attachment.
        for (ConstRef<Attachment> Color : Colors)
        {
            Ref<D3D11Attachment> Attachment = Pass.Colors.emplace_back();
            Attachment.Source       = Color.Source ? mTextures[Color.Source].Resource : nullptr;
            Attachment.SourceLevel  = Color.SourceLevel;
            Attachment.Target       = mTextures[Color.Target].Resource;
            Attachment.TargetLevel  = Color.TargetLevel;
            Attachment.TargetFormat = As<1>(mTextures[Color.Target].Format);

            if (Color.Source)
            {
                const DXGI_FORMAT Format = As<1>(mTextures[Color.Source].Format);
                const CD3D11_RENDER_TARGET_VIEW_DESC Description(D3D11_RTV_DIMENSION_TEXTURE2DMS, Format, Color.SourceLevel);
                CheckIfFail(mDevice->CreateRenderTargetView(
                    Attachment.Source.Get(), & Description, Attachment.Accessor.GetAddressOf()));
            }
            else
            {
                const DXGI_FORMAT Format = As<1>(mTextures[Color.Target].Format);
                const CD3D11_RENDER_TARGET_VIEW_DESC Description(D3D11_RTV_DIMENSION_TEXTURE2D, Format, Color.TargetLevel);
                CheckIfFail(mDevice->CreateRenderTargetView(
                    Attachment.Target.Get(), & Description, Attachment.Accessor.GetAddressOf()));
            }
        }

        // Create and assign its depth-stencil view (DSV) if an auxiliary attachment is provided.
        if (Auxiliary.Target > 0)
        {
            const Ptr<ID3D11Texture2D> Attachment = mTextures[Auxiliary.Target].Resource.Get();

            const DXGI_FORMAT Format = As<2>(mTextures[Auxiliary.Target].Format);
            const CD3D11_DEPTH_STENCIL_VIEW_DESC Description(
                D3D11_DSV_DIMENSION_TEXTURE2D, Format, Auxiliary.TargetLevel);
            CheckIfFail(mDevice->CreateDepthStencilView(Attachment, & Description, Pass.Auxiliary.GetAddressOf()));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeletePass(Object ID)
    {
        mPasses[ID].~D3D11Pass();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreatePipeline(Object ID, ConstSpan<Byte> Vertex, ConstSpan<Byte> Fragment, ConstRef<Descriptor> Descriptor)
    {
        Ref<D3D11Pipeline> Pipeline = mPipelines[ID];

        CheckIfFail(mDevice->CreateVertexShader(Vertex.data(), Vertex.size(), nullptr, Pipeline.VS.GetAddressOf()));
        CheckIfFail(mDevice->CreatePixelShader(Fragment.data(), Fragment.size(), nullptr, Pipeline.PS.GetAddressOf()));

        {
            D3D11_BLEND_DESC Description = CD3D11_BLEND_DESC(CD3D11_DEFAULT());

            Description.RenderTarget[0].BlendEnable           = !(
                   Descriptor.BlendSrcColor       == BlendFactor::One
                && Descriptor.BlendSrcAlpha       == BlendFactor::One
                && Descriptor.BlendDstColor       == BlendFactor::Zero
                && Descriptor.BlendDstAlpha       == BlendFactor::Zero
                && Descriptor.BlendEquationColor  == BlendFunction::Add
                && Descriptor.BlendEquationAlpha  == BlendFunction::Add);
            Description.RenderTarget[0].SrcBlend              = As(Descriptor.BlendSrcColor);
            Description.RenderTarget[0].DestBlend             = As(Descriptor.BlendDstColor);
            Description.RenderTarget[0].BlendOp               = As(Descriptor.BlendEquationColor);
            Description.RenderTarget[0].SrcBlendAlpha         = As(Descriptor.BlendSrcAlpha);
            Description.RenderTarget[0].DestBlendAlpha        = As(Descriptor.BlendDstAlpha);
            Description.RenderTarget[0].BlendOpAlpha          = As(Descriptor.BlendEquationAlpha);
            Description.RenderTarget[0].RenderTargetWriteMask = As(Descriptor.BlendMask);

            CheckIfFail(mDevice->CreateBlendState(& Description, Pipeline.BS.GetAddressOf()));
        }

        {
            D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

            Description.DepthEnable    = Descriptor.DepthTest != TestCondition::Always || Descriptor.DepthMask;
            Description.DepthFunc      = As(Descriptor.DepthTest);
            Description.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(Descriptor.DepthMask);
            Description.StencilEnable  = Descriptor.StencilFrontTest      != TestCondition::Always
                                      || Descriptor.StencilFrontFail      != TestAction::Keep
                                      || Descriptor.StencilFrontDepthFail != TestAction::Keep
                                      || Descriptor.StencilFrontDepthPass != TestAction::Keep
                                      || Descriptor.StencilBackTest       != TestCondition::Always
                                      || Descriptor.StencilBackFail       != TestAction::Keep
                                      || Descriptor.StencilBackDepthFail  != TestAction::Keep
                                      || Descriptor.StencilBackDepthPass  != TestAction::Keep;
            Description.StencilReadMask              = Descriptor.StencilReadMask;
            Description.StencilWriteMask             = Descriptor.StencilWriteMask;
            Description.FrontFace.StencilFunc        = As(Descriptor.StencilFrontTest);
            Description.FrontFace.StencilFailOp      = As(Descriptor.StencilFrontFail);
            Description.FrontFace.StencilDepthFailOp = As(Descriptor.StencilFrontDepthFail);
            Description.FrontFace.StencilPassOp      = As(Descriptor.StencilFrontDepthPass);
            Description.BackFace.StencilFunc         = As(Descriptor.StencilBackTest);
            Description.BackFace.StencilFailOp       = As(Descriptor.StencilBackFail);
            Description.BackFace.StencilDepthFailOp  = As(Descriptor.StencilBackDepthFail);
            Description.BackFace.StencilPassOp       = As(Descriptor.StencilBackDepthPass);

            CheckIfFail(mDevice->CreateDepthStencilState(& Description, Pipeline.DS.GetAddressOf()));
        }

        {
            D3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());

            Description.FrontCounterClockwise = TRUE;
            Description.DepthBias             = Descriptor.DepthBias;
            Description.DepthBiasClamp        = Descriptor.DepthBiasClamp;
            Description.SlopeScaledDepthBias  = Descriptor.DepthBiasSlope;
            Description.DepthClipEnable       = Descriptor.DepthClip;
            Description.CullMode              = As(Descriptor.Cull);
            Description.ScissorEnable         = TRUE;
            Description.MultisampleEnable     = TRUE;
            Description.AntialiasedLineEnable = TRUE;
            Description.FillMode              = As(Descriptor.Fill);

            CheckIfFail(mDevice->CreateRasterizerState(& Description, Pipeline.RS.GetAddressOf()));
        }

        {
            D3D11_INPUT_ELEMENT_DESC Description[kMaxAttributes];
            UInt                     Count = 0;

            for (ConstRef<Attribute> Attribute : Descriptor.InputAttributes)
            {
                Ref<D3D11_INPUT_ELEMENT_DESC> Element = Description[Count++];

                Element.SemanticName         = As<0>(Attribute.Semantic);
                Element.SemanticIndex        = As<1>(Attribute.Semantic);
                Element.Format               = As(Attribute.Format);
                Element.AlignedByteOffset    = Attribute.Offset;
                Element.InputSlot            = Attribute.Stream;
                Element.InputSlotClass       = Attribute.Divisor ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
                Element.InstanceDataStepRate = Attribute.Divisor;
            }

            CheckIfFail(mDevice->CreateInputLayout(
                Description, Count, Vertex.data(), Vertex.size(), Pipeline.IL.GetAddressOf()));
        }

        Pipeline.PT = As(Descriptor.InputPrimitive);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeletePipeline(Object ID)
    {
        mPipelines[ID].~D3D11Pipeline();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateTexture(Object ID, Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Mipmaps, Samples Samples, ConstSpan<Byte> Data)
    {
        CD3D11_TEXTURE2D_DESC Description(As<0>(Format), Width, Height, 1, Mipmaps);
        Description.Usage      = As(Access);
        Description.BindFlags  = Layout != TextureLayout::Target ? D3D11_BIND_SHADER_RESOURCE : 0;
        Description.MiscFlags  = Mipmaps < 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
        Description.SampleDesc = mProperties.Multisample[As(Samples)];

        if (Layout != TextureLayout::Source)
        {
            switch (Format)
            {
                case TextureFormat::D16Float:
                case TextureFormat::D32Float:
                case TextureFormat::D24S8UIntNorm:
                case TextureFormat::D32S8UIntNorm:
                    Description.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
                    break;
                default:
                    Description.BindFlags |= D3D11_BIND_RENDER_TARGET;
                    break;
            }
        }

        const ConstPtr<D3D11_SUBRESOURCE_DATA> Memory = Fill(Data.data(), Mipmaps, Width, Height, Format);
        CheckIfFail(mDevice->CreateTexture2D(&Description, Memory, mTextures[ID].Resource.GetAddressOf()));

        if (Layout != TextureLayout::Target)
        {
            mTextures[ID].Format = Format;

            const D3D11_SRV_DIMENSION Type
                = Samples != Samples::X1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

            const CD3D11_SHADER_RESOURCE_VIEW_DESC Descriptor(Type, As<1>(Format), 0, Mipmaps);
            CheckIfFail(mDevice->CreateShaderResourceView(
                mTextures[ID].Resource.Get(), &Descriptor, mTextures[ID].Accessor.GetAddressOf()));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UpdateTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height, UInt32 Pitch, ConstSpan<Byte> Data)
    {
        const D3D11_BOX Offset = CD3D11_BOX(X, Y, 0, X + Width, Y + Height, 1);
        mDeviceImmediate->UpdateSubresource(mTextures[ID].Resource.Get(), Level, &Offset, Data.data(), Pitch, 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteTexture(Object ID)
    {
        mTextures[ID].~D3D11Texture();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CopyTexture(Object DstTexture, UInt8 DstLevel, UInt16 DstX, UInt16 DstY, Object SrcTexture, UInt8 SrcLevel, UInt16 SrcX, UInt16 SrcY, UInt16 Width, UInt16 Height)
    {
        const D3D11_BOX Offset = CD3D11_BOX(SrcX, SrcY, 0, SrcX + Width, SrcY + Height, 1);

        const Ptr<ID3D11Texture2D> Target = mTextures[DstTexture].Resource.Get();
        const Ptr<ID3D11Texture2D> Source = mTextures[SrcTexture].Resource.Get();
        mDeviceImmediate->CopySubresourceRegion(Target, DstLevel, DstX, DstY, 0, Source, SrcLevel, &Offset);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Blob D3D11Driver::ReadTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height)
    {
        return { }; // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Prepare(Object ID, ConstRef<Viewport> Viewport, Clear Target, ConstRef<Color> Tint, Real32 Depth, UInt8 Stencil)
    {
        Vector<Ptr<ID3D11RenderTargetView>, kMaxAttachments> Attachments;

        for (ConstRef<D3D11Attachment> Attachment : mPasses[ID].Colors)
        {
            Attachments.emplace_back(Attachment.Accessor.Get());

            if (Target != Clear::Auxiliary)
            {
                mDeviceImmediate->ClearRenderTargetView(Attachments.back(), reinterpret_cast<ConstPtr<FLOAT>>(&Tint));
            }
        }

        if (Target != Clear::Color)
        {
            constexpr UINT Mode = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
            mDeviceImmediate->ClearDepthStencilView(mPasses[ID].Auxiliary.Get(), Mode, Depth, Stencil);
        }

        const Ptr<ID3D11DepthStencilView> DSAttachment = (mPasses[ID].Auxiliary ? mPasses[ID].Auxiliary.Get() : nullptr);
        mDeviceImmediate->OMSetRenderTargets(Attachments.size(), Attachments.data(), DSAttachment);

        const CD3D11_VIEWPORT Rect(
            Viewport.X,
            Viewport.Y,
            Viewport.Width,
            Viewport.Height,
            Viewport.MinDepth,
            Viewport.MaxDepth);
        mDeviceImmediate->RSSetViewports(1, &Rect);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Submit(ConstSpan<Submission> Submissions)
    {
        for (UInt32 Batch = 0; Batch < Submissions.size(); ++Batch)
        {
            ConstRef<Submission> NewestSubmission = Submissions[Batch];
            ConstRef<Submission> OldestSubmission = Batch > 0 ? Submissions[Batch - 1] : mDeviceStates;

            // Apply vertices
            ApplyVertexResources(OldestSubmission, NewestSubmission);

            // Apply indices
            if (OldestSubmission.Indices.Buffer != NewestSubmission.Indices.Buffer ||
                OldestSubmission.Indices.Offset != NewestSubmission.Indices.Offset ||
                OldestSubmission.Indices.Stride != NewestSubmission.Indices.Stride)
            {
                ConstRef<D3D11Buffer> Buffer = mBuffers[NewestSubmission.Indices.Buffer];
                const DXGI_FORMAT     Format =
                    NewestSubmission.Indices.Stride == 1 ? DXGI_FORMAT_R8_UINT  :
                    NewestSubmission.Indices.Stride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
                mDeviceImmediate->IASetIndexBuffer(Buffer.Resource.Get(), Format, NewestSubmission.Indices.Offset);
            }

            // Apply the scissor rect
            const Bool ScissorDirty = (OldestSubmission.Scissor.X      != NewestSubmission.Scissor.X)
                                      || (OldestSubmission.Scissor.Y      != NewestSubmission.Scissor.Y)
                                      || (OldestSubmission.Scissor.Width  != NewestSubmission.Scissor.Width)
                                      || (OldestSubmission.Scissor.Height != NewestSubmission.Scissor.Height);

            if (ScissorDirty)
            {
                const RECT Scissor = CD3D11_RECT(
                    NewestSubmission.Scissor.X,
                    NewestSubmission.Scissor.Y,
                    NewestSubmission.Scissor.X + NewestSubmission.Scissor.Width,
                    NewestSubmission.Scissor.Y + NewestSubmission.Scissor.Height);
                mDeviceImmediate->RSSetScissorRects(1, &Scissor);
            }

            // Apply pipeline or stencil value
            if (OldestSubmission.Pipeline != NewestSubmission.Pipeline)
            {
                ConstRef<D3D11Pipeline> Old = mPipelines[OldestSubmission.Pipeline];
                ConstRef<D3D11Pipeline> New = mPipelines[NewestSubmission.Pipeline];

                if (Old.VS != New.VS)
                {
                    mDeviceImmediate->VSSetShader(New.VS.Get(), nullptr, 0);
                }
                if (Old.PS != New.PS)
                {
                    mDeviceImmediate->PSSetShader(New.PS.Get(), nullptr, 0);
                }
                if (Old.GS != New.GS)
                {
                    mDeviceImmediate->GSSetShader(New.GS.Get(), nullptr, 0);
                }
                if (Old.BS != New.BS)
                {
                    mDeviceImmediate->OMSetBlendState(New.BS.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);
                }
                if (Old.DS != New.DS || OldestSubmission.Stencil != NewestSubmission.Stencil)
                {
                    mDeviceImmediate->OMSetDepthStencilState(New.DS.Get(), NewestSubmission.Stencil);
                }
                if (Old.RS != New.RS)
                {
                    mDeviceImmediate->RSSetState(New.RS.Get());
                }
                if (Old.IL != New.IL)
                {
                    mDeviceImmediate->IASetInputLayout(New.IL.Get());
                }
                if (Old.PT != New.PT)
                {
                    mDeviceImmediate->IASetPrimitiveTopology(New.PT);
                }
            }
            else if (OldestSubmission.Stencil != NewestSubmission.Stencil)
            {
                mDeviceImmediate->OMSetDepthStencilState(
                    mPipelines[NewestSubmission.Pipeline].DS.Get(), NewestSubmission.Stencil);
            }

            // Apply stage(s) resources
            ApplySamplerResources(OldestSubmission, NewestSubmission);
            ApplyTextureResources(OldestSubmission, NewestSubmission);
            ApplyUniformResources(OldestSubmission, NewestSubmission);

            // Issue draw command
            const UInt32 Count     = NewestSubmission.Command.Count;
            const UInt32 Offset    = NewestSubmission.Command.Offset;
            const SInt32 Base      = NewestSubmission.Command.Base;
            const UInt32 Instances = NewestSubmission.Command.Instances;

            if (NewestSubmission.Indices.Buffer)
            {
                if (Instances > 1)
                {
                    mDeviceImmediate->DrawIndexedInstanced(Count, Instances, Offset, Base, 0);
                }
                else
                {
                    mDeviceImmediate->DrawIndexed(Count, Offset, Base);
                }
            }
            else
            {
                if (Instances > 1)
                {
                    mDeviceImmediate->DrawInstanced(Count, Instances, Offset, 0);
                }
                else
                {
                    mDeviceImmediate->Draw(Count, Offset);
                }
            }
        }

        // Apply cache
        mDeviceStates = Submissions.back();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Commit(Object ID)
    {
        /// Resolve multisample color attachments into their corresponding single-sample targets.
        for (ConstRef<D3D11Attachment> Attachment : mPasses[ID].Colors)
        {
            if (Attachment.Source)
            {
                mDeviceImmediate->ResolveSubresource(
                    Attachment.Target.Get(), Attachment.TargetLevel,
                    Attachment.Source.Get(), Attachment.SourceLevel, Attachment.TargetFormat);
            }
        }

        /// Present the swap chain if this is the primary rendering pass.
        if (ID == kDisplay)
        {
            const UInt Interval = 0;
            const UInt Flag     = mProperties.Tearing ? DXGI_PRESENT_ALLOW_TEARING : 0;
            CheckIfFail(mDisplay->Present(Interval, Flag));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::LoadAdapters()
    {
        ComPtr<IDXGIAdapter1> DXGIAdapter;
        for (UInt Index = 0; SUCCEEDED(mDeviceFactory->EnumAdapters1(Index, DXGIAdapter.GetAddressOf())); ++Index)
        {
            DXGI_ADAPTER_DESC1 DXGIDescription;
            if (SUCCEEDED(DXGIAdapter->GetDesc1(&DXGIDescription)))
            {
                if ((DXGIDescription.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0)
                {
                    Ref<Adapter> AdapterInfo = mCapabilities.Adapters.emplace_back();

                    AdapterInfo.Description          = ConvertUTF16ToUTF8(DXGIDescription.Description);
                    AdapterInfo.DedicatedMemoryInMBs = DXGIDescription.DedicatedVideoMemory >> 20;
                    AdapterInfo.SharedMemoryInMBs    = DXGIDescription.SharedSystemMemory >> 20;
                    AdapterInfo.SystemMemoryInMBs    = DXGIDescription.DedicatedSystemMemory >> 20;
                }
            }

            if (ComPtr<IDXGIOutput> DXGIOutput; SUCCEEDED(DXGIAdapter->EnumOutputs(0, DXGIOutput.GetAddressOf())))
            {
                UINT        Length = 0;
                DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;

                if (SUCCEEDED(DXGIOutput->GetDisplayModeList(Format, 0, &Length, nullptr)))
                {
                    Vector<DXGI_MODE_DESC> Descriptions(Length);
                    CheckIfFail(DXGIOutput->GetDisplayModeList(Format, 0, &Length, Descriptions.data()));

                    for (Ref<DXGI_MODE_DESC> Description : Descriptions)
                    {
                        Ref<Resolution> Resolution = mCapabilities.Adapters[Index].Resolutions.emplace_back();
                        Resolution.Width     = Description.Width;
                        Resolution.Height    = Description.Height;
                        Resolution.Frequency = Description.RefreshRate.Numerator / Description.RefreshRate.Denominator;
                    }
                }
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::LoadCapabilities()
    {
        mCapabilities.Backend = Backend::D3D11;

        // Determine the supported shader model based on the Direct3D feature level.
        switch (mDevice->GetFeatureLevel())
        {
            case D3D_FEATURE_LEVEL_12_1:
            case D3D_FEATURE_LEVEL_12_0:
                mCapabilities.Language = Language::V6;
                break;
            case D3D_FEATURE_LEVEL_11_1:
            case D3D_FEATURE_LEVEL_11_0:
                mCapabilities.Language = Language::V5;
                break;
            case D3D_FEATURE_LEVEL_10_1:
            case D3D_FEATURE_LEVEL_10_0:
                mCapabilities.Language = Language::V4;
                break;
            case D3D_FEATURE_LEVEL_9_3:
                mCapabilities.Language = Language::V3;
                break;
            case D3D_FEATURE_LEVEL_9_2:
                mCapabilities.Language = Language::V2;
                break;
            case D3D_FEATURE_LEVEL_9_1:
                mCapabilities.Language = Language::V1;
                break;
            default:
                break;
        }

        // Check if we support tearing mode
        ComPtr<IDXGIFactory5> DXGIFactory5;
        if (SUCCEEDED(mDeviceFactory.As<IDXGIFactory5>(& DXGIFactory5)))
        {
            BOOL AllowAdaptive = FALSE;
            CheckIfFail(DXGIFactory5->CheckFeatureSupport(
                DXGI_FEATURE_PRESENT_ALLOW_TEARING, & AllowAdaptive, sizeof(AllowAdaptive)));
            mProperties.Tearing = AllowAdaptive;
        }

        // Query supported multisample anti-aliasing (MSAA) levels for the device.
        for (UInt32 Level = 1, Last = 1; Level <= kMaxSamples; ++Level)
        {
            UInt32 Quality = 0;

            // TODO Check other formats
            const HRESULT Result = mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, Level, &Quality);

            if (SUCCEEDED(Result) && Quality > 0)
            {
                mProperties.Multisample[Level] = DXGI_SAMPLE_DESC { Level, Quality - 1 };
            }
            else
            {
                mProperties.Multisample[Level] = mProperties.Multisample[Last];
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::LoadStates()
    {
        mDeviceStates = Submission();
        mDeviceStates.Scissor = Scissor(0, 0, 0, 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateSwapchain(Ref<D3D11Pass> Pass, UInt Display, UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        const UINT Flags = (mProperties.Tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

        DXGI_SWAP_CHAIN_DESC Description { };
        Description.BufferCount       = 2;
        Description.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Description.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | Flags;
        Description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Description.BufferDesc.Width  = Width;
        Description.BufferDesc.Height = Height;
        Description.SampleDesc        = { 1, 0 };
        Description.OutputWindow      = reinterpret_cast<HWND>(Display);
        Description.Windowed          = true;

        ComPtr<IDXGIFactory4> DXGIFactory4;
        if (SUCCEEDED(mDeviceFactory.As<IDXGIFactory4>(&DXGIFactory4)))
        {
            Description.SwapEffect = mProperties.Tearing ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
        }
        else
        {
            Description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        }

        CheckIfFail(mDeviceFactory->CreateSwapChain(mDevice.Get(), &Description, mDisplay.GetAddressOf()));
        CheckIfFail(mDeviceFactory->MakeWindowAssociation(Description.OutputWindow, DXGI_MWA_NO_WINDOW_CHANGES));

        CreateSwapchainResources(Pass, Width, Height, Samples);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        Ref<D3D11Attachment> Attachment = Pass.Colors.emplace_back();
        Attachment.SourceLevel  = 0;
        Attachment.TargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        Attachment.TargetLevel  = 0;

        // Acquire the swapchain back-buffer and create a render target view for it.
        CheckIfFail(mDisplay->GetBuffer(
            0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(Attachment.Target.GetAddressOf())));

        // Create an MSAA render target if multisampling is enabled.
        if (Samples > 1)
        {
            D3D11_TEXTURE2D_DESC ColorTextureDescription { };
            ColorTextureDescription.Width      = Width;
            ColorTextureDescription.Height     = Height;
            ColorTextureDescription.MipLevels  = 1;
            ColorTextureDescription.ArraySize  = 1;
            ColorTextureDescription.Format     = DXGI_FORMAT_R8G8B8A8_UNORM;
            ColorTextureDescription.SampleDesc = mProperties.Multisample[Samples];
            ColorTextureDescription.Usage      = D3D11_USAGE_DEFAULT;
            ColorTextureDescription.BindFlags  = D3D11_BIND_RENDER_TARGET;

            CheckIfFail(mDevice->CreateTexture2D(
                &ColorTextureDescription, nullptr, reinterpret_cast<ID3D11Texture2D **>(Attachment.Source.GetAddressOf())));

            const CD3D11_RENDER_TARGET_VIEW_DESC ColorViewDescription(
                D3D11_RTV_DIMENSION_TEXTURE2DMS, DXGI_FORMAT_R8G8B8A8_UNORM);
            CheckIfFail(mDevice->CreateRenderTargetView(
                Attachment.Source.Get(), &ColorViewDescription, Attachment.Accessor.GetAddressOf()));
        }
        else
        {
            CheckIfFail(mDevice->CreateRenderTargetView(Attachment.Target.Get(), nullptr, Attachment.Accessor.GetAddressOf()));
        }

        // Create a depth-stencil buffer matching the swapchain dimensions and sample count.
        D3D11_TEXTURE2D_DESC DepthTextureDescription { };
        DepthTextureDescription.Width      = Width;
        DepthTextureDescription.Height     = Height;
        DepthTextureDescription.MipLevels  = 1;
        DepthTextureDescription.ArraySize  = 1;
        DepthTextureDescription.Format     = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DepthTextureDescription.SampleDesc = mProperties.Multisample[Samples];
        DepthTextureDescription.Usage      = D3D11_USAGE_DEFAULT;
        DepthTextureDescription.BindFlags  = D3D11_BIND_DEPTH_STENCIL;

        ComPtr<ID3D11Texture2D> Depth;
        CheckIfFail(mDevice->CreateTexture2D(&DepthTextureDescription, nullptr, Depth.GetAddressOf()));
        CheckIfFail(mDevice->CreateDepthStencilView(Depth.Get(), nullptr, Pass.Auxiliary.GetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<D3D11Driver::D3D11Sampler> D3D11Driver::GetOrCreateSampler(ConstRef<Sampler> Descriptor)
    {
        Ref<D3D11Sampler> Sampler = mSamplers[
            static_cast<UInt32>(Descriptor.WrapModeU)       |
            static_cast<UInt32>(Descriptor.WrapModeV)  << 2 |
            static_cast<UInt32>(Descriptor.Filter)     << 4];

        if (Sampler.Resource == nullptr)
        {
            UINT Anisotropic;

            switch (Descriptor.Filter)
            {
                case TextureFilter::Anisotropic2x:
                    Anisotropic = 2;
                    break;
                case TextureFilter::Anisotropic4x:
                    Anisotropic = 4;
                    break;
                case TextureFilter::Anisotropic8x:
                    Anisotropic = 8;
                    break;
                case TextureFilter::Anisotropic16x:
                    Anisotropic = 16;
                    break;
                default:
                    Anisotropic = 0;
                    break;
            }

            const CD3D11_SAMPLER_DESC SamplerDescriptor(
                As(Descriptor.Filter),
                As(Descriptor.WrapModeU),
                As(Descriptor.WrapModeV),
                D3D11_TEXTURE_ADDRESS_CLAMP,
                0,
                Anisotropic,
                D3D11_COMPARISON_NEVER,
                nullptr,
                -FLT_MAX,
                +FLT_MAX);

            CheckIfFail(mDevice->CreateSamplerState(&SamplerDescriptor, Sampler.Resource.GetAddressOf()));
        }
        return Sampler;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyVertexResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        Ptr<ID3D11Buffer> Array[kMaxFetches];
        UINT ArrayOffset[kMaxFetches];
        UINT ArrayStride[kMaxFetches];
        UInt Min = kMaxFetches;
        UInt Max = 0u;

        for (UInt Element = 0; Element < kMaxFetches; ++Element)
        {
            ConstRef<Stream> Old = Oldest.Vertices[Element];
            ConstRef<Stream> New = Newest.Vertices[Element];

            if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Stride != New.Stride)
            {
                Min = Base::Min(Element, Min);
                Max = Base::Max(Element + 1, Max);
            }

            Array[Element]       = mBuffers[New.Buffer].Resource.Get();
            ArrayOffset[Element] = New.Offset;
            ArrayStride[Element] = New.Stride;
        }

        if (Min != kMaxFetches && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->IASetVertexBuffers(Min, Count, Array + Min, ArrayStride + Min, ArrayOffset + Min);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplySamplerResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        Ptr<ID3D11SamplerState> Array[kMaxSlots];
        UInt Min = kMaxSlots;
        UInt Max = 0u;

        for (UInt Element = 0; Element < kMaxSlots; ++Element)
        {
            ConstRef<D3D11Sampler> PrevSampler = GetOrCreateSampler(Oldest.Samplers[Element]);
            ConstRef<D3D11Sampler> NextSampler = GetOrCreateSampler(Newest.Samplers[Element]);

            if (PrevSampler.Resource != NextSampler.Resource)
            {
                Min = Base::Min(Element, Min);
                Max = Base::Max(Element + 1, Max);
            }
            Array[Element] = NextSampler.Resource.Get();
        }

        if (Min != kMaxSlots && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->PSSetSamplers(Min, Count, Array + Min); // TODO: Do we also need other stages?
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyTextureResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        Ptr<ID3D11ShaderResourceView> Array[kMaxSlots];
        UInt Min = kMaxSlots;
        UInt Max = 0u;

        for (UInt Element = 0; Element < kMaxSlots && Newest.Textures[Element] != 0; ++Element)
        {
            if (Oldest.Textures[Element] != Newest.Textures[Element])
            {
                Min = Base::Min(Element, Min);
                Max = Base::Max(Element + 1, Max);
            }
            Array[Element] = mTextures[Newest.Textures[Element]].Accessor.Get();
        }

        if (Min != kMaxSlots && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->PSSetShaderResources(Min, Count, Array + Min);    // TODO: Do we also need other stages?
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyUniformResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        Ptr<ID3D11Buffer> Array[kMaxUniforms];
        UINT ArrayOffset[kMaxUniforms];
        UINT ArrayLength[kMaxUniforms];
        UInt Min = kMaxUniforms;
        UInt Max = 0u;

        for (UInt Element = 0; Element < kMaxUniforms; ++Element)
        {
            ConstRef<Stream> Old = Oldest.Uniforms[Element];
            ConstRef<Stream> New = Newest.Uniforms[Element];

            if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Stride != New.Stride)
            {
                Min = Base::Min(Element, Min);
                Max = Base::Max(Element + 1, Max);
            }

            Array[Element]       = mBuffers[New.Buffer].Resource.Get();
            ArrayOffset[Element] = New.Offset / 16;
            ArrayLength[Element] = New.Stride / 16;
        }

        if (Min != kMaxUniforms && Max > 0)
        {
            const UInt Count = Max - Min;

            mDeviceImmediate->VSSetConstantBuffers1(Min, Count, Array + Min, ArrayOffset + Min, ArrayLength + Min);
            mDeviceImmediate->PSSetConstantBuffers1(Min, Count, Array + Min, ArrayOffset + Min, ArrayLength + Min);
        }
    }
}