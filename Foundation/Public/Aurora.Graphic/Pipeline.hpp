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
#include "Aurora.Content/Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Pipeline final : public Content::AbstractResource<Pipeline>
    {
        friend class AbstractResource;

    public:

        // -=(Undocumented)=-
        explicit Pipeline(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Any<Array<Data, k_MaxStages>> Shaders, Any<Array<TextureSlot, k_MaxSlots>> Slots, Any<Descriptor> Properties);

        // -=(Undocumented)=-
        Object GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        TextureSlot GetSlot(UInt32 Slot) const
        {
            return mSlots[Slot];
        }

        // -=(Undocumented)=-
        ConstRef<Descriptor> GetProperties() const
        {
            return mProperties;
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object                         mID;
        Array<Data, k_MaxStages>       mShaders;    // TODO: Sparse
        Array<TextureSlot, k_MaxSlots> mSlots;      // TODO: Sparse
        Descriptor                     mProperties;
    };
}