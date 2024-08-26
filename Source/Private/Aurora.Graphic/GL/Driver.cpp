// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(BlendFactor Value)
    {
        static constexpr UInt k_Mapping[] = {
            GL_ZERO,                       // BlendFactor::Zero
            GL_ONE,                        // BlendFactor::One
            GL_SRC_COLOR,                  // BlendFactor::SrcColor
            GL_ONE_MINUS_SRC_COLOR,        // BlendFactor::OneMinusSrcColor
            GL_SRC_ALPHA,                  // BlendFactor::SrcAlpha
            GL_ONE_MINUS_SRC_ALPHA,        // BlendFactor::OneMinusSrcAlpha
            GL_DST_COLOR,                  // BlendFactor::DstColor
            GL_ONE_MINUS_DST_COLOR,        // BlendFactor::OneMinusDstColor
            GL_DST_ALPHA,                  // BlendFactor::DstAlpha
            GL_ONE_MINUS_DST_ALPHA         // BlendFactor::OneMinusDstAlpha
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(BlendFunction Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_FUNC_ADD,                        // BlendFunction::Add
            GL_FUNC_SUBTRACT,                   // BlendFunction::Subtract
            GL_FUNC_REVERSE_SUBTRACT,           // BlendFunction::ReverseSubtract
            GL_MIN,                             // BlendFunction::Min
            GL_MAX                              // BlendFunction::Max
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Clear Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_COLOR_BUFFER_BIT,                                                    // Clear::Color
            GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,                            // Clear::Auxiliary
            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,      // Clear::All
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Cull Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_NONE,                            // Cull::None
            GL_BACK,                            // Cull::None
            GL_FRONT,                           // Cull::Front
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TestAction Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_KEEP,                  // TestAction::Keep
            GL_REPLACE,               // TestAction::Replace
            GL_ZERO,                  // TestAction::Zero
            GL_DECR,                  // TestAction::Decrease
            GL_DECR_WRAP,             // TestAction::DecreaseOrWrap
            GL_INVERT,                // TestAction::Invert
            GL_INCR,                  // TestAction::Increase
            GL_INCR_WRAP              // TestAction::IncreaseOrWrap
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TestCondition Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_ALWAYS,                  // TestCondition::Always
            GL_NEVER,                   // TestCondition::Never
            GL_GREATER,                 // TestCondition::Greater
            GL_GEQUAL,                  // TestCondition::GreaterEqual
            GL_EQUAL,                   // TestCondition::Equal
            GL_NOTEQUAL,                // TestCondition::NotEqual
            GL_LESS,                    // TestCondition::Less
            GL_LEQUAL                   // TestCondition::LessEqual
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TextureEdge Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_CLAMP_TO_EDGE,            // TextureEdge::Clamp
            GL_REPEAT,                   // TextureEdge::Repeat
            GL_MIRRORED_REPEAT           // TextureEdge::Mirror
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TextureFilter Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_NEAREST,                 // TextureFilter::Nearest
            GL_LINEAR,                  // TextureFilter::Bilinear
            GL_LINEAR_MIPMAP_LINEAR,    // TextureFilter::Trilinear
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    template<UInt Data>
    static auto As(TextureFormat Value)
    {
        constexpr static std::tuple<UInt, UInt, UInt, UInt> k_Mapping[] =
        {
            //            Internal                                  Format                     Type               Depth
            { GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,       GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,       GL_NONE,              64 }, // TextureFormat::BC1UIntNorm
            { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_NONE,              64 }, // TextureFormat::BC1UIntNorm_sRGB
            { GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,       GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,       GL_NONE,             128 }, // TextureFormat::BC2UIntNorm
            { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_NONE,             128 }, // TextureFormat::BC2UIntNorm_sRGB
            { GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,       GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,       GL_NONE,             128 }, // TextureFormat::BC3UIntNorm
            { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_NONE,             128 }, // TextureFormat::BC3UIntNorm_sRGB
            { GL_COMPRESSED_RED_RGTC1_EXT,            GL_COMPRESSED_RED_RGTC1_EXT,            GL_NONE,              64 }, // TextureFormat::BC4UIntNorm
            { GL_COMPRESSED_RED_GREEN_RGTC2_EXT,      GL_COMPRESSED_RED_GREEN_RGTC2_EXT,      GL_NONE,             128 }, // TextureFormat::BC5UIntNorm
            { GL_R8I,                                 GL_RED,                                 GL_BYTE,               8 }, // TextureFormat::R8SInt
            { GL_R8_SNORM,                            GL_RED,                                 GL_BYTE,               8 }, // TextureFormat::R8SIntNorm
            { GL_R8UI,                                GL_RED_INTEGER,                         GL_UNSIGNED_BYTE,      8 }, // TextureFormat::R8UInt
            { GL_R8,                                  GL_RED,                                 GL_UNSIGNED_BYTE,      8 }, // TextureFormat::R8UIntNorm
            { GL_R16I,                                GL_RED,                                 GL_SHORT,             16 }, // TextureFormat::R16SInt
            { GL_R16_SNORM,                           GL_RED,                                 GL_SHORT,             16 }, // TextureFormat::R16SIntNorm
            { GL_R16UI,                               GL_RED_INTEGER,                         GL_UNSIGNED_SHORT,    16 }, // TextureFormat::R16UInt
            { GL_R16,                                 GL_RED,                                 GL_UNSIGNED_SHORT,    16 }, // TextureFormat::R16UIntNorm
            { GL_R16F,                                GL_RED,                                 GL_HALF_FLOAT,        16 }, // TextureFormat::R16Float
            { GL_R32I,                                GL_RED,                                 GL_INT,               32 }, // TextureFormat::R32SInt
            { GL_R32UI,                               GL_RED_INTEGER,                         GL_UNSIGNED_INT,      32 }, // TextureFormat::R32UInt
            { GL_R32F,                                GL_RED,                                 GL_FLOAT,             32 }, // TextureFormat::R32Float
            { GL_RG8,                                 GL_RG,                                  GL_UNSIGNED_BYTE,     16 }, // TextureFormat::RG8SInt
            { GL_RG8_SNORM,                           GL_RG,                                  GL_BYTE,              16 }, // TextureFormat::RG8SIntNorm
            { GL_RG8UI,                               GL_RG_INTEGER,                          GL_UNSIGNED_BYTE,     16 }, // TextureFormat::RG8UInt
            { GL_RG8,                                 GL_RG,                                  GL_UNSIGNED_BYTE,     16 }, // TextureFormat::RG8UIntNorm
            { GL_RG16I,                               GL_RG,                                  GL_SHORT,             32 }, // TextureFormat::RG16SInt
            { GL_RG16_SNORM,                          GL_RG,                                  GL_SHORT,             32 }, // TextureFormat::RG16SIntNorm
            { GL_RG16UI,                              GL_RG_INTEGER,                          GL_UNSIGNED_SHORT,    32 }, // TextureFormat::RG16UInt
            { GL_RG16,                                GL_RG,                                  GL_UNSIGNED_SHORT,    32 }, // TextureFormat::RG16UIntNorm
            { GL_RG16F,                               GL_RG,                                  GL_HALF_FLOAT,        32 }, // TextureFormat::RG16Float
            { GL_RG32I,                               GL_RG,                                  GL_INT,               64 }, // TextureFormat::RG32SInt
            { GL_RG32UI,                              GL_RG_INTEGER,                          GL_UNSIGNED_INT,      64 }, // TextureFormat::RG32UInt
            { GL_RG32F,                               GL_RG,                                  GL_FLOAT,             64 }, // TextureFormat::RG32Float
            { GL_RGB32I,                              GL_RGB,                                 GL_INT,               96 }, // TextureFormat::RGB32SInt
            { GL_RGB32UI,                             GL_RGB_INTEGER,                         GL_UNSIGNED_INT,      96 }, // TextureFormat::RGB32UInt
            { GL_RGB32F,                              GL_RGB,                                 GL_FLOAT,             96 }, // TextureFormat::RGB32Float
            { GL_RGBA8I,                              GL_RGBA,                                GL_INT,               32 }, // TextureFormat::RGBA8SInt
            { GL_RGBA8_SNORM,                         GL_RGBA,                                GL_BYTE,              32 }, // TextureFormat::RGBA8SIntNorm
            { GL_RGBA8UI,                             GL_RGBA_INTEGER,                        GL_UNSIGNED_BYTE,     32 }, // TextureFormat::RGBA8UInt
            { GL_RGBA8,                               GL_RGBA,                                GL_UNSIGNED_BYTE,     32 }, // TextureFormat::RGBA8UIntNorm
            { GL_SRGB8_ALPHA8,                        GL_RGBA,                                GL_UNSIGNED_BYTE,     32 }, // TextureFormat::RGBA8UIntNorm_sRGB
            { GL_BGRA8_EXT,                           GL_BGRA,                                GL_UNSIGNED_BYTE,     32 }, // TextureFormat::BGRA8UIntNorm
            { GL_SRGB8_ALPHA8,                        GL_BGRA,                                GL_UNSIGNED_BYTE,     32 }, // TextureFormat::BGRA8UIntNorm_sRGB
            { GL_RGBA16I,                             GL_RGBA,                                GL_SHORT,             64 }, // TextureFormat::RGBA16SInt
            { GL_RGBA16_SNORM,                        GL_RGBA,                                GL_SHORT,             64 }, // TextureFormat::RGBA16SIntNorm
            { GL_RGBA16UI,                            GL_RGBA_INTEGER,                        GL_UNSIGNED_SHORT,    64 }, // TextureFormat::RGBA16UInt
            { GL_RGBA16,                              GL_RGBA,                                GL_UNSIGNED_SHORT,    64 }, // TextureFormat::RGBA16UIntNorm
            { GL_RGBA16F,                             GL_RGBA,                                GL_HALF_FLOAT,        64 }, // TextureFormat::RGBA16Float
            { GL_RGBA32I,                             GL_RGBA,                                GL_INT,              128 }, // TextureFormat::RGBA32SInt
            { GL_RGBA32UI,                            GL_RGBA_INTEGER,                        GL_UNSIGNED_INT,     128 }, // TextureFormat::RGBA32UInt
            { GL_RGBA32F,                             GL_RGBA,                                GL_FLOAT,            128 }, // TextureFormat::RGBA32Float
            { GL_DEPTH_COMPONENT32F,                  GL_DEPTH_COMPONENT,                     GL_FLOAT,             32 }, // TextureFormat::D32Float
            { GL_DEPTH_COMPONENT16,                   GL_DEPTH_COMPONENT,                     GL_UNSIGNED_SHORT,    16 }, // TextureFormat::D16X0UIntNorm
            { GL_DEPTH_COMPONENT24,                   GL_DEPTH_COMPONENT,                     GL_UNSIGNED_INT,      24 }, // TextureFormat::D24X0UIntNorm
            { GL_DEPTH24_STENCIL8,                    GL_DEPTH_STENCIL,                       GL_UNSIGNED_INT_24_8, 32 }, // TextureFormat::D24S8UIntNorm
            { GL_DEPTH_COMPONENT32,                   GL_DEPTH_COMPONENT,                     GL_UNSIGNED_INT,      32 }, // TextureFormat::D32X0UIntNorm
        };
        return std::get<Data>(k_Mapping[CastEnum(Value)]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Usage Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_ARRAY_BUFFER,           // Usage::Vertex
            GL_ELEMENT_ARRAY_BUFFER,   // Usage::Index
            GL_UNIFORM_BUFFER,         // Usage::Uniform
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(VertexFormat Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_HALF_FLOAT,              // VertexFormat::Float16x2
            GL_HALF_FLOAT,              // VertexFormat::Float16x4
            GL_FLOAT,                   // VertexFormat::Float32x1
            GL_FLOAT,                   // VertexFormat::Float32x2
            GL_FLOAT,                   // VertexFormat::Float32x3
            GL_FLOAT,                   // VertexFormat::Float32x4
            GL_BYTE,                    // VertexFormat::SInt8x4
            GL_BYTE,                    // VertexFormat::SIntNorm8x4
            GL_UNSIGNED_BYTE,           // VertexFormat::UInt8x4
            GL_UNSIGNED_BYTE,           // VertexFormat::UIntNorm8x4
            GL_SHORT,                   // VertexFormat::SInt16x2
            GL_SHORT,                   // VertexFormat::SIntNorm16x2
            GL_UNSIGNED_SHORT,          // VertexFormat::UInt16x2
            GL_UNSIGNED_SHORT,          // VertexFormat::UIntNorm16x2
            GL_SHORT,                   // VertexFormat::SInt16x4
            GL_SHORT,                   // VertexFormat::SIntNorm16x4
            GL_UNSIGNED_SHORT,          // VertexFormat::UInt16x4
            GL_UNSIGNED_SHORT,          // VertexFormat::UIntNorm16x4
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(VertexTopology Value)
    {
        constexpr static UInt k_Mapping[] = {
            GL_POINTS,       // VertexTopology::Point
            GL_LINES,        // VertexTopology::Line
            GL_TRIANGLES,    // VertexTopology::Triangle
        };
        return k_Mapping[CastEnum(Value)];
    }


    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLDriver::Initialize(Ptr<SDL_Window> Swapchain, UInt32 Width, UInt32 Height)
    {
        LoadCapabilities();

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::Reset(UInt Width, UInt Height)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<const Capabilities> GLDriver::GetCapabilities() const
    {
        return mCapabilities;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::CreateBuffer(Object ID, Usage Type, UInt Capacity, CPtr<UInt8> Data)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<UInt8> Data)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::DeleteBuffer(Object ID)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::CreatePass(Object ID, CPtr<Object> Colors, Object Auxiliary)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::DeletePass(Object ID)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::CreatePipeline(Object ID, CPtr<UInt8> Vertex, CPtr<UInt8> Pixel, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::DeletePipeline(Object ID)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt8> Data)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt8> Data)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data GLDriver::ReadTexture(Object ID, UInt Level, Recti Offset)
    {
        return Data();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt GLDriver::QueryTexture(Object ID)
    {
        return 0;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::DeleteTexture(Object ID)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::Submit(CPtr<Submission> Submissions)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::Commit(Object ID, Bool Synchronised)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLDriver::LoadCapabilities()
    {
        mCapabilities.Backend = Backend::GL;
    }
}

