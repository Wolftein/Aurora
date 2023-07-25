// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Wolftein. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Platform/Window.hpp"
#include "GLFWMouse.hpp"
#include "GLFWKeyboard.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Platform
{
    // -=(Undocumented)=-
    class GLFWWindow final : public Window
    {
    public:

        // -=(Undocumented)=-
        GLFWWindow(Ref<Subsystem::Context> System);

        // -=(Undocumented)=-
        ~GLFWWindow();

        // -=(Undocumented)=-
        Bool Create(CStr Title, UInt Width, UInt Height);

        // \see Window::Poll
        Bool Poll() override;

        // \see Window::Close
        void Close() override;

        // \see Window::GetHandle
        Any GetHandle() const override;

        // \see Window::SetTitle
        void SetTitle(CStr Title) override;

        // \see Window::SetFocus
        void SetFocus() override;

        // \see Window::IsFocused
        Bool IsFocused() const override;

        // \see Window::SetMode
        void SetMode(Bool Fullscreen) override;

        // \see Window::IsWindowed
        Bool IsWindowed() const override;

        // \see Window::IsFullscreen
        Bool IsFullscreen() const override;

        // \see Window::SetSize
        void SetSize(Vector2i Size) override;

        // \see Window::GetSize
        Vector2i GetSize() const override;

        // \see Window::SetPosition
        void SetPosition(Vector2i Position) override;

        // \see Window::GetPosition
        Vector2i GetPosition() const override;

        // \see Window::SetClipboard
        void SetClipboard(CStr Text) override;

        // \see Window::GetClipboard
        SStr GetClipboard() const override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<GLFWwindow>    mHandle;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<GLFWMouse>    mMouse;
        SPtr<GLFWKeyboard> mKeyboard;
    };
}