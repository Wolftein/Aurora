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
        virtual Bool OnEvent(ConstRef<Event> Event);

    private:

        // -=(Undocumented)=-
        virtual Bool OnKeyType(UInt32 Codepoint);

        // -=(Undocumented)=-
        virtual Bool OnKeyUp(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnKeyDown(Key Key);

        // -=(Undocumented)=-
        virtual Bool OnMouseMove(Real32 X, Real32 Y, Real32 DeltaX, Real32 DeltaY);

        // -=(Undocumented)=-
        virtual Bool OnMouseScroll(Real32 DeltaX, Real32 DeltaY);

        // -=(Undocumented)=-
        virtual Bool OnMouseUp(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnMouseDown(Button Button);

        // -=(Undocumented)=-
        virtual Bool OnWindowFocus(Bool Focused);

        // -=(Undocumented)=-
        virtual Bool OnWindowResize(UInt32 Width, UInt32 Height);

        // -=(Undocumented)=-
        virtual Bool OnWindowExit();
    };
}