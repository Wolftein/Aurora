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

#include "D3D11Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    inline Bool CheckIfFail(HRESULT Result)
    {
        if (FAILED(Result))
        {
            TCHAR Buffer[1024];
            ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, Result, 0, Buffer, 1024, NULL);

            Log::Critical("D3D11Driver: {}", Buffer);
            return false;
        }
        return true;
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(TextureFormat Value)
    {
        constexpr static DXGI_FORMAT k_Mapping[] = {
            DXGI_FORMAT_BC1_UNORM,            // TextureFormat::BC1UIntNorm
            DXGI_FORMAT_BC1_UNORM_SRGB,       // TextureFormat::BC1UIntNorm_sRGB
            DXGI_FORMAT_BC2_UNORM,            // TextureFormat::BC2UIntNorm
            DXGI_FORMAT_BC2_UNORM_SRGB,       // TextureFormat::BC2UIntNorm_sRGB
            DXGI_FORMAT_BC3_UNORM,            // TextureFormat::BC3UIntNorm
            DXGI_FORMAT_BC3_UNORM_SRGB,       // TextureFormat::BC3UIntNorm_sRGB
            DXGI_FORMAT_BC4_UNORM,            // TextureFormat::BC4UIntNorm
            DXGI_FORMAT_BC5_UNORM,            // TextureFormat::BC5UIntNorm
            DXGI_FORMAT_R8_SINT,              // TextureFormat::R8SInt
            DXGI_FORMAT_R8_SNORM,             // TextureFormat::R8SIntNorm
            DXGI_FORMAT_R8_UINT,              // TextureFormat::R8UInt
            DXGI_FORMAT_R8_UNORM,             // TextureFormat::R8UIntNorm
            DXGI_FORMAT_R16_SINT,             // TextureFormat::R16SInt
            DXGI_FORMAT_R16_SNORM,            // TextureFormat::R16SIntNorm
            DXGI_FORMAT_R16_UINT,             // TextureFormat::R16UInt
            DXGI_FORMAT_R16_UNORM,            // TextureFormat::R16UIntNorm
            DXGI_FORMAT_R16_FLOAT,            // TextureFormat::R16Float
            DXGI_FORMAT_R32_SINT,             // TextureFormat::R32SInt
            DXGI_FORMAT_R32_UINT,             // TextureFormat::R32UInt
            DXGI_FORMAT_R32_FLOAT,            // TextureFormat::R32Float
            DXGI_FORMAT_R8G8_SINT,            // TextureFormat::RG8SInt
            DXGI_FORMAT_R8G8_SNORM,           // TextureFormat::RG8SIntNorm
            DXGI_FORMAT_R8G8_UINT,            // TextureFormat::RG8UInt
            DXGI_FORMAT_R8G8_UNORM,           // TextureFormat::RG8UIntNorm
            DXGI_FORMAT_R16G16_SINT,          // TextureFormat::RG16SInt
            DXGI_FORMAT_R16G16_SNORM,         // TextureFormat::RG16SIntNorm
            DXGI_FORMAT_R16G16_UINT,          // TextureFormat::RG16UInt
            DXGI_FORMAT_R16G16_UNORM,         // TextureFormat::RG16UIntNorm
            DXGI_FORMAT_R16G16_FLOAT,         // TextureFormat::RG16Float
            DXGI_FORMAT_R32G32_SINT,          // TextureFormat::RG32SInt
            DXGI_FORMAT_R32G32_UINT,          // TextureFormat::RG32UInt
            DXGI_FORMAT_R32G32_FLOAT,         // TextureFormat::RG32Float
            DXGI_FORMAT_R32G32B32_SINT,       // TextureFormat::RGB32SInt
            DXGI_FORMAT_R32G32B32_UINT,       // TextureFormat::RGB32UInt
            DXGI_FORMAT_R32G32B32_FLOAT,      // TextureFormat::RGB32Float
            DXGI_FORMAT_R8G8B8A8_SINT,        // TextureFormat::RGBA8SInt
            DXGI_FORMAT_R8G8B8A8_SNORM,       // TextureFormat::RGBA8SIntNorm
            DXGI_FORMAT_R8G8B8A8_UINT,        // TextureFormat::RGBA8UInt
            DXGI_FORMAT_R8G8B8A8_UNORM,       // TextureFormat::RGBA8UIntNorm
            DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,  // TextureFormat::RGBA8UIntNorm_sRGB
            DXGI_FORMAT_B8G8R8A8_UNORM,       // TextureFormat::BGRA8UIntNorm
            DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,  // TextureFormat::BGRA8UIntNorm_sRGB
            DXGI_FORMAT_R16G16B16A16_SINT,    // TextureFormat::RGBA16SInt
            DXGI_FORMAT_R16G16B16A16_SNORM,   // TextureFormat::RGBA16SIntNorm
            DXGI_FORMAT_R16G16B16A16_UINT,    // TextureFormat::RGBA16UInt
            DXGI_FORMAT_R16G16B16A16_UNORM,   // TextureFormat::RGBA16UIntNorm
            DXGI_FORMAT_R16G16B16A16_FLOAT,   // TextureFormat::RGBA16Float
            DXGI_FORMAT_R32G32B32A32_SINT,    // TextureFormat::RGBA32SInt
            DXGI_FORMAT_R32G32B32A32_UINT,    // TextureFormat::RGBA32UInt
            DXGI_FORMAT_R32G32B32A32_FLOAT,   // TextureFormat::RGBA32Float
            DXGI_FORMAT_D32_FLOAT,            // TextureFormat::D32Float
            DXGI_FORMAT_D16_UNORM,            // TextureFormat::D16X0UIntNorm
            DXGI_FORMAT_D24_UNORM_S8_UINT,    // TextureFormat::D24S8UIntNorm
            DXGI_FORMAT_D32_FLOAT_S8X24_UINT, // TextureFormat::D32S8UIntNorm
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto Fill(Ptr<const UInt8> Data, UInt Layer, UInt Width, UInt Height, TextureFormat Layout)
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
            32,     // TextureFormat::D24Float
            32,     // TextureFormat::D32Float
            32,     // TextureFormat::D16X0UIntNorm
            32,     // TextureFormat::D24X0UIntNorm
            32,     // TextureFormat::D24S8UIntNorm
            32,     // TextureFormat::D32X0UIntNorm
        };

        const UInt32 Depth = kMapping[CastEnum(Layout)];

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
        return static_cast<Ptr<D3D11_SUBRESOURCE_DATA>>(nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Usage Value)
    {
        constexpr static D3D11_BIND_FLAG k_Mapping[] = {
            D3D11_BIND_VERTEX_BUFFER,          // Usage::Vertex
            D3D11_BIND_INDEX_BUFFER,           // Usage::Index
            D3D11_BIND_CONSTANT_BUFFER,        // Usage::Uniform
        };
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
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
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(const Ptr<WCHAR> Value)
    {
        Char Buffer[MAX_PATH];
        ::WideCharToMultiByte(CP_ACP, 0, Value, -1, Buffer, MAX_PATH, nullptr, nullptr);
        return SStr(Buffer);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool D3D11Driver::Initialize(Ptr<SDL_Window> Swapchain, UInt32 Width, UInt32 Height)
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

        Bool Successful = (D3D11CreateDevicePtr && CreateDXGIFactoryPtr);

        if (Successful)
        {
            ComPtr<ID3D11Device>        Device;
            ComPtr<ID3D11DeviceContext> DeviceImmediate;

            UInt Flags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
            Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

            const D3D_FEATURE_LEVEL Direct3DFeatureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };

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
                    Log::Info("D3D11Driver: Enabling software mode (WARP)");
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

                    if (Swapchain)
                    {
                        const Ptr<void> Display = SDL_GetPointerProperty(
                            SDL_GetWindowProperties(Swapchain), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                        CreatePass(k_Default, reinterpret_cast<UInt>(Display), Width, Height);
                    }
                }
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
        const UINT Flags = (mCapabilities.Adaptive ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);
        mPasses[k_Default].Display->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, Flags);

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

    void D3D11Driver::CreateBuffer(Object ID, Usage Type, UInt Capacity, CPtr<UInt8> Data)
    {
        const D3D11_BIND_FLAG   Binding    = As(Type);
        const D3D11_BUFFER_DESC Descriptor = CD3D11_BUFFER_DESC(
            Binding != D3D11_BIND_CONSTANT_BUFFER ? Capacity : Align(Capacity, 256),
            Binding,
            Data.empty() ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE,
            0);

        D3D11_SUBRESOURCE_DATA      Content {
            Data.data(), static_cast<UINT>(Data.size())
        };
        Ptr<D3D11_SUBRESOURCE_DATA> Pointer = (Data.empty() ? nullptr : & Content);

        CheckIfFail(mDevice->CreateBuffer(& Descriptor, Pointer, mBuffers[ID].Object.GetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<UInt8> Data)
    {
        const D3D11_BOX Destination  = CD3D11_BOX(Offset, 0, 0, Offset + Data.size(), 1, 1);
        const D3D11_COPY_FLAGS Flags = (Discard ? D3D11_COPY_DISCARD : D3D11_COPY_NO_OVERWRITE);

        // TODO: Investigate if creating a staging buffer and do CopySubResource is better
        mDeviceImmediate->UpdateSubresource1(mBuffers[ID].Object.Get(), 0, & Destination, Data.data(), 0, 0, Flags);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteBuffer(Object ID)
    {
        mBuffers[ID].~D3D11Buffer();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreatePass(Object ID, UInt Display, UInt Width, UInt Height)
    {
        DXGI_SWAP_CHAIN_DESC Description { 0 };
        Description.BufferCount       = 3;
        Description.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Description.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
             | (mCapabilities.Adaptive ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);
        Description.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        Description.BufferDesc.Width  = Width;
        Description.BufferDesc.Height = Height;
        Description.SampleDesc        = { 1, 0 };
        Description.OutputWindow      = reinterpret_cast<HWND>(Display);
        Description.Windowed          = true;

        ComPtr<IDXGIFactory4> DXGIFactor4;
        if (SUCCEEDED(mDeviceFactory.As<IDXGIFactory4>(& DXGIFactor4)))
        {
            Description.SwapEffect = (mCapabilities.Adaptive ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD);
        }
        else
        {
            Description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        }

        CheckIfFail(mDeviceFactory->CreateSwapChain(mDevice.Get(), & Description, mPasses[ID].Display.GetAddressOf()));
        CheckIfFail(mDeviceFactory->MakeWindowAssociation(Description.OutputWindow, DXGI_MWA_NO_WINDOW_CHANGES));

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
            CheckIfFail(mDevice->CreateRenderTargetView(Attachment, & Description, Pass.Color[Index].GetAddressOf()));
        }

        if (Auxiliary)
        {
            Ptr<ID3D11Texture2D> Attachment = mTextures[Auxiliary].Object.Get();

            const CD3D11_DEPTH_STENCIL_VIEW_DESC Description(D3D11_DSV_DIMENSION_TEXTURE2D);
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

    void D3D11Driver::CreatePipeline(Object ID, CPtr<UInt8> Vertex, CPtr<UInt8> Fragment, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties)
    {
        Ref<D3D11Pipeline> Pipeline = mPipelines[ID];

        CheckIfFail(mDevice->CreateVertexShader(Vertex.data(), Vertex.size(), nullptr, Pipeline.VS.GetAddressOf()));
        CheckIfFail(mDevice->CreatePixelShader(Fragment.data(), Fragment.size(), nullptr, Pipeline.PS.GetAddressOf()));

        if (!Geometry.empty())
        {
            CheckIfFail(
                mDevice->CreateGeometryShader(Geometry.data(), Geometry.size(), nullptr, Pipeline.GS.GetAddressOf()));
        }

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

            CheckIfFail(mDevice->CreateBlendState(& Description, Pipeline.BS.GetAddressOf()));
        }

        {
            D3D11_DEPTH_STENCIL_DESC Description = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

            Description.DepthEnable                  = (
                Properties.DepthCondition != TestCondition::Always || Properties.DepthMask);
            Description.DepthFunc                    = As(Properties.DepthCondition);
            Description.DepthWriteMask               = static_cast<D3D11_DEPTH_WRITE_MASK>(Properties.DepthMask);
            Description.StencilEnable                = (
                       Properties.StencilCondition   != TestCondition::Always
                    || Properties.StencilOnFail      != TestAction::Keep
                    || Properties.StencilOnDepthFail != TestAction::Keep
                    || Properties.StencilOnDepthPass != TestAction::Keep);
            Description.StencilReadMask              = Properties.StencilMask;
            Description.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
            Description.FrontFace.StencilFunc        = As(Properties.StencilCondition);
            Description.FrontFace.StencilFailOp      = As(Properties.StencilOnFail);
            Description.FrontFace.StencilDepthFailOp = As(Properties.StencilOnDepthFail);
            Description.FrontFace.StencilPassOp      = As(Properties.StencilOnDepthPass);
            Description.BackFace.StencilFunc         = Description.FrontFace.StencilFunc;
            Description.BackFace.StencilFailOp       = Description.FrontFace.StencilFailOp;
            Description.BackFace.StencilDepthFailOp  = Description.FrontFace.StencilDepthFailOp;
            Description.BackFace.StencilPassOp       = Description.FrontFace.StencilPassOp;

            CheckIfFail(mDevice->CreateDepthStencilState(& Description, Pipeline.DS.GetAddressOf()));
        }

        {
            D3D11_RASTERIZER_DESC Description = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());

            Description.CullMode		  = As(Properties.Cull);
            Description.ScissorEnable     = TRUE;
            Description.MultisampleEnable = TRUE;
            Description.FillMode          = Properties.Fill ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

            CheckIfFail(mDevice->CreateRasterizerState(& Description, Pipeline.RS.GetAddressOf()));
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

            CheckIfFail(mDevice->CreateInputLayout(Description, Count, Vertex.data(), Vertex.size(), Pipeline.IL.GetAddressOf()));
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

    void D3D11Driver::CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt8> Data)
    {
        CD3D11_TEXTURE2D_DESC Description(As(Format), Width, Height, 1, Layer);
        Description.Usage     = (Data.empty() ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE);
        Description.BindFlags = (Layout != TextureLayout::Destination ? D3D11_BIND_SHADER_RESOURCE : 0);
        Description.MiscFlags = (Layer < 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);

        if (Layout != TextureLayout::Source)
        {
            switch (Format)
            {
            case TextureFormat::D32Float:
            case TextureFormat::D16X0UIntNorm:
            case TextureFormat::D24X0UIntNorm:
            case TextureFormat::D24S8UIntNorm:
            case TextureFormat::D32S8UIntNorm:
                Description.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
                break;
            default:
                Description.BindFlags |= D3D11_BIND_RENDER_TARGET;
                break;
            }
        }

        Ptr<const D3D11_SUBRESOURCE_DATA> Memory = Fill(Data.data(), Layer, Width, Height, Format);
        CheckIfFail(mDevice->CreateTexture2D(& Description, Memory, & mTextures[ID].Object));

        if (Layout != TextureLayout::Destination)
        {
            const CD3D11_SHADER_RESOURCE_VIEW_DESC Type(D3D11_SRV_DIMENSION_TEXTURE2D, As(Format), 0, Layer);
            CheckIfFail(mDevice->CreateShaderResourceView(
                mTextures[ID].Object.Get(), & Type, mTextures[ID].Resource.GetAddressOf()));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt8> Data)
    {
        const D3D11_BOX Rect {
            static_cast<UINT>(Offset.GetLeft()),
            static_cast<UINT>(Offset.GetTop()),
            0,
            static_cast<UINT>(Offset.GetRight()),
            static_cast<UINT>(Offset.GetBottom()),
            1
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

    Data D3D11Driver::ReadTexture(Object ID, UInt Level, Recti Offset)
    {
        D3D11_TEXTURE2D_DESC Description;
        mTextures[ID].Object->GetDesc(& Description);

        // Create a staging texture to be able to read the texture back.
        ComPtr<ID3D11Texture2D> Buffer;

        Description.BindFlags      = 0;
        Description.Usage          = D3D11_USAGE_STAGING;
        Description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        CheckIfFail(mDevice->CreateTexture2D(& Description, nullptr, Buffer.GetAddressOf()));

        // Copy the resource between GPU -> CPU.
        const D3D11_BOX SrcData = CD3D11_BOX(
            Offset.GetLeft(), Offset.GetTop(), 0, Offset.GetRight(), Offset.GetBottom(), 1);
        mDeviceImmediate->CopySubresourceRegion(Buffer.Get(), 0, 0, 0, 0, mTextures[ID].Object.Get(), Level, & SrcData);

        // Map the staging texture and copy into a new buffer.
        Data                     Bytes;
        D3D11_MAPPED_SUBRESOURCE Memory;

        if (SUCCEEDED(mDeviceImmediate->Map(Buffer.Get(), 0, D3D11_MAP_READ, 0, & Memory)))
        {
            Bytes = Data(Description.Height * Memory.RowPitch);

            std::memcpy(Bytes.GetData(), Memory.pData, Description.Height * Memory.RowPitch);

            mDeviceImmediate->Unmap(Buffer.Get(), 0);
        }
        return Move(Bytes);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt D3D11Driver::QueryTexture(Object ID)
    {
        Ptr<IDXGISurface> Surface;
        mTextures[ID].Object->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<Ptr<LPVOID>>(& Surface));
        return reinterpret_cast<UInt>(Surface);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::DeleteTexture(Object ID)
    {
        mTextures[ID].~D3D11Texture();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil)
    {
        Array<Ptr<ID3D11RenderTargetView>, k_MaxAttachments> Attachments { 0 };

        for (UInt Index = 0; Index < k_MaxAttachments; ++Index)
        {
            Ref<const ComPtr<ID3D11RenderTargetView>> View = mPasses[ID].Color[Index];
            if (View)
            {
                Attachments[Index] = View.Get();

                if (Target != Clear::Auxiliary)
                {
                    const Array<FLOAT, 4> Color {
                        Tint.GetRed(), Tint.GetGreen(), Tint.GetBlue(), Tint.GetAlpha()
                    };
                    mDeviceImmediate->ClearRenderTargetView(View.Get(), Color.data());
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
        constexpr Submission k_DefaultSubmission { .Scissor = { 0, 0, 0, 0 } };

        for (UInt Batch = 0; Batch < Submissions.size(); ++Batch)
        {
            Ref<const Submission> NewestSubmission = Submissions[Batch];
            Ref<const Submission> OldestSubmission = (Batch > 0 ? Submissions[Batch - 1] : k_DefaultSubmission);

            // Apply vertices
            ApplyVertexResources(OldestSubmission, NewestSubmission);

            // Apply indices
            if (OldestSubmission.Indices.Buffer != NewestSubmission.Indices.Buffer ||
                OldestSubmission.Indices.Offset != NewestSubmission.Indices.Offset ||
                OldestSubmission.Indices.Stride != NewestSubmission.Indices.Stride)
            {
                Ref<const D3D11Buffer> Buffer = mBuffers[NewestSubmission.Indices.Buffer];
                const DXGI_FORMAT      Format = (NewestSubmission.Indices.Stride == sizeof(UInt16)
                    ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);
                mDeviceImmediate->IASetIndexBuffer(Buffer.Object.Get(), Format, NewestSubmission.Indices.Offset);
            }

            // Apply the scissor rect
            if (OldestSubmission.Scissor != NewestSubmission.Scissor)
            {
                const RECT Rect {
                    NewestSubmission.Scissor.GetLeft(),  NewestSubmission.Scissor.GetTop(),
                    NewestSubmission.Scissor.GetRight(), NewestSubmission.Scissor.GetBottom()
                };
                mDeviceImmediate->RSSetScissorRects(1, & Rect);
            }

            // Apply pipeline or stencil value
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
            if (NewestSubmission.Indices.Buffer)
            {
                const UINT IndicesCount          = NewestSubmission.Primitive.Count;
                const UINT IndicesStartLocation  = NewestSubmission.Primitive.Offset;
                const UINT VerticesStartLocation = NewestSubmission.Primitive.Base;

                mDeviceImmediate->DrawIndexed(IndicesCount, IndicesStartLocation, VerticesStartLocation);
            }
            else
            {
                const UINT VerticesCount         = NewestSubmission.Primitive.Count;
                const UINT VerticesStartLocation = NewestSubmission.Primitive.Offset;

                mDeviceImmediate->Draw(VerticesCount, VerticesStartLocation);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::Commit(Object ID, Bool Synchronised)
    {
        Ref<const ComPtr<IDXGISwapChain>> Display = mPasses[ID].Display;

        if (Display)
        {
            const UInt Sync = Synchronised ? 1 : 0;
            const UInt Flag = Synchronised ? 0 : mCapabilities.Adaptive ? DXGI_PRESENT_ALLOW_TEARING : 0;
            Display->Present(Sync, Flag);
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
            if (SUCCEEDED(DXGIAdapter->GetDesc1(AddressOf(DXGIDescription))))
            {
                if ((DXGIDescription.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0)
                {
                    Ref<Graphic::Adapter> AdapterInfo = mCapabilities.Adapters.emplace_back();

                    AdapterInfo.Description          = As(DXGIDescription.Description);
                    AdapterInfo.DedicatedMemoryInMBs = DXGIDescription.DedicatedVideoMemory >> 20;
                    AdapterInfo.SharedMemoryInMBs    = DXGIDescription.SharedSystemMemory >> 20;
                    AdapterInfo.SystemMemoryInMBs    = DXGIDescription.DedicatedSystemMemory >> 20;
                }
            }

            ComPtr<IDXGIOutput> DXGIOutput;
            if (SUCCEEDED(DXGIAdapter->EnumOutputs(0, DXGIOutput.GetAddressOf())))
            {
                UINT        Length = 0;
                DXGI_FORMAT Format = DXGI_FORMAT_B8G8R8A8_UNORM;

                if (SUCCEEDED(DXGIOutput->GetDisplayModeList(Format, 0, AddressOf(Length), nullptr)))
                {
                    Vector<DXGI_MODE_DESC> Descriptions(Length);
                    DXGIOutput->GetDisplayModeList(Format, 0, AddressOf(Length), Descriptions.data());

                    for (Ref<DXGI_MODE_DESC> Description : Descriptions)
                    {
                        Ref<Graphic::Resolution> Resolution = mCapabilities.Adapters[Index].Displays.emplace_back();
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

        // Check if we support tearing mode
        ComPtr<IDXGIFactory5> DXGIFactory5;
        if (SUCCEEDED(mDeviceFactory.As<IDXGIFactory5>(& DXGIFactory5)))
        {
            BOOL AllowAdaptive = FALSE;
            DXGIFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, & AllowAdaptive, sizeof(AllowAdaptive));
            mCapabilities.Adaptive = AllowAdaptive;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt Width, UInt Height)
    {
        ComPtr<ID3D11Texture2D> Color;
        Pass.Display->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(Color.GetAddressOf()));
        CheckIfFail(mDevice->CreateRenderTargetView(Color.Get(), nullptr, Pass.Color[k_Default].GetAddressOf()));

        D3D11_TEXTURE2D_DESC Description { 0 };
        Description.Width      = Width;
        Description.Height     = Height;
        Description.MipLevels  = 1;
        Description.ArraySize  = 1;
        Description.Format     = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        Description.SampleDesc = { 1, 0 };
        Description.Usage      = D3D11_USAGE_DEFAULT;
        Description.BindFlags  = D3D11_BIND_DEPTH_STENCIL;

        ComPtr<ID3D11Texture2D> Depth;
        CheckIfFail(mDevice->CreateTexture2D(& Description, nullptr, Depth.GetAddressOf()));
        CheckIfFail(mDevice->CreateDepthStencilView(Depth.Get(), nullptr, Pass.Auxiliary.GetAddressOf()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<D3D11Driver::D3D11Sampler> D3D11Driver::GetOrCreateSampler(Ref<const Sampler> Descriptor)
    {
        Ref<D3D11Sampler> Sampler = mSamplers[
            static_cast<UInt>(Descriptor.EdgeU)       |
            static_cast<UInt>(Descriptor.EdgeV)  << 2 |
            static_cast<UInt>(Descriptor.Filter) << 4];

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

            CheckIfFail(mDevice->CreateSamplerState(& SamplerDescriptor, Sampler.Resource.GetAddressOf()));
        }
        return Sampler;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyVertexResources(Ref<const Submission> Oldest, Ref<const Submission> Newest)
    {
        Ptr<ID3D11Buffer> Array[k_MaxFetches];
        UINT ArrayOffset[k_MaxFetches];
        UINT ArrayStride[k_MaxFetches];
        UInt Min = k_MaxFetches;
        UInt Max = 0u;

        for (UInt Element = 0; Element < k_MaxFetches; ++Element)
        {
            Ref<const Binding> Old = Oldest.Vertices[Element];
            Ref<const Binding> New = Newest.Vertices[Element];

            if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Stride != New.Stride)
            {
                Min = Core::Min(Element, Min);
                Max = Core::Max(Element + 1, Max);
            }

            Array[Element]       = mBuffers[New.Buffer].Object.Get();
            ArrayOffset[Element] = New.Offset;
            ArrayStride[Element] = New.Stride;
        }

        if (Min != k_MaxFetches && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->IASetVertexBuffers(Min, Count, Array + Min, ArrayStride + Min, ArrayOffset + Min);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplySamplerResources(Ref<const Submission> Oldest, Ref<const Submission> Newest)
    {
        Ptr<ID3D11SamplerState> Array[k_MaxSources];
        UInt Min = k_MaxSources;
        UInt Max = 0u;

        for (UInt Element = 0; Element < k_MaxSources; ++Element)
        {
            Ref<const D3D11Sampler> PrevSampler = GetOrCreateSampler(Oldest.Samplers[Element]);
            Ref<const D3D11Sampler> NextSampler = GetOrCreateSampler(Newest.Samplers[Element]);

            if (PrevSampler.Resource != NextSampler.Resource)
            {
                Min = Core::Min(Element, Min);
                Max = Core::Max(Element + 1, Max);
            }
            Array[Element] = NextSampler.Resource.Get();
        }

        if (Min != k_MaxSources && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->PSSetSamplers(Min, Count, Array + Min); // TODO: Do we also need other stages?
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyTextureResources(Ref<const Submission> Oldest, Ref<const Submission> Newest)
    {
        Ptr<ID3D11ShaderResourceView> Array[k_MaxSources];
        UInt Min = k_MaxSources;
        UInt Max = 0u;

        for (UInt Element = 0; Element < k_MaxSources && Newest.Textures[Element] != 0; ++Element)
        {
            if (Oldest.Textures[Element] != Newest.Textures[Element])
            {
                Min = Core::Min(Element, Min);
                Max = Core::Max(Element + 1, Max);
            }
            Array[Element] = mTextures[Newest.Textures[Element]].Resource.Get();
        }

        if (Min != k_MaxSources && Max > 0)
        {
            const UInt Count = Max - Min;
            mDeviceImmediate->PSSetShaderResources(Min, Count, Array + Min);    // TODO: Do we also need other stages?
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void D3D11Driver::ApplyUniformResources(Ref<const Submission> Oldest, Ref<const Submission> Newest)
    {
        Ptr<ID3D11Buffer> Array[k_MaxUniforms];
        UINT ArrayOffset[k_MaxUniforms];
        UINT ArrayLength[k_MaxUniforms];
        UInt Min = k_MaxUniforms;
        UInt Max = 0u;

        for (UInt Element = 0; Element < k_MaxUniforms; ++Element)
        {
            Ref<const Binding> Old = Oldest.Uniforms[Element];
            Ref<const Binding> New = Newest.Uniforms[Element];

            if (Old.Buffer != New.Buffer || Old.Offset != New.Offset || Old.Length != New.Length)
            {
                Min = Core::Min(Element, Min);
                Max = Core::Max(Element + 1, Max);
            }

            Array[Element]       = mBuffers[New.Buffer].Object.Get();
            ArrayOffset[Element] = Align(New.Offset, 16);
            ArrayLength[Element] = Align(New.Length, 16);
        }

        if (Min != k_MaxUniforms && Max > 0)
        {
            const UInt Count = Max - Min;

            // TODO: Do we also need other stages?
            mDeviceImmediate->VSSetConstantBuffers1(Min, Count, Array + Min, ArrayOffset + Min, ArrayLength + Min);
            mDeviceImmediate->PSSetConstantBuffers1(Min, Count, Array + Min, ArrayOffset + Min, ArrayLength + Min);
            mDeviceImmediate->GSSetConstantBuffers1(Min, Count, Array + Min, ArrayOffset + Min, ArrayLength + Min);
        }
    }
}

