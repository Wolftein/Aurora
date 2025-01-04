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

#include "Emitter.hpp"
#include "Sound.hpp"
#include "Listener.hpp"

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
        virtual ~Driver() = default;

        // -=(Undocumented)=-
        virtual Bool Initialize(CStr Device, UInt8 Submixes) = 0;

        // -=(Undocumented)=-
        virtual ConstRef<Capabilities> GetCapabilities() const = 0;

        // -=(Undocumented)=-
        virtual void Tick() = 0;

        // -=(Undocumented)=-
        virtual void Suspend() = 0;

        // -=(Undocumented)=-
        virtual void Restore() = 0;

        // -=(Undocumented)=-
        virtual void SetListener(Ref<Listener> Listener) = 0;

        // -=(Undocumented)=-
        virtual void SetMasterVolume(Real32 Volume) = 0;

        // -=(Undocumented)=-
        virtual void SetSubmixVolume(UInt8 Submix, Real32 Volume) = 0;

        // -=(Undocumented)=-
        virtual Real32 GetMasterVolume() = 0;

        // -=(Undocumented)=-
        virtual Real32 GetSubmixVolume(UInt8 Submix) = 0;

        // -=(Undocumented)=-
        virtual Object Play(UInt8 Submix, ConstSPtr<Sound> Sound, ConstSPtr<Emitter> Emitter, Bool Repeat) = 0;

        // -=(Undocumented)=-
        virtual void SetGain(Object Instance, Real32 Gain) = 0;

        // -=(Undocumented)=-
        virtual void Start(Object Instance) = 0;

        // -=(Undocumented)=-
        virtual void Pause(Object Instance) = 0;

        // -=(Undocumented)=-
        virtual void Stop(Object Instance, Bool Immediately) = 0;

        // -=(Undocumented)=-
        virtual void Stop(ConstSPtr<Emitter> Emitter, Bool Immediately) = 0;
    };
}