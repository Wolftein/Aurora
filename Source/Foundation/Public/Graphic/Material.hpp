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

#include "Texture.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Material final : public Content::AbstractResource<Hash("Material")>
    {
    public:

        // -=(Undocumented)=-
        Material(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        Object GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt Slot, Ref<const SPtr<Texture>> Texture)
        {
            mTextures[Slot] = Texture;
        }

        // -=(Undocumented)=-
        Ref<const SPtr<Texture>> GetTexture(UInt Slot) const
        {
            return mTextures[Slot];
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt Offset, Type Parameter)
        {
            constexpr UInt Size = sizeof(Type) / sizeof(Vector4f) + 1;

            if (Offset + Size > mParameters.size())
            {
                mParameters.resize(mParameters.size() + Align<16>(Size));
            }

            * (reinterpret_cast<Ptr<Type>>(reinterpret_cast<UInt>(mParameters.data()) + Offset)) = Parameter;
        }

        // -=(Undocumented)=-
        CPtr<const Vector4f> GetParameters() const
        {
            return mParameters;
        }

    protected:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDispose(Ref<Subsystem::Context>)
        void OnDispose(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object                             mID;
        Array<SPtr<Texture>, k_MaxSources> mTextures;
        Vector<Vector4f>                   mParameters; // TODO: Replace with pre allocated chunked memory
    };
}