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

#include "Poller.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetKey(UInt32 Value)
    {
        switch (Value)
        {
        case SDLK_SPACE:
            return Key::Space;
        case SDLK_APOSTROPHE:
            return Key::Apostrophe;
        case SDLK_COMMA:
            return Key::Comma;
        case SDLK_MINUS:
            return Key::Minus;
        case SDLK_PERIOD:
            return Key::Period;
        case SDLK_SLASH:
            return Key::Slash;
        case SDLK_0:
            return Key::Alpha0;
        case SDLK_1:
            return Key::Alpha1;
        case SDLK_2:
            return Key::Alpha2;
        case SDLK_3:
            return Key::Alpha3;
        case SDLK_4:
            return Key::Alpha4;
        case SDLK_5:
            return Key::Alpha5;
        case SDLK_6:
            return Key::Alpha6;
        case SDLK_7:
            return Key::Alpha7;
        case SDLK_8:
            return Key::Alpha8;
        case SDLK_9:
            return Key::Alpha9;
        case SDLK_SEMICOLON:
            return Key::Semicolon;
        case SDLK_EQUALS:
            return Key::Equal;
        case SDLK_A:
            return Key::A;
        case SDLK_B:
            return Key::B;
        case SDLK_C:
            return Key::C;
        case SDLK_D:
            return Key::D;
        case SDLK_E:
            return Key::E;
        case SDLK_F:
            return Key::F;
        case SDLK_G:
            return Key::G;
        case SDLK_H:
            return Key::H;
        case SDLK_I:
            return Key::I;
        case SDLK_J:
            return Key::J;
        case SDLK_K:
            return Key::K;
        case SDLK_L:
            return Key::L;
        case SDLK_M:
            return Key::M;
        case SDLK_N:
            return Key::N;
        case SDLK_O:
            return Key::O;
        case SDLK_P:
            return Key::P;
        case SDLK_Q:
            return Key::Q;
        case SDLK_R:
            return Key::R;
        case SDLK_S:
            return Key::S;
        case SDLK_T:
            return Key::T;
        case SDLK_U:
            return Key::U;
        case SDLK_V:
            return Key::V;
        case SDLK_W:
            return Key::W;
        case SDLK_X:
            return Key::X;
        case SDLK_Y:
            return Key::Y;
        case SDLK_Z:
            return Key::Z;
        case SDLK_LEFTBRACKET:
            return Key::LeftBracket;
        case SDLK_BACKSLASH:
            return Key::Backslash;
        case SDLK_RIGHTBRACKET:
            return Key::RightBracket;
        case SDLK_GRAVE:
            return Key::Grave;
        case SDLK_ESCAPE:
            return Key::Escape;
        case SDLK_RETURN:
            return Key::Enter;
        case SDLK_TAB:
            return Key::Tab;
        case SDLK_BACKSPACE:
            return Key::Backspace;
        case SDLK_INSERT:
            return Key::Insert;
        case SDLK_DELETE:
            return Key::Delete;
        case SDLK_RIGHT:
            return Key::Right;
        case SDLK_LEFT:
            return Key::Left;
        case SDLK_DOWN:
            return Key::Down;
        case SDLK_UP:
            return Key::Up;
        case SDLK_PAGEUP:
            return Key::PageUp;
        case SDLK_PAGEDOWN:
            return Key::PageDown;
        case SDLK_HOME:
            return Key::Home;
        case SDLK_END:
            return Key::End;
        case SDLK_CAPSLOCK:
            return Key::Capital;
        case SDLK_SCROLLLOCK:
            return Key::Scroll;
        case SDLK_NUMLOCKCLEAR:
            return Key::Lock;
        case SDLK_PRINTSCREEN:
            return Key::Print;
        case SDLK_PAUSE:
            return Key::Pause;
        case SDLK_F1:
            return Key::F1;
        case SDLK_F2:
            return Key::F2;
        case SDLK_F3:
            return Key::F3;
        case SDLK_F4:
            return Key::F4;
        case SDLK_F5:
            return Key::F5;
        case SDLK_F6:
            return Key::F6;
        case SDLK_F7:
            return Key::F7;
        case SDLK_F8:
            return Key::F8;
        case SDLK_F9:
            return Key::F9;
        case SDLK_F10:
            return Key::F10;
        case SDLK_F11:
            return Key::F11;
        case SDLK_F12:
            return Key::F12;
        case SDLK_F13:
        case SDLK_F14:
        case SDLK_F15:
        case SDLK_F16:
        case SDLK_F17:
        case SDLK_F18:
        case SDLK_F19:
        case SDLK_F20:
        case SDLK_F21:
        case SDLK_F22:
        case SDLK_F23:
        case SDLK_F24:
            return Key::Unknown;
        case SDLK_KP_0:
            return Key::Keypad0;
        case SDLK_KP_1:
            return Key::Keypad1;
        case SDLK_KP_2:
            return Key::Keypad2;
        case SDLK_KP_3:
            return Key::Keypad3;
        case SDLK_KP_4:
            return Key::Keypad4;
        case SDLK_KP_5:
            return Key::Keypad5;
        case SDLK_KP_6:
            return Key::Keypad6;
        case SDLK_KP_7:
            return Key::Keypad7;
        case SDLK_KP_8:
            return Key::Keypad8;
        case SDLK_KP_9:
            return Key::Keypad9;
        case SDLK_KP_DECIMAL:
            return Key::Decimal;
        case SDLK_KP_DIVIDE:
            return Key::Divide;
        case SDLK_KP_MULTIPLY:
            return Key::Multiply;
        case SDLK_KP_MINUS:
            return Key::Subtract;
        case SDLK_KP_PLUS:
            return Key::Add;
        case SDLK_KP_ENTER:
            return Key::Enter;
        case SDLK_KP_EQUALS:
            return Key::Equal;
        case SDLK_LSHIFT:
            return Key::LeftShift;
        case SDLK_LCTRL:
            return Key::LeftCtrl;
        case SDLK_LALT:
            return Key::LeftAlt;
        case SDLK_RSHIFT:
            return Key::RightShift;
        case SDLK_RCTRL:
            return Key::RightCtrl;
        case SDLK_RALT:
            return Key::RightAlt;
        case SDLK_MENU:
            return Key::Menu;
        default:
            return Key::Unknown;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetButton(UInt32 Value)
    {
        switch (Value)
        {
        case SDL_BUTTON_LEFT:
            return Button::Left;
        case SDL_BUTTON_MIDDLE:
            return Button::Middle;
        case SDL_BUTTON_RIGHT:
            return Button::Right;
        case SDL_BUTTON_X1:
            return Button::Back;
        case SDL_BUTTON_X2:
            return Button::Forward;
        default:
                return Button::Unknown;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Poller::Poller()
        : mBuffer { }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    CPtr<const Event> Poller::Poll()
    {
        // Pump all event(s) from devices
        SDL_PumpEvents();

        // Grab all event(s) from SDL at once
        Array<SDL_Event, k_MaxEventsPerFrame> Stack;
        const SInt32 Count = SDL_PeepEvents(Stack.data(), Stack.size(), SDL_GETEVENT, SDL_EVENT_FIRST, SDL_EVENT_LAST);

        // Transform SDL event(s) to Aurora event(s)
        UInt32 Element = 0;

        for (Ref<const SDL_Event> Event : CPtr<const SDL_Event>(Stack.data(), Count))
        {
            Ref<Input::Event> Result = mBuffer[Element++];
            Result.Time = static_cast<Real64>(Event.common.timestamp) / SDL_NS_PER_SECOND;

            switch (Event.type)
            {
            case SDL_EVENT_WINDOW_RESIZED:
                Result.Type = Event::Type::WindowResize;
                Result.WindowResize.Width  = Event.window.data1;
                Result.WindowResize.Height = Event.window.data2;
                break;
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                Result.Type                = Event::Type::WindowFocus;
                Result.WindowFocus.State   = true;
                break;
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                Result.Type                = Event::Type::WindowFocus;
                Result.WindowFocus.State   = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                Result.Type                = Event::Type::WindowExit;
                break;
            case SDL_EVENT_KEY_DOWN:
                Result.Type                = Event::Type::KeyDown;
                Result.KeyAction.Key       = GetKey(Event.key.key);
                break;
            case SDL_EVENT_KEY_UP:
                Result.Type                = Event::Type::KeyUp;
                Result.KeyAction.Key       = GetKey(Event.key.key);
                break;
            case SDL_EVENT_TEXT_INPUT:
                Result.Type                = Event::Type::KeyType;
                Result.KeyType.Codepoint   = Event.text.text[0]; // @TODO: Android support
                break;
            case SDL_EVENT_MOUSE_MOTION:
                Result.Type                = Event::Type::MouseMove;
                Result.MouseAxis.X         = static_cast<SInt32>(Event.motion.x);
                Result.MouseAxis.Y         = static_cast<SInt32>(Event.motion.y);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                Result.Type                = Event::Type::MouseDown;
                Result.MouseAction.Button  = GetButton(Event.button.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                Result.Type                = Event::Type::MouseUp;
                Result.MouseAction.Button  = GetButton(Event.button.button);
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                Result.Type                = Event::Type::MouseScroll;
                Result.MouseAxis.X         = static_cast<SInt32>(Event.wheel.x);
                Result.MouseAxis.Y         = static_cast<SInt32>(Event.wheel.y);
                break;
            default:
                Result.Type                = Event::Type::Unknown;
                break;
            }
        }
        return CPtr<const Event>(mBuffer.data(), Count);
    }
}