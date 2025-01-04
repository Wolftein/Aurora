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

#include "Aurora.Audio/Decoder.hpp"
#include <dr_mp3.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class MP3Decoder final : public Decoder
    {
    private:

        // -=(Undocumented)=-
        constexpr static UInt BUFFER_SIZE = 192000 * 4;

    public:

        // -=(Undocumented)=-
        MP3Decoder(Any<Data> File);

        // -=(Undocumented)=-
        ~MP3Decoder() override;

        // -=(Undocumented)=-
        Bool Read(Ref<Frame> Output) override;

        // -=(Undocumented)=-
        Bool Seek(UInt Cursor) override;

        // -=(Undocumented)=-
        Kind GetKind() const
        {
            return Kind::IEEE;
        }

        // -=(Undocumented)=-
        UInt8 GetDepth() const
        {
            return 32;
        }

        // -=(Undocumented)=-
        UInt8 GetChannel() const
        {
            return mDescription.channels;
        }

        // -=(Undocumented)=-
        UInt32 GetFrequency() const
        {
            return mDescription.sampleRate;
        }

        // -=(Undocumented)=-
        UInt32 GetDuration() const
        {
            return mDuration;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Data   mFile;
        drmp3  mDescription;
        Real32 mBuffer[BUFFER_SIZE];
        UInt32 mDuration;
    };
}