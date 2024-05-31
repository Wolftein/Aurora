// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Input/Service.hpp>
#include <sciter-x.h>
#include <sciter-x-key-codes.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=(Undocumented)=-
    class SciterInput final : public Input::Listener
    {
    public:

        // -=(Undocumented)=-
        SciterInput(HWINDOW Handle)
            : mHandle        { Handle },
              mKeys          { 0 },
              mMousePosition { 0, 0 },
              mMouseButtons  { MOUSE_BUTTONS(0) }
        {
        }

    private:

        // \see Listener::OnKeyType
        Bool OnKeyType(UInt Codepoint) override;

        // \see Listener::OnKeyDown
        Bool OnKeyDown(Input::Key Key) override;

        // \see Listener::OnKeyUp
        Bool OnKeyUp(Input::Key Key) override;

        // \see Listener::OnMouseMove
        Bool OnMouseMove(UInt X, UInt Y) override;

        // \see Listener::OnMouseScroll
        Bool OnMouseScroll(SInt X, SInt Y) override;

        // \see Listener::OnMouseUp
        Bool OnMouseUp(Input::Button Button) override;

        // \see Listener::OnMouseDown
        Bool OnMouseDown(Input::Button Button) override;

        // \see Listener::OnWindowFocus
        Bool OnWindowFocus(Bool Focused) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        HWINDOW       mHandle;
        UINT          mKeys;
        POINT         mMousePosition;
        MOUSE_BUTTONS mMouseButtons;
    };
}