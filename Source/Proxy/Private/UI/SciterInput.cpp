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

#include "SciterInput.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetKey(Input::Key Key)
    {
        switch (Key)
        {
        case Input::Key::Space:
            return SC_KB_CODES::KB_SPACE;
        case Input::Key::Apostrophe:
            return SC_KB_CODES::KB_APOSTROPHE;
        case Input::Key::Comma:
            return SC_KB_CODES::KB_COMMA;
        case Input::Key::Minus:
            return SC_KB_CODES::KB_MINUS;
        case Input::Key::Period:
            return SC_KB_CODES::KB_PERIOD;
        case Input::Key::Slash:
            return SC_KB_CODES::KB_SLASH;
        case Input::Key::Alpha0:
            return SC_KB_CODES::KB_0;
        case Input::Key::Alpha1:
            return SC_KB_CODES::KB_1;
        case Input::Key::Alpha2:
            return SC_KB_CODES::KB_2;
        case Input::Key::Alpha3:
            return SC_KB_CODES::KB_3;
        case Input::Key::Alpha4:
            return SC_KB_CODES::KB_4;
        case Input::Key::Alpha5:
            return SC_KB_CODES::KB_5;
        case Input::Key::Alpha6:
            return SC_KB_CODES::KB_6;
        case Input::Key::Alpha7:
            return SC_KB_CODES::KB_7;
        case Input::Key::Alpha8:
            return SC_KB_CODES::KB_8;
        case Input::Key::Alpha9:
            return SC_KB_CODES::KB_9;
        case Input::Key::Semicolon:
            return SC_KB_CODES::KB_SEMICOLON;
        case Input::Key::Equal:
            return SC_KB_CODES::KB_EQUAL;
        case Input::Key::A:
            return SC_KB_CODES::KB_A;
        case Input::Key::B:
            return SC_KB_CODES::KB_B;
        case Input::Key::C:
            return SC_KB_CODES::KB_C;
        case Input::Key::D:
            return SC_KB_CODES::KB_D;
        case Input::Key::E:
            return SC_KB_CODES::KB_E;
        case Input::Key::F:
            return SC_KB_CODES::KB_F;
        case Input::Key::G:
            return SC_KB_CODES::KB_G;
        case Input::Key::H:
            return SC_KB_CODES::KB_H;
        case Input::Key::I:
            return SC_KB_CODES::KB_I;
        case Input::Key::J:
            return SC_KB_CODES::KB_J;
        case Input::Key::K:
            return SC_KB_CODES::KB_K;
        case Input::Key::L:
            return SC_KB_CODES::KB_L;
        case Input::Key::M:
            return SC_KB_CODES::KB_M;
        case Input::Key::N:
            return SC_KB_CODES::KB_N;
        case Input::Key::O:
            return SC_KB_CODES::KB_O;
        case Input::Key::P:
            return SC_KB_CODES::KB_P;
        case Input::Key::Q:
            return SC_KB_CODES::KB_Q;
        case Input::Key::R:
            return SC_KB_CODES::KB_R;
        case Input::Key::S:
            return SC_KB_CODES::KB_S;
        case Input::Key::T:
            return SC_KB_CODES::KB_T;
        case Input::Key::U:
            return SC_KB_CODES::KB_U;
        case Input::Key::V:
            return SC_KB_CODES::KB_V;
        case Input::Key::W:
            return SC_KB_CODES::KB_W;
        case Input::Key::X:
            return SC_KB_CODES::KB_X;
        case Input::Key::Y:
            return SC_KB_CODES::KB_Y;
        case Input::Key::Z:
            return SC_KB_CODES::KB_Z;
        case Input::Key::Backslash:
            return SC_KB_CODES::KB_BACKSLASH;
        case Input::Key::Grave:
            return SC_KB_CODES::KB_GRAVE_ACCENT;
        case Input::Key::Escape:
            return SC_KB_CODES::KB_ESCAPE;
        case Input::Key::Enter:
            return SC_KB_CODES::KB_ENTER;
        case Input::Key::Tab:
            return SC_KB_CODES::KB_TAB;
        case Input::Key::Backspace:
            return SC_KB_CODES::KB_BACKSPACE;
        case Input::Key::Insert:
            return SC_KB_CODES::KB_INSERT;
        case Input::Key::Delete:
            return SC_KB_CODES::KB_DELETE;
        case Input::Key::Right:
            return SC_KB_CODES::KB_RIGHT;
        case Input::Key::Left:
            return SC_KB_CODES::KB_LEFT;
        case Input::Key::Down:
            return SC_KB_CODES::KB_DOWN;
        case Input::Key::Up:
            return SC_KB_CODES::KB_UP;
        case Input::Key::PageUp:
            return SC_KB_CODES::KB_PAGE_UP;
        case Input::Key::PageDown:
            return SC_KB_CODES::KB_PAGE_DOWN;
        case Input::Key::Home:
            return SC_KB_CODES::KB_HOME;
        case Input::Key::End:
            return SC_KB_CODES::KB_END;
        case Input::Key::Capital:
            return SC_KB_CODES::KB_CAPS_LOCK;
        case Input::Key::Scroll:
            return SC_KB_CODES::KB_SCROLL_LOCK;
        case Input::Key::Lock:
            return SC_KB_CODES::KB_NUM_LOCK;
        case Input::Key::Print:
            return SC_KB_CODES::KB_PRINT_SCREEN;
        case Input::Key::Pause:
            return SC_KB_CODES::KB_PAUSE;
        case Input::Key::F1:
            return SC_KB_CODES::KB_F1;
        case Input::Key::F2:
            return SC_KB_CODES::KB_F2;
        case Input::Key::F3:
            return SC_KB_CODES::KB_F3;
        case Input::Key::F4:
            return SC_KB_CODES::KB_F4;
        case Input::Key::F5:
            return SC_KB_CODES::KB_F5;
        case Input::Key::F6:
            return SC_KB_CODES::KB_F6;
        case Input::Key::F7:
            return SC_KB_CODES::KB_F7;
        case Input::Key::F8:
            return SC_KB_CODES::KB_F8;
        case Input::Key::F9:
            return SC_KB_CODES::KB_F9;
        case Input::Key::F10:
            return SC_KB_CODES::KB_F10;
        case Input::Key::F11:
            return SC_KB_CODES::KB_F11;
        case Input::Key::F12:
            return SC_KB_CODES::KB_F12;
        case Input::Key::Keypad0:
            return SC_KB_CODES::KB_KP_0;
        case Input::Key::Keypad1:
            return SC_KB_CODES::KB_KP_1;
        case Input::Key::Keypad2:
            return SC_KB_CODES::KB_KP_2;
        case Input::Key::Keypad3:
            return SC_KB_CODES::KB_KP_3;
        case Input::Key::Keypad4:
            return SC_KB_CODES::KB_KP_4;
        case Input::Key::Keypad5:
            return SC_KB_CODES::KB_KP_5;
        case Input::Key::Keypad6:
            return SC_KB_CODES::KB_KP_6;
        case Input::Key::Keypad7:
            return SC_KB_CODES::KB_KP_7;
        case Input::Key::Keypad8:
            return SC_KB_CODES::KB_KP_8;
        case Input::Key::Keypad9:
            return SC_KB_CODES::KB_KP_9;
        case Input::Key::Decimal:
            return SC_KB_CODES::KB_KP_DECIMAL;
        case Input::Key::Divide:
            return SC_KB_CODES::KB_KP_DIVIDE;
        case Input::Key::Multiply:
            return SC_KB_CODES::KB_KP_MULTIPLY;
        case Input::Key::Subtract:
            return SC_KB_CODES::KB_KP_SUBTRACT;
        case Input::Key::Add:
            return SC_KB_CODES::KB_KP_ADD;
        case Input::Key::LeftBracket:
            return SC_KB_CODES::KB_LEFT_BRACKET;
        case Input::Key::LeftShift:
            return SC_KB_CODES::KB_LEFT_SHIFT;
        case Input::Key::LeftCtrl:
            return SC_KB_CODES::KB_LEFT_CONTROL;
        case Input::Key::LeftAlt:
            return SC_KB_CODES::KB_LEFT_ALT;
        case Input::Key::LeftSuper:
            return SC_KB_CODES::KB_LEFT_SUPER;
        case Input::Key::Menu:
            return SC_KB_CODES::KB_MENU;
        case Input::Key::RightBracket:
            return SC_KB_CODES::KB_RIGHT_BRACKET;
        case Input::Key::RightShift:
            return SC_KB_CODES::KB_RIGHT_SHIFT;
        case Input::Key::RightCtrl:
            return SC_KB_CODES::KB_RIGHT_CONTROL;
        case Input::Key::RightAlt:
            return SC_KB_CODES::KB_RIGHT_ALT;
        case Input::Key::RightSuper:
            return SC_KB_CODES::KB_RIGHT_SUPER;
        case Input::Key::Unknown:
            break;
        }
        return SC_KB_CODES::KB_UNKNOWN;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetButton(Input::Button Button)
    {
        switch (Button)
        {
        case Input::Button::Left:
            return MOUSE_BUTTONS::MAIN_MOUSE_BUTTON;
        case Input::Button::Right:
            return MOUSE_BUTTONS::PROP_MOUSE_BUTTON;
        case Input::Button::Middle:
            return MOUSE_BUTTONS::MIDDLE_MOUSE_BUTTON;
        case Input::Button::Back:
        case Input::Button::Forward:
        case Input::Button::Unknown:
            break;
        }
        return MOUSE_BUTTONS::MAIN_MOUSE_BUTTON;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnEvent(Ref<const Input::Event> Event)
    {
        if (Event.Type == Input::Event::Type::MouseMove)       // Handle reset double click event
        {
            mMouseClickButton = Input::Button::Unknown;
        }
        else if (Event.Type == Input::Event::Type::MouseUp)    // Handle double click event
        {
            if (mMouseClickButton == Event.MouseAction.Button && Event.Time - mMouseClickTime <= 0.5)
            {
                OnMouseDoubleClick(Event.MouseAction.Button);
                mMouseClickButton = Input::Button::Unknown;
            }
            else
            {
                mMouseClickTime   = Event.Time;
                mMouseClickButton = Event.MouseAction.Button;
            }
        }
        return Input::Listener::OnEvent(Event);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnKeyType(UInt Codepoint)
    {
        return SciterProcX(mHandle, SCITER_X_MSG_KEY(KEY_CHAR, Codepoint, KEYBOARD_STATES(mStates)));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnKeyDown(Input::Key Key)
    {
        if (Key == Input::Key::LeftShift)
        {
            mStates |= SHIFT_KEY_PRESSED;
        }
        else if (Key == Input::Key::RightShift)
        {
            mStates |= RIGHT_SHIFT_KEY_PRESSED;
        }
        else if (Key == Input::Key::LeftCtrl || Key == Input::Key::RightCtrl)
        {
            mStates |= CONTROL_KEY_PRESSED;
        }
        else if (Key == Input::Key::LeftAlt || Key == Input::Key::RightAlt)
        {
            mStates |= ALT_KEY_PRESSED;
        }

        if (SciterProcX(mHandle, SCITER_X_MSG_KEY(KEY_DOWN, GetKey(Key), KEYBOARD_STATES(mStates))))
        {
            sciter::dom::element Element(sciter::dom::element::focus_element(NULL));
            return Element.root() != Element.operator HELEMENT();
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnKeyUp(Input::Key Key)
    {
        if (Key == Input::Key::LeftShift)
        {
            mStates &= ~SHIFT_KEY_PRESSED;
        }
        else if (Key == Input::Key::RightShift)
        {
            mStates &= ~RIGHT_SHIFT_KEY_PRESSED;
        }
        else if (Key == Input::Key::LeftCtrl || Key == Input::Key::RightCtrl)
        {
            mStates &= ~CONTROL_KEY_PRESSED;
        }
        else if (Key == Input::Key::LeftAlt || Key == Input::Key::RightAlt)
        {
            mStates &= ~ALT_KEY_PRESSED;
        }

        if (SciterProcX(mHandle, SCITER_X_MSG_KEY(KEY_UP, GetKey(Key), KEYBOARD_STATES(mStates))))
        {
            sciter::dom::element Element(sciter::dom::element::focus_element(NULL));
            return Element.root() != Element.operator HELEMENT();
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnMouseMove(UInt X, UInt Y)
    {
        mMousePosition.x = X;
        mMousePosition.y = Y;
        return SciterProcX(
            mHandle, SCITER_X_MSG_MOUSE(MOUSE_MOVE, mMouseButtons, KEYBOARD_STATES(mStates), mMousePosition));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnMouseScroll(SInt X, SInt Y)
    {
        const UINT Deltas = static_cast<UInt16>(X * 120) << 16 | static_cast<UInt16>(Y * 120);
        return SciterProcX(
            mHandle, SCITER_X_MSG_MOUSE(MOUSE_WHEEL, MOUSE_BUTTONS(Deltas), KEYBOARD_STATES(mStates), mMousePosition));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnMouseUp(Input::Button Button)
    {
        mMouseButtons = MOUSE_BUTTONS(0);
        return SciterProcX(
            mHandle, SCITER_X_MSG_MOUSE(MOUSE_UP, GetButton(Button), KEYBOARD_STATES(mStates), mMousePosition));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnMouseDown(Input::Button Button)
    {
        mMouseButtons = GetButton(Button);
        return SciterProcX(
            mHandle, SCITER_X_MSG_MOUSE(MOUSE_DOWN, mMouseButtons, KEYBOARD_STATES(mStates), mMousePosition));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnWindowFocus(Bool Focused)
    {
        SciterProcX(mHandle, SCITER_X_MSG_FOCUS(Focused));
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnWindowResize(SInt Width, SInt Height)
    {
        SciterProcX(mHandle, SCITER_X_MSG_SIZE(Width, Height));

        mCbDocumentResize(Width, Height);

        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterInput::OnMouseDoubleClick(Input::Button Button)
    {
        return SciterProcX(
            mHandle, SCITER_X_MSG_MOUSE(MOUSE_DCLICK, mMouseButtons, KEYBOARD_STATES(mStates), mMousePosition));
    }
}