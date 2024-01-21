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
#include <Platform/Window.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("6BFE0341-7B50-11EE-ADD5-1418C3A8EDB8"), pointer_default(unique)]
    __interface Window_
    {
        HRESULT Poll([out, retval] vbBool * Result);

        HRESULT Close();

        HRESULT SetTitle([in] vbStr8 Title);

        HRESULT SetVisible([in] vbBool Visible);

        HRESULT IsVisible([out, retval] vbBool * Result);

        HRESULT SetFocus();

        HRESULT IsFocused([out, retval] vbBool * Result);

        HRESULT SetMode([in] vbBool Fullscreen, [in] vbBool Borderless);

        HRESULT IsWindowed([out, retval] vbBool * Result);

        HRESULT IsFullscreen([out, retval] vbBool * Result);

        HRESULT SetSize([in] vbInt32 Width, [in] vbInt32 Height);

        HRESULT GetWidth([out, retval] vbInt32 * Result);

        HRESULT GetHeight([out, retval] vbInt32 * Result);

        HRESULT GetScaleX([out, retval] vbReal32 * Result);

        HRESULT GetScaleY([out, retval] vbReal32 * Result);

        HRESULT SetPosition([in] vbInt32 X, [in] vbInt32 Y) ;

        HRESULT GetX([out, retval] vbInt32 * Result);

        HRESULT GetY([out, retval] vbInt32 * Result);

        HRESULT SetClipboard([in] vbStr8 Text);

        HRESULT GetClipboard([out, retval] vbStr16 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("6CD06485-7B50-11EE-ADD5-1418C3A8EDB8")]
    class ATL_NO_VTABLE Window : public Window_, public CSmartPtrWrapper<Platform::Window>
    {
    public:

        // \see Window_::Poll
        HRESULT Poll(vbBool * Result) override;

        // \see Window_::Close
        HRESULT Close() override;

        // \see Window_::SetTitle
        HRESULT SetTitle(vbStr8 Title) override;

        // \see Window_::SetVisible
        HRESULT SetVisible(vbBool Visible) override;

        // \see Window_::IsVisible
        HRESULT IsVisible(vbBool * Result) override;

        // \see Window_::SetFocus
        HRESULT SetFocus() override;

        // \see Window_::IsFocused
        HRESULT IsFocused(vbBool * Result) override;

        // \see Window_::SetMode
        HRESULT SetMode(vbBool Fullscreen, vbBool Borderless) override;

        // \see Window_::IsWindowed
        HRESULT IsWindowed(vbBool * Result) override;

        // \see Window_::IsFullscreen
        HRESULT IsFullscreen(vbBool * Result) override;

        // \see Window_::SetSize
        HRESULT SetSize(vbInt32 Width, vbInt32 Height) override;

        // \see Window_::GetWidth
        HRESULT GetWidth(vbInt32 * Result) override;

        // \see Window_::GetHeight
        HRESULT GetHeight(vbInt32 * Result) override;

        // \see Window_::GetScaleX
        HRESULT GetScaleX(vbReal32 * Result) override;

        // \see Window_::GetScaleY
        HRESULT GetScaleY(vbReal32 * Result) override;

        // \see Window_::SetPosition
        HRESULT SetPosition(vbInt32 X, vbInt32 Y) override;

        // \see Window_::GetX
        HRESULT GetX(vbInt32 * Result) override;

        // \see Window_::GetY
        HRESULT GetY(vbInt32 * Result) override;

        // \see Window_::SetClipboard
        HRESULT SetClipboard(vbStr8 Text) override;

        // \see Window_::GetClipboard
        HRESULT GetClipboard(vbStr16 * Result) override;
    };

}