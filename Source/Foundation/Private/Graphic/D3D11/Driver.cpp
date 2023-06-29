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

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    inline void ThrowIfFail(HRESULT Result)
    {
        if (FAILED(Result))
        {
            throw std::runtime_error("D3D11 - ThrowIfFail"); // TODO Placeholder....
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(BlendColor Value)
    {
        return static_cast<D3D11_COLOR_WRITE_ENABLE>(Value);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(BlendFactor Value)
    {
        static constexpr D3D11_BLEND k_Mapping[] = {
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
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(BlendFunction Value)
    {
        constexpr static D3D11_BLEND_OP k_Mapping[] = {
            D3D11_BLEND_OP_ADD,                     // BlendFunction::Add
            D3D11_BLEND_OP_SUBTRACT,                // BlendFunction::Subtract
            D3D11_BLEND_OP_REV_SUBTRACT,            // BlendFunction::ReverseSubtract
            D3D11_BLEND_OP_MIN,                     // BlendFunction::Min
            D3D11_BLEND_OP_MAX                      // BlendFunction::Max
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Cull Value)
    {
        constexpr static D3D11_CULL_MODE k_Mapping[] = {
            D3D11_CULL_NONE,                // Cull::None
            D3D11_CULL_BACK,                // Cull::Back
            D3D11_CULL_FRONT,               // Cull::Front
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TestAction Value)
    {
        constexpr static D3D11_STENCIL_OP k_Mapping[] = {
            D3D11_STENCIL_OP_KEEP,                  // TestAction::Keep
            D3D11_STENCIL_OP_REPLACE,               // TestAction::Replace
            D3D11_STENCIL_OP_ZERO,                  // TestAction::Zero
            D3D11_STENCIL_OP_DECR,                  // TestAction::Decrease
            D3D11_STENCIL_OP_DECR_SAT,              // TestAction::DecreaseOrWrap
            D3D11_STENCIL_OP_INVERT,                // TestAction::Invert
            D3D11_STENCIL_OP_INCR,                  // TestAction::Increase
            D3D11_STENCIL_OP_INCR_SAT               // TestAction::IncreaseOrWrap
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TestCondition Value)
    {
        constexpr static D3D11_COMPARISON_FUNC k_Mapping[] = {
            D3D11_COMPARISON_ALWAYS,                // TestCondition::Always
            D3D11_COMPARISON_NEVER,                 // TestCondition::Never
            D3D11_COMPARISON_GREATER,               // TestCondition::Greater
            D3D11_COMPARISON_GREATER_EQUAL,         // TestCondition::GreaterEqual
            D3D11_COMPARISON_EQUAL,                 // TestCondition::Equal
            D3D11_COMPARISON_NOT_EQUAL,             // TestCondition::NotEqual
            D3D11_COMPARISON_LESS,                  // TestCondition::Less
            D3D11_COMPARISON_LESS_EQUAL             // TestCondition::LessEqual
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TextureEdge Value)
    {
        constexpr static D3D11_TEXTURE_ADDRESS_MODE k_Mapping[] = {
            D3D11_TEXTURE_ADDRESS_CLAMP,            // TextureEdge::Clamp
            D3D11_TEXTURE_ADDRESS_WRAP,             // TextureEdge::Repeat
            D3D11_TEXTURE_ADDRESS_MIRROR            // TextureEdge::Mirror
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TextureFilter Value)
    {
        constexpr static D3D11_FILTER k_Mapping[] = {
            D3D11_FILTER_MIN_MAG_MIP_POINT,            // TextureFilter::Nearest
            D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,     // TextureFilter::Bilinear
            D3D11_FILTER_MIN_MAG_MIP_LINEAR,           // TextureFilter::Trilinear
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    template<Bool Resource>
    static auto As(TextureFormat Value)
    {
        constexpr static DXGI_FORMAT k_Mapping[] = {
            DXGI_FORMAT_BC1_UNORM,           // TextureFormat::BC1UIntNorm
            DXGI_FORMAT_BC1_UNORM_SRGB,      // TextureFormat::BC1UIntNorm_sRGB
            DXGI_FORMAT_BC2_UNORM,           // TextureFormat::BC2UIntNorm
            DXGI_FORMAT_BC2_UNORM_SRGB,      // TextureFormat::BC2UIntNorm_sRGB
            DXGI_FORMAT_BC3_UNORM,           // TextureFormat::BC3UIntNorm
            DXGI_FORMAT_BC3_UNORM_SRGB,      // TextureFormat::BC3UIntNorm_sRGB
            DXGI_FORMAT_BC4_UNORM,           // TextureFormat::BC4UIntNorm
            DXGI_FORMAT_BC5_UNORM,           // TextureFormat::BC5UIntNorm
            DXGI_FORMAT_BC7_UNORM,           // TextureFormat::BC7UIntNorm
            DXGI_FORMAT_BC7_UNORM_SRGB,      // TextureFormat::BC7UIntNorm_sRGB
            DXGI_FORMAT_R8_SINT,             // TextureFormat::R8SInt
            DXGI_FORMAT_R8_SNORM,            // TextureFormat::R8SIntNorm
            DXGI_FORMAT_R8_UINT,             // TextureFormat::R8UInt
            DXGI_FORMAT_R8_UNORM,            // TextureFormat::R8UIntNorm
            DXGI_FORMAT_R16_SINT,            // TextureFormat::R16SInt
            DXGI_FORMAT_R16_SNORM,           // TextureFormat::R16SIntNorm
            DXGI_FORMAT_R16_UINT,            // TextureFormat::R16UInt
            DXGI_FORMAT_R16_UNORM,           // TextureFormat::R16UIntNorm
            DXGI_FORMAT_R16_FLOAT,           // TextureFormat::R16Float
            DXGI_FORMAT_R32_SINT,            // TextureFormat::R32SInt
            DXGI_FORMAT_R32_UINT,            // TextureFormat::R32UInt
            DXGI_FORMAT_R32_FLOAT,           // TextureFormat::R32Float
            DXGI_FORMAT_R8G8_SINT,           // TextureFormat::RG8SInt
            DXGI_FORMAT_R8G8_SNORM,          // TextureFormat::RG8SIntNorm
            DXGI_FORMAT_R8G8_UINT,           // TextureFormat::RG8UInt
            DXGI_FORMAT_R8G8_UNORM,          // TextureFormat::RG8UIntNorm
            DXGI_FORMAT_R16G16_SINT,         // TextureFormat::RG16SInt
            DXGI_FORMAT_R16G16_SNORM,        // TextureFormat::RG16SIntNorm
            DXGI_FORMAT_R16G16_UINT,         // TextureFormat::RG16UInt
            DXGI_FORMAT_R16G16_UNORM,        // TextureFormat::RG16UIntNorm
            DXGI_FORMAT_R16G16_FLOAT,        // TextureFormat::RG16Float
            DXGI_FORMAT_R32G32_SINT,         // TextureFormat::RG32SInt
            DXGI_FORMAT_R32G32_UINT,         // TextureFormat::RG32UInt
            DXGI_FORMAT_R32G32_FLOAT,        // TextureFormat::RG32Float
            DXGI_FORMAT_R32G32B32_SINT,      // TextureFormat::RGB32SInt
            DXGI_FORMAT_R32G32B32_UINT,      // TextureFormat::RGB32UInt
            DXGI_FORMAT_R32G32B32_FLOAT,     // TextureFormat::RGB32Float
            DXGI_FORMAT_R8G8B8A8_SINT,       // TextureFormat::RGBA8SInt
            DXGI_FORMAT_R8G8B8A8_SNORM,      // TextureFormat::RGBA8SIntNorm
            DXGI_FORMAT_R8G8B8A8_UINT,       // TextureFormat::RGBA8UInt
            DXGI_FORMAT_R8G8B8A8_UNORM,      // TextureFormat::RGBA8UIntNorm
            DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, // TextureFormat::RGBA8UIntNorm_sRGB
            DXGI_FORMAT_R16G16B16A16_SINT,   // TextureFormat::RGBA16SInt
            DXGI_FORMAT_R16G16B16A16_SNORM,  // TextureFormat::RGBA16SIntNorm
            DXGI_FORMAT_R16G16B16A16_UINT,   // TextureFormat::RGBA16UInt
            DXGI_FORMAT_R16G16B16A16_UNORM,  // TextureFormat::RGBA16UIntNorm
            DXGI_FORMAT_R16G16B16A16_FLOAT,  // TextureFormat::RGBA16Float
            DXGI_FORMAT_R32G32B32A32_SINT,   // TextureFormat::RGBA32SInt
            DXGI_FORMAT_R32G32B32A32_UINT,   // TextureFormat::RGBA32UInt
            DXGI_FORMAT_R32G32B32A32_FLOAT,  // TextureFormat::RGBA32Float
            DXGI_FORMAT_R32_TYPELESS,        // TextureFormat::D16Float
            DXGI_FORMAT_R32_TYPELESS,        // TextureFormat::D24Float
            DXGI_FORMAT_R32_TYPELESS,        // TextureFormat::D32Float
            DXGI_FORMAT_R16_TYPELESS,        // TextureFormat::D16X0UIntNorm
            DXGI_FORMAT_R24G8_TYPELESS,      // TextureFormat::D24X0UIntNorm
            DXGI_FORMAT_R24G8_TYPELESS,      // TextureFormat::D24S8UIntNorm
            DXGI_FORMAT_R24G8_TYPELESS,      // TextureFormat::D32X0UIntNorm
        };

        if constexpr (Resource)
        {
            switch (Value)
            {
            case TextureFormat::D16Float:
            case TextureFormat::D24Float:
            case TextureFormat::D32Float:
                return DXGI_FORMAT_R32_FLOAT;
            case TextureFormat::D16X0UIntNorm:
                return DXGI_FORMAT_R16_UNORM;
            case TextureFormat::D24X0UIntNorm:
            case TextureFormat::D24S8UIntNorm:
            case TextureFormat::D32X0UIntNorm:
                return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            default:
                break;
            }
        }
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto Fill(const UInt08 * Data, UInt Layer, UInt Width, UInt Height, TextureFormat Layout)
    {
        constexpr static UInt08 kMapping[] = {
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
            64,     // TextureFormat::RGBA16SInt
            64,     // TextureFormat::RGBA16SIntNorm
            64,     // TextureFormat::RGBA16UInt
            64,     // TextureFormat::RGBA16UIntNorm
            64,     // TextureFormat::RGBA16Float
            128,    // TextureFormat::RGBA32SInt
            128,    // TextureFormat::RGBA32UInt
            128,    // TextureFormat::RGBA32Float
            16,     // TextureFormat::D16Float
            32,     // TextureFormat::D24Float
            32,     // TextureFormat::D32Float
            32,     // TextureFormat::D16X0UIntNorm
            32,     // TextureFormat::D24X0UIntNorm
            32,     // TextureFormat::D24S8UIntNorm
            32,     // TextureFormat::D32X0UIntNorm
        };

        const UInt32 Depth = kMapping[static_cast<UInt08>(Layout)];

        if (Data)
        {
            static D3D11_SUBRESOURCE_DATA Content[k_MaxMipmap];

            for (UInt32 Level = 0; Level < Layer; ++Level)
            {
                Content[Level].pSysMem          = Data;
                Content[Level].SysMemPitch      = Width * (Depth / 8);
                Content[Level].SysMemSlicePitch = 0;

                Data    += (Width * Depth * Height);
                Width  <<= 2u;
                Height <<= 2u;
            }
            return Content;
        }
        return static_cast<D3D11_SUBRESOURCE_DATA *>(nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(VertexFormat Value)
    {
        constexpr static DXGI_FORMAT k_Mapping[] = {
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
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(VertexTopology Value)
    {
        constexpr static D3D11_PRIMITIVE_TOPOLOGY k_Mapping[] = {
            D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,       // VertexTopology::Point
            D3D11_PRIMITIVE_TOPOLOGY_LINELIST,        // VertexTopology::Line
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,    // VertexTopology::Triangle
        };
        return k_Mapping[static_cast<UInt32>(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool D3D11Driver::Initialise(Any Display, UInt Width, UInt Height)
    {
        decltype(& D3D11CreateDevice)  D3D11CreateDevicePtr = nullptr;
        decltype(& CreateDXGIFactory1) CreateDXGIFactoryPtr = nullptr;

        if (auto Dll = ::LoadLibraryEx("D3D11.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32); Dll != nullptr)
        {
            D3D11CreateDevicePtr = (decltype(& D3D11CreateDevice)) GetProcAddress(Dll, "D3D11CreateDevice");
        }
        if (auto Dll = ::LoadLibraryEx("DXGI.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32); Dll != nullptr)
        {
            CreateDXGIFactoryPtr = (decltype(& CreateDXGIFactory1)) GetProcAddress(Dll, "CreateDXGIFactory1");
        }

        const Bool Successful = (D3D11CreateDevicePtr && CreateDXGIFactoryPtr);

        if (Successful)
        {
            ComPtr<ID3D11Device>        Device;
            ComPtr<ID3D11DeviceContext> DeviceImmediate;

            UInt Flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(_DEBUG)
            Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

            ThrowIfFail(D3D11CreateDevicePtr(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                Flags,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                Device.GetAddressOf(),
                nullptr,
                DeviceImmediate.GetAddressOf()));

            ThrowIfFail(Device.As<ID3D11Device1>(& mDevice));
            ThrowIfFail(DeviceImmediate.As<ID3D11DeviceContext1>(& mDeviceImmediate));

            ThrowIfFail(CreateDXGIFactoryPtr(IID_PPV_ARGS(& mDisplayFactory)));

            LoadCapabilities();

            if (Display.has_value())
            {
                CreatePass(k_Default, Display, Width, Height);
            }
        }
        return Successful;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Reset(UInt Width, UInt Height)
    {
        if (mDeviceImmediate)
        {
            mDeviceImmediate->ClearState();
        }

        mPasses[k_Default].Color[k_Default]->Release();
        mPasses[k_Default].Auxiliary->Release();

        // Changes the swap chain's back buffer size, format, and number of buffers
        mPasses[k_Default].Display->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);

        // Recreate the swap chain's resources
        CreateSwapchainResources(mPasses[k_Default], Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<const Capabilities> D3D11Driver::GetCapabilities() const
    {
        return mCapabilities;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateBuffer(Object ID, Bool Geometry, UInt Capacity, CPtr<UInt08> Data)
    {
        const D3D11_BUFFER_DESC Descriptor = CD3D11_BUFFER_DESC(
            Geometry     ? Capacity : Align<256>(Capacity),
            Geometry     ? D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_CONSTANT_BUFFER,
            Data.empty() ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE,
            Data.empty() ? D3D11_CPU_ACCESS_WRITE : 0);

        D3D11_SUBRESOURCE_DATA      Content {
            Data.data(), static_cast<UINT>(Data.size())
        };
        Ptr<D3D11_SUBRESOURCE_DATA> Pointer = (Data.empty() ? nullptr : & Content);

        Ref<D3D11Buffer> Buffer = mBuffers[ID];
        Buffer.Offset   = 0;
        Buffer.Capacity = Capacity;

        ThrowIfFail(mDevice->CreateBuffer(& Descriptor, Pointer, Buffer.Resource.GetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<void> D3D11Driver::Map(Object ID, Ref<UInt> Offset, UInt Length)
    {
        Ref<D3D11Buffer> Buffer = mBuffers[ID];

        Offset = (Buffer.Offset + Length > Buffer.Capacity ? 0u : Buffer.Offset);

        Buffer.Offset = Offset + Length;

        return Map(ID, !Offset, Offset, Length);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<void> D3D11Driver::Map(Object ID, Bool Discard, UInt Offset, UInt Length)
    {
        const D3D11_MAP          Mode = (Discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE);
        D3D11_MAPPED_SUBRESOURCE Memory;

        if (FAILED(mDeviceImmediate->Map(mBuffers[ID].Resource.Get(), 0, Mode, 0, & Memory)))
        {
            return nullptr;
        }
        return static_cast<Ptr<UInt08>>(Memory.pData) + Offset;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Unmap(Object ID)
    {
        mDeviceImmediate->Unmap(mBuffers[ID].Resource.Get(), 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteBuffer(Object ID)
    {
        mBuffers[ID].~D3D11Buffer();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreatePass(Object ID, Any Display, UInt Width, UInt Height)
    {
        DXGI_SWAP_CHAIN_DESC Description { 0 };
        Description.BufferCount       = 2;
        Description.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Description.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        Description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Description.BufferDesc.Width  = Width;
        Description.BufferDesc.Height = Height;
        Description.SampleDesc        = { 1, 0 };
        Description.OutputWindow      = eastl::any_cast<HWND>(Display);
        Description.Windowed          = true;
        Description.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;

        ThrowIfFail(
            mDisplayFactory->CreateSwapChain(mDevice.Get(), & Description, mPasses[ID].Display.GetAddressOf()));

        ThrowIfFail(
            mDisplayFactory->MakeWindowAssociation(eastl::any_cast<HWND>(Display), DXGI_MWA_NO_WINDOW_CHANGES));

        CreateSwapchainResources(mPasses[ID], Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreatePass(Object ID, CPtr<UInt> Colors, UInt Auxiliary)
    {
        Ref<D3D11Pass> Pass = mPasses[ID];

        for (UInt Index = 0; Index < Colors.size(); ++Index)
        {
            Ptr<ID3D11Texture2D> Attachment = mTextures[Colors[Index]].Object.Get();

            const CD3D11_RENDER_TARGET_VIEW_DESC Description(D3D11_RTV_DIMENSION_TEXTURE2D);
            ThrowIfFail(mDevice->CreateRenderTargetView(Attachment, & Description, Pass.Color[Index].GetAddressOf()));
        }

        if (Auxiliary)
        {
            Ptr<ID3D11Texture2D> Attachment = mTextures[Auxiliary].Object.Get();

            const CD3D11_DEPTH_STENCIL_VIEW_DESC Description(D3D11_DSV_DIMENSION_TEXTURE2D);
            ThrowIfFail(mDevice->CreateDepthStencilView(Attachment, & Description, Pass.Auxiliary.GetAddressOf()));
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

    void D3D11Driver::CreatePipeline(Object ID, CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties)
    {
        Ref<D3D11Pipeline> Pipeline = mPipelines[ID];

        ThrowIfFail(mDevice->CreateVertexShader(Vertex.data(), Vertex.size(), nullptr, Pipeline.VS.GetAddressOf()));
        ThrowIfFail(mDevice->CreatePixelShader(Fragment.data(), Fragment.size(), nullptr, Pipeline.PS.GetAddressOf()));

        {
            D3D11_BLEND_DESC Description = CD3D11_BLEND_DESC(CD3D11_DEFAULT());

            Description.RenderTarget[0].BlendEnable           = !(
                Properties.BlendColorSrcFactor == BlendFactor::One
                    && Properties.BlendAlphaSrcFactor == BlendFactor::One
                    && Properties.BlendColorDstFactor == BlendFactor::Zero
                    && Properties.BlendAlphaDstFactor == BlendFactor::Zero
                    && Properties.BlendColorEquation  == BlendFunction::Add
                    && Properties.BlendAlphaEquation  == BlendFunction::Add);
            Description.RenderTarget[0].SrcBlend              = As(Properties.BlendColorSrcFactor);
            Description.RenderTarget[0].DestBlend             = As(Properties.BlendColorDstFactor);
            Description.RenderTarget[0].BlendOp               = As(Properties.BlendColorEquation);
            Description.RenderTarget[0].SrcBlendAlpha         = As(Properties.BlendAlphaSrcFactor);
            Description.RenderTarget[0].DestBlendAlpha        = As(Properties.BlendAlphaDstFactor);
            Description.RenderTarget[0].BlendOpAlpha          = As(Properties.BlendAlphaEquation);
            Description.RenderTarget[0].RenderTargetWriteMask = As(Properties.BlendMask);

            ThrowIfFail(mDevice->CreateBlendState(& Description, Pipeline.BS.GetAddressOf()));
        }

        {
            D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

            Description.DepthEnable                  = (
                Properties.DepthCondition != TestCondition::Always || Properties.DepthMask);
            Description.DepthFunc                    = As(Properties.DepthCondition);
            Description.DepthWriteMask               = static_cast<D3D11_DEPTH_WRITE_MASK>(Properties.DepthMask);
            Description.StencilEnable                = (
                Properties.StencilCondition != TestCondition::Always
                    || Properties.StencilOnFail    != TestAction::Keep
                    || Properties.StencilOnZFail   != TestAction::Keep
                    || Properties.StencilOnZPass   != TestAction::Keep);
            Description.StencilReadMask              = Properties.StencilMask;
            Description.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
            Description.FrontFace.StencilFunc        = As(Properties.StencilCondition);
            Description.FrontFace.StencilFailOp      = As(Properties.StencilOnFail);
            Description.FrontFace.StencilDepthFailOp = As(Properties.StencilOnZFail);
            Description.FrontFace.StencilPassOp      = As(Properties.StencilOnZPass);
            Description.BackFace.StencilFunc         = Description.FrontFace.StencilFunc;
            Description.BackFace.StencilFailOp       = Description.FrontFace.StencilFailOp;
            Description.BackFace.StencilDepthFailOp  = Description.FrontFace.StencilDepthFailOp;
            Description.BackFace.StencilPassOp       = Description.FrontFace.StencilPassOp;

            ThrowIfFail(mDevice->CreateDepthStencilState(& Description, Pipeline.DS.GetAddressOf()));
        }

        {
            D3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());

            Description.CullMode		  = As(Properties.Cull);
            Description.ScissorEnable     = TRUE;
            Description.MultisampleEnable = TRUE;
            Description.FillMode          = Properties.Fill ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

            ThrowIfFail(mDevice->CreateRasterizerState(& Description, Pipeline.RS.GetAddressOf()));
        }

        {
            D3D11_INPUT_ELEMENT_DESC Description[k_MaxAttributes];
            UInt                     Count = 0;

            for (; Count < k_MaxAttributes && Properties.InputLayout[Count].Slot >= 0; ++Count)
            {
                D3D11_INPUT_ELEMENT_DESC  & Element = Description[Count];

                Element.SemanticName         = "ATTRIBUTE";
                Element.SemanticIndex        = Properties.InputLayout[Count].Slot;
                Element.Format               = As(Properties.InputLayout[Count].Format);
                Element.AlignedByteOffset    = Properties.InputLayout[Count].Offset;
                Element.InputSlot            = 0;
                Element.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
                Element.InstanceDataStepRate = 0;
            }

            ThrowIfFail(mDevice->CreateInputLayout(Description, Count, Vertex.data(), Vertex.size(), Pipeline.IL.GetAddressOf()));
        }

        Pipeline.PT = As(Properties.InputTopology);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeletePipeline(Object ID)
    {
        mPipelines[ID].~D3D11Pipeline();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data)
    {
        CD3D11_TEXTURE2D_DESC Description(As<false>(Format), Width, Height, 1, Layer);
        Description.Usage     = (Data.empty() ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE);
        Description.BindFlags = (Layout != TextureLayout::Destination ? D3D11_BIND_SHADER_RESOURCE : 0);
        Description.MiscFlags = (Layer < 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);

        if (Layout != TextureLayout::Source)
        {
            switch (Format)
            {
            case TextureFormat::D16Float:
            case TextureFormat::D24Float:
            case TextureFormat::D32Float:
            case TextureFormat::D16X0UIntNorm:
            case TextureFormat::D24X0UIntNorm:
            case TextureFormat::D24S8UIntNorm:
            case TextureFormat::D32X0UIntNorm:
                Description.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
                break;
            default:
                Description.BindFlags |= D3D11_BIND_RENDER_TARGET;
                break;
            }
        }

        Ptr<const D3D11_SUBRESOURCE_DATA> Memory = Fill(Data.data(), Layer, Width, Height, Format);
        ThrowIfFail(mDevice->CreateTexture2D(& Description, Memory, & mTextures[ID].Object));

        if (Layout != TextureLayout::Destination)
        {
            const CD3D11_SHADER_RESOURCE_VIEW_DESC Type(D3D11_SRV_DIMENSION_TEXTURE2D, As<true>(Format), 0, Layer);
            ThrowIfFail(mDevice->CreateShaderResourceView(
                mTextures[ID].Object.Get(), & Type, mTextures[ID].Resource.GetAddressOf()));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt08> Data)
    {
        const D3D11_BOX Rect {
            static_cast<UINT>(Offset.GetLeft()),
            static_cast<UINT>(Offset.GetTop()),
            0,
            static_cast<UINT>(Offset.GetRight()),
            static_cast<UINT>(Offset.GetBottom()),
            0
        };
        mDeviceImmediate->UpdateSubresource(mTextures[ID].Object.Get(), Level, & Rect, Data.data(), Pitch, 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset)
    {
        const D3D11_BOX SrcData = CD3D11_BOX(
            SrcOffset.GetLeft(), SrcOffset.GetTop(), 0, SrcOffset.GetRight(), SrcOffset.GetBottom(), 1);

        const Ptr<ID3D11Texture2D> DstTexture = mTextures[Destination].Object.Get();
        const Ptr<ID3D11Texture2D> SrcTexture = mTextures[Source].Object.Get();

        mDeviceImmediate->CopySubresourceRegion(
            DstTexture, DstLevel, DstOffset.GetX(), DstOffset.GetY(), DstOffset.GetZ(), SrcTexture, SrcLevel, & SrcData);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UPtr<UInt08[]> D3D11Driver::ReadTexture(Object ID, UInt Level, Recti Offset)
    {
        D3D11_TEXTURE2D_DESC Description;
        mTextures[ID].Object->GetDesc(& Description);

        // Create a staging texture to be able to read the texture back.
        ComPtr<ID3D11Texture2D> Buffer;

        Description.BindFlags      = 0;
        Description.Usage          = D3D11_USAGE_STAGING;
        Description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        ThrowIfFail(mDevice->CreateTexture2D(& Description, nullptr, Buffer.GetAddressOf()));

        // Copy the resource between GPU -> CPU.
        const D3D11_BOX SrcData = CD3D11_BOX(
            Offset.GetLeft(), Offset.GetTop(), 0, Offset.GetRight(), Offset.GetBottom(), 1);
        mDeviceImmediate->CopySubresourceRegion(Buffer.Get(), 0, 0, 0, 0, mTextures[ID].Object.Get(), Level, & SrcData);

        // Map the staging texture and copy into a new buffer.
        UPtr<UInt08[]>           Bytes;
        D3D11_MAPPED_SUBRESOURCE Memory;

        if (SUCCEEDED(mDeviceImmediate->Map(Buffer.Get(), 0, D3D11_MAP_READ, 0, & Memory)))
        {
            Bytes = NewUniquePtr<UInt08[]>(Description.Height * Memory.RowPitch);

            memcpy(Bytes.get(), Memory.pData, Description.Height * Memory.RowPitch);

            mDeviceImmediate->Unmap(Buffer.Get(), 0);
        }
        return Move(Bytes);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteTexture(Object ID)
    {
        mTextures[ID].~D3D11Texture();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil)
    {
        Stack<Ptr<ID3D11RenderTargetView>, k_MaxAttachments> Attachments;

        for (Ref<const ComPtr<ID3D11RenderTargetView>> View : mPasses[ID].Color)
        {
            if (View)
            {
                Attachments.push_back(View.Get());

                if (Target != Clear::Auxiliary)
                {
                    mDeviceImmediate->ClearRenderTargetView(View.Get(), reinterpret_cast<Real32 *>(& Tint));
                }
            }
        }

        if (Target != Clear::Color)
        {
            constexpr UINT Mode = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;

            mDeviceImmediate->ClearDepthStencilView(mPasses[ID].Auxiliary.Get(), Mode, Depth, Stencil);
        }

        mDeviceImmediate->OMSetRenderTargets(Attachments.size(), Attachments.data(), mPasses[ID].Auxiliary.Get());

        const CD3D11_VIEWPORT Rect(
            Viewport.GetX(),
            Viewport.GetY(),
            Viewport.GetWidth(),
            Viewport.GetHeight());
        mDeviceImmediate->RSSetViewports(1, & Rect);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Submit(CPtr<Submission> Submissions)
    {
        Submission EmptyDummySubmission;

        for (UInt Index = 0; Index < Submissions.size(); ++Index)
        {
            Ref<const Submission> NewestSubmission = Submissions[Index];
            Ref<const Submission> OldestSubmission = (Index > 0 ? Submissions[Index - 1] : EmptyDummySubmission);

            // Apply vertices when it has changed
            if (OldestSubmission.Vertices.Buffer != NewestSubmission.Vertices.Buffer)
            {
                Ref<const D3D11Buffer> Buffer = mBuffers[NewestSubmission.Vertices.Buffer];
                const UINT Stride = NewestSubmission.Vertices.Stride;
                const UINT Offset = 0;
                mDeviceImmediate->IASetVertexBuffers(0, 1, Buffer.Resource.GetAddressOf(), & Stride, & Offset);
            }

            // Apply indices when it has changed
            if (OldestSubmission.Indices.Buffer != NewestSubmission.Indices.Buffer)
            {
                Ref<const D3D11Buffer> Buffer = mBuffers[NewestSubmission.Indices.Buffer];
                const DXGI_FORMAT      Format = (NewestSubmission.Indices.Stride == sizeof(UInt16)
                                                 ? DXGI_FORMAT_R16_UINT
                                                 : DXGI_FORMAT_R32_UINT);
                mDeviceImmediate->IASetIndexBuffer(Buffer.Resource.Get(), Format, 0);
            }

            // Apply uniforms when it has changed
            {
                Ptr<ID3D11Buffer> Array[k_MaxUniforms];
                UINT   ArrayOffset[k_MaxUniforms];
                UINT   ArrayLength[k_MaxUniforms];
                UInt32 Min = k_MaxUniforms;
                UInt32 Max = 0u;

                for (UInt32 Element = 0; Element < k_MaxUniforms; ++Element)
                {
                    Ref<const Binding> Old = OldestSubmission.Uniforms[Element];
                    Ref<const Binding> New = NewestSubmission.Uniforms[Element];

                    if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Length != New.Length)
                    {
                        Min = min(Element, Min);
                        Max = max(Element + 1, Max);
                    }

                    Array[Element]       = mBuffers[New.Buffer].Resource.Get();
                    ArrayOffset[Element] = Align<16>(New.Offset);
                    ArrayLength[Element] = Align<16>(New.Length);
                }

                if (Min != k_MaxUniforms && Max != 0u)
                {
                    const UInt32 Count = Max - Min;

                    mDeviceImmediate->VSSetConstantBuffers1(
                        Min, Count, & Array[Min], & ArrayOffset[Min], & ArrayLength[Min]);
                    mDeviceImmediate->PSSetConstantBuffers1(
                        Min, Count, & Array[Min], & ArrayOffset[Min], & ArrayLength[Min]);
                }
            }

            // Apply the scissor test when it has changed, or it's the first batch
            if (Index == 0 || OldestSubmission.Scissor != NewestSubmission.Scissor)
            {
                const RECT Rect {
                    NewestSubmission.Scissor.GetLeft(),  NewestSubmission.Scissor.GetTop(),
                    NewestSubmission.Scissor.GetRight(), NewestSubmission.Scissor.GetBottom()
                };
                mDeviceImmediate->RSSetScissorRects(1, & Rect);
            }

            // Apply pipeline when it has changed
            if (OldestSubmission.Pipeline != NewestSubmission.Pipeline)
            {
                Ref<const D3D11Pipeline> Old = mPipelines[OldestSubmission.Pipeline];
                Ref<const D3D11Pipeline> New = mPipelines[NewestSubmission.Pipeline];

                if (Old.VS != New.VS)
                {
                    mDeviceImmediate->VSSetShader(New.VS.Get(), nullptr, 0);
                }
                if (Old.PS != New.PS)
                {
                    mDeviceImmediate->PSSetShader(New.PS.Get(), nullptr, 0);
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

            // Apply textures when it has changed
            UInt Sources = 0;

            {
                Ptr<ID3D11ShaderResourceView> Array[k_MaxSources];
                UInt32 Min = k_MaxSources;
                UInt32 Max = 0u;

                for (UInt32 Element = 0; Element < k_MaxSources && NewestSubmission.Textures[Element] != 0; ++Element)
                {
                    ++Sources;

                    if (OldestSubmission.Textures[Element] != NewestSubmission.Textures[Element])
                    {
                        Min = min(Element, Min);
                        Max = max(Element + 1, Max);
                    }
                    Array[Element] = mTextures[NewestSubmission.Textures[Element]].Resource.Get();
                }

                if (Min != k_MaxSources && Max != 0u)
                {
                    const UInt32 Count = Max - Min;

                    mDeviceImmediate->VSSetShaderResources(Min, Count, & Array[Min]);
                    mDeviceImmediate->PSSetShaderResources(Min, Count, & Array[Min]);
                }
            }

            // Apply samplers when it has changed, or it's the first batch
            {
                Ptr<ID3D11SamplerState> Array[k_MaxSources];
                UInt32 Min = Sources;
                UInt32 Max = 0u;

                for (UInt32 Element = 0; Element < Sources; ++Element)
                {
                    Ref<D3D11Sampler> PrevSampler = GetOrCreateSampler(OldestSubmission.Samplers[Element]);
                    Ref<D3D11Sampler> NextSampler = GetOrCreateSampler(NewestSubmission.Samplers[Element]);

                    if (Index == 0 || PrevSampler.Resource != NextSampler.Resource)
                    {
                        Min = min(Element, Min);
                        Max = max(Element + 1, Max);
                    }
                    Array[Element] = NextSampler.Resource.Get();
                }

                if (Min != Sources && Max != 0u)
                {
                    const UInt32 Count = Max - Min;

                    mDeviceImmediate->VSSetSamplers(Min, Count, & Array[Min]);
                    mDeviceImmediate->PSSetSamplers(Min, Count, & Array[Min]);
                }
            }

            // Draw!
            if (NewestSubmission.Indices.Buffer != 0)
            {
                mDeviceImmediate->DrawIndexed(
                    NewestSubmission.Indices.Length, NewestSubmission.Indices.Offset, NewestSubmission.Vertices.Offset);
            }
            else
            {
                mDeviceImmediate->Draw(NewestSubmission.Vertices.Length, NewestSubmission.Vertices.Offset);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Commit(Object ID, Bool Synchronised)
    {
        mPasses[ID].Display->Present(Synchronised ? 1 : 0, 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::LoadCapabilities()
    {
        mCapabilities.Backend = Backend::Direct3D11;

        switch (mDevice->GetFeatureLevel())
        {
        case D3D_FEATURE_LEVEL_12_1:
        case D3D_FEATURE_LEVEL_12_0:
            mCapabilities.Language = Language::Version_6;
            break;
        case D3D_FEATURE_LEVEL_11_1:
        case D3D_FEATURE_LEVEL_11_0:
            mCapabilities.Language = Language::Version_5;
            break;
        case D3D_FEATURE_LEVEL_10_1:
        case D3D_FEATURE_LEVEL_10_0:
            mCapabilities.Language = Language::Version_4;
            break;
        case D3D_FEATURE_LEVEL_9_3:
            mCapabilities.Language = Language::Version_3;
            break;
        case D3D_FEATURE_LEVEL_9_2:
            mCapabilities.Language = Language::Version_2;
            break;
        case D3D_FEATURE_LEVEL_9_1:
            mCapabilities.Language = Language::Version_1;
            break;
        default:
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt Width, UInt Height)
    {
        ComPtr<ID3D11Texture2D> Color;
        Pass.Display->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(Color.GetAddressOf()));
        ThrowIfFail(mDevice->CreateRenderTargetView(Color.Get(), nullptr, Pass.Color[k_Default].GetAddressOf()));

        D3D11_TEXTURE2D_DESC Description { 0 };
        Description.Width      = Width;
        Description.Height     = Height;
        Description.MipLevels  = 1;
        Description.ArraySize  = 1;
        Description.Format     = DXGI_FORMAT_D24_UNORM_S8_UINT;
        Description.SampleDesc = { 1, 0 };
        Description.Usage      = D3D11_USAGE_DEFAULT;
        Description.BindFlags  = D3D11_BIND_DEPTH_STENCIL;

        ComPtr<ID3D11Texture2D> Depth;
        ThrowIfFail(mDevice->CreateTexture2D(& Description, nullptr, Depth.GetAddressOf()));
        ThrowIfFail(mDevice->CreateDepthStencilView(Depth.Get(), nullptr, Pass.Auxiliary.GetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<D3D11Driver::D3D11Sampler> D3D11Driver::GetOrCreateSampler(Ref<const Sampler> Descriptor)
    {
        Ref<D3D11Sampler> Sampler = mSamplers[
            static_cast<UInt>(Descriptor.EdgeU)       |
            static_cast<UInt>(Descriptor.EdgeV)  << 3 |
            static_cast<UInt>(Descriptor.Filter) << 6];

        if (!Sampler.Resource)
        {
            const CD3D11_SAMPLER_DESC SamplerDescriptor(
                As(Descriptor.Filter),
                As(Descriptor.EdgeU),
                As(Descriptor.EdgeV),
                D3D11_TEXTURE_ADDRESS_CLAMP,
                0,
                0,
                D3D11_COMPARISON_NEVER,
                nullptr,
                0,
                D3D11_FLOAT32_MAX);

            ThrowIfFail(mDevice->CreateSamplerState(& SamplerDescriptor, Sampler.Resource.GetAddressOf()));
        }
        return Sampler;
    }
}

