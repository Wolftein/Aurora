// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Sound.hpp"
#include "Emitter.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class Driver
    {
    public:

        // -=(Undocumented)=-
        static constexpr inline UInt k_MaxMixes    = 128;

        // -=(Undocumented)=-
        static constexpr inline UInt k_MaxSubmixes = 16;

    public:

        // -=(Undocumented)=-
        virtual ~Driver() = default;

        // -=(Undocumented)=-
        virtual Bool Initialise(UInt32 Submixes) = 0;

        // -=(Undocumented)=-
        virtual void Advance() = 0;

        // -=(Undocumented)=-
        virtual void Suspend() = 0;

        // -=(Undocumented)=-
        virtual void Restore() = 0;

        // -=(Undocumented)=-
        virtual void SetListener(Ref<const Vector3f> Position, Ref<const Vector4f> Orientation) = 0;

        // -=(Undocumented)=-
        virtual void SetMasterVolume(Real32 Volume) = 0;

        // -=(Undocumented)=-
        virtual void SetSubmixVolume(UInt Submix, Real32 Volume) = 0;

        // -=(Undocumented)=-
        virtual Real32 GetMasterVolume() = 0;

        // -=(Undocumented)=-
        virtual Real32 GetSubmixVolume(UInt Submix) = 0;

        // -=(Undocumented)=-
        virtual UInt Play(UInt Submix, Ref<const SPtr<Sound>> Sound, Ref<const SPtr<Emitter>> Emitter, Bool Repeat) = 0;

        // -=(Undocumented)=-
        virtual void SetGain(UInt Instance, Real32 Gain) = 0;

        // -=(Undocumented)=-
        virtual void SetPitch(UInt Instance, Real32 Pitch) = 0;

        // -=(Undocumented)=-
        virtual void Start(UInt Instance) = 0;

        // -=(Undocumented)=-
        virtual void Stop(UInt Instance, Bool Immediately) = 0;

        // -=(Undocumented)=-
        virtual void Halt(UInt Submix) = 0;
    };
}