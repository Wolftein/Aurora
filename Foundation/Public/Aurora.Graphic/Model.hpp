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

#include "Material.hpp"
#include "Mesh.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Model final : public Content::AbstractResource<Model>
    {
        friend class AbstractResource;

    public:

        // -=(Undocumented)=-
        Model(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(ConstSPtr<Mesh> Mesh, Any<Array<SPtr<Material>, Mesh::k_MaxPrimitives>> Materials);

        // -=(Undocumented)=-
        ConstSPtr<Mesh> GetMesh() const
        {
            return mMesh;
        }

        // -=(Undocumented)=-
        ConstSPtr<Material> GetMaterial(UInt32 Slot) const
        {
            return mMaterials[Slot];
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Mesh>                                   mMesh;         // TODO: Allow to share resource
        Array<SPtr<Material>, Mesh::k_MaxPrimitives> mMaterials;    // TODO: Allow to share resource
    };
}
