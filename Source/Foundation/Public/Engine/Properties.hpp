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

#include "Platform/Common.hpp"

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
        void SetWindowMode(Platform::Decoration Mode)
        {
            mWindowMode = Mode;
        }

        // -=(Undocumented)=-
        Platform::Decoration GetWindowMode() const
        {
            return mWindowMode;
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

        Any                  mWindowHandle;
        SStr                 mWindowTitle;
        UInt                 mWindowWidth;
        UInt                 mWindowHeight;
        Platform::Decoration mWindowMode;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SStr                 mLogFilename;
    };
}