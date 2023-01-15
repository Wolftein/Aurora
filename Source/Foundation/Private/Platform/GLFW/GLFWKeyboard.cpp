// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "GLFWKeyboard.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Platform
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWKeyboard::OnKeyType(UInt Codepoint)
    {
        const Input::Event Event {
            .Type = Input::Event::Type::KeyType, .KeyType = { Codepoint }
        };
        Invoke(Event);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void GLFWKeyboard::OnKeyAction(SInt Key, SInt Scancode, SInt Action, SInt Mods)
    {
        const auto Type = (Action == GLFW_RELEASE ? Input::Event::Type::KeyUp : Input::Event::Type::KeyDown);

        const Input::Event Event {
            .Type = Type, .KeyAction = { Translate(Key) }
        };
        Invoke(Event);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Input::Key GLFWKeyboard::Translate(SInt Key) const
    {
        switch (Key)
        {
        case GLFW_KEY_SPACE:
            return Input::Key::Space;
        case GLFW_KEY_APOSTROPHE:
            return Input::Key::Apostrophe;
        case GLFW_KEY_COMMA:
            return Input::Key::Comma;
        case GLFW_KEY_MINUS:
            return Input::Key::Minus;
        case GLFW_KEY_PERIOD:
            return Input::Key::Period;
        case GLFW_KEY_SLASH:
            return Input::Key::Slash;
        case GLFW_KEY_0:
            return Input::Key::Alpha0;
        case GLFW_KEY_1:
            return Input::Key::Alpha1;
        case GLFW_KEY_2:
            return Input::Key::Alpha2;
        case GLFW_KEY_3:
            return Input::Key::Alpha3;
        case GLFW_KEY_4:
            return Input::Key::Alpha4;
        case GLFW_KEY_5:
            return Input::Key::Alpha5;
        case GLFW_KEY_6:
            return Input::Key::Alpha6;
        case GLFW_KEY_7:
            return Input::Key::Alpha7;
        case GLFW_KEY_8:
            return Input::Key::Alpha8;
        case GLFW_KEY_9:
            return Input::Key::Alpha9;
        case GLFW_KEY_SEMICOLON:
            return Input::Key::Semicolon;
        case GLFW_KEY_EQUAL:
            return Input::Key::Equal;
        case GLFW_KEY_A:
            return Input::Key::A;
        case GLFW_KEY_B:
            return Input::Key::B;
        case GLFW_KEY_C:
            return Input::Key::C;
        case GLFW_KEY_D:
            return Input::Key::D;
        case GLFW_KEY_E:
            return Input::Key::E;
        case GLFW_KEY_F:
            return Input::Key::F;
        case GLFW_KEY_G:
            return Input::Key::G;
        case GLFW_KEY_H:
            return Input::Key::H;
        case GLFW_KEY_I:
            return Input::Key::I;
        case GLFW_KEY_J:
            return Input::Key::J;
        case GLFW_KEY_K:
            return Input::Key::K;
        case GLFW_KEY_L:
            return Input::Key::L;
        case GLFW_KEY_M:
            return Input::Key::M;
        case GLFW_KEY_N:
            return Input::Key::N;
        case GLFW_KEY_O:
            return Input::Key::O;
        case GLFW_KEY_P:
            return Input::Key::P;
        case GLFW_KEY_Q:
            return Input::Key::Q;
        case GLFW_KEY_R:
            return Input::Key::R;
        case GLFW_KEY_S:
            return Input::Key::S;
        case GLFW_KEY_T:
            return Input::Key::T;
        case GLFW_KEY_U:
            return Input::Key::U;
        case GLFW_KEY_V:
            return Input::Key::V;
        case GLFW_KEY_W:
            return Input::Key::W;
        case GLFW_KEY_X:
            return Input::Key::X;
        case GLFW_KEY_Y:
            return Input::Key::Y;
        case GLFW_KEY_Z:
            return Input::Key::Z;
        case GLFW_KEY_LEFT_BRACKET:
            return Input::Key::LeftBracket;
        case GLFW_KEY_BACKSLASH:
            return Input::Key::Backslash;
        case GLFW_KEY_RIGHT_BRACKET:
            return Input::Key::RightBracket;
        case GLFW_KEY_GRAVE_ACCENT:
            return Input::Key::Grave;
        case GLFW_KEY_WORLD_1:
            return Input::Key::Unknown;
        case GLFW_KEY_WORLD_2:
            return Input::Key::Unknown;
        case GLFW_KEY_ESCAPE:
            return Input::Key::Escape;
        case GLFW_KEY_ENTER:
            return Input::Key::Enter;
        case GLFW_KEY_TAB:
            return Input::Key::Tab;
        case GLFW_KEY_BACKSPACE:
            return Input::Key::Backspace;
        case GLFW_KEY_INSERT:
            return Input::Key::Insert;
        case GLFW_KEY_DELETE:
            return Input::Key::Delete;
        case GLFW_KEY_RIGHT:
            return Input::Key::Right;
        case GLFW_KEY_LEFT:
            return Input::Key::Left;
        case GLFW_KEY_DOWN:
            return Input::Key::Down;
        case GLFW_KEY_UP:
            return Input::Key::Up;
        case GLFW_KEY_PAGE_UP:
            return Input::Key::PageUp;
        case GLFW_KEY_PAGE_DOWN:
            return Input::Key::PageDown;
        case GLFW_KEY_HOME:
            return Input::Key::Home;
        case GLFW_KEY_END:
            return Input::Key::End;
        case GLFW_KEY_CAPS_LOCK:
            return Input::Key::Capital;
        case GLFW_KEY_SCROLL_LOCK:
            return Input::Key::Scroll;
        case GLFW_KEY_NUM_LOCK:
            return Input::Key::Lock;
        case GLFW_KEY_PRINT_SCREEN:
            return Input::Key::Print;
        case GLFW_KEY_PAUSE:
            return Input::Key::Pause;
        case GLFW_KEY_F1:
            return Input::Key::F1;
        case GLFW_KEY_F2:
            return Input::Key::F2;
        case GLFW_KEY_F3:
            return Input::Key::F3;
        case GLFW_KEY_F4:
            return Input::Key::F4;
        case GLFW_KEY_F5:
            return Input::Key::F5;
        case GLFW_KEY_F6:
            return Input::Key::F6;
        case GLFW_KEY_F7:
            return Input::Key::F7;
        case GLFW_KEY_F8:
            return Input::Key::F8;
        case GLFW_KEY_F9:
            return Input::Key::F9;
        case GLFW_KEY_F10:
            return Input::Key::F10;
        case GLFW_KEY_F11:
            return Input::Key::F11;
        case GLFW_KEY_F12:
            return Input::Key::F12;
        case GLFW_KEY_F13:
            return Input::Key::Unknown;
        case GLFW_KEY_F14:
            return Input::Key::Unknown;
        case GLFW_KEY_F15:
            return Input::Key::Unknown;
        case GLFW_KEY_F16:
            return Input::Key::Unknown;
        case GLFW_KEY_F17:
            return Input::Key::Unknown;
        case GLFW_KEY_F18:
            return Input::Key::Unknown;
        case GLFW_KEY_F19:
            return Input::Key::Unknown;
        case GLFW_KEY_F20:
            return Input::Key::Unknown;
        case GLFW_KEY_F21:
            return Input::Key::Unknown;
        case GLFW_KEY_F22:
            return Input::Key::Unknown;
        case GLFW_KEY_F23:
            return Input::Key::Unknown;
        case GLFW_KEY_F24:
            return Input::Key::Unknown;
        case GLFW_KEY_F25:
            return Input::Key::Unknown;
        case GLFW_KEY_KP_0:
            return Input::Key::Keypad0;
        case GLFW_KEY_KP_1:
            return Input::Key::Keypad1;
        case GLFW_KEY_KP_2:
            return Input::Key::Keypad2;
        case GLFW_KEY_KP_3:
            return Input::Key::Keypad3;
        case GLFW_KEY_KP_4:
            return Input::Key::Keypad4;
        case GLFW_KEY_KP_5:
            return Input::Key::Keypad5;
        case GLFW_KEY_KP_6:
            return Input::Key::Keypad6;
        case GLFW_KEY_KP_7:
            return Input::Key::Keypad7;
        case GLFW_KEY_KP_8:
            return Input::Key::Keypad8;
        case GLFW_KEY_KP_9:
            return Input::Key::Keypad9;
        case GLFW_KEY_KP_DECIMAL:
            return Input::Key::Decimal;
        case GLFW_KEY_KP_DIVIDE:
            return Input::Key::Divide;
        case GLFW_KEY_KP_MULTIPLY:
            return Input::Key::Multiply;
        case GLFW_KEY_KP_SUBTRACT:
            return Input::Key::Subtract;
        case GLFW_KEY_KP_ADD:
            return Input::Key::Add;
        case GLFW_KEY_KP_ENTER:
            return Input::Key::Enter;
        case GLFW_KEY_KP_EQUAL:
            return Input::Key::Equal;
        case GLFW_KEY_LEFT_SHIFT:
            return Input::Key::LeftShift;
        case GLFW_KEY_LEFT_CONTROL:
            return Input::Key::LeftCtrl;
        case GLFW_KEY_LEFT_ALT:
            return Input::Key::LeftAlt;
        case GLFW_KEY_LEFT_SUPER:
            return Input::Key::LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT:
            return Input::Key::RightShift;
        case GLFW_KEY_RIGHT_CONTROL:
            return Input::Key::RightCtrl;
        case GLFW_KEY_RIGHT_ALT:
            return Input::Key::RightAlt;
        case GLFW_KEY_RIGHT_SUPER:
            return Input::Key::RightSuper;
        case GLFW_KEY_MENU:
            return Input::Key::Menu;
        }
        return Input::Key::Unknown;
    }
}