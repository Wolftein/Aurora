// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Graphic/Driver.hpp"
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

        // \see Driver::Initialise
        Bool Initialise(Any Display, UInt Width, UInt Height) override;

        // \see Driver::Reset
        void Reset(UInt Width, UInt Height) override;

        // \see Driver::GetCapabilities
        Ref<const Capabilities> GetCapabilities() const override;

        // \see Driver::CreateBuffer
        void CreateBuffer(Object ID, Bool Geometry, UInt Capacity, CPtr<UInt08> Data) override;

        // \see Driver::Map
        Ptr<void> Map(Object ID, Ref<UInt> Offset, UInt Length) override;

        // \see Driver::Map
        Ptr<void> Map(Object ID, Bool Discard, UInt Offset, UInt Length) override;

        // \see Driver::Unmap
        void Unmap(Object ID) override;

        // \see Driver::DeleteBuffer
        void DeleteBuffer(Object ID) override;

        // \see Driver::CreatePass
        void CreatePass(Object ID, Any Display, UInt Width, UInt Height) override;

        // \see Driver::CreatePass
        void CreatePass(Object ID, CPtr<UInt> Colors, UInt Auxiliary) override;

        // \see Driver::DeletePass
        void DeletePass(Object ID) override;

        // \see Driver::CreatePipeline
        void CreatePipeline(Object ID, CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties) override;

        // \see Driver::DeletePipeline
        void DeletePipeline(Object ID) override;

        // \see Driver::CreateSampler
        void CreateSampler(Object ID, TextureEdge EdgeU, TextureEdge EdgeV, TextureFilter Filter) override;

        // \see Driver::DeleteSampler
        void DeleteSampler(Object ID) override;

        // \see Driver::CreateTexture
        void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data) override;

        // \see Driver::UpdateTexture
        void UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt08> Data) override;

        // \see Driver::DeleteTexture
        void DeleteTexture(Object ID) override;

        // \see Driver::Prepare
        void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil) override;

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
            ComPtr<ID3D11Buffer> Resource;
            UInt                 Offset;
            UInt                 Capacity;
        };

        // -=(Undocumented)=-
        struct D3D11Pass
        {
            ComPtr<ID3D11RenderTargetView> Color[k_MaxAttachments];
            ComPtr<ID3D11DepthStencilView> Auxiliary;
            ComPtr<IDXGISwapChain>         Display;
        };

        // -=(Undocumented)=-
        struct D3D11Pipeline
        {
            ComPtr<ID3D11VertexShader>      VS;
            ComPtr<ID3D11PixelShader>       PS;
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
        void LoadCapabilities();

        // -=(Undocumented)=-
        void CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt Width, UInt Height);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ComPtr<ID3D11Device1>        mDevice;
        ComPtr<ID3D11DeviceContext1> mDeviceImmediate;
        ComPtr<IDXGIFactory1>        mDisplayFactory;
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