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

#include <Engine/Kernel.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [export, uuid("5C9EF6AF-F29B-49EB-B72E-D584841F4485")]
    typedef struct Kernel_Properties
    {
        vbInt32 WindowHandle;
        vbStr16 WindowTitle;
        vbInt32 WindowWidth;
        vbInt32 WindowHeight;
    } Kernel_Properties;

    // -=(Undocumented)=-
    [object, uuid("E8030C3B-EFE0-421B-A794-E4618D6D77FC"), pointer_default(unique)]
    __interface Kernel_
    {
        HRESULT Initialize([in] Kernel_Properties * Properties);

        HRESULT Tick();

        HRESULT GetAudioService([out, retval] Audio_Service_ ** Result);

        HRESULT GetContentService([out, retval] Content_Service_ ** Result);

        HRESULT GetGraphicService([out, retval] Graphic_Service_ ** Result);
    };

    // -=(Undocumented)=-
    [appobject, coclass, uuid("A6857177-F1DB-41EF-8148-C6EB05A627CC")]
    class ATL_NO_VTABLE Kernel : public Kernel_, public CObjectWrapper<Engine::Kernel>
    {
    public:

        // \see Engine_::Initialize
        HRESULT Initialize(Kernel_Properties * Properties) override;

        // \see Engine_::Tick
        HRESULT Tick() override;

        // \see Engine_::GetAudioService
        HRESULT GetAudioService(Audio_Service_ ** Result) override;

        // \see Engine_::GetContentService
        HRESULT GetContentService(Content_Service_ ** Result) override;

        // \see Engine_::GetGraphicService
        HRESULT GetGraphicService(Graphic_Service_ ** Result) override;
    };
}