// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Primitive.hpp"
#include <SDL3/SDL.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Engine
{
    /// \brief Represents a platform-specific window used to display rendered output and receive input.
    class Device final
    {
    public:

        /// \brief Default constructor.
        Device();

        /// \brief Constructs a window with the given parameters.
        ///
        /// \param Handle     Platform-specific window handle (e.g., HWND on Windows, X11 Window on Linux).
        /// \param Title      UTF-8 title of the window.
        /// \param Width      Width of the window in pixels.
        /// \param Height     Height of the window in pixels.
        /// \param Fullscreen `true` to start in fullscreen mode, otherwise `false`.
        /// \param Borderless `true` to remove window borders, otherwise `false`.
        Device(UInt Handle, ConstText Title, UInt16 Width, UInt16 Height, Bool Fullscreen, Bool Borderless);

        /// \brief Destroys the window and releases its resources.
        ~Device();

        /// \brief Returns the internal SDL window handle.
        ///
        /// \return Pointer to the underlying SDL_Window.
        AURORA_INLINE Ptr<SDL_Window> GetHandle()
        {
            return mHandle;
        }

        /// \brief Sets the window title.
        ///
        /// \param Title UTF-8 title string.
        AURORA_INLINE void SetTitle(ConstText Title)
        {
            SDL_SetWindowTitle(mHandle, Title.data());
        }

        /// \brief Returns the current window title.
        ///
        /// \return UTF-8 title string.
        AURORA_INLINE ConstText GetTitle() const
        {
            return SDL_GetWindowTitle(mHandle);
        }

        /// \brief Shows or hides the window.
        ///
        /// \param Visible `true` to show the window, otherwise `false`.
        AURORA_INLINE void SetVisible(Bool Visible)
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

        /// \brief Checks whether the window is currently visible.
        ///
        /// \return `true` if visible, otherwise `false`.
        AURORA_INLINE Bool IsVisible() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_HIDDEN) == 0;
        }

        /// \brief Brings the window to the foreground and focuses it.
        AURORA_INLINE void SetFocus()
        {
            SDL_RaiseWindow(mHandle);
        }

        /// \brief Checks whether the window has input focus.
        ///
        /// \return `true` if focused, otherwise `false`.
        AURORA_INLINE Bool IsFocused() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_INPUT_FOCUS) != 0;
        }

        /// \brief Sets the fullscreen and borderless mode.
        ///
        /// \param Fullscreen `true` to enable fullscreen, otherwise `false`.
        /// \param Borderless `true` to enable borderless mode, otherwise `false`.
        void SetMode(Bool Fullscreen, Bool Borderless);

        /// \brief Checks whether the window is in borderless mode.
        ///
        /// \return `true` if borderless, otherwise `false`.
        AURORA_INLINE Bool IsBorderless() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_BORDERLESS) != 0;
        }

        /// \brief Checks whether the window is in windowed mode.
        ///
        /// \return `true` if windowed, otherwise `false`.
        AURORA_INLINE Bool IsWindowed() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_FULLSCREEN) == 0;
        }

        /// \brief Checks whether the window is in fullscreen mode.
        ///
        /// \return `true` if fullscreen, otherwise `false`.
        AURORA_INLINE Bool IsFullscreen() const
        {
            return (SDL_GetWindowFlags(mHandle) & SDL_WINDOW_FULLSCREEN) != 0;
        }

        /// \brief Sets the window size in pixels.
        ///
        /// \param Width  New window width in pixels.
        /// \param Height New window height in pixels.
        AURORA_INLINE void Resize(UInt16 Width, UInt16 Height)
        {
            SDL_SetWindowSize(mHandle, Width, Height);
        }

        /// \brief Returns the width of the window.
        ///
        /// \return Width in pixels.
        AURORA_INLINE UInt16 GetWidth() const
        {
            int Width;
            SDL_GetWindowSize(mHandle, &Width, nullptr);
            return Width;
        }

        /// \brief Returns the height of the window.
        ///
        /// \return Height in pixels.
        AURORA_INLINE UInt16 GetHeight() const
        {
            int Height;
            SDL_GetWindowSize(mHandle, nullptr, &Height);
            return Height;
        }

        /// \brief Returns the DPI scale factor for the window.
        ///
        /// \return Display scale factor.
        AURORA_INLINE Real32 GetScale() const
        {
            return SDL_GetWindowDisplayScale(mHandle);
        }

        /// \brief Sets the clipboard text.
        ///
        /// \param Text UTF-8 string to copy to clipboard.
        AURORA_INLINE void SetClipboard(ConstText Text)
        {
            SDL_SetClipboardText(Text.data());
        }

        /// \brief Returns the current clipboard text.
        ///
        /// \return UTF-8 clipboard contents.
        AURORA_INLINE Text GetClipboard() const
        {
            return SDL_GetClipboardText();
        }

        /// \brief Constrains the mouse to the window in relative mode.
        ///
        /// \param Constrain `true` to enable relative mouse mode, otherwise `false`.
        AURORA_INLINE void SetConstrain(Bool Constrain)
        {
            SDL_SetWindowRelativeMouseMode(mHandle, Constrain);
        }

        /// \brief Checks whether the mouse is constrained to the window.
        ///
        /// \return `true` if constrained, otherwise `false`.
        AURORA_INLINE Bool IsConstrained() const
        {
            return SDL_GetWindowRelativeMouseMode(mHandle);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<SDL_Window> mHandle;
    };
}
