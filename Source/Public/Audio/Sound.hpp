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
        void Load(Format Format, UInt Size, UInt Depth, UInt Channel, UInt Frequency, ConstSPtr<Decoder> Decoder);

        // -=(Undocumented)=-
        void Seek(UInt Frame);

        // -=(Undocumented)=-
        Bool Read(Ref<CPtr<UInt08>> Output);

        // -=(Undocumented)=-
        Format GetFormat() const
        {
            return mFormat;
        }

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

    protected:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDispose(Ref<Subsystem::Context>)
        void OnDispose(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Decoder> mDecoder;
        Format        mFormat;
        UInt          mSize;
        UInt          mDepth;
        UInt          mChannel;
        UInt          mFrequency;
    };
}