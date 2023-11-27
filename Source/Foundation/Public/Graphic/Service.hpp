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
        Ref<const Capabilities> GetCapabilities() const
        {
            return mDriver->GetCapabilities();
        }

        // -=(Undocumented)=-
        template<typename T>
        UInt CreateBuffer(Usage Type, UInt Capacity, CPtr<T> Data = CPtr<T>())
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<Ptr<UInt08>>(Data.data()), Data.size_bytes());

            return CreateBuffer(Type, Capacity * sizeof(T), Bytes);
        }

        // -=(Undocumented)=-
        UInt CreateBuffer(Usage Type, UInt Capacity, CPtr<UInt08> Data = CPtr<UInt08>());

        // -=(Undocumented)=-
        template<typename T>
        void UpdateBuffer(Object ID, Bool Discard, UInt Offset, CPtr<T> Data)
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<Ptr<UInt08>>(Data.data()), Data.size_bytes());

            mDriver->UpdateBuffer(ID, Discard, Offset, Bytes);
        }

        // -=(Undocumented)=-
        void DeleteBuffer(Object ID);

        // -=(Undocumented)=-
        UInt CreateMaterial();

        // -=(Undocumented)=-
        void DeleteMaterial(Object ID);

        // -=(Undocumented)=-
        UInt CreatePass(Any Display, UInt Width, UInt Height);

        // -=(Undocumented)=-
        UInt CreatePass(CPtr<UInt> Colors, UInt Auxiliary);

        // -=(Undocumented)=-
        void DeletePass(Object ID);

        // -=(Undocumented)=-
        UInt CreatePipeline(CPtr<UInt08> Vertex, CPtr<UInt08> Fragment, CPtr<UInt08> Geometry, Ref<const Descriptor> Properties);

        // -=(Undocumented)=-
        void DeletePipeline(Object ID);

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
        void UpdateTexture(Object ID, UInt Level, Recti Offset, UInt Pitch, CPtr<T> Data)
        {
            const CPtr<UInt08> Bytes = CPtr<UInt08>(reinterpret_cast<Ptr<UInt08>>(Data.data()), Data.size_bytes());

            mDriver->UpdateTexture(ID, Level, Offset, Pitch, Bytes);
        }

        // -=(Undocumented)=-
        void CopyTexture(Object Destination, UInt DstLevel, Vector3f DstOffset, Object Source, UInt SrcLevel, Recti SrcOffset);

        // -=(Undocumented)=-
        UPtr<UInt08[]> ReadTexture(Object ID, UInt Level, Recti Offset);

        // -=(Undocumented)=-
        template<typename Type>
        Type QueryTexture(Object ID)
        {
            return std::any_cast<Type>(mDriver->QueryTexture(ID));
        }

        // -=(Undocumented)=-
        void DeleteTexture(Object ID);

        // -=(Undocumented)=-
        void Prepare(Object ID, Rectf Viewport, Clear Target, Color Tint, Real32 Depth, UInt08 Stencil);

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