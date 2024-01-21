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

#include <Input/Listener.hpp>
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
        template<typename Callback>
        SciterInput(HWND Handle, Callback OnDocumentResize)
            : mHandle           { Handle },
              mCbDocumentResize { OnDocumentResize },
              mMouseButtons     { MOUSE_BUTTONS(0) },
              mStates           { 0 },
              mMouseClickTime   { 0 },
              mMouseClickButton { Input::Button::Unknown }
        {
        }

    private:

        // \see Input::Listener::OnEvent
        Bool OnEvent(Ref<const Input::Event> Event) override;

        // \see Input::Listener::OnKeyType
        Bool OnKeyType(UInt Codepoint) override;

        // \see Input::Listener::OnKeyDown
        Bool OnKeyDown(Input::Key Key) override;

        // \see Input::Listener::OnKeyUp
        Bool OnKeyUp(Input::Key Key) override;

        // \see Input::Listener::OnMouseMove
        Bool OnMouseMove(UInt X, UInt Y) override;

        // \see Input::Listener::OnMouseScroll
        Bool OnMouseScroll(SInt X, SInt Y) override;

        // \see Input::Listener::OnMouseUp
        Bool OnMouseUp(Input::Button Button) override;

        // \see Input::Listener::OnMouseDown
        Bool OnMouseDown(Input::Button Button) override;

        // \see Input::Listener::OnWindowFocus
        Bool OnWindowFocus(Bool Focused) override;

        // \see Input::Listener::OnWindowResize
        Bool OnWindowResize(SInt Width, SInt Height) override;

    private:

        // -=(Undocumented)=-
        Bool OnMouseDoubleClick(Input::Button Button);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        HWND                    mHandle;
        FPtr<void (SInt, SInt)> mCbDocumentResize;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        POINT                   mMousePosition;
        MOUSE_BUTTONS           mMouseButtons;
        UINT                    mStates;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real64                  mMouseClickTime;
        Input::Button           mMouseClickButton;
    };
}