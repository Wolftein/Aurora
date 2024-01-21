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

#include "Core/COM_Common.hpp"
#include <Audio/Emitter.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("9FC27518-0218-4B0C-AF01-F7E96E115FC3"), pointer_default(unique)]
    __interface Audio_Emitter_
    {
        HRESULT SetPosition([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT SetVelocity([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT SetAttenuation([in] vbReal32 Attenuation);

        HRESULT SetInnerRadius([in] vbReal32 Radius);

        HRESULT SetInnerRadiusAngle([in] vbReal32 Radius);

        HRESULT GetPositionX([out, retval] vbReal32 * Result);

        HRESULT GetPositionY([out, retval] vbReal32 * Result);

        HRESULT GetPositionZ([out, retval] vbReal32 * Result);

        HRESULT GetVelocityX([out, retval] vbReal32 * Result);

        HRESULT GetVelocityY([out, retval] vbReal32 * Result);

        HRESULT GetVelocityZ([out, retval] vbReal32 * Result);

        HRESULT GetAttenuation([out, retval] vbReal32 * Result);

        HRESULT GetInnerRadius([out, retval] vbReal32 * Result);

        HRESULT GetInnerRadiusAngle([out, retval] vbReal32 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("ABBAE0F7-F07B-4B11-9268-2FFF8C5B1822")]
    class ATL_NO_VTABLE Audio_Emitter : public Audio_Emitter_, public CSmartPtrWrapper<Audio::Emitter>
    {
    public:

        // \inheritDoc
        HRESULT FinalConstruct();

        // \inheritDoc
        HRESULT SetPosition(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \inheritDoc
        HRESULT SetVelocity(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \inheritDoc
        HRESULT SetAttenuation(vbReal32 Attenuation) override;

        // \inheritDoc
        HRESULT SetInnerRadius(vbReal32 Radius) override;

        // \inheritDoc
        HRESULT SetInnerRadiusAngle(vbReal32 Radius) override;

        // \inheritDoc
        HRESULT GetPositionX(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetPositionY(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetPositionZ(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetVelocityX(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetVelocityY(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetVelocityZ(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetAttenuation(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetInnerRadius(vbReal32 * Result) override;

        // \inheritDoc
        HRESULT GetInnerRadiusAngle(vbReal32 * Result) override;
    };

}