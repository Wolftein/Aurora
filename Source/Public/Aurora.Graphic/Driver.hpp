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
        virtual Bool Initialize(Ptr<SDL_Window> Swapchain, UInt32 Width, UInt32 Height) = 0;

        // -=(Undocumented)=-
        virtual void Reset(UInt Width, UInt Height) = 0;

        // -=(Undocumented)=-
        virtual Ref<const Capabilities> GetCapabilities() const = 0;

        // -=(Undocumented)=-
        virtual void CreateBuffer(Object ID, Usage Type, UInt Capacity, CPtr<UInt8> Data) = 0;

        // -=(Undocumented)=
        virtual void UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void DeleteBuffer(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(Object ID, CPtr<Object> Colors, Object Auxiliary) = 0;

        // -=(Undocumented)=-
        virtual void DeletePass(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePipeline(Object ID, CPtr<UInt8> Vertex, CPtr<UInt8> Fragment, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties) = 0;

        // -=(Undocumented)=-
        virtual void DeletePipeline(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset) = 0;

        // -=(Undocumented)=-
        virtual Data ReadTexture(Object ID, UInt Level, Recti Offset) = 0;

        // -=(Undocumented)=-
        virtual UInt QueryTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void DeleteTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil) = 0;

        // -=(Undocumented)=-
        virtual void Submit(CPtr<Submission> Submissions) = 0;

        // -=(Undocumented)=-
        virtual void Commit(Object ID, Bool Synchronised) = 0;
    };
}