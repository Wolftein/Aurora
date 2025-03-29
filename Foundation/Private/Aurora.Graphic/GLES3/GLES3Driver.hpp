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
#include "GLES3.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class GLES3Driver final : public Driver
    {
    public:

        // \see Driver::Initialize
        Bool Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples) override;

        // \see Driver::Reset
        void Reset(UInt16 Width, UInt16 Height, UInt8 Samples) override;

        // \see Driver::GetCapabilities
        ConstRef<Capabilities> GetCapabilities() const override;

        // \see Driver::CreateBuffer
        void CreateBuffer(Object ID, Usage Type, Bool Immutable, ConstPtr<UInt8> Data, UInt32 Length) override;

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
        void CreatePipeline(Object ID, CPtr<const UInt8> Vertex, CPtr<const UInt8> Pixel, CPtr<const UInt8> Geometry, ConstRef<Descriptor> Properties) override;

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
        struct GLES3Buffer
        {
            // -=(Undocumented)=-
            UInt32 ID;

            // -=(Undocumented)=-
            UInt32 Type;

            // -=(Undocumented)=-
            GLES3Buffer()
                : ID { 0 }
            {
            }

            // -=(Undocumented)=-
            ~GLES3Buffer()
            {
                if (ID)
                {
                    glDeleteBuffers(1, AddressOf(ID));
                }
                ID = 0;
            }
        };

        // -=(Undocumented)=-
        struct GLES3Pass
        {
            // TODO
        };

        // -=(Undocumented)=-
        struct GLES3Pipeline
        {
            // -=(Undocumented)=-
            UInt32                            Program;

            // -=(Undocumented)=-
            GLint                             Cull;

            // -=(Undocumented)=-
            GLint                             Fill;

            // -=(Undocumented)=-
            GLboolean                         BlendEnabled;

            // -=(Undocumented)=-
            GLint                             BlendMask;

            // -=(Undocumented)=-
            GLint                             BlendColorSrcFactor;

            // -=(Undocumented)=-
            GLint                             BlendColorDstFactor;

            // -=(Undocumented)=-
            GLint                             BlendColorEquation;

            // -=(Undocumented)=-
            GLint                             BlendAlphaSrcFactor;

            // -=(Undocumented)=-
            GLint                             BlendAlphaDstFactor;

            // -=(Undocumented)=-
            GLint                             BlendAlphaEquation;

            // -=(Undocumented)=-
            GLboolean                         DepthEnabled;

            // -=(Undocumented)=-
            GLint                             DepthMask;

            // -=(Undocumented)=-
            GLint                             DepthCondition;

            // -=(Undocumented)=-
            GLboolean                         StencilEnabled;

            // -=(Undocumented)=-
            GLint                             StencilMask;

            // -=(Undocumented)=-
            GLint                             StencilCondition;

            // -=(Undocumented)=-
            GLint                             StencilOnFail;

            // -=(Undocumented)=-
            GLint                             StencilOnZFail;

            // -=(Undocumented)=-
            GLint                             StencilOnZPass;

            // -=(Undocumented)=-
            GLint                             InputTopology;

            // -=(Undocumented)=-
            GLint                             InputStride;

            // -=(Undocumented)=-
            Array<Attribute, k_MaxAttributes> InputAttributes;

            // -=(Undocumented)=-
            GLES3Pipeline()
                : Program { 0 }
            {
            }

            // -=(Undocumented)=-
            ~GLES3Pipeline()
            {
                if (Program)
                {
                    glDeleteProgram(Program);
                }
                Program = 0;
            }
        };

        // -=(Undocumented)=-
        struct GLES3Sampler
        {
            // -=(Undocumented)=-
            UInt32 ID;

            // -=(Undocumented)=-
            GLES3Sampler()
                : ID { 0 }
            {
            }

            // -=(Undocumented)=-
            ~GLES3Sampler()
            {
                if (ID)
                {
                    glDeleteSamplers(1, AddressOf(ID));
                }
                ID = 0;
            }
        };

        // -=(Undocumented)=-
        struct GLES3Texture
        {
            // -=(Undocumented)=-
            UInt32 ID;

            // -=(Undocumented)=-
            UInt32 Format;

            // -=(Undocumented)=-
            UInt32 Type;

            // -=(Undocumented)=-
            GLES3Texture()
                : ID { 0 }
            {
            }

            // -=(Undocumented)=-
            ~GLES3Texture()
            {
                if (ID)
                {
                    glDeleteTextures(1, AddressOf(ID));
                }
                ID = 0;
            }
        };

    private:

        // -=(Undocumented)=-
        void LoadCapabilities();

        // -=(Undocumented)=-
        void LoadDefaults();

        // -=(Undocumented)=-
        Bool Compile(UInt32 Program, UInt32 Type, CPtr<const UInt8> Shader);

        // -=(Undocumented)=-
        Ref<GLES3Sampler> GetOrCreateSampler(ConstRef<Sampler> Descriptor);

        // -=(Undocumented)=-
        void ApplyVertex(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        // -=(Undocumented)=-
        void ApplyPipeline(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

        // -=(Undocumented)=-
        void ApplyResources(ConstRef<Submission> Oldest, ConstRef<Submission> Newest);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<SDL_Window> mDevice;
        SDL_GLContext   mContext;
        Capabilities    mCapabilities;
        Submission      mSubmission;
        Rectf           mViewport;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        GLES3Buffer     mBuffers[k_MaxBuffers];
        GLES3Pass       mPasses[k_MaxPasses];
        GLES3Pipeline   mPipelines[k_MaxPipelines];
        GLES3Sampler    mSamplers[k_MaxSamplers];
        GLES3Texture    mTextures[k_MaxTextures];
    };
}