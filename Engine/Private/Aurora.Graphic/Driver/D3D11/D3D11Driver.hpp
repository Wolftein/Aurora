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

#include "Aurora.Graphic/Driver.hpp"
#include <d3d11_4.h>
#include <wrl/client.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Encapsulate the graphic driver implementation using Direct3D 11.
    class D3D11Driver final : public Driver
    {
    public:

        /// \copydoc Driver::Initialize
        Bool Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, Samples Samples) override;

        /// \copydoc Driver::Reset
        void Reset(UInt16 Width, UInt16 Height, Samples Samples) override;

        /// \copydoc Driver::GetCapabilities
        ConstRef<Capabilities> GetCapabilities() const override;

        /// \copydoc Driver::CreateBuffer
        void CreateBuffer(Object ID, Access Access, Usage Usage, UInt32 Length, ConstSpan<Byte> Data) override;

        /// \copydoc Driver::UpdateBuffer
        void UpdateBuffer(Object ID, UInt32 Offset, Bool Invalidate, ConstSpan<Byte> Data) override;

        /// \copydoc Driver::DeleteBuffer
        void DeleteBuffer(Object ID) override;

        /// \copydoc Driver::ResizeBuffer
        void ResizeBuffer(Object ID, UInt32 Size) override;

        /// \copydoc Driver::CopyBuffer
        void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size) override;

        /// \copydoc Driver::ReadBuffer
        Blob ReadBuffer(Object ID, UInt32 Offset, UInt32 Size) override;

        /// \copydoc Driver::MapBuffer
        Ptr<Byte> MapBuffer(Object ID, UInt32 Offset, UInt32 Size, Bool Invalidate) override;

        /// \copydoc Driver::UnmapBuffer
        void UnmapBuffer(Object ID) override;

        /// \copydoc Driver::CreatePass
        void CreatePass(Object ID, ConstSpan<Attachment> Colors, ConstRef<Attachment> Auxiliary) override;

        /// \copydoc Driver::DeletePass
        void DeletePass(Object ID) override;

        /// \copydoc Driver::CreatePipeline
        void CreatePipeline(Object ID, ConstSpan<Byte> Vertex, ConstSpan<Byte> Fragment, ConstRef<Descriptor> Descriptor) override;

        /// \copydoc Driver::DeletePipeline
        void DeletePipeline(Object ID) override;

        /// \copydoc Driver::CreateTexture
        void CreateTexture(Object ID, Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Mipmaps, Samples Samples, ConstSpan<Byte> Data) override;

        /// \copydoc Driver::UpdateTexture
        void UpdateTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height, UInt32 Pitch, ConstSpan<Byte> Data) override;

        /// \copydoc Driver::DeleteTexture
        void DeleteTexture(Object ID) override;

        /// \copydoc Driver::CopyTexture
        void CopyTexture(Object DstTexture, UInt8 DstLevel, UInt16 DstX, UInt16 DstY, Object SrcTexture, UInt8 SrcLevel, UInt16 SrcX, UInt16 SrcY, UInt16 Width, UInt16 Height) override;

        /// \copydoc Driver::ReadTexture
        Blob ReadTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height) override;

        /// \copydoc Driver::Prepare
        void Prepare(Object ID, ConstRef<Viewport> Viewport, Clear Target, ConstRef<Color> Tint, Real32 Depth, UInt8 Stencil) override;

        /// \copydoc Driver::Submit
        void Submit(ConstSpan<Submission> Submissions) override;

        /// \copydoc Driver::Commit
        void Commit(Object ID) override;

    private:

        /// \brief Smart pointer for managing COM (Component Object Model) objects.
        ///
        /// \tparam Type COM interface type (must implement IUnknown)
        template<typename Type>
        using ComPtr = Microsoft::WRL::ComPtr<Type>;

        /// \brief Internal wrapper for Direct3D 11 attachment resources.
        struct D3D11Attachment final
        {
            ComPtr<ID3D11RenderTargetView> Accessor;
            ComPtr<ID3D11Resource>         Source;
            UInt32                         SourceLevel;
            ComPtr<ID3D11Resource>         Target;
            UInt32                         TargetLevel;
            DXGI_FORMAT                    TargetFormat;
        };

        /// \brief Internal wrapper for Direct3D 11 buffer resources.
        struct D3D11Buffer final
        {
            ComPtr<ID3D11Buffer> Resource;
            D3D11_USAGE          Kind;
        };

        /// \brief Internal wrapper for Direct3D 11 pass resources.
        struct D3D11Pass final
        {
            /// \brief Destructor.
            AURORA_INLINE ~D3D11Pass()
            {
                Colors.clear();
            }

            Vector<D3D11Attachment, kMaxAttachments> Colors;
            ComPtr<ID3D11DepthStencilView>           Auxiliary;
        };

        /// \brief Internal wrapper for Direct3D 11 pipeline resources.
        struct D3D11Pipeline final
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

        /// \brief Direct3D 11 device capabilities and feature support.
        struct D3D11Properties final
        {
            /// Supported multisampling configurations.
            Array<DXGI_SAMPLE_DESC, kMaxSamples + 1> Multisample;   // TODO Add per format

            /// Indicates support for variable refresh rate (tearing).
            Bool                                     Tearing;
        };

        /// \brief Internal wrapper for Direct3D 11 sampler resources.
        struct D3D11Sampler final
        {
            ComPtr<ID3D11SamplerState> Resource;
        };

        /// \brief Internal wrapper for Direct3D 11 texture resources.
        struct D3D11Texture final
        {
            ComPtr<ID3D11Texture2D>          Resource;
            ComPtr<ID3D11ShaderResourceView> Accessor;
            TextureFormat                    Format;
        };

        /// \brief Queries available GPU adapters.
        void LoadAdapters();

        /// \brief Queries device capabilities.
        void LoadCapabilities();

        /// \brief Initializes default rendering states.
        void LoadStates();

        /// \brief Creates a swapchain for the specified display and associates it with a render pass.
        ///
        /// \param Pass    Render pass object that will be linked to the swapchain.
        /// \param Display Display index to present to.
        /// \param Width   Width of the swapchain back-buffer in pixels.
        /// \param Height  Height of the swapchain back-buffer in pixels.
        /// \param Samples Multisampling count for anti-aliasing.
        void CreateSwapchain(Ref<D3D11Pass> Pass, UInt Display, UInt16 Width, UInt16 Height, UInt8 Samples);

        /// \brief Creates the render targets and depth resources for a swapchain-backed render pass.
        ///
        /// \param Pass    Render pass object that will own the swapchain attachments.
        /// \param Width   Width of the back-buffer in pixels.
        /// \param Height  Height of the back-buffer in pixels.
        /// \param Samples Multisampling count for anti-aliasing.
        void CreateSwapchainResources(Ref<D3D11Pass> Pass, UInt16 Width, UInt16 Height, UInt8 Samples);

        /// \brief Retrieves an existing sampler or creates a new one.
        ///
        /// \param Descriptor Sampler description used to configure the sampler state.
        /// \return Reference to the retrieved or newly created sampler.
        Ref<D3D11Sampler> GetOrCreateSampler(ConstRef<Sampler> Descriptor);

        /// \brief Applies vertex buffer bindings from the latest submission.
        ///
        /// \param Oldest Previous submission state used for comparison.
        /// \param Newest Current submission state containing updated resources.
        void ApplyVertexResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        /// \brief Applies sampler bindings from the latest submission.
        ///
        /// \param Oldest Previous submission state used for comparison.
        /// \param Newest Current submission state containing updated resources.
        void ApplySamplerResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        /// \brief Applies texture bindings from the latest submission.
        ///
        /// \param Oldest Previous submission state used for comparison.
        /// \param Newest Current submission state containing updated resources.
        void ApplyTextureResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        /// \brief Applies uniform buffer bindings from the latest submission.
        ///
        /// \param Oldest Previous submission state used for comparison.
        /// \param Newest Current submission state containing updated resources.
        void ApplyUniformResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Capabilities                        mCapabilities;
        D3D11Properties                     mProperties;
        ComPtr<ID3D11Device1>               mDevice;
        ComPtr<ID3D11DeviceContext1>        mDeviceImmediate;
        ComPtr<IDXGIFactory1>               mDeviceFactory;
        Submission                          mDeviceStates;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        ComPtr<IDXGISwapChain>              mDisplay;
        Array<D3D11Buffer, kMaxBuffers>     mBuffers;
        Array<D3D11Pass, kMaxPasses>        mPasses;
        Array<D3D11Pipeline, kMaxPipelines> mPipelines;
        Array<D3D11Sampler, kMaxSamplers>   mSamplers;
        Array<D3D11Texture, kMaxTextures>   mTextures;

    };

}
