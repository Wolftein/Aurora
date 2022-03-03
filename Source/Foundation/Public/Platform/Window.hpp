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

#include "Core/Core.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Platform
{
    // -=(Undocumented)=-
    class Window
    {
    public:

        // -=(Undocumented)=-
        virtual ~Window() = default;

        // -=(Undocumented)=-
        virtual Bool Poll() = 0;

        // -=(Undocumented)=-
        virtual void Close() = 0;

        // -=(Undocumented)=-
        virtual Any GetHandle() const = 0;

        // -=(Undocumented)=-
        virtual void SetTitle(CStr Title) = 0;

        // -=(Undocumented)=-
        virtual void SetFocus() = 0;

        // -=(Undocumented)=-
        virtual Bool IsFocused() const = 0;

        // -=(Undocumented)=-
        virtual void SetMode(Bool Fullscreen) = 0;

        // -=(Undocumented)=-
        virtual Bool IsWindowed() const = 0;

        // -=(Undocumented)=-
        virtual Bool IsFullscreen() const = 0;

        // -=(Undocumented)=-
        virtual void SetSize(Vector2i Size) = 0;

        // -=(Undocumented)=-
        virtual Vector2i GetSize() const = 0;

        // -=(Undocumented)=-
        virtual void SetPosition(Vector2i Position) = 0;

        // -=(Undocumented)=-
        virtual Vector2i GetPosition() const = 0;
    };
}