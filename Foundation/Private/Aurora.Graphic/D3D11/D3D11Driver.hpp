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
        Bool Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples) override;

        // \see Driver::Reset
        void Reset(UInt16 Width, UInt16 Height, UInt8 Samples) override;

        // \see Driver::GetCapabilities
        ConstRef<Capabilities> GetCapabilities() const override;

        // \see Driver::CreateBuffer
        void CreateBuffer(Object ID, Usage Type, UInt32 Capacity, CPtr<const UInt8> Data) override;

        // \see Driver::CopyBuffer
        void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size) override;

        // \see Driver::UpdateBuffer
        void UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, CPtr<const UInt8> Data) override;

        // \see Driver::DeleteBuffer
        void DeleteBuffer(Object ID) override;

        // \see Driver::CreatePass
        void CreatePass(Object ID, CPtr<const Attachment> Colors, CPtr<const Attachment> Resolves, ConstRef<Attachment> Auxiliary) override;

        // \see Driver::DeletePass
        void DeletePass(Object ID) override;

        // \see Driver::CreatePipeline
        void CreatePipeline(Object ID, CPtr<const UInt8> Vertex, CPtr<const UInt8> Fragment, CPtr<const UInt8> Geometry, ConstRef<Descriptor> Properties) override;

        // \see Driver::DeletePipeline
        void DeletePipeline(Object ID) override;

        // \see Driver::CreateTexture
        void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Level, UInt8 Samples, CPtr<const UInt8> Data) override;

        // \see Driver::UpdateTexture
        void UpdateTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset, UInt32 Pitch, CPtr<const UInt8> Data) override;

        // \see Driver::CopyTexture
        void CopyTexture(Object DstTexture, UInt8 DstLevel, ConstRef<Vector2i> DstOffset, Object SrcTexture, UInt8 SrcLevel, ConstRef<Recti> SrcOffset) override;

        // \see Driver::ReadTexture
        Data ReadTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset) override;

        // \see Driver::DeleteTexture
        void DeleteTexture(Object ID) override;

        // \see Driver::Prepare
        void Prepare(Object ID, ConstRef<Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil) override;

        // \see Driver::Submit
        void Submit(CPtr<const Submission> Submissions) override;

        // \see Driver::Commit
        void Commit(Object ID, Bool Synchronised) override;

    private:

        // -=(Undocumented)=-
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;

        // -=(Undocumented)=-
        struct D3D11Attachment
        {
            ComPtr<ID3D11Resource>         Object;
            ComPtr<ID3D11RenderTargetView> Resource;
            UInt32                         Level;
        };

        // -=(Undocumented)=-
        struct D3D11Limits
        {
            Array<DXGI_SAMPLE_DESC, k_MaxSamples + 1> Multisample;
        };

        // -=(Undocumented)=-
        struct D3D11Buffer
        {
            ComPtr<ID3D11Buffer>           Object;
        };

        // -=(Undocumented)=-
        struct D3D11Pass
        {
            ComPtr<IDXGISwapChain>         Display;
            D3D11Attachment                Color[k_MaxAttachments];     // @TODO Stack
            D3D11Attachment                Resolves[k_MaxAttachments];  // @TODO Stack
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
        void CreateSwapchain(Ref<D3D11Pass> Pass, UInt Display, UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        void CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        Ref<D3D11Sampler> GetOrCreateSampler(ConstRef<Sampler> Descriptor);

        // -=(Undocumented)=-
        void ApplyVertexResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        // -=(Undocumented)=-
        void ApplySamplerResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        // -=(Undocumented)=-
        void ApplyTextureResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        // -=(Undocumented)=-
        void ApplyUniformResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ComPtr<ID3D11Device1>        mDevice;
        ComPtr<ID3D11DeviceContext1> mDeviceImmediate;
        ComPtr<IDXGIFactory1>        mDeviceFactory;
        Capabilities                 mCapabilities;
        D3D11Limits                  mLimits;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        D3D11Buffer                  mBuffers[k_MaxBuffers];
        D3D11Pass                    mPasses[k_MaxPasses];
        D3D11Pipeline                mPipelines[k_MaxPipelines];
        D3D11Sampler                 mSamplers[k_MaxSamplers];
        D3D11Texture                 mTextures[k_MaxTextures];
    };
}