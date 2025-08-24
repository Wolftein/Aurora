// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Base.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    /// \brief Constants used across the Input module.
    enum
    {
        /// \brief Maximum number of input listeners supported at once.
        kMaxListeners = 16
    };

    /// \brief Enumerates possible key or button actions.
    enum class Action : UInt8
    {
        Pressed,  ///< Key or button was pressed.
        Held,     ///< Key or button is being held down.
        Released, ///< Key or button was released.
    };

    /// \brief Enumerates mouse buttons.
    enum class Button : UInt8
    {
        Unknown, ///< Unknown or unsupported mouse button.
        Left,    ///< Left mouse button.
        Middle,  ///< Middle mouse button.
        Right,   ///< Right mouse button.
        Back,    ///< Back navigation button on mouse.
        Forward, ///< Forward navigation button on mouse.
    };

    /// \brief Enumerates physical key codes.
    enum class Key : UInt8
    {
        Unknown,        ///< Unknown or unmapped key.
        Space,          ///< Spacebar.
        Apostrophe,     ///< Apostrophe (') key.
        Comma,          ///< Comma (,) key.
        Minus,          ///< Minus (-) key.
        Period,         ///< Period (.) key.
        Slash,          ///< Forward slash (/) key.
        Alpha0,         ///< Number key 0.
        Alpha1,         ///< Number key 1.
        Alpha2,         ///< Number key 2.
        Alpha3,         ///< Number key 3.
        Alpha4,         ///< Number key 4.
        Alpha5,         ///< Number key 5.
        Alpha6,         ///< Number key 6.
        Alpha7,         ///< Number key 7.
        Alpha8,         ///< Number key 8.
        Alpha9,         ///< Number key 9.
        Semicolon,      ///< Semicolon (;) key.
        Equal,          ///< Equal sign (=) key.
        A,              ///< Key A.
        B,              ///< Key B.
        C,              ///< Key C.
        D,              ///< Key D.
        E,              ///< Key E.
        F,              ///< Key F.
        G,              ///< Key G.
        H,              ///< Key H.
        I,              ///< Key I.
        J,              ///< Key J.
        K,              ///< Key K.
        L,              ///< Key L.
        M,              ///< Key M.
        N,              ///< Key N.
        O,              ///< Key O.
        P,              ///< Key P.
        Q,              ///< Key Q.
        R,              ///< Key R.
        S,              ///< Key S.
        T,              ///< Key T.
        U,              ///< Key U.
        V,              ///< Key V.
        W,              ///< Key W.
        X,              ///< Key X.
        Y,              ///< Key Y.
        Z,              ///< Key Z.
        Backslash,      ///< Backslash (\) key.
        Grave,          ///< Grave accent (`) key.
        Escape,         ///< Escape key.
        Enter,          ///< Enter/Return key.
        Tab,            ///< Tab key.
        Backspace,      ///< Backspace key.
        Insert,         ///< Insert key.
        Delete,         ///< Delete key.
        Right,          ///< Right arrow key.
        Left,           ///< Left arrow key.
        Down,           ///< Down arrow key.
        Up,             ///< Up arrow key.
        PageUp,         ///< Page Up key.
        PageDown,       ///< Page Down key.
        Home,           ///< Home key.
        End,            ///< End key.
        Capital,        ///< Caps Lock key.
        Scroll,         ///< Scroll Lock key.
        Lock,           ///< Generic lock key.
        Print,          ///< Print Screen key.
        Pause,          ///< Pause/Break key.
        F1,             ///< F1 function key.
        F2,             ///< F2 function key.
        F3,             ///< F3 function key.
        F4,             ///< F4 function key.
        F5,             ///< F5 function key.
        F6,             ///< F6 function key.
        F7,             ///< F7 function key.
        F8,             ///< F8 function key.
        F9,             ///< F9 function key.
        F10,            ///< F10 function key.
        F11,            ///< F11 function key.
        F12,            ///< F12 function key.
        Keypad0,        ///< Keypad number 0.
        Keypad1,        ///< Keypad number 1.
        Keypad2,        ///< Keypad number 2.
        Keypad3,        ///< Keypad number 3.
        Keypad4,        ///< Keypad number 4.
        Keypad5,        ///< Keypad number 5.
        Keypad6,        ///< Keypad number 6.
        Keypad7,        ///< Keypad number 7.
        Keypad8,        ///< Keypad number 8.
        Keypad9,        ///< Keypad number 9.
        Decimal,        ///< Numpad decimal point.
        Divide,         ///< Numpad division.
        Multiply,       ///< Numpad multiplication.
        Subtract,       ///< Numpad subtraction.
        Add,            ///< Numpad addition.
        LeftBracket,    ///< Left bracket ([) key.
        LeftShift,      ///< Left Shift key.
        LeftCtrl,       ///< Left Control key.
        LeftAlt,        ///< Left Alt key.
        LeftSuper,      ///< Left Super key (Windows/Command).
        Menu,           ///< Menu/Context key.
        RightBracket,   ///< Right bracket (]) key.
        RightShift,     ///< Right Shift key.
        RightCtrl,      ///< Right Control key.
        RightAlt,       ///< Right Alt key.
        RightSuper,     ///< Right Super key (Windows/Command).
    };

    /// \brief Represents a single input event and its data.
    struct Event final
    {
        /// \brief Enumerates the type of input event.
        enum class Type : UInt8
        {
            Unknown,      ///< Unknown or unsupported event.
            KeyType,      ///< Character input event (text typing).
            KeyUp,        ///< Key release event.
            KeyDown,      ///< Key press event.
            MouseMove,    ///< Mouse movement event.
            MouseScroll,  ///< Mouse scroll wheel event.
            MouseUp,      ///< Mouse button release event.
            MouseDown,    ///< Mouse button press event.
            WindowFocus,  ///< Window focus gained or lost.
            WindowResize, ///< Window size change event.
            WindowExit,   ///< Window close request.
        };

        /// \brief Timestamp of the event in seconds.
        Real64 Time;

        /// \brief The event type.
        Type   Type;

        /// \brief Event-specific data.
        union
        {
            /// \brief Data for \ref Type::KeyType (character input).
            struct
            {
                UInt32 Codepoint; ///< Unicode codepoint of the typed character.
            } KeyType;

            /// \brief Data for \ref Type::KeyUp and \ref Type::KeyDown.
            struct
            {
                Key Key; ///< Physical key code.
            } KeyAction;

            /// \brief Data for \ref Type::MouseMove.
            struct
            {
                Real32 X;      ///< Current X position.
                Real32 Y;      ///< Current Y position.
                Real32 DeltaX; ///< Change in X since last event.
                Real32 DeltaY; ///< Change in Y since last event.
            } MouseAxis;

            /// \brief Data for \ref Type::MouseUp and \ref Type::MouseDown.
            struct
            {
                Button Button; ///< Mouse button identifier.
            } MouseAction;

            /// \brief Data for \ref Type::WindowFocus.
            struct
            {
                Bool State; ///< `true` if focus gained, `false` if lost.
            } WindowFocus;

            /// \brief Data for \ref Type::WindowResize.
            struct
            {
                UInt32 Width;  ///< New window width in pixels.
                UInt32 Height; ///< New window height in pixels.
            } WindowResize;
        };
    };
}