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
        virtual Ref<const Capabilities> GetCapabilities() const = 0;

        // -=(Undocumented)=-
        virtual void CreateBuffer(Object ID, Bool Geometry, UInt Capacity, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual Ptr<void> Map(Object ID, Ref<UInt> Offset, UInt Length) = 0;

        // -=(Undocumented)=-
        virtual Ptr<void> Map(Object ID, Bool Discard, UInt Offset, UInt Length) = 0;

        // -=(Undocumented)=-
        virtual void Unmap(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void DeleteBuffer(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(Object ID, Any Display, UInt Width, UInt Height) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(Object ID, CPtr<UInt> Colors, UInt Auxiliary) = 0;

        // -=(Undocumented)=-
        virtual void DeletePass(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePipeline(Object ID, CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties) = 0;

        // -=(Undocumented)=-
        virtual void DeletePipeline(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt08> Data) = 0;

        // -=(Undocumented)=-
        virtual void CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset) = 0;

        // -=(Undocumented)=-
        virtual UPtr<UInt08[]> ReadTexture(Object ID, UInt Level, Recti Offset) = 0;

        // -=(Undocumented)=-
        virtual void DeleteTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil) = 0;

        // -=(Undocumented)=-
        virtual void Submit(CPtr<Submission> Submissions) = 0;

        // -=(Undocumented)=-
        virtual void Commit(Object ID, Bool Synchronised) = 0;
    };
}