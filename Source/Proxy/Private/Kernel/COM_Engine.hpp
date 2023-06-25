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

#include "Audio/COM_Service.hpp"
#include "Content/COM_Service.hpp"
#include "Graphic/COM_Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [export, uuid("5C9EF6AF-F29B-49EB-B72E-D584841F4485")]
    typedef struct Engine_Config
    {
        vbInt32 GraphicDisplay;
        vbInt32 GraphicDisplayWidth;
        vbInt32 GraphicDisplayHeight;
    } Engine_Config;

    // -=(Undocumented)=-
    [object, uuid("E8030C3B-EFE0-421B-A794-E4618D6D77FC"), pointer_default(unique)]
    __interface Engine_
    {
        HRESULT Initialize([in] Engine_Config * Configuration);

        HRESULT Tick();

        [propget] HRESULT Audio([out, retval] Audio_Service_ ** Result);

        [propget] HRESULT Content([out, retval] Content_Service_ ** Result);

        [propget] HRESULT Graphic([out, retval] Graphic_Service_ ** Result);
    };

    // -=(Undocumented)=-
    [appobject, coclass, uuid("A6857177-F1DB-41EF-8148-C6EB05A627CC")]
    class ATL_NO_VTABLE Engine : public Engine_, public CObjectWrapper<Subsystem::Context>
    {
    public:

        // \see Engine_::Initialize
        HRESULT Initialize(Engine_Config * Configuration) override;

        // \see Engine_::Tick
        HRESULT Tick() override;

        // \see Engine_::Audio
        HRESULT get_Audio(Audio_Service_ ** Result);

        // \see Engine_::Content
        HRESULT get_Content(Content_Service_ ** Result);

        // \see Engine_::Graphic
        HRESULT get_Graphic(Graphic_Service_ ** Result);
    };
}