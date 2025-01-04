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
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

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
        void CreateBuffer(Object ID, Usage Type, Bool Immutable, Ptr<const UInt8> Data, UInt32 Length) override;

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
        void LoadCapabilities();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Capabilities mCapabilities;
    };
}