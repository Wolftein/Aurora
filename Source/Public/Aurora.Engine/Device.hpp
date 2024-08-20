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

#include "Aurora.Input/Common.hpp"
#include "Aurora.Graphic/Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=(Undocumented)=-
    class Device
    {
    public:

        // -=(Undocumented)=-
        Device(UInt Handle, CStr Title, UInt16 Width, UInt16 Height, Graphic::Backend Backend, Bool Fullscreen, Bool Borderless);

        // -=(Undocumented)=-
        ~Device();

        // -=(Undocumented)=-
        UInt GetHandle() const
        {
#ifdef     SDL_PLATFORM_WIN32
            Ptr<const Char> Property = SDL_PROP_WINDOW_WIN32_HWND_POINTER;
#else
            Ptr<const Char> Property = nullptr;  // TODO X11 / Cocoa / Wayland / Android
#endif  // SDL_PLATFORM_WIN32
            return reinterpret_cast<UInt>(SDL_GetPointerProperty(SDL_GetWindowProperties(mWindow), Property, nullptr));
        }

        // -=(Undocumented)=-
        void SetTitle(CStr Title)
        {
            SDL_SetWindowTitle(mWindow, Title.data());
        }

        // -=(Undocumented)=-
        CStr GetTitle() const
        {
            return SDL_GetWindowTitle(mWindow);
        }

        // -=(Undocumented)=-
        void SetVisible(Bool Visible)
        {
            if (Visible)
            {
                SDL_ShowWindow(mWindow);
            }
            else
            {
                SDL_HideWindow(mWindow);
            }
        }

        // -=(Undocumented)=-
        Bool IsVisible() const
        {
            return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_HIDDEN) == 0;
        }

        // -=(Undocumented)=-
        void SetFocus()
        {
            SDL_RaiseWindow(mWindow);
        }

        // -=(Undocumented)=-
        Bool IsFocused() const
        {
            return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_INPUT_FOCUS) != 0;
        }

        // -=(Undocumented)=-
        void SetMode(Bool Fullscreen, Bool Borderless);

        // -=(Undocumented)=-
        Bool IsBorderless() const
        {
            return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_BORDERLESS) != 0;
        }

        // -=(Undocumented)=-
        Bool IsWindowed() const
        {
            return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN) == 0;
        }

        // -=(Undocumented)=-
        Bool IsFullscreen() const
        {
            return (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN) != 0;
        }

        // -=(Undocumented)=-
        void SetSize(UInt16 Width, UInt16 Height)
        {
            mWidth  = Width;
            mHeight = Height;
            SDL_SetWindowSize(mWindow, Width, Height);
        }

        // -=(Undocumented)=-
        UInt16 GetWidth() const
        {
            return mWidth;
        }

        // -=(Undocumented)=-
        UInt16 GetHeight() const
        {
            return mHeight;
        }

        // -=(Undocumented)=-
        void SetClipboard(CStr Text)
        {
            SDL_SetClipboardText(Text.data());
        }

        // -=(Undocumented)=-
        SStr GetClipboard() const
        {
            return SDL_GetClipboardText();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<SDL_Window> mWindow;
        UInt16          mWidth;
        UInt16          mHeight;
    };
}