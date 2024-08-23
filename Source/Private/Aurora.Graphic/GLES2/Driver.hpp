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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class GLES2Driver final : public Driver
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

        // \see Driver::CreatePass
        void CreatePass(Object ID, CPtr<Object> Colors, Object Auxiliary) override;

        // \see Driver::DeletePass
        void DeletePass(Object ID) override;

        // \see Driver::CreatePipeline
        void CreatePipeline(Object ID, CPtr<UInt8> Vertex, CPtr<UInt8> Pixel, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties) override;

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
        void LoadCapabilities();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Capabilities mCapabilities;
    };
}