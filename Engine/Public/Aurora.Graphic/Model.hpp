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

#include "Material.hpp"
#include "Mesh.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Represents a model asset within the content system.
    class Model final : public Content::AbstractResource<Model>
    {
        friend class AbstractResource;

    public:

        /// \brief Alias for a fixed-capacity list of material references.
        /// 
        /// Represents the collection of materials assigned to a model, typically one per mesh primitive.
        using Palette = Vector<Tracker<Material>, Mesh::kMaxPrimitives>;

    public:

        /// \brief Constructs a new model with the given resource key.
        /// 
        /// \param Key URI that uniquely identifies this model asset.
        Model(AnyRef<Content::Uri> Key);

        /// \brief Initializes the model with mesh geometry and associated materials.
        /// 
        /// \param Mesh    Reference to the mesh resource defining geometry and structure.
        /// \param Palette Reference to the material list, where each entry corresponds to a mesh primitive.
        void Load(ConstTracker<Mesh> Mesh, AnyRef<Palette> Palette);

        /// \brief Returns the mesh associated with the model.
        /// 
        /// \return Reference to the mesh resource.
        AURORA_INLINE ConstTracker<Mesh> GetMesh() const
        {
            return mMesh;
        }

        /// \brief Returns the material assigned to a given slot.
        /// 
        /// \param Slot Index of the mesh primitive.
        /// \return Reference to the material resource for the given slot.
        AURORA_INLINE ConstTracker<Material> GetMaterial(UInt32 Slot) const
        {
            return mPalette[Slot];
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Tracker<Mesh> mMesh;
        Palette       mPalette;
    };
}
