// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
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

        // \see Driver::CreateBuffer
        void CreateBuffer(UInt ID, Bool Geometry, UInt Capacity, CPtr<UInt08> Data) override;

        // \see Driver::UpdateBuffer
        void UpdateBuffer(UInt ID, Bool Discard, UInt Offset, CPtr<UInt08> Data) override;

        // \see Driver::DeleteBuffer
        void DeleteBuffer(UInt ID) override;

        // \see Driver::CreatePass
        void CreatePass(UInt ID, CPtr<UInt> Colors, UInt Auxiliary) override;

        // \see Driver::DeletePass
        void DeletePass(UInt ID) override;

        // \see Driver::CreatePipeline
        void CreatePipeline(UInt ID, CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties) override;

        // \see Driver::DeletePipeline
        void DeletePipeline(UInt ID) override;

        // \see Driver::CreateSampler
        void CreateSampler(UInt ID, TextureEdge EdgeX, TextureEdge EdgeY, TextureFilter Filter) override;

        // \see Driver::DeleteSampler
        void DeleteSampler(UInt ID) override;

        // \see Driver::CreateTexture
        void CreateTexture(UInt ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data) override;

        // \see Driver::UpdateTexture
        void UpdateTexture(UInt ID, UInt Level, Rect Offset, UInt Pitch, CPtr<UInt08> Data) override;

        // \see Driver::DeleteTexture
        void DeleteTexture(UInt ID) override;

        // \see Driver::Prepare
        void Prepare(UInt ID, Rect Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil) override;

        // \see Driver::Submit
        void Submit(CPtr<Submission> Submissions) override;

        // \see Driver::Commit
        void Commit(Bool Synchronised) override;

    private:

        // -=(Undocumented)=-
        template<typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;

        // -=(Undocumented)=-
        struct D3D11Buffer
        {
            ComPtr<ID3D11Buffer> Resource;
        };

        // -=(Undocumented)=-
        struct D3D11Pass
        {
            ComPtr<ID3D11RenderTargetView> Color[k_MaxAttachments];
            ComPtr<ID3D11DepthStencilView> Auxiliary;
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
        void CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt Width, UInt Height);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ComPtr<ID3D11Device1>        mDevice;
        ComPtr<ID3D11DeviceContext1> mDeviceImmediate;
        ComPtr<IDXGISwapChain>       mDisplay;
        ComPtr<IDXGIFactory1>        mDisplayFactory;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        D3D11Buffer                  mBuffers[k_MaxBuffers];
        D3D11Pass                    mPasses[k_MaxPasses];
        D3D11Pipeline                mPipelines[k_MaxPipelines];
        D3D11Sampler                 mSamplers[k_MaxSamplers];
        D3D11Texture                 mTextures[k_MaxTextures];
    };
}