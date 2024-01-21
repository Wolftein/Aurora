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

#include "COM_Emitter.hpp"
#include "COM_Sound.hpp"
#include <Audio/Service.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("3B36E9C8-1E5E-498E-95E6-3E349209AE86"), pointer_default(unique)]
    __interface Audio_Service_
    {
        HRESULT Suspend();

        HRESULT Restore();

        HRESULT SetListener([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT SetMasterVolume([in] vbReal32 Volume);

        HRESULT SetSubmixVolume([in] vbInt32 Submix, [in] vbReal32 Volume);

        HRESULT GetMasterVolume([out, retval] vbReal32 * Volume);

        HRESULT GetSubmixVolume([in] vbInt32 Submix, [out, retval] vbReal32 * Volume);

        HRESULT Play([in] vbInt32 Submix, [in] Audio_Sound_ * Sound, [in] Audio_Emitter_ * Emitter, [in] vbBool Repeat, [out, retval] vbInt32 * Instance);

        HRESULT SetGain([in] vbInt32 Instance, [in] vbReal32 Gain);

        HRESULT SetPitch([in] vbInt32 Instance, [in] vbReal32 Pitch);

        HRESULT Start([in] vbInt32 Instance);

        HRESULT Pause([in] vbInt32 Instance);

        HRESULT StopByID([in] vbInt32 Instance, [in, defaultvalue(1)] vbBool Immediately);

        HRESULT StopByEmitter([in] Audio_Emitter_ * Emitter, [in, defaultvalue(1)] vbBool Immediately);

        HRESULT Halt([in] vbInt32 Submix);
    };

    // -=(Undocumented)=-
    [coclass, uuid("A9F6B270-0BA3-4B32-A60F-5B7225415D33")]
    class ATL_NO_VTABLE Audio_Service : public Audio_Service_, public CSmartPtrWrapper<Audio::Service>
    {
    public:

        // \inheritDoc
        HRESULT Suspend() override;

        // \inheritDoc
        HRESULT Restore() override;

        // \inheritDoc
        HRESULT SetListener(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \inheritDoc
        HRESULT SetMasterVolume(vbReal32 Volume) override;

        // \inheritDoc
        HRESULT SetSubmixVolume(vbInt32 Submix, vbReal32 Volume) override;

        // \inheritDoc
        HRESULT GetMasterVolume(vbReal32 * Volume) override;

        // \inheritDoc
        HRESULT GetSubmixVolume(vbInt32 Submix, vbReal32 * Volume) override;

        // \inheritDoc
        HRESULT Play(vbInt32 Submix, Audio_Sound_ * Sound, Audio_Emitter_ * Emitter, vbBool Repeat, vbInt32 * Instance) override;

        // \inheritDoc
        HRESULT SetGain(vbInt32 Instance, vbReal32 Gain) override;

        // \inheritDoc
        HRESULT SetPitch(vbInt32 Instance, vbReal32 Pitch) override;

        // \inheritDoc
        HRESULT Start(vbInt32 Instance) override;

        // \inheritDoc
        HRESULT Pause(vbInt32 Instance) override;

        // \inheritDoc
        HRESULT StopByID(vbInt32 Instance, vbBool Immediately) override;

        // \inheritDoc
        HRESULT StopByEmitter(Audio_Emitter_ * Emitter, vbBool Immediately) override;

        // \inheritDoc
        HRESULT Halt(vbInt32 Submix) override;
    };
}