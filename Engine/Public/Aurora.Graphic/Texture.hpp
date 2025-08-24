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
    /// \brief Represents a texture asset within the content system.
    class Texture final : public Content::AbstractResource<Texture>
    {
        friend class AbstractResource;

    public:

        /// \brief Constructs a texture resource with a given content key.
        ///
        /// \param Key Unique URI identifying this texture asset.
        explicit Texture(AnyRef<Content::Uri> Key);

        /// \brief Loads texture data into the resource.
        ///
        /// \param Access  Specifies the CPU/GPU access mode.
        /// \param Format  Pixel format of the texture data.
        /// \param Layout  Memory layout of the texture.
        /// \param Width   Width in pixels.
        /// \param Height  Height in pixels.
        /// \param Level   Mipmap level to load.
        /// \param Samples Samples per pixel (multisampling).
        /// \param Data    Raw texture data.
        void Load(Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Level, Samples Samples, AnyRef<Blob> Data);

        /// \brief Loads a standard source texture (non-render target).
        ///
        /// \param Format Pixel format.
        /// \param Width  Width in pixels.
        /// \param Height Height in pixels.
        /// \param Level  Mipmap level.
        /// \param Data   Raw texture data.
        AURORA_INLINE void Load(TextureFormat Format, UInt16 Width, UInt16 Height, UInt8 Level, AnyRef<Blob> Data)
        {
            Load(Access::Device, Format, TextureLayout::Source, Width, Height, Level, Samples::X1, Move(Data));
        }

        /// \brief Returns the GPU object ID associated with this texture.
        ///
        /// \return The driver-level texture handle.
        AURORA_INLINE Object GetID() const
        {
            return mID;
        }

        /// \brief Returns the access mode of the texture.
        ///
        /// \return The access mode.
        AURORA_INLINE Access GetAccess() const
        {
            return mAccess;
        }

        /// \brief Returns the pixel format of the texture.
        ///
        /// \return The format used for storage and sampling.
        AURORA_INLINE TextureFormat GetFormat() const
        {
            return mFormat;
        }

        /// \brief Returns the layout of the texture in memory.
        ///
        /// \return The layout type.
        AURORA_INLINE TextureLayout GetLayout() const
        {
            return mLayout;
        }

        /// \brief Returns the width of the texture in pixels.
        ///
        /// \return The width in pixels.
        AURORA_INLINE UInt16 GetWidth() const
        {
            return mWidth;
        }

        /// \brief Returns the height of the texture in pixels.
        ///
        /// \return The height in pixels.
        AURORA_INLINE UInt16 GetHeight() const
        {
            return mHeight;
        }

        /// \brief Returns the number of mipmap levels.
        ///
        /// \return The number of mipmap levels in the texture.
        AURORA_INLINE UInt8 GetLevel() const
        {
            return mLevel;
        }

        /// \brief Returns the number of samples used for multisampling.
        ///
        /// \return The number of samples.
        AURORA_INLINE Samples GetSamples() const
        {
            return mSamples;
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Object         mID;
        Access         mAccess;
        TextureFormat  mFormat;
        TextureLayout  mLayout;
        UInt16         mWidth;
        UInt16         mHeight;
        UInt8          mLevel;
        Samples        mSamples;
        Blob           mData;
    };
}
