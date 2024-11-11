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

#include "Aurora.Base/Type.hpp"

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
        void SetWindowHandle(UInt Handle)
        {
            mWindowHandle = Handle;
        }

        // -=(Undocumented)=-
        UInt GetWindowHandle() const
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
        void SetWindowWidth(UInt16 Width)
        {
            mWindowWidth = Width;
        }

        // -=(Undocumented)=-
        UInt16 GetWindowWidth() const
        {
            return mWindowWidth;
        }

        // -=(Undocumented)=-
        void SetWindowHeight(UInt16 Height)
        {
            mWindowHeight = Height;
        }

        // -=(Undocumented)=-
        UInt16 GetWindowHeight() const
        {
            return mWindowHeight;
        }

        // -=(Undocumented)=-
        void SetWindowSamples(UInt8 Samples)
        {
            mWindowSamples = Samples;
        }

        // -=(Undocumented)=-
        UInt16 GetWindowSamples() const
        {
            return mWindowSamples;
        }

        // -=(Undocumented)=-
        void SetWindowFullscreen(Bool Fullscreen)
        {
            mWindowFullscreen = Fullscreen;
        }

        // -=(Undocumented)=-
        Bool IsWindowFullscreen() const
        {
            return mWindowFullscreen;
        }

        // -=(Undocumented)=-
        void SetWindowBorderless(Bool Borderless)
        {
            mWindowBorderless = Borderless;
        }

        // -=(Undocumented)=-
        Bool IsWindowBorderless() const
        {
            return mWindowBorderless;
        }

        // -=(Undocumented)=-
        void SetAudioDevice(CStr Device)
        {
            mAudioDevice = Device;
        }

        // -=(Undocumented)=-
        CStr GetAudioDevice() const
        {
            return mAudioDevice;
        }

        // -=(Undocumented)=-
        void SetVideoDriver(CStr Driver)
        {
            mVideoDriver = Driver;
        }

        // -=(Undocumented)=-
        CStr GetVideoDriver() const
        {
            return mVideoDriver;
        }

        // -=(Undocumented)=-
        void SetVideoDevice(CStr Device)
        {
            mVideoDevice = Device;
        }

        // -=(Undocumented)=-
        CStr GetVideoDevice() const
        {
            return mVideoDevice;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt   mWindowHandle;
        SStr   mWindowTitle;
        UInt16 mWindowWidth;
        UInt16 mWindowHeight;
        UInt8  mWindowSamples;
        Bool   mWindowFullscreen;
        Bool   mWindowBorderless;
        SStr   mAudioDevice;
        SStr   mVideoDriver;
        SStr   mVideoDevice;
    };
}