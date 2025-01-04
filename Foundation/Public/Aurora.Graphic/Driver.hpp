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
        virtual Bool Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples) = 0;

        // -=(Undocumented)=-
        virtual void Reset(UInt16 Width, UInt16 Height, UInt8 Samples) = 0;

        // -=(Undocumented)=-
        virtual ConstRef<Capabilities> GetCapabilities() const = 0;

        // -=(Undocumented)=-
        virtual void CreateBuffer(Object ID, Usage Type, Bool Immutable, Ptr<const UInt8> Data, UInt32 Length) = 0;

        // -=(Undocumented)=-
        virtual void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size) = 0;

        // -=(Undocumented)=
        virtual void UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void DeleteBuffer(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePass(Object ID, CPtr<const Attachment> Colors, CPtr<const Attachment> Resolves, ConstRef<Attachment> Auxiliary) = 0;

        // -=(Undocumented)=-
        virtual void DeletePass(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreatePipeline(Object ID, CPtr<const UInt8> Vertex, CPtr<const UInt8> Fragment, CPtr<const UInt8> Geometry, ConstRef<Descriptor> Properties) = 0;

        // -=(Undocumented)=-
        virtual void DeletePipeline(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void CreateTexture(Object ID, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Level, UInt8 Samples, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void UpdateTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset, UInt32 Pitch, CPtr<const UInt8> Data) = 0;

        // -=(Undocumented)=-
        virtual void CopyTexture(Object DstTexture, UInt8 DstLevel, ConstRef<Vector2i> DstOffset, Object SrcTexture, UInt8 SrcLevel, ConstRef<Recti> SrcOffset) = 0;

        // -=(Undocumented)=-
        virtual Data ReadTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset) = 0;

        // -=(Undocumented)=-
        virtual void DeleteTexture(Object ID) = 0;

        // -=(Undocumented)=-
        virtual void Prepare(Object ID, ConstRef<Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil) = 0;

        // -=(Undocumented)=-
        virtual void Submit(CPtr<const Submission> Submissions) = 0;

        // -=(Undocumented)=-
        virtual void Commit(Object ID, Bool Synchronised) = 0;
    };
}