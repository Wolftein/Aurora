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

#define GLAD_GL_IMPLEMENTATION
#include "GLES3Driver.hpp"

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
            GL_BACK,                            // Cull::Back
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
            GL_DECR_WRAP,             // TestAction::DecreaseSaturate
            GL_INVERT,                // TestAction::Invert
            GL_INCR,                  // TestAction::Increase
            GL_INCR_WRAP              // TestAction::IncreaseSaturate
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

    template<UInt Data>
    static auto As(VertexFormat Value)
    {
        constexpr static std::tuple<UInt, UInt, UInt> k_Mapping[] = {
            //   Format          // Size      // Normalized
            { GL_HALF_FLOAT,         2,         GL_FALSE },     // VertexFormat::Float16x2
            { GL_HALF_FLOAT,         4,         GL_FALSE },     // VertexFormat::Float16x4
            { GL_FLOAT,              1,         GL_FALSE },     // VertexFormat::Float32x1
            { GL_FLOAT,              2,         GL_FALSE },     // VertexFormat::Float32x2
            { GL_FLOAT,              3,         GL_FALSE },     // VertexFormat::Float32x3
            { GL_FLOAT,              4,         GL_FALSE },     // VertexFormat::Float32x4
            { GL_BYTE,               4,         GL_FALSE },     // VertexFormat::SInt8x4
            { GL_BYTE,               4,         GL_TRUE  },     // VertexFormat::SIntNorm8x4
            { GL_UNSIGNED_BYTE,      4,         GL_FALSE },     // VertexFormat::UInt8x4
            { GL_UNSIGNED_BYTE,      4,         GL_TRUE  },     // VertexFormat::UIntNorm8x4
            { GL_SHORT,              2,         GL_FALSE },     // VertexFormat::SInt16x2
            { GL_SHORT,              2,         GL_TRUE  },     // VertexFormat::SIntNorm16x2
            { GL_UNSIGNED_SHORT,     2,         GL_FALSE },     // VertexFormat::UInt16x2
            { GL_UNSIGNED_SHORT,     2,         GL_TRUE  },     // VertexFormat::UIntNorm16x2
            { GL_SHORT,              4,         GL_FALSE },     // VertexFormat::SInt16x4
            { GL_SHORT,              4,         GL_TRUE  },     // VertexFormat::SIntNorm16x4
            { GL_UNSIGNED_SHORT,     4,         GL_FALSE },     // VertexFormat::UInt16x4
            { GL_UNSIGNED_SHORT,     4,         GL_TRUE  },     // VertexFormat::UIntNorm16x4
        };
        return std::get<Data>(k_Mapping[CastEnum(Value)]);
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

    static auto AsIndexFormat(UInt32 Stride)
    {
        switch (Stride)
        {
        case sizeof(UInt8):
            return GL_UNSIGNED_BYTE;
        case sizeof(UInt16):
            return GL_UNSIGNED_SHORT;
        default:
            return GL_UNSIGNED_INT;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLES3Driver::Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        mDevice  = Swapchain;
        mContext = SDL_GL_CreateContext(Swapchain);

#if defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_EMSCRIPTEN)
        gladLoadGLES2(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
#else
        gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
#endif

        LoadCapabilities();
        LoadDefaults();

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::Reset(UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        glViewport(0, 0, Width, Height);
        // @TODO Multisample
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    ConstRef<Capabilities> GLES3Driver::GetCapabilities() const
    {
        return mCapabilities;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CreateBuffer(Object ID, Usage Type, Bool Immutable, Ptr<const UInt8> Data, UInt32 Length)
    {
        Ref<GLES3Buffer> Buffer = mBuffers[ID];
        Buffer.Type             = As(Type);

        glGenBuffers(1, AddressOf(Buffer.ID));
        glBindBuffer(Buffer.Type, Buffer.ID);
        glBufferData(Buffer.Type, Length, Data, Immutable ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size)
    {
        glBindBuffer(GL_COPY_READ_BUFFER,  mBuffers[SrcBuffer].ID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, mBuffers[DstBuffer].ID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, SrcOffset, DstOffset, Size);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, CPtr<const UInt8> Data)
    {
        const UInt32 Access = GL_MAP_WRITE_BIT | (Discard ? GL_MAP_INVALIDATE_BUFFER_BIT : GL_MAP_UNSYNCHRONIZED_BIT);

        ConstRef<GLES3Buffer> Buffer = mBuffers[ID];
        glBindBuffer(Buffer.Type, Buffer.ID);

        if (Ptr<void> Memory = glMapBufferRange(Buffer.Type, Offset, Data.size(), Access))
        {
            std::memcpy(Memory, Data.data(), Data.size());
            glUnmapBuffer(Buffer.Type);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::DeleteBuffer(Object ID)
    {
        mBuffers[ID].~GLES3Buffer();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CreatePass(Object ID, CPtr<const Attachment> Colors, CPtr<const Attachment> Resolves, ConstRef<Attachment> Auxiliary)
    {
        // TODO FBO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::DeletePass(Object ID)
    {
        mPasses[ID].~GLES3Pass();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CreatePipeline(Object ID, CPtr<const UInt8> Vertex, CPtr<const UInt8> Pixel, CPtr<const UInt8> Geometry, ConstRef<Descriptor> Properties)
    {
        Ref<GLES3Pipeline> Pipeline  = mPipelines[ID];

        // Compile all shader(s) into the Pipeline's program and link them.
        Pipeline.Program = glCreateProgram();

        Compile(Pipeline.Program, GL_VERTEX_SHADER, Vertex);
        Compile(Pipeline.Program, GL_FRAGMENT_SHADER, Pixel);

        if (!Geometry.empty())
        {
            Compile(Pipeline.Program, GL_GEOMETRY_SHADER, Geometry);
        }

        glLinkProgram(Pipeline.Program);

        // Check if we linked the program with all shader(s)
        GLint Success;
        glGetProgramiv(Pipeline.Program, GL_LINK_STATUS, AddressOf(Success));
        if (!Success)
        {
            GLchar Error[512];
            glGetProgramInfoLog(Pipeline.Program, sizeof(Error), nullptr, Error);
            Log::Critical("GLES3Driver: Failed to link program {}", Error);
        }

        // Pre-compile all state properties.
        Pipeline.Cull                = As(Properties.Cull);
        Pipeline.Fill                = Properties.Fill ? GL_FILL : GL_LINE;
        Pipeline.BlendEnabled        = !(
                   Properties.BlendColorSrcFactor == BlendFactor::One
                && Properties.BlendAlphaSrcFactor == BlendFactor::One
                && Properties.BlendColorDstFactor == BlendFactor::Zero
                && Properties.BlendAlphaDstFactor == BlendFactor::Zero
                && Properties.BlendColorEquation  == BlendFunction::Add
                && Properties.BlendAlphaEquation  == BlendFunction::Add);
        Pipeline.BlendMask           = static_cast<UInt32>(Properties.BlendMask);
        Pipeline.BlendColorSrcFactor = As(Properties.BlendColorSrcFactor);
        Pipeline.BlendColorDstFactor = As(Properties.BlendColorDstFactor);
        Pipeline.BlendColorEquation  = As(Properties.BlendColorEquation);
        Pipeline.BlendAlphaSrcFactor = As(Properties.BlendAlphaSrcFactor);
        Pipeline.BlendAlphaDstFactor = As(Properties.BlendAlphaDstFactor);
        Pipeline.BlendAlphaEquation  = As(Properties.BlendAlphaEquation);
        Pipeline.DepthEnabled        = Properties.DepthCondition != TestCondition::Always || Properties.DepthMask;
        Pipeline.DepthMask           = Properties.DepthMask;
        Pipeline.DepthCondition      = As(Properties.DepthCondition);
        Pipeline.StencilEnabled      = Properties.StencilCondition   != TestCondition::Always
                                    || Properties.StencilOnFail      != TestAction::Keep
                                    || Properties.StencilOnDepthFail != TestAction::Keep
                                    || Properties.StencilOnDepthPass != TestAction::Keep;
        Pipeline.StencilMask         = Properties.StencilMask;
        Pipeline.StencilCondition    = As(Properties.StencilCondition);
        Pipeline.StencilOnFail       = As(Properties.StencilOnFail);
        Pipeline.StencilOnZFail      = As(Properties.StencilOnDepthFail);
        Pipeline.StencilOnZPass      = As(Properties.StencilOnDepthPass);
        Pipeline.InputTopology       = As(Properties.InputTopology);

        for (UInt32 Count = 0; Count < k_MaxAttributes && Properties.InputLayout[Count].ID != VertexSemantic::None; ++Count)
        {
            Pipeline.InputAttributes[Count] = Properties.InputLayout[Count];
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::DeletePipeline(Object ID)
    {
        mPipelines[ID].~GLES3Pipeline();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Level, UInt8 Samples, CPtr<const UInt8> Data)
    {
        Ref<GLES3Texture> Texture = mTextures[ID];
        Texture.Format            = As<1>(Format);
        Texture.Type              = As<2>(Format);

        ConstPtr<UInt8> Bytes = Data.data();
        const UInt32 Kind     = As<0>(Format);
        const UInt32 Depth    = As<3>(Format);

        glGenTextures(1, AddressOf(Texture.ID));
        glBindTexture(GL_TEXTURE_2D, Texture.ID);

        for (UInt8 Index = 0; Index < Level; ++Index)
        {
            // @TODO: Compressed format(s)
            glTexImage2D(GL_TEXTURE_2D, Index, Kind, Width, Height, 0, Texture.Format, Texture.Type, Bytes);

            Bytes += (Width * Depth * Height);
            Width  = Max(1, Width  >> 1);
            Height = Max(1, Height >> 1);
        }

        if (Level == 0)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::UpdateTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset, UInt32 Pitch, CPtr<const UInt8> Data)
    {
        ConstRef<GLES3Texture> Texture = mTextures[ID];

        glBindTexture(GL_TEXTURE_2D, Texture.ID);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, Pitch);
        // @TODO: Compressed format(s)
        glTexSubImage2D(
                GL_TEXTURE_2D,
                Level,
                Offset.GetLeft(),
                Offset.GetTop(),
                Offset.GetWidth(),
                Offset.GetHeight(),
                Texture.Format,
                Texture.Type,
                Data.data());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::CopyTexture(Object DstTexture, UInt8 DstLevel, ConstRef<Vector2i> DstOffset, Object SrcTexture, UInt8 SrcLevel, ConstRef<Recti> SrcOffset)
    {
        // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data GLES3Driver::ReadTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset)
    {
        return Data();  // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::DeleteTexture(Object ID)
    {
        mTextures[ID].~GLES3Texture();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::Prepare(Object ID, ConstRef<Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil)
    {
        // @TODO FBO
        // @TODO Multisample
        // @TODO Cache
        glClearColor(Tint.GetRed(), Tint.GetGreen(), Tint.GetBlue(), Tint.GetAlpha());

#if defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_EMSCRIPTEN)
        glClearDepthf(Depth);
#else
        glClearDepth(Depth);
#endif

        glClearStencil(Stencil);
        glScissor(0, 0, Viewport.GetWidth(), Viewport.GetHeight());
        glClear(As(Target));

        if (mViewport != Viewport)
        {
            mViewport = Viewport;
            glViewport(Viewport.GetLeft(), Viewport.GetTop(), Viewport.GetWidth(), Viewport.GetHeight());
        }

        mSubmission.Scissor.Set(0, 0, Viewport.GetWidth(), Viewport.GetHeight());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::Submit(CPtr<const Submission> Submissions)
    {
        // Apply all job(s).
        for (UInt32 Batch = 0; Batch < Submissions.size(); ++Batch)
        {
            ConstRef<Submission> NewestSubmission = Submissions[Batch];
            ConstRef<Submission> OldestSubmission = Batch > 0 ? Submissions[Batch - 1] : mSubmission;

            // Apply the scissor rect
            if (OldestSubmission.Scissor != NewestSubmission.Scissor)
            {
                glScissor(NewestSubmission.Scissor.GetLeft(),
                          mViewport.GetHeight() - NewestSubmission.Scissor.GetBottom(),
                          NewestSubmission.Scissor.GetWidth(),
                          NewestSubmission.Scissor.GetHeight());
            }

            // Apply pipeline
            ApplyPipeline(OldestSubmission, NewestSubmission);

            // Apply stage(s) resources
            ApplyResources(OldestSubmission, NewestSubmission);

            // Apply vertices
            ApplyVertex(OldestSubmission, NewestSubmission);

            // Apply indices
            if (OldestSubmission.Indices.Buffer != NewestSubmission.Indices.Buffer)
            {
                Ref<const GLES3Buffer> Buffer = mBuffers[NewestSubmission.Indices.Buffer];
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer.ID);
            }

            // Issue draw command
            const UInt32 Count     = NewestSubmission.Primitive.Count;
            const UInt32 Offset    = NewestSubmission.Primitive.Offset;
            const SInt32 Base      = NewestSubmission.Primitive.Base;
            const UInt32 Instances = NewestSubmission.Primitive.Instances;
            const UInt32 Topology  = mPipelines[NewestSubmission.Pipeline].InputTopology;

            if (NewestSubmission.Indices.Buffer)
            {
                const UInt32 Stride     = AsIndexFormat(NewestSubmission.Indices.Stride);
                const Ptr<void> Pointer = reinterpret_cast<Ptr<void>>(
                        OldestSubmission.Indices.Offset + Offset * NewestSubmission.Indices.Stride);

                if (Base)
                {
                    if (Instances)
                    {
                        glDrawElementsInstancedBaseVertex(Topology, Count, Stride, Pointer, Instances, Base);
                    }
                    else
                    {
                        glDrawElementsBaseVertex(Topology, Count, Stride, Pointer, Base);
                    }
                }
                else
                {
                    if (Instances)
                    {
                        glDrawElementsInstanced(Topology, Count, Stride, Pointer, Instances);
                    }
                    else
                    {
                        glDrawElements(Topology, Count, Stride, Pointer);
                    }
                }
            }
            else
            {
                if (Instances)
                {
                    glDrawArraysInstanced(Topology, Offset, Count, Instances);
                }
                else
                {
                    glDrawArrays(Topology, Offset, Count);
                }
            }
        }

        // Apply cache
        mSubmission = Submissions.back();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::Commit(Object ID, Bool Synchronised)
    {
        if (ID == k_Default)
        {
            SDL_GL_SetSwapInterval(Synchronised ? 1 : 0);
            SDL_GL_SwapWindow(mDevice);
        }
        else
        {
            // @TODO FBO
            // @TODO Multisample
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::LoadCapabilities()
    {
        mCapabilities.Backend = Backend::GLES3;
        // @TODO Capabilities
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::LoadDefaults()
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glEnable(GL_SCISSOR_TEST);
        glFrontFace(GL_CCW);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT,   1);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLES3Driver::Compile(UInt32 Program, UInt32 Type, CPtr<const UInt8> Shader)
    {
        if (const UInt32 ID = glCreateShader(Type); ID > 0)
        {
            const auto Data = reinterpret_cast<Ptr<const GLchar>>(Shader.data());
            const auto Size = static_cast<GLint>(Shader.size());

            glShaderSource(ID, 1, AddressOf(Data), AddressOf(Size));
            glCompileShader(ID);

            GLint Success;
            glGetShaderiv(ID, GL_COMPILE_STATUS, AddressOf(Success));

            if (Success)
            {
                glAttachShader(Program, ID);
                glDeleteShader(ID);
                return true;
            }
            else
            {
                GLchar Error[512];
                glGetShaderInfoLog(ID, sizeof(Error), nullptr, Error);
                Log::Critical("GLES3Driver: Failed to compile shader {}", Error);
            }
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<GLES3Driver::GLES3Sampler> GLES3Driver::GetOrCreateSampler(ConstRef<Sampler> Descriptor)
    {
        Ref<GLES3Sampler> Sampler = mSamplers[
                static_cast<UInt32>(Descriptor.EdgeU)       |
                static_cast<UInt32>(Descriptor.EdgeV)  << 2 |
                static_cast<UInt32>(Descriptor.Filter) << 4];

        if (!Sampler.ID)
        {
            glGenSamplers(1, AddressOf(Sampler.ID));

            switch (Descriptor.Filter)
            {
            case TextureFilter::Nearest:
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            case TextureFilter::Bilinear:
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            case TextureFilter::Trilinear:
            case TextureFilter::Anisotropic:
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            }

            glSamplerParameteri(Sampler.ID, GL_TEXTURE_WRAP_S, As(Descriptor.EdgeU));
            glSamplerParameteri(Sampler.ID, GL_TEXTURE_WRAP_T, As(Descriptor.EdgeV));

            if (Descriptor.Filter == TextureFilter::Anisotropic)
            {
                glSamplerParameteri(Sampler.ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16); // @TODO Maximum possible anisotropic
            }
        }
        return Sampler;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::ApplyVertex(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        ConstRef<GLES3Pipeline> OldestPipeline = mPipelines[Oldest.Pipeline];
        ConstRef<GLES3Pipeline> NewestPipeline = mPipelines[Newest.Pipeline];

        SInt32 Slot   = -1;
        UInt32 Stride = 0;
        UInt32 Offset = 0;

        for (UInt32 Element = 0; Element < k_MaxAttributes; ++Element)
        {
            ConstRef<Attribute> Old = OldestPipeline.InputAttributes[Element];
            ConstRef<Attribute> New = NewestPipeline.InputAttributes[Element];

            if (Slot != New.Slot)
            {
                Slot   = New.Slot;
                Stride = Newest.Vertices[Slot].Stride;
                Offset = Newest.Vertices[Slot].Offset;

                if (Newest.Vertices[Slot].Buffer != Oldest.Vertices[Slot].Buffer)
                {
                    glBindBuffer(GL_ARRAY_BUFFER, Newest.Vertices[Slot].Buffer);
                }
            }

            if (New.ID != VertexSemantic::None)
            {
                if (Old.ID == VertexSemantic::None)
                {
                    glEnableVertexAttribArray(Element);
                }

                if (   Old.Format  != New.Format
                    || Old.Divisor != New.Divisor
                    || Old.Slot    != New.Slot
                    || Old.Offset  != New.Offset
                    || Newest.Vertices[Slot].Offset != Oldest.Vertices[Slot].Offset
                    || Newest.Vertices[Slot].Stride != Oldest.Vertices[Slot].Stride)
                {
                    glVertexAttribPointer(Element,
                                          As<1>(New.Format),
                                          As<0>(New.Format),
                                          As<2>(New.Format),
                                          Stride,
                                          reinterpret_cast<const void*>(Offset + New.Offset));
                }

                if (Old.Divisor != New.Divisor)
                {
                    glVertexAttribDivisor(Element, New.Divisor);
                }
            }
            else
            {
                if (Old.ID != VertexSemantic::None)
                {
                    glDisableVertexAttribArray(Element);
                }
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::ApplyPipeline(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        if (Oldest.Pipeline != Newest.Pipeline)
        {
            ConstRef<GLES3Pipeline> Old = mPipelines[Oldest.Pipeline];
            ConstRef<GLES3Pipeline> New = mPipelines[Newest.Pipeline];

            if (New.Cull != Old.Cull)
            {
                if (New.Cull == GL_NONE)
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    if (Old.Cull == GL_NONE)
                    {
                        glEnable(GL_CULL_FACE);
                    }
                    glCullFace(New.Cull);
                }
            }

#if !defined(SDL_PLATFORM_ANDROID) && !defined(SDL_PLATFORM_EMSCRIPTEN)
            if (New.Fill != Old.Fill)
            {
                glPolygonMode(GL_FRONT_AND_BACK, New.Fill);
            }
#endif

            if (New.BlendEnabled != Old.BlendEnabled)
            {
                if (New.BlendEnabled)
                {
                    glEnable(GL_BLEND);
                }
                else
                {
                    glDisable(GL_BLEND);
                }
            }

            if (   New.BlendColorSrcFactor != Old.BlendColorSrcFactor
                || New.BlendColorDstFactor != Old.BlendColorDstFactor
                || New.BlendAlphaSrcFactor != Old.BlendAlphaSrcFactor
                || New.BlendAlphaDstFactor != Old.BlendAlphaDstFactor)
            {
                glBlendFuncSeparate(New.BlendColorSrcFactor, New.BlendColorDstFactor,
                                    New.BlendAlphaSrcFactor, New.BlendAlphaDstFactor);
            }

            if (New.BlendColorEquation != Old.BlendColorEquation || New.BlendAlphaEquation != Old.BlendAlphaEquation)
            {
                glBlendEquationSeparate(New.BlendColorEquation, New.BlendAlphaEquation);
            }

            if (New.DepthEnabled != Old.DepthEnabled)
            {
                if (New.DepthEnabled)
                {
                    glEnable(GL_DEPTH_TEST);
                }
                else
                {
                    glDisable(GL_DEPTH_TEST);
                }
            }

            if (New.DepthMask != Old.DepthMask)
            {
                glDepthMask(New.DepthMask);
            }

            if (New.DepthCondition != Old.DepthCondition)
            {
                glDepthFunc(New.DepthCondition);
            }

            if (New.StencilEnabled != Old.StencilEnabled)
            {
                if (New.StencilEnabled)
                {
                    glEnable(GL_STENCIL_TEST);
                }
                else
                {
                    glDisable(GL_STENCIL_TEST);
                }
            }

            if (New.StencilMask != Old.StencilMask)
            {
                glStencilMask(New.StencilMask);
            }

            if (New.StencilCondition != Old.StencilCondition || Newest.Stencil != Oldest.Stencil)
            {
                glStencilFunc(New.StencilCondition, Newest.Stencil, 0xFF);
            }

            if (   New.StencilOnFail  != Old.StencilOnFail
                || New.StencilOnZFail != Old.StencilOnZFail
                || New.StencilOnZPass != Old.StencilOnZPass)
            {
                glStencilOp(New.StencilOnFail, New.StencilOnZFail, New.StencilOnZPass);
            }

            if (New.Program != Old.Program)
            {
                glUseProgram(New.Program);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLES3Driver::ApplyResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest)
    {
        // Apply texture(s)
        for (UInt32 Index = 0; Index < k_MaxSlots && Newest.Textures[Index] != 0; ++Index)
        {
            const UInt32 OldTexture = Oldest.Textures[Index];
            const UInt32 NewTexture = Newest.Textures[Index];

            if (OldTexture != NewTexture)
            {
                glActiveTexture(GL_TEXTURE0 + Index);
                glBindTexture(GL_TEXTURE_2D, mTextures[NewTexture].ID);
            }
        }

        // Apply sampler(s)
        for (UInt32 Index = 0; Index < k_MaxSlots; ++Index)
        {
            ConstRef<GLES3Sampler> Old = GetOrCreateSampler(Oldest.Samplers[Index]);
            ConstRef<GLES3Sampler> New = GetOrCreateSampler(Newest.Samplers[Index]);

            if (Old.ID != New.ID)
            {
                glBindSampler(Index, New.ID);
            }
        }

        // Apply uniform buffer(s)
        for (UInt32 Register = 0; Register < k_MaxUniforms; ++Register)
        {
            Ref<const Binding> Old = Oldest.Uniforms[Register];
            Ref<const Binding> New = Newest.Uniforms[Register];

            if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Stride != New.Stride)
            {
                glBindBufferRange(GL_UNIFORM_BUFFER, Register, mBuffers[New.Buffer].ID, New.Offset, New.Stride);
            }
        }
    }
}

