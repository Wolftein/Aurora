// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Device.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Device::Device(UInt Handle, CStr Title, UInt16 Width, UInt16 Height, Graphic::Backend Backend, Bool Fullscreen, Bool Borderless)
        : mHandle { nullptr }
    {
        const SDL_PropertiesID Config = SDL_CreateProperties();
        SDL_SetStringProperty(Config, SDL_PROP_WINDOW_CREATE_TITLE_STRING, Title.data());
        SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
        SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
        SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, Width);
        SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, Height);
        SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, Fullscreen);
        SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, Borderless);
        SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
        SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN, true);
        SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, !Borderless);

        if (Backend == Graphic::Backend::GLES3)
        {
            SDL_SetBooleanProperty(Config, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
        }

#ifdef     SDL_PLATFORM_WIN32

        if (Handle)
        {
            SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, static_cast<SInt64>(Handle));
        }

#endif  // SDL_PLATFORM_WIN32

        mHandle = SDL_CreateWindowWithProperties(Config);
        if (! mHandle)
        {
            Log::Error("Failed to create device: '{}'", SDL_GetError());
        }

#ifdef     SDL_PLATFORM_WIN32

        SDL_StartTextInput(mHandle);

#endif  // SDL_PLATFORM_WIN32
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Device::~Device()
    {
        if (mHandle)
        {
            SDL_DestroyWindow(mHandle);
        }
        SDL_Quit();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Device::SetMode(Bool Fullscreen, Bool Borderless)
    {
        if (Fullscreen)
        {
            mSize.Set(GetWidth(), GetHeight());

            SDL_DisplayMode FullscreenDisplayMode;
            if (!Borderless)
            {
                SDL_GetClosestFullscreenDisplayMode(0, mSize.GetX(), mSize.GetY(), 0, true, & FullscreenDisplayMode);
            }

            const Ptr<const SDL_DisplayMode> Display = (Borderless ? nullptr : & FullscreenDisplayMode);
            SDL_SetWindowFullscreenMode(mHandle, Display);
            SDL_SetWindowFullscreen(mHandle, true);
        }
        else
        {
            SDL_SetWindowSize(mHandle, mSize.GetX(), mSize.GetY());
            SDL_SetWindowFullscreen(mHandle, false);
            SDL_SetWindowPosition(mHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            SDL_SetWindowBordered(mHandle, !Borderless);
        }
    }
}