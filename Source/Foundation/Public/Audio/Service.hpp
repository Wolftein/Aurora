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

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class Service final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Service(Ref<Context> Context);

        // \see Subsystem::OnTick
        void OnTick() override;

        // -=(Undocumented)=-
        Bool Initialise(Backend Backend, UInt32 Submixes);

        // -=(Undocumented)=-
        void Suspend();

        // -=(Undocumented)=-
        void Restore();

        // -=(Undocumented)=-
        void SetListener(Ref<const Vector3f> Position);

        // -=(Undocumented)=-
        void SetMasterVolume(Real32 Volume);

        // -=(Undocumented)=-
        void SetSubmixVolume(UInt Submix, Real32 Volume);

        // -=(Undocumented)=-
        Real32 GetMasterVolume();

        // -=(Undocumented)=-
        Real32 GetSubmixVolume(UInt Submix);

        // -=(Undocumented)=-
        Object Play(UInt Submix, Ref<const SPtr<Sound>> Sound, Ref<const SPtr<Emitter>> Emitter, Bool Repeat);

        // -=(Undocumented)=-
        void SetGain(Object Instance, Real32 Gain);

        // -=(Undocumented)=-
        void SetPitch(Object Instance, Real32 Pitch);

        // -=(Undocumented)=-
        void Start(Object Instance);

        // -=(Undocumented)=-
        void Pause(Object Instance);

        // -=(Undocumented)=-
        void Stop(Object Instance, Bool Immediately);

        // -=(Undocumented)=-
        void Stop(Ref<const SPtr<Emitter>> Emitter, Bool Immediately);

        // -=(Undocumented)=-
        void Halt(UInt Submix);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Driver> mDriver;
    };
}