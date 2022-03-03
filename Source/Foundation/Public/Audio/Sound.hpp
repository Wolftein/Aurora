// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
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
#include "Content/Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class Sound final : public Content::AbstractResource<Hash("Sound")>
    {
    public:

        // -=(Undocumented)=-
        Sound(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(UInt Size, UInt Depth, UInt Channel, UInt Frequency, UPtr<Decoder> Decoder);

        // -=(Undocumented)=-
        void Seek(UInt Frame);

        // -=(Undocumented)=-
        Bool Read(Ref<CPtr<UInt08>> Output);

        // -=(Undocumented)=-
        UInt GetDepth() const
        {
            return mDepth;
        }

        // -=(Undocumented)=-
        UInt GetChannel() const
        {
            return mChannel;
        }

        // -=(Undocumented)=-
        UInt GetFrequency() const
        {
            return mFrequency;
        }

    public:

        // \see Resource::OnLoad(Ref<Subsystem::Context>)
        Bool OnLoad(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnUnload(Ref<Subsystem::Context>)
        void OnUnload(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Decoder> mDecoder;
        UInt          mSize;
        UInt          mDepth;
        UInt          mChannel;
        UInt          mFrequency;
    };
}