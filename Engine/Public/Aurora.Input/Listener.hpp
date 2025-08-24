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

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    /// \brief Base interface for receiving and handling input and window events.
    class Listener : public virtual Trackable
    {
    public:

        /// \brief Dispatches a raw event to the appropriate specialized handler.
        ///
        /// \param Event The input or window event to process.
        /// \return `true` if the event was handled and consumed, otherwise `false`.
        virtual Bool OnEvent(ConstRef<Event> Event);

    private:

        /// \brief Called when a character is typed (text input).
        ///
        /// \param Codepoint UTF-32 codepoint of the typed character.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnKeyType(UInt32 Codepoint);

        /// \brief Called when a key is released.
        ///
        /// \param Key The key that was released.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnKeyUp(Key Key);

        /// \brief Called when a key is pressed.
        ///
        /// \param Key The key that was pressed.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnKeyDown(Key Key);

        /// \brief Called when the mouse cursor moves.
        ///
        /// \param X      New X position in pixels or normalized coordinates.
        /// \param Y      New Y position.
        /// \param DeltaX Change in X since the last mouse move event.
        /// \param DeltaY Change in Y since the last mouse move event.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnMouseMove(Real32 X, Real32 Y, Real32 DeltaX, Real32 DeltaY);

        /// \brief Called when the mouse wheel is scrolled.
        ///
        /// \param DeltaX Horizontal scroll delta.
        /// \param DeltaY Vertical scroll delta.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnMouseScroll(Real32 DeltaX, Real32 DeltaY);

        /// \brief Called when a mouse button is released.
        ///
        /// \param Button The mouse button that was released.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnMouseUp(Button Button);

        /// \brief Called when a mouse button is pressed.
        ///
        /// \param Button The mouse button that was pressed.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnMouseDown(Button Button);

        /// \brief Called when the window focus changes.
        ///
        /// \param Focused `true` if the window gained focus, `false` if it lost focus.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnWindowFocus(Bool Focused);

        /// \brief Called when the window is resized.
        ///
        /// \param Width  New window width in pixels.
        /// \param Height New window height in pixels.
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnWindowResize(UInt32 Width, UInt32 Height);

        /// \brief Called when the window is closing or a close request is made.
        ///
        /// \return `true` if the event was handled, otherwise `false`.
        virtual Bool OnWindowExit();
    };
}
