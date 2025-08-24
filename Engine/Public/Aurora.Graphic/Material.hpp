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
    /// \brief Represents a material asset within the content system.
    class Material final : public Content::AbstractResource<Material>
    {
        friend class AbstractResource;

    public:

        /// \brief Enumerates the available rendering modes for a material.
        enum class Kind : UInt8
        {
            Opaque,         ///< No blending.
            Alpha,          ///< Standard blending using alpha.
            Additive,       ///< Additive color blend.
            Multiply,       ///< Multiplicative color blend.
            Subtract,       ///< Subtractive color blend.
        };

    public:

        /// \brief Constructs a material resource with the given URI key.
        /// 
        /// \param Key URI path identifying this material resource.
        explicit Material(AnyRef<Content::Uri> Key);

        /// \brief Retrieves the material’s unique object ID.
        /// 
        /// \return Unique object identifier.
        AURORA_INLINE Object GetID() const
        {
            return mID;
        }

        /// \brief Sets the material’s rendering kind.
        /// 
        /// \param Kind Blending mode to use for rendering.
        AURORA_INLINE void SetKind(Kind Kind)
        {
            mKind = Kind;
        }

        /// \brief Gets the current rendering kind of the material.
        /// 
        /// \return The material’s blending mode.
        AURORA_INLINE Kind GetKind() const
        {
            return mKind;
        }

        /// \brief Assigns a texture to the material for a given semantic slot.
        /// 
        /// \param Semantic Type of texture usage.
        /// \param Texture  Handle to the texture to assign.
        AURORA_INLINE void SetTexture(TextureSemantic Semantic, ConstTracker<Texture> Texture)
        {
            mTextures[Enum::Cast(Semantic)] = Texture;
        }

        /// \brief Retrieves a texture bound to the given semantic slot.
        /// 
        /// \param Semantic Type of texture usage.
        /// \return Handle to the assigned texture.
        AURORA_INLINE ConstTracker<Texture> GetTexture(TextureSemantic Semantic) const
        {
            return mTextures[Enum::Cast(Semantic)];
        }

        /// \brief Assigns a sampler to the material for the given semantic slot.
        /// 
        /// \param Semantic Type of texture usage.
        /// \param Sampler  Sampler state object.
        AURORA_INLINE void SetSampler(TextureSemantic Semantic, ConstRef<Sampler> Sampler)
        {
            mSamplers[Enum::Cast(Semantic)] = Sampler;
        }

        /// \brief Retrieves the sampler associated with the given semantic.
        /// 
        /// \param Semantic Type of texture usage.
        /// \return Reference to the sampler state.
        AURORA_INLINE ConstRef<Sampler> GetSampler(TextureSemantic Semantic) const
        {
            return mSamplers[Enum::Cast(Semantic)];
        }

        /// \brief Sets a shader parameter value at the given offset.
        /// 
        /// \param Offset    Byte offset into the parameter buffer.
        /// \param Parameter Value to write.
        template<typename Type>
        AURORA_INLINE void SetParameter(UInt32 Offset, Type Parameter)
        {
            if (constexpr UInt Size = sizeof(Type); Offset + Size >= mParameters.size())
            {
                mParameters.resize(Offset + Size);
            }

            * reinterpret_cast<Ptr<Type>>(mParameters.data() + Offset) = Parameter;
        }

        /// \brief Returns the raw parameter buffer contents.
        /// 
        /// \return Span over the material parameter byte array.
        AURORA_INLINE ConstSpan<Byte> GetParameters() const
        {
            return mParameters;
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object                             mID;
        Kind                               mKind;
        Array<Tracker<Texture>, kMaxSlots> mTextures;
        Array<Sampler, kMaxSlots>          mSamplers;
        Vector<Byte>                       mParameters; // TODO Proper support for alignment
    };
}