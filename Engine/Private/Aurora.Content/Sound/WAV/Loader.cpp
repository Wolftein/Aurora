// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Loader.hpp"
#include "Codec.hpp"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool WAVLoader::Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data)
    {
        drwav Description;

        if (drwav_init_memory(& Description, Data.GetData(), Data.GetSize(), nullptr))
        {
            // Determine the runtime audio format from the WAV file's format tag.
            Audio::Format Format
                = (Description.fmt.formatTag == DR_WAVE_FORMAT_ADPCM      ? Audio::Format::ADPCM :
                   Description.fmt.formatTag == DR_WAVE_FORMAT_IEEE_FLOAT ? Audio::Format::IEEE  : Audio::Format::PCM);

            // Build the schema from the WAV file header.
            Audio::Schema Schema(
                Description.totalPCMFrameCount,
                Description.sampleRate,
                Format,
                Description.channels,
                Description.bitsPerSample >> 3);

            // Allocate a buffer for the decoded PCM frames and read them into memory.
            Blob Frames(Schema.Duration * Schema.Stride * Schema.Layout);
            drwav_read_pcm_frames(& Description, Schema.Duration, Frames.GetData<Byte>());

            // Load the decoded PCM data into the sound asset using the WAV codec adapter.
            const Tracker<Audio::Sound> Asset = Tracker<Audio::Sound>::Cast(Scope.GetResource());
            Asset->Load(Move(Frames), Move(Schema), NewUniquePtr<Audio::WAVCodec>());
            return true;
        }
        return false;
    }
}