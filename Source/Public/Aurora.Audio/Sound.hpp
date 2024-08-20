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

#include "Decoder.hpp"
#include "Aurora.Content/Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class Sound final : public Content::AbstractResource<Sound>
    {
        friend class Content::AbstractResource<Sound>;

    public:

        // -=(Undocumented)=-
        Sound(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Kind Kind, UInt32 Duration, UInt8 Depth, UInt8 Channel, UInt32 Frequency, UPtr<Decoder> Decoder);

        // -=(Undocumented)=-
        Bool Read(Ref<Decoder::Frame> Output)
        {
            return mDecoder->Read(Output);
        }

        // -=(Undocumented)=-
        Bool Seek(UInt Cursor)
        {
            return mDecoder->Seek(Cursor);
        }

        // -=(Undocumented)=-
        Kind GetKind() const
        {
            return mKind;
        }

        // -=(Undocumented)=-
        UInt32 GetDuration() const
        {
            return mDuration;
        }

        // -=(Undocumented)=-
        UInt8 GetDepth() const
        {
            return mDepth;
        }

        // -=(Undocumented)=-
        UInt8 GetChannel() const
        {
            return mChannel;
        }

        // -=(Undocumented)=-
        UInt32 GetFrequency() const
        {
            return mFrequency;
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Decoder> mDecoder;
        Kind          mKind;
        UInt32        mDuration;
        UInt8         mDepth;
        UInt8         mChannel;
        UInt32        mFrequency;
    };
}