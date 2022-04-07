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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class NoneDriver final : public Driver
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
        void CreatePipeline(UInt ID, CPtr<UInt08> Vertex, CPtr<UInt08> Pixel, Ref<const Descriptor> Properties) override;

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
    };
}