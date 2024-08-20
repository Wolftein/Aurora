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

#include "Common.hpp"

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

    private:

        // -=(Undocumented)=-
        virtual Bool OnKeyType(UInt Codepoint);

        // -=(Undocumented)=-
        virtual Bool OnKeyUp(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnKeyDown(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnMouseMove(UInt16 X, UInt16 Y);

        // -=(Undocumented)=-
        virtual Bool OnMouseScroll(SInt16 X, SInt16 Y);

        // -=(Undocumented)=-
        virtual Bool OnMouseUp(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnMouseDown(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnWindowFocus(Bool Focused);

        // -=(Undocumented)=-
        virtual Bool OnWindowResize(UInt16 Width, UInt16 Height);

        // -=(Undocumented)=-
        virtual Bool OnWindowExit();
    };
}