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

#include "Audio/Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class NoneDriver final : public Driver
    {
    public:

        // \see Driver::Initialise
        Bool Initialise(UInt32 Submixes) override;

        // \see Driver::Advance
        void Advance() override;

        // \see Driver::Suspend
        void Suspend() override;

        // \see Driver::Restore
        void Restore() override;

        // \see Driver::SetListener
        void SetListener(Ref<const Vector3f> Position, Ref<const Vector4f> Orientation) override;

        // \see Driver::SetMasterVolume
        void SetMasterVolume(Real32 Volume) override;

        // \see Driver::SetMasterVolume
        void SetSubmixVolume(UInt Submix, Real32 Volume) override;

        // \see Driver::GetMasterVolume
        Real32 GetMasterVolume() override;

        // \see Driver::GetSubmixVolume
        Real32 GetSubmixVolume(UInt Submix) override;

        // \see Driver::Play
        Object Play(UInt Submix, Ref<const SPtr<Sound>> Sound, Ref<const SPtr<Emitter>> Emitter, Bool Repeat) override;

        // \see Driver::SetGain
        void SetGain(Object Instance, Real32 Gain) override;

        // \see Driver::SetPitch
        void SetPitch(Object Instance, Real32 Pitch) override;

        // \see Driver::Start
        void Start(Object Instance) override;

        // \see Driver::Stop
        void Stop(Object Instance, Bool Immediately) override;

        // \see Driver::Stop
        void Stop(Ref<const SPtr<Emitter>> Emitter, Bool Immediately) override;

        // \see Driver::Halt
        void Halt(UInt Submix) override;
    };
}