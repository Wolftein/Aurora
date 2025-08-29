// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
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

    Device::Device()
        : mHandle { nullptr }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Device::Device(UInt Handle, ConstText Title, UInt16 Width, UInt16 Height, Bool Fullscreen, Bool Borderless)
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

#ifdef     SDL_PLATFORM_WIN32

        if (Handle)
        {
            SDL_SetNumberProperty(Config, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, static_cast<SInt64>(Handle));
        }

#endif  // SDL_PLATFORM_WIN32

        mHandle = SDL_CreateWindowWithProperties(Config);

#ifdef     SDL_PLATFORM_WIN32
        if (mHandle)
        {
            SDL_StartTextInput(mHandle);
        }
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
            SDL_DisplayMode SuitableDisplayMode;
            if (!Borderless)
            {
                const SDL_DisplayID DisplayID = SDL_GetDisplayForWindow(mHandle);
                SDL_GetClosestFullscreenDisplayMode(DisplayID, GetWidth(), GetHeight(), 0, true, &SuitableDisplayMode);
            }
            SDL_SetWindowFullscreenMode(mHandle, Borderless ? nullptr : &SuitableDisplayMode);
        }
        else
        {
            SDL_SetWindowBordered(mHandle, !Borderless);
        }
        SDL_SetWindowFullscreen(mHandle, Fullscreen);
    }
}