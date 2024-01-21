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

#include "Audio/COM_Service.hpp"

#include "Content/COM_Service.hpp"

#include "Graphic/COM_Service.hpp"

#include "Graphic/COM_Renderer.hpp"

#include "Platform/COM_Service.hpp"

#include "Platform/COM_Window.hpp"

#include "UI/COM_SciterHost.hpp"

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
        vbBool  WindowFullscreen;
        vbBool  WindowBorderless;
        vbStr16 LogFilename;
    } Kernel_Properties;

    // -=(Undocumented)=-
    [object, uuid("E8030C3B-EFE0-421B-A794-E4618D6D77FC"), pointer_default(unique)]
    __interface Kernel_
    {
        HRESULT Initialize([in] Kernel_Properties * Properties);

        HRESULT Tick();

        [propget]
        HRESULT Audio([out, retval] Audio_Service_ ** Result);

        [propget]
        HRESULT Content([out, retval] Content_Service_ ** Result);

        [propget]
        HRESULT Display([out, retval] Window_ ** Result);

        [propget]
        HRESULT Graphics([out, retval] Graphic_Service_ ** Result);

        [propget]
        HRESULT Platform([out, retval] Platform_Service_ ** Result);

        [propget]
        HRESULT Renderer([out, retval] Graphic_Renderer_ ** Result);

        [propget]
        HRESULT Sciter([out, retval] Sciter_Service_ ** Result);
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

        // \see Engine_::get_Audio
        HRESULT get_Audio(Audio_Service_ ** Result);

        // \see Engine_::get_Content
        HRESULT get_Content( Content_Service_ ** Result);

        // \see Engine_::get_Display
        HRESULT get_Display(Window_ ** Result);

        // \see Engine_::get_Graphics
        HRESULT get_Graphics(Graphic_Service_ ** Result);

        // \see Engine_::get_Platform
        HRESULT get_Platform(Platform_Service_ ** Result);

        // \see Engine_::get_Renderer
        HRESULT get_Renderer(Graphic_Renderer_ ** Result);

        // \see Engine_::get_Sciter
        HRESULT get_Sciter(Sciter_Service_ ** Result);

    private:

        // -=(Undocumented)=-
        SPtr<Graphic::Renderer> GetOrCreateRenderer();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Graphic::Renderer> mRenderer;
    };
}