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

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Driver
    {
    public:

        // -=(Undocumented)=-
        virtual ~Driver() = default;

        // -=(Undocumented)=-
        virtual Bool Initialise(Any Display, UInt Width, UInt Height) = 0;

        // -=(Undocumented)=-
        virtual void Reset(UInt Width, UInt Height) = 0;

        // -=(Undocumented)=-
        virtual void CreateBuffer(UInt ID, Bool Geometry, UInt Capacity, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateBuffer(UInt ID, Bool Discard, UInt Offset, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void DeleteBuffer(UInt ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(UInt ID, CPtr<UInt> Colors, UInt Auxiliary) = 0;

        // -=(Undocumented)=-
        virtual void DeletePass(UInt ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePipeline(UInt ID, CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties) = 0;

        // -=(Undocumented)=-
        virtual void DeletePipeline(UInt ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateSampler(UInt ID, TextureEdge EdgeX, TextureEdge EdgeY, TextureFilter Filter) = 0;

        // -=(Undocumented)=-
        virtual void DeleteSampler(UInt ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateTexture(UInt ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateTexture(UInt ID, UInt Level, Rect Offset, UInt Pitch, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void DeleteTexture(UInt ID) = 0;

        // -=(Undocumented)=-
        virtual void Prepare(UInt ID, Rect Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil) = 0;

        // -=(Undocumented)=-
        virtual void Submit(CPtr<Submission> Submissions) = 0;

        // -=(Undocumented)=-
        virtual void Commit(Bool Synchronised) = 0;
    };
}