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

#include "Texture.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Material final : public Content::AbstractResource<Material>
    {
        friend class AbstractResource;

    public:

        // -=(Undocumented)=-
        static constexpr UInt k_Alignment = 16;

    public:

        // -=(Undocumented)=-
        explicit Material(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        Object GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        void SetResidence(Bool Residence)
        {
            mResidence = Residence;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt Slot, ConstSPtr<Texture> Texture)
        {
            mTextures[Slot] = Texture;
        }

        // -=(Undocumented)=-
        ConstSPtr<Texture> GetTexture(UInt Slot) const
        {
            return mTextures[Slot];
        }

        // -=(Undocumented)=-
        void SetSampler(UInt Slot, Ref<const Sampler> Sampler)
        {
            mSamplers[Slot] = Sampler;
        }

        // -=(Undocumented)=-
        Ref<const Sampler> GetSampler(UInt Slot) const
        {
            return mSamplers[Slot];
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt Offset, Type Parameter)
        {
            if (constexpr UInt Size = sizeof(Type) / sizeof(Vector4f) + 1; Offset + Size > mParameters.size())
            {
                mParameters.resize(mParameters.size() + Align(Size, k_Alignment));
            }

            * reinterpret_cast<Ptr<Type>>(reinterpret_cast<UInt>(mParameters.data() + Offset)) = Parameter;
        }

        // -=(Undocumented)=-
        CPtr<const Vector4f> GetParameters() const
        {
            return mParameters;
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object                             mID;
        Bool                               mResidence;
        Array<SPtr<Texture>, k_MaxSources> mTextures;
        Array<Sampler, k_MaxSources>       mSamplers;
        Vector<Vector4f>                   mParameters; // TODO: Replace with pre allocated chunked memory
    };
}