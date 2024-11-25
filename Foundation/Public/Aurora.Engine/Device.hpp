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
        Ptr<SDL_Window> GetHandle()
        {
            return mHandle;
        }

        // -=(Undocumented)=-
        void SetTitle(CStr Title)
        {
            SDL_SetWindowTitle(mHandle, Title.data());
        }

        // -=(Undocumented)=-
        CStr GetTitle() const
        {
            return SDL_GetWindowTitle(mHandle);
        }

        // -=(Undocumented)=-
        void SetVisible(Bool Visible)
        {
            if (Visible)
            {
                SDL_ShowWindow(mHandle);
            }
            else
            {
                SDL_HideWindow(mHandle);
            }
        }

        // -=(Undocumented)=-
        Bool IsVisible() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_HIDDEN) == 0;
        }

        // -=(Undocumented)=-
        void SetFocus()
        {
            SDL_RaiseWindow(mHandle);
        }

        // -=(Undocumented)=-
        Bool IsFocused() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_INPUT_FOCUS) != 0;
        }

        // -=(Undocumented)=-
        void SetMode(Bool Fullscreen, Bool Borderless);

        // -=(Undocumented)=-
        Bool IsBorderless() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_BORDERLESS) != 0;
        }

        // -=(Undocumented)=-
        Bool IsWindowed() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_FULLSCREEN) == 0;
        }

        // -=(Undocumented)=-
        Bool IsFullscreen() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_FULLSCREEN) != 0;
        }

        // -=(Undocumented)=-
        void SetPosition(UInt16 X, UInt16 Y)
        {
            SDL_SetWindowPosition(mHandle, X, Y);
        }

        // -=(Undocumented)=-
        Vector2i GetPosition() const
        {
            int X;
            int Y;
            SDL_GetWindowPosition(mHandle, AddressOf(X), AddressOf(Y));
            return Vector2i(X, Y);
        }

        // -=(Undocumented)=-
        void SetSize(UInt16 Width, UInt16 Height)
        {
            SDL_SetWindowSize(mHandle, Width, Height);
        }

        // -=(Undocumented)=-
        UInt16 GetWidth() const
        {
            int Width;
            SDL_GetWindowSize(mHandle, AddressOf(Width), nullptr);
            return Width;
        }

        // -=(Undocumented)=-
        UInt16 GetHeight() const
        {
            int Height;
            SDL_GetWindowSize(mHandle, nullptr, AddressOf(Height));
            return Height;
        }

        // -=(Undocumented)=-
        Real32 GetScale() const
        {
            return SDL_GetWindowDisplayScale(mHandle);
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

        // -=(Undocumented)=-
        void SetConstrain(Bool Constrain)
        {
            SDL_SetWindowRelativeMouseMode(mHandle, Constrain);
        }

        // -=(Undocumented)=-
        Bool IsConstrained() const
        {
            return SDL_GetWindowRelativeMouseMode(mHandle);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<SDL_Window> mHandle;
        Vector2i        mSize;
    };
}