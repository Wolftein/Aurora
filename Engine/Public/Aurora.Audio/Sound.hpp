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

#include "Codec.hpp"
#include "Aurora.Content/Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    /// \brief Represents an audio asset within the content system.
    class Sound final : public Content::AbstractResource<Sound>
    {
        friend class AbstractResource;

    public:

        /// \brief Constructs the sound resource with a unique key.
        ///
        /// \param Key Unique URI identifying this sound.
        explicit Sound(AnyRef<Content::Uri> Key);

        /// \brief Loads the sound resource from raw audio data.
        ///
        /// \param Data   Audio stream buffer (compressed or uncompressed).
        /// \param Schema Audio stream schema.
        /// \param Codec  Codec instance capable of decoding this audio stream.
        void Load(AnyRef<Blob> Data, AnyRef<Schema> Schema, Unique<Codec> Codec);

        /// \brief Gets the total duration of the sound.
        ///
        /// \return Total length in sample frames.
        AURORA_INLINE UInt64 GetDuration() const
        {
            return mSchema.Duration;
        }

        /// \brief Gets the playback frequency of the sound.
        ///
        /// \return Frequency in Hz.
        AURORA_INLINE UInt32 GetFrequency() const
        {
            return mSchema.Frequency;
        }

        /// \brief Gets the runtime audio sample format.
        ///
        /// \return Audio sample format used at runtime.
        AURORA_INLINE Format GetFormat() const
        {
            return mSchema.Format;
        }

        /// \brief Gets the channel layout of the sound.
        ///
        /// \return Audio channel layout.
        AURORA_INLINE UInt8 GetLayout() const
        {
            return mSchema.Layout;
        }

        /// \brief Gets the stride per sample for a single channel.
        ///
        /// \return Bytes per sample.
        AURORA_INLINE UInt8 GetStride() const
        {
            return mSchema.Stride;
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    public:

        /// \brief Creates a decoder for the specified sound resource.
        ///
        /// \param Sound Sound resource to decode.
        /// \return Unique pointer to a decoder for this sound.
        static Unique<Decoder> Decode(ConstRef<Sound> Sound)
        {
            return Sound.mCodec->Decode(Sound.mData, Sound.mSchema);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Schema        mSchema;
        Blob          mData;
        Unique<Codec> mCodec;
    };
}
