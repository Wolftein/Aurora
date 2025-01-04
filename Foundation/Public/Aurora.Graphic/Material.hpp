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
        explicit Material(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        Object GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        void SetOwnership(Bool Ownership)
        {
            mOwnership = Ownership;
        }

        // -=(Undocumented)=-
        Bool HasOwnership() const
        {
            return mOwnership;
        }

        // -=(Undocumented)=-
        void SetTexture(TextureSlot Slot, ConstSPtr<Texture> Texture)
        {
            mTextures[CastEnum(Slot)] = Texture;
        }

        // -=(Undocumented)=-
        ConstSPtr<Texture> GetTexture(TextureSlot Slot) const
        {
            return mTextures[CastEnum(Slot)];
        }

        // -=(Undocumented)=-
        void SetSampler(TextureSlot Slot, ConstRef<Sampler> Sampler)
        {
            mSamplers[CastEnum(Slot)] = Sampler;
        }

        // -=(Undocumented)=-
        ConstRef<Sampler> GetSampler(TextureSlot Slot) const
        {
            return mSamplers[CastEnum(Slot)];
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt32 Offset, Type Parameter)
        {
            if (constexpr UInt Size = sizeof(Type); Offset + Size >= mParameters.size())
            {
                mParameters.resize(Offset + Size);
            }

            * reinterpret_cast<Ptr<Type>>(mParameters.data() + Offset) = Parameter;
        }

        // -=(Undocumented)=-
        CPtr<const UInt8> GetParameters() const
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
        Bool                               mOwnership;  // TODO: Allow to share resource without flag
        Array<SPtr<Texture>, k_MaxSources> mTextures;   // TODO: Sparse
        Array<Sampler, k_MaxSources>       mSamplers;   // TODO: Sparse
        Vector<UInt8>                      mParameters; // TODO: Replace with pre allocated chunked memory
    };
}