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

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    // @TODO: Needs improvement over the interface for ease use.
    class Service final : public AbstractSubsystem<Service>
    {
    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // -=(Undocumented)=-
        Bool Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        void Reset(UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        Ref<const Capabilities> GetCapabilities() const
        {
            return mDriver->GetCapabilities();
        }

        // -=(Undocumented)=-
        Object CreateBuffer(Usage Type, UInt32 Capacity)
        {
            return CreateBuffer(Type, Capacity, Data());
        }

        // -=(Undocumented)=-
        Object CreateBuffer(Usage Type, UInt32 Capacity, Any<Data> Data);

        // -=(Undocumented)=-
        void CopyBuffer(Object Destination, UInt32 DstOffset, Object Source, UInt32 SrcOffset, UInt32 Size);

        // -=(Undocumented)=-
        void UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, Any<Data> Data);

        // -=(Undocumented)=-
        void DeleteBuffer(Object ID);

        // -=(Undocumented)=-
        Object CreateMaterial();

        // -=(Undocumented)=-
        void DeleteMaterial(Object ID);

        // -=(Undocumented)=-
        Object CreatePass(CPtr<Attachment> Colors, CPtr<Attachment> Resolves, Attachment Auxiliary);

        // -=(Undocumented)=-
        void DeletePass(Object ID);

        // -=(Undocumented)=-
        Object CreatePipeline(Any<Data> Vertex, Any<Data> Fragment, Any<Data> Geometry, Ref<const Descriptor> Properties);

        // -=(Undocumented)=-
        void DeletePipeline(Object ID);

        // -=(Undocumented)=-
        Object CreateTexture(TextureFormat Format, TextureLayout Layout, UInt32 Width ,UInt32 Height, UInt8 Level, UInt8 Samples, Any<Data> Data);

        // -=(Undocumented)=-
        void UpdateTexture(Object ID, UInt8 Level, Ref<const Recti> Offset, UInt32 Pitch, Any<Data> Data);

        // -=(Undocumented)=-
        void CopyTexture(Object Destination, UInt8 DstLevel, Ref<const Vector2i> DstOffset, Object Source, UInt8 SrcLevel, Ref<const Recti> SrcOffset);

        // -=(Undocumented)=-
        Data ReadTexture(Object ID, UInt8 Level, Ref<const Recti> Offset);

        // -=(Undocumented)=-
        UInt QueryTexture(Object ID);

        // -=(Undocumented)=-
        void DeleteTexture(Object ID);

        // -=(Undocumented)=-
        void Prepare(Object ID, Ref<const Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil);

        // -=(Undocumented)=-
        void Submit(CPtr<Submission> Submissions);

        // -=(Undocumented)=-
        void Commit(Object ID, Bool Synchronised);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Driver>           mDriver;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<k_MaxBuffers>   mBuffers;
        Handle<k_MaxMaterials> mMaterials;
        Handle<k_MaxPasses>    mPasses;
        Handle<k_MaxPipelines> mPipelines;
        Handle<k_MaxTextures>  mTextures;
    };
}