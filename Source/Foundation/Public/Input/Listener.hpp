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

#include "Device.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=(Undocumented)=-
    class Listener
    {
    public:

        // -=(Undocumented)=-
        virtual ~Listener() = default;

        // -=(Undocumented)=-
        virtual Bool OnEvent(Ref<const Event> Event);

        // -=(Undocumented)=-
        virtual Bool OnKeyType(UInt Codepoint);

        // -=(Undocumented)=-
        virtual Bool OnKeyUp(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnKeyDown(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnMouseMove(UInt X, UInt Y);

        // -=(Undocumented)=-
        virtual Bool OnMouseScroll(SInt X, SInt Y);

        // -=(Undocumented)=-
        virtual Bool OnMouseUp(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnMouseDown(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnWindowFocus(Bool Focused);

        // -=(Undocumented)=-
        virtual Bool OnWindowResize(SInt Width, SInt Height);
    };
}