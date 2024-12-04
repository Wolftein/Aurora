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

#include "Aurora.Base/Trait.hpp"
#include "Aurora.Base/Type.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=(Undocumented)=-
    enum class Action
    {
        Pressed,
        Held,
        Released,
    };

    // -=(Undocumented)=-
    enum class Button
    {
        Unknown,
        Left,
        Middle,
        Right,
        Back,
        Forward,
    };

    // -=(Undocumented)=-
    enum class Key
    {
        Unknown,
        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        Alpha0,
        Alpha1,
        Alpha2,
        Alpha3,
        Alpha4,
        Alpha5,
        Alpha6,
        Alpha7,
        Alpha8,
        Alpha9,
        Semicolon,
        Equal,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Backslash,
        Grave,
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        Capital,
        Scroll,
        Lock,
        Print,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        Decimal,
        Divide,
        Multiply,
        Subtract,
        Add,
        LeftBracket,
        LeftShift,
        LeftCtrl,
        LeftAlt,
        LeftSuper,
        Menu,
        RightBracket,
        RightShift,
        RightCtrl,
        RightAlt,
        RightSuper,
    };

    // -=(Undocumented)=-
    struct Event
    {
        // -=(Undocumented)=-
        enum class Type
        {
            Unknown,
            KeyType,
            KeyUp,
            KeyDown,
            MouseMove,
            MouseScroll,
            MouseUp,
            MouseDown,
            WindowFocus,
            WindowResize,
            WindowExit,
        };

        // -=(Undocumented)=-
        Real64 Time;

        // -=(Undocumented)=-
        Type   Type;

        // -=(Undocumented)=-
        union
        {
            struct
            {
                UInt32 Codepoint;
            } KeyType;

            struct
            {
                Key Key;
            } KeyAction;

            struct
            {
                Real32 X;
                Real32 Y;
                Real32 XDelta;
                Real32 YDelta;
            } MouseAxis;

            struct
            {
                Button Button;
            } MouseAction;

            struct
            {
                Bool   State;
            } WindowFocus;

            struct
            {
                UInt32 Width;
                UInt32 Height;
            } WindowResize;
        };
    };
}