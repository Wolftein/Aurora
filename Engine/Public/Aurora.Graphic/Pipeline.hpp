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
    /// \brief Represents a pipeline asset within the content system.
    class Pipeline final : public Content::AbstractResource<Pipeline>
    {
        friend class AbstractResource;

    public:

        /// \brief Represents a mapping between a semantic and a register index.
        template<typename Format>
        struct Mapping final
        {
            /// \brief Default constructor for a mapping.
            AURORA_INLINE constexpr Mapping() = default;

            /// \brief Constructs a mapping with specified semantic and register.
            ///
            /// \param Semantic Semantic meaning of the binding.
            /// \param Register Register index where this binding is located.
            AURORA_INLINE constexpr Mapping(Format Semantic, UInt32 Register)
                : Semantic { Semantic },
                  Register { Register }
            {
            }

            /// Semantic meaning of the binding.
            Format Semantic = Format::None;

            /// Register index where this binding is located.
            UInt32 Register = 0;
        };

        /// \brief Fixed-capacity list of mappings used for semantic-to-register bindings.
        template<typename Format, UInt Limit>
        using MappingList = Vector<Mapping<Format>, Limit>;

    public:

        /// \brief Constructs the pipeline with a given resource identifier.
        ///
        /// \param Key The unique URI identifying this pipeline asset.
        explicit Pipeline(AnyRef<Content::Uri> Key);

        /// \brief Loads the pipeline configuration.
        ///
        /// \param Stages     Compiled bytecode for each shader stage (vertex, fragment, etc.).
        /// \param Textures   Texture semantic-to-slot bindings.
        /// \param Properties Pipeline-specific render state and metadata.
        void Load(AnyRef<Array<Blob, kMaxStages>> Stages, AnyRef<MappingList<TextureSemantic, kMaxSlots>> Textures, AnyRef<Descriptor> Properties);

        /// \brief Returns the GPU-specific pipeline identifier.
        ///
        /// \return Driver-level pipeline handle.
        AURORA_INLINE Object GetID() const
        {
            return mID;
        }

        /// \brief Returns the list of texture slot bindings.
        ///
        /// \return Span over the texture semantic-to-slot mappings.
        AURORA_INLINE ConstSpan<Mapping<TextureSemantic>> GetTextures() const
        {
            return mTextures;
        }

        /// \brief Returns the pipeline descriptor properties.
        ///
        /// \return Descriptor associated with this pipeline.
        AURORA_INLINE ConstRef<Descriptor> GetProperties() const
        {
            return mProperties;
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object                                  mID;
        Array<Blob, kMaxStages>                 mStages;
        MappingList<TextureSemantic, kMaxSlots> mTextures;
        Descriptor                              mProperties;
    };
}
