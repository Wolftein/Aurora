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

#include "Aurora.Graphic/Driver.hpp"
#include <d3d11_4.h>
#include <wrl/client.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class D3D11Driver final : public Driver
    {
    public:

        // \see Driver::Initialize
        Bool Initialize(Ptr<SDL_Window> Swapchain, UInt32 Width, UInt32 Height) override;

        // \see Driver::Reset
        void Reset(UInt Width, UInt Height) override;

        // \see Driver::GetCapabilities
        Ref<const Capabilities> GetCapabilities() const override;

        // \see Driver::CreateBuffer
        void CreateBuffer(Object ID, Usage Type, UInt Capacity, CPtr<UInt8> Data) override;

        // \see Driver::UpdateBuffer
        void UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<UInt8> Data) override;

        // \see Driver::DeleteBuffer
        void DeleteBuffer(Object ID) override;

        // -=(Undocumented)=-
        void CreatePass(Object ID, UInt Display, UInt Width, UInt Height);

        // \see Driver::CreatePass
        void CreatePass(Object ID, CPtr<Object> Colors, Object Auxiliary) override;

        // \see Driver::DeletePass
        void DeletePass(Object ID) override;

        // \see Driver::CreatePipeline
        void CreatePipeline(Object ID, CPtr<UInt8> Vertex, CPtr<UInt8> Fragment, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties) override;

        // \see Driver::DeletePipeline
        void DeletePipeline(Object ID) override;

        // \see Driver::CreateTexture
        void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt8> Data) override;

        // \see Driver::UpdateTexture
        void UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt8> Data) override;

        // \see Driver::CopyTexture
        void CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset) override;

        // \see Driver::ReadTexture
        Data ReadTexture(Object ID, UInt Level, Recti Offset) override;

        // \see Driver::QueryTexture
        UInt QueryTexture(Object ID) override;

        // \see Driver::DeleteTexture
        void DeleteTexture(Object ID) override;

        // \see Driver::Prepare
        void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil) override;

        // \see Driver::Submit
        void Submit(CPtr<Submission> Submissions) override;

        // \see Driver::Commit
        void Commit(Object ID, Bool Synchronised) override;

    private:

        // -=(Undocumented)=-
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;

        // -=(Undocumented)=-
        struct D3D11Buffer
        {
            ComPtr<ID3D11Buffer>           Object;
        };

        // -=(Undocumented)=-
        struct D3D11Pass
        {
            ComPtr<IDXGISwapChain>         Display;
            ComPtr<ID3D11RenderTargetView> Color[k_MaxAttachments];
            ComPtr<ID3D11DepthStencilView> Auxiliary;
        };

        // -=(Undocumented)=-
        struct D3D11Pipeline
        {
            ComPtr<ID3D11VertexShader>      VS;
            ComPtr<ID3D11PixelShader>       PS;
            ComPtr<ID3D11GeometryShader>    GS;
            ComPtr<ID3D11BlendState>        BS;
            ComPtr<ID3D11DepthStencilState> DS;
            ComPtr<ID3D11RasterizerState>   RS;
            ComPtr<ID3D11InputLayout>       IL;
            D3D11_PRIMITIVE_TOPOLOGY        PT;
        };

        // -=(Undocumented)=-
        struct D3D11Sampler
        {
            ComPtr<ID3D11SamplerState> Resource;
        };

        // -=(Undocumented)=-
        struct D3D11Texture
        {
            ComPtr<ID3D11Texture2D>          Object;
            ComPtr<ID3D11ShaderResourceView> Resource;
        };

    private:

        // -=(Undocumented)=-
        void LoadAdapters();

        // -=(Undocumented)=-
        void LoadCapabilities();

        // -=(Undocumented)=-
        void CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt Width, UInt Height);

        // -=(Undocumented)=-
        Ref<D3D11Sampler> GetOrCreateSampler(Ref<const Sampler> Descriptor);

        // -=(Undocumented)=-
        void ApplyVertexResources(Ref<const Submission> Oldest, Ref<const Submission> Newest);

        // -=(Undocumented)=-
        void ApplySamplerResources(Ref<const Submission> Oldest, Ref<const Submission> Newest);

        // -=(Undocumented)=-
        void ApplyTextureResources(Ref<const Submission> Oldest, Ref<const Submission> Newest);

        // -=(Undocumented)=-
        void ApplyUniformResources(Ref<const Submission> Oldest, Ref<const Submission> Newest);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ComPtr<ID3D11Device1>        mDevice;
        ComPtr<ID3D11DeviceContext1> mDeviceImmediate;
        ComPtr<IDXGIFactory1>        mDeviceFactory;
        Capabilities                 mCapabilities;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        D3D11Buffer                  mBuffers[k_MaxBuffers];
        D3D11Pass                    mPasses[k_MaxPasses];
        D3D11Pipeline                mPipelines[k_MaxPipelines];
        D3D11Sampler                 mSamplers[k_MaxSamplers];
        D3D11Texture                 mTextures[k_MaxTextures];
    };
}