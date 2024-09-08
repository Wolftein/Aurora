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
    class Service final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Service(Ref<Context> Context);

        // \see Subsystem::OnTick(Real64)
        void OnTick(Real64 Time) override;

        // -=(Undocumented)=-
        Bool Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt32 Width, UInt32 Height);

        // -=(Undocumented)=-
        void Reset(UInt16 Width, UInt16 Height);

        // -=(Undocumented)=-
        Ref<const Capabilities> GetCapabilities() const
        {
            return mDriver->GetCapabilities();
        }

        // -=(Undocumented)=-
        template<typename T>
        Object CreateBuffer(Usage Type, UInt Capacity, CPtr<T> Data = CPtr<T>())
        {
            const CPtr<UInt8> Bytes = CPtr<UInt8>(reinterpret_cast<Ptr<UInt8>>(Data.data()), Data.size_bytes());

            return CreateBuffer(Type, Capacity * sizeof(T), Bytes);
        }

        // -=(Undocumented)=-
        Object CreateBuffer(Usage Type, UInt Capacity, CPtr<UInt8> Data = CPtr<UInt8>());

        // -=(Undocumented)=-
        template<typename T>
        void UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<T> Data)
        {
            const CPtr<UInt8> Bytes = CPtr<UInt8>(reinterpret_cast<Ptr<UInt8>>(Data.data()), Data.size_bytes());

            mDriver->UpdateBuffer(ID, Discard, Offset, Bytes);
        }

        // -=(Undocumented)=-
        void DeleteBuffer(Object ID);

        // -=(Undocumented)=-
        Object CreateMaterial();

        // -=(Undocumented)=-
        void DeleteMaterial(Object ID);

        // -=(Undocumented)=-
        Object CreatePass(CPtr<Object> Colors, Object Auxiliary);

        // -=(Undocumented)=-
        void DeletePass(Object ID);

        // -=(Undocumented)=-
        Object CreatePipeline(CPtr<UInt8> Vertex, CPtr<UInt8> Fragment, CPtr<UInt8> Geometry, Ref<const Descriptor> Properties);

        // -=(Undocumented)=-
        void DeletePipeline(Object ID);

        // -=(Undocumented)=-
        template<typename T>
        Object CreateTexture(TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Layer, CPtr<T> Data)
        {
            const CPtr<UInt8> Bytes = CPtr<UInt8>(reinterpret_cast<Ptr<UInt8>>(Data.data()), Data.size_bytes());

            return CreateTexture(Format, Layout, Width, Height, Layer, Bytes);
        }

        // -=(Undocumented)=-
        Object CreateTexture(TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Layer, CPtr<UInt8> Data = CPtr<UInt8>());

        // -=(Undocumented)=-
        template<typename T>
        void UpdateTexture(Object ID, UInt8 Level, Recti Offset, UInt Pitch, CPtr<T> Data)
        {
            const CPtr<UInt8> Bytes = CPtr<UInt8>(reinterpret_cast<Ptr<UInt8>>(Data.data()), Data.size_bytes());

            mDriver->UpdateTexture(ID, Level, Offset, Pitch, Bytes);
        }

        // -=(Undocumented)=-
        void CopyTexture(Object Destination, UInt8 DstLevel, Vector3f DstOffset, Object Source, UInt8 SrcLevel, Recti SrcOffset);

        // -=(Undocumented)=-
        Data ReadTexture(Object ID, UInt8 Level, Recti Offset);

        // -=(Undocumented)=-
        UInt QueryTexture(Object ID);

        // -=(Undocumented)=-
        void DeleteTexture(Object ID);

        // -=(Undocumented)=-
        void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil);

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