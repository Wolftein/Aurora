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

        // \see Subsystem::OnTick(Real64)
        void OnTick(Real64 Time) override;

        // -=(Undocumented)=-
        Bool Initialize(Backend Backend, CStr Device);

        // -=(Undocumented)=-
        Bool Reset(CStr Device);

        // -=(Undocumented)=-
        Ref<const Capabilities> GetCapabilities() const;

        // -=(Undocumented)=-
        void Suspend();

        // -=(Undocumented)=-
        void Restore();

        // -=(Undocumented)=-
        void SetListener(Ref<Listener> Listener);

        // -=(Undocumented)=-
        void SetMasterVolume(Real32 Volume);

        // -=(Undocumented)=-
        void SetSubmixVolume(UInt8 Submix, Real32 Volume);

        // -=(Undocumented)=-
        Real32 GetMasterVolume();

        // -=(Undocumented)=-
        Real32 GetSubmixVolume(UInt8 Submix);

        // -=(Undocumented)=-
        Object Play(UInt8 Submix, ConstSPtr<Sound> Sound, ConstSPtr<Emitter> Emitter, Bool Repeat);

        // -=(Undocumented)=-
        void SetGain(Object Instance, Real32 Gain);

        // -=(Undocumented)=-
        void Start(Object Instance);

        // -=(Undocumented)=-
        void Pause(Object Instance);

        // -=(Undocumented)=-
        void Stop(Object Instance, Bool Immediately);

        // -=(Undocumented)=-
        void Stop(ConstSPtr<Emitter> Emitter, Bool Immediately);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Driver> mDriver;
    };
}