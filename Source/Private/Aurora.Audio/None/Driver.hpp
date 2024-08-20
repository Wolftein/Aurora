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

#include "Aurora.Audio/Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class NoneDriver final : public Driver
    {
    public:

        // \see Driver::Initialize
        Bool Initialize(CStr Device, UInt8 Submixes) override;

        // \see Driver::GetCapabilities
        Ref<const Capabilities> GetCapabilities() const override;

        // \see Driver::Tick
        void Tick() override;

        // \see Driver::Suspend
        void Suspend() override;

        // \see Driver::Restore
        void Restore() override;

        // \see Driver::SetListener
        void SetListener(Ref<Listener> Listener) override;

        // \see Driver::SetMasterVolume
        void SetMasterVolume(Real32 Volume) override;

        // \see Driver::SetMasterVolume
        void SetSubmixVolume(UInt8 Submix, Real32 Volume) override;

        // \see Driver::GetMasterVolume
        Real32 GetMasterVolume() override;

        // \see Driver::GetSubmixVolume
        Real32 GetSubmixVolume(UInt8 Submix) override;

        // \see Driver::Play
        Object Play(UInt8 Submix, ConstSPtr<Sound> Sound, ConstSPtr<Emitter> Emitter, Bool Repeat) override;

        // \see Driver::SetGain
        void SetGain(Object Instance, Real32 Gain) override;

        // \see Driver::Start
        void Start(Object Instance) override;

        // \see Driver::Pause
        void Pause(Object Instance) override;

        // \see Driver::Stop
        void Stop(Object Instance, Bool Immediately) override;

        // \see Driver::Stop
        void Stop(ConstSPtr<Emitter> Emitter, Bool Immediately) override;
    };
}