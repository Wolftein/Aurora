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
        virtual void Reset(UInt32 Width, UInt32 Height) = 0;

        // -=(Undocumented)=-
        virtual Ref<const Capabilities> GetCapabilities() const = 0;

        // -=(Undocumented)=-
        virtual void CreateBuffer(Object ID, Usage Type, UInt32 Capacity, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void CopyBuffer(Object Destination, UInt32 DstOffset, Object Source, UInt32 SrcOffset, UInt32 Size) = 0;

        // -=(Undocumented)=
        virtual void UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void DeleteBuffer(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(Object ID, CPtr<Object> Colors, Object Auxiliary) = 0;

        // -=(Undocumented)=-
        virtual void DeletePass(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePipeline(Object ID, CPtr<const UInt8> Vertex, CPtr<const UInt8> Fragment, CPtr<const UInt8> Geometry, Ref<const Descriptor> Properties) = 0;

        // -=(Undocumented)=-
        virtual void DeletePipeline(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt32 Width, UInt32 Height, UInt8 Level, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateTexture(Object ID, UInt8 Level, Ref<const Recti> Offset, UInt32 Pitch, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void CopyTexture(Object Destination, UInt8 DstLevel, Ref<const Vector2i> DstOffset, Object Source, UInt8 SrcLevel, Ref<const Recti> SrcOffset) = 0;

        // -=(Undocumented)=-
        virtual Data ReadTexture(Object ID, UInt8 Level, Ref<const Recti> Offset) = 0;

        // -=(Undocumented)=-
        virtual UInt QueryTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void DeleteTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void Prepare(Object ID, Ref<const Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil) = 0;

        // -=(Undocumented)=-
        virtual void Submit(CPtr<Submission> Submissions) = 0;

        // -=(Undocumented)=-
        virtual void Commit(Object ID, Bool Synchronised) = 0;
    };
}