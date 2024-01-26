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

#include "Core/Serialization/TOML/Parser.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=(Undocumented)=-
    class Properties final
    {
    public:

        // -=(Undocumented)=-
        Properties();

        // -=(Undocumented)=-
        void Load(Ref<TOMLParser> Parser);

        // -=(Undocumented)=-
        void SetWindowHandle(Any Handle)
        {
            mWindowHandle = Handle;
        }

        // -=(Undocumented)=-
        Any GetWindowHandle() const
        {
            return mWindowHandle;
        }

        // -=(Undocumented)=-
        void SetWindowTitle(CStr Title)
        {
            mWindowTitle = Title;
        }

        // -=(Undocumented)=-
        CStr GetWindowTitle() const
        {
            return mWindowTitle;
        }

        // -=(Undocumented)=-
        void SetWindowWidth(UInt Width)
        {
            mWindowWidth = Width;
        }

        // -=(Undocumented)=-
        UInt GetWindowWidth() const
        {
            return mWindowWidth;
        }

        // -=(Undocumented)=-
        void SetWindowHeight(UInt Height)
        {
            mWindowHeight = Height;
        }

        // -=(Undocumented)=-
        UInt GetWindowHeight() const
        {
            return mWindowHeight;
        }

        // -=(Undocumented)=-
        void SetWindowMode(Bool Fullscreen, Bool Borderless)
        {
            mWindowFullscreen = Fullscreen;
            mWindowBorderless = Borderless;
        }

        // -=(Undocumented)=-
        Bool IsWindowFullscreen() const
        {
            return mWindowFullscreen;
        }

        // -=(Undocumented)=-
        Bool IsWindowBorderless() const
        {
            return mWindowBorderless;
        }

        // -=(Undocumented)=-
        void SetLogFilename(CStr Filename)
        {
            mLogFilename = Filename;
        }

        // -=(Undocumented)=-
        CStr GetLogFilename() const
        {
            return mLogFilename;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Any  mWindowHandle;
        SStr mWindowTitle;
        UInt mWindowWidth;
        UInt mWindowHeight;
        Bool mWindowFullscreen;
        Bool mWindowBorderless;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SStr mLogFilename;
    };
}