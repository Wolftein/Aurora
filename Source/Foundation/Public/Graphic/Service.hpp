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

        // \see Subsystem::OnTick
        void OnTick() override;

        // -=(Undocumented)=-
        Bool Initialise(Backend Backend, Any Display, UInt Width, UInt Height);

        // -=(Undocumented)=-
        void Reset(UInt Width, UInt Height);

        // -=(Undocumented)=-
        template<typename T>
        UInt CreateBuffer(Bool Geometry, UInt Capacity, CPtr<T> Data = CPtr<T>())
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<UInt08 *>(Data.data()), Data.size_bytes());

            return CreateBuffer(Geometry, Capacity * sizeof(T), Bytes);
        }

        // -=(Undocumented)=-
        UInt CreateBuffer(Bool Geometry, UInt Capacity, CPtr<UInt08> Data = CPtr<UInt08>());
  
        // -=(Undocumented)=-
        Ptr<void> Map(UInt ID, Bool Discard, UInt Offset, UInt Length);

        // -=(Undocumented)=-
        void Unmap(UInt ID);

        // -=(Undocumented)=-
        void DeleteBuffer(UInt ID);

        // -=(Undocumented)=-
        UInt CreateMaterial();

        // -=(Undocumented)=-
        void DeleteMaterial(UInt ID);

        // -=(Undocumented)=-
        UInt CreatePass(CPtr<UInt> Colors, UInt Auxiliary);

        // -=(Undocumented)=-
        void DeletePass(UInt ID);

        // -=(Undocumented)=-
        UInt CreatePipeline(CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, Ref<const Descriptor> Properties);

        // -=(Undocumented)=-
        void DeletePipeline(UInt ID);

        // -=(Undocumented)=-
        UInt CreateSampler(TextureEdge EdgeU, TextureEdge EdgeV, TextureFilter Filter);

        // -=(Undocumented)=-
        void DeleteSampler(UInt ID);

        // -=(Undocumented)=-
        template<typename T>
        UInt CreateTexture(TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<T> Data)
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<Ptr<UInt08>>(Data.data()), Data.size_bytes());

            return CreateTexture(Format, Layout, Width, Height, Layer, Bytes);
        }

        // -=(Undocumented)=-
        UInt CreateTexture(TextureFormat Format, TextureLayout Layout, UInt Width, UInt Height, UInt Layer, CPtr<UInt08> Data = CPtr<UInt08>());

        // -=(Undocumented)=-
        template<typename T>
        void UpdateTexture(UInt ID, UInt Level, Recti Offset, UInt Pitch, CPtr<T> Data)
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<Ptr<UInt08>>(Data.data()), Data.size_bytes());

            UpdateTexture(ID, Level, Offset, Pitch, Bytes);
        }

        // -=(Undocumented)=-
        void UpdateTexture(UInt ID, UInt Level, Recti Offset, UInt Pitch, CPtr<UInt08> Data);

        // -=(Undocumented)=-
        void DeleteTexture(UInt ID);

        // -=(Undocumented)=-
        void Prepare(UInt ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil);

        // -=(Undocumented)=-
        void Submit(CPtr<Submission> Submissions);

        // -=(Undocumented)=-
        void Commit(Bool Synchronised);

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
        Handle<k_MaxSamplers>  mSamplers;
        Handle<k_MaxTextures>  mTextures;
    };
}