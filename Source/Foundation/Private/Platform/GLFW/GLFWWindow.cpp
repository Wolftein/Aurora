// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Wolftein. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "GLFWWindow.hpp"
#include "Input/Service.hpp"
#include <GLFW/glfw3native.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Platform
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    GLFWWindow::GLFWWindow(Ref<Subsystem::Context> System)
         : mHandle   { nullptr },
           mKeyboard { NewPtr<GLFWKeyboard>() },
           mMouse    { NewPtr<GLFWMouse>() }
    {
        const SPtr<Input::Service> Input = System.GetSubsystem<Input::Service>();
        Input->Register<Input::Keyboard>(mKeyboard);
        Input->Register<Input::Mouse>(mMouse);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    GLFWWindow::~GLFWWindow()
    {
        if (mHandle)
        {
            glfwDestroyWindow(mHandle);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLFWWindow::Create(CStr Title, UInt Width, UInt Height, Decoration Mode)
    {
        glfwWindowHint(GLFW_CLIENT_API,   GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE,      GLFW_FALSE);

        Ptr<GLFWmonitor> Monitor         = glfwGetPrimaryMonitor();
        Ptr<const GLFWvidmode> VideoMode = glfwGetVideoMode(Monitor);

        switch (Mode)
        {
        case Decoration::Fullscreen:
            glfwWindowHint(GLFW_RED_BITS,     VideoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS,   VideoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS,    VideoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, VideoMode->refreshRate);

            Width  = VideoMode->width;
            Height = VideoMode->height;
            break;
        case Decoration::FullscreenExclusive:
            break;
        case Decoration::Windowed:
        case Decoration::WindowedBorderless:
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            break;
        }

        const Bool HasDecoration = (Mode == Decoration::Windowed);
        glfwWindowHint(GLFW_DECORATED, HasDecoration);
        glfwWindowHint(GLFW_RESIZABLE, HasDecoration);

        const Bool IsFullscreen = (Mode == Decoration::Fullscreen || Mode == Decoration::FullscreenExclusive);
        mHandle = glfwCreateWindow(Width, Height, Title.data(), IsFullscreen ? Monitor : nullptr, nullptr);

        if (Mode == Decoration::Windowed || Mode == Decoration::WindowedBorderless)
        {
            glfwSetWindowPos(mHandle, (VideoMode->width - Width ) / 2, (VideoMode->height - Height) / 2);
        }

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(mHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        glfwShowWindow(mHandle);
        glfwSetWindowUserPointer(mHandle, this);

        glfwSetKeyCallback(mHandle,  [](Ptr<GLFWwindow> Window, int Key, int Scancode, int Action, int Mods)
        {
            Ptr<GLFWWindow> Self = static_cast<Ptr<GLFWWindow>>(glfwGetWindowUserPointer(Window));

            (Self->mKeyboard)->OnKeyAction(Key, Scancode, Action, Mods);
        });

        glfwSetCharCallback(mHandle, [](Ptr<GLFWwindow> Window, unsigned int Codepoint)
        {
            Ptr<GLFWWindow> Self = static_cast<Ptr<GLFWWindow>>(glfwGetWindowUserPointer(Window));

            (Self->mKeyboard)->OnKeyType(Codepoint);
        });

        glfwSetCursorPosCallback(mHandle, [](Ptr<GLFWwindow> Window, Real64 X, Real64 Y)
        {
            Ptr<GLFWWindow> Self = static_cast<Ptr<GLFWWindow>>(glfwGetWindowUserPointer(Window));

            (Self->mMouse)->OnMouseMove(X, Y);
        });

        glfwSetMouseButtonCallback(mHandle, [](Ptr<GLFWwindow> Window, int Button, int Action, int Mods)
        {
            Ptr<GLFWWindow> Self = static_cast<Ptr<GLFWWindow>>(glfwGetWindowUserPointer(Window));

            (Self->mMouse)->OnMouseAction(Button, Action, Mods);
        });

        glfwSetScrollCallback(mHandle, [](Ptr<GLFWwindow> Window, Real64 X, Real64 Y)
        {
            Ptr<GLFWWindow> Self = static_cast<Ptr<GLFWWindow>>(glfwGetWindowUserPointer(Window));

            (Self->mMouse)->OnMouseScroll(X, Y);
        });

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLFWWindow::Poll()
    {
        glfwPollEvents();

        return !glfwWindowShouldClose(mHandle);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::Close()
    {
        glfwSetWindowShouldClose(mHandle, GLFW_TRUE);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Any GLFWWindow::GetHandle() const
    {
        return glfwGetWin32Window(mHandle);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetTitle(CStr Title)
    {
        glfwSetWindowTitle(mHandle, Title.data());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetFocus()
    {
        glfwFocusWindow(mHandle);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLFWWindow::IsFocused() const
    {
        return (glfwGetWindowAttrib(mHandle, GLFW_FOCUSED) == GLFW_TRUE);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetMode(Decoration Mode)
    {
        switch (Mode)
        {
        case Decoration::Fullscreen:
        case Decoration::FullscreenExclusive:
            mDimension = GetSize();

            glfwSetWindowAttrib(mHandle, GLFW_RESIZABLE, GLFW_FALSE);
            glfwSetWindowAttrib(mHandle, GLFW_DECORATED, GLFW_FALSE);

            if (Mode == Decoration::FullscreenExclusive)
            {
                glfwSetWindowMonitor(mHandle, glfwGetPrimaryMonitor(), 0, 0, mDimension.GetX(), mDimension.GetY(), 0);
            }
            else
            {
                Ptr<const GLFWvidmode> VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                glfwSetWindowMonitor(mHandle, glfwGetPrimaryMonitor(), 0, 0,
                    VideoMode->width,
                    VideoMode->height,
                    VideoMode->refreshRate);
            }
            break;
        case Decoration::Windowed:
        case Decoration::WindowedBorderless:
            Ptr<const GLFWvidmode> VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            const UInt X = (VideoMode->width  - mDimension.GetX()) / 2;
            const UInt Y = (VideoMode->height - mDimension.GetY()) / 2;

            glfwSetWindowMonitor(mHandle, nullptr, X, Y, mDimension.GetX(), mDimension.GetY(), 0);

            const Bool HasDecoration = (Mode == Decoration::Windowed);
            glfwSetWindowAttrib(mHandle, GLFW_DECORATED, HasDecoration);
            glfwSetWindowAttrib(mHandle, GLFW_RESIZABLE, HasDecoration);
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLFWWindow::IsWindowed() const
    {
        return (glfwGetWindowMonitor(mHandle) == nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLFWWindow::IsFullscreen() const
    {
        return (glfwGetWindowMonitor(mHandle) != nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetSize(Vector2i Size)
    {
        glfwSetWindowSize(mHandle, Size.GetX(), Size.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2i GLFWWindow::GetSize() const
    {
        SInt32 Width, Height;
        glfwGetWindowSize(mHandle, & Width, & Height);
        return Vector2i(Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2f GLFWWindow::GetScale() const
    {
        Real32 XScale, YScale;
        glfwGetWindowContentScale(mHandle, & XScale, & YScale);
        return Vector2f(XScale, YScale);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetPosition(Vector2i Position)
    {
        glfwSetWindowPos(mHandle, Position.GetX(), Position.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2i GLFWWindow::GetPosition() const
    {
        SInt32 X, Y;
        glfwGetWindowPos(mHandle, & X, & Y);
        return Vector2i(X, Y);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWWindow::SetClipboard(CStr Text)
    {
        glfwSetClipboardString(mHandle, Text.data());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SStr GLFWWindow::GetClipboard() const
    {
        return SStr(glfwGetClipboardString(mHandle));
    }
}