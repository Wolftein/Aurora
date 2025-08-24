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
#include "Aurora.Math/Vector2.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Input
{
    /// \brief Represents a standard mouse input device.
    class Mouse final
    {
    public:

        /// \brief The total number of supported mouse buttons.
        constexpr static UInt32 kMaxButtons = Enum::Count<Button>();

    public:

        /// \brief Updates the internal mouse state based on incoming input events.
        ///
        /// \param Events A span of input events to process.
        void Poll(ConstSpan<Event> Events);

        /// \brief Clears all stored mouse state.
        void Reset();

        /// \brief Gets the current mouse position.
        ///
        /// \return The current mouse position as a 2D vector.
        AURORA_INLINE Vector2 GetPosition() const
        {
            return mThisMousePosition;
        }

        /// \brief Gets the current mouse scroll delta.
        ///
        /// \return The current scroll delta as a 2D vector.
        AURORA_INLINE Vector2 GetScroll() const
        {
            return mThisMouseScroll;
        }

        /// \brief Checks if a mouse button was pressed during the current frame.
        ///
        /// \param Button The button to check.
        /// \return `true` if the button transitioned from released to pressed this frame, otherwise `false`.
        AURORA_INLINE Bool IsButtonPressed(Button Button) const
        {
            return !mLastButtons.test(Enum::Cast(Button)) && mThisButtons.test(Enum::Cast(Button));
        }

        /// \brief Checks if a mouse button is currently held down.
        ///
        /// \param Button The button to check.
        /// \return `true` if the button is currently pressed, otherwise `false`.
        AURORA_INLINE Bool IsButtonHeld(Button Button) const
        {
            return mThisButtons.test(Enum::Cast(Button));
        }

        /// \brief Checks if a mouse button was released during the current frame.
        ///
        /// \param Button The button to check.
        /// \return `true` if the button transitioned from pressed to released this frame, otherwise `false`.
        AURORA_INLINE Bool IsButtonReleased(Button Button) const
        {
            return mLastButtons.test(Enum::Cast(Button)) && !mThisButtons.test(Enum::Cast(Button));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2             mThisMousePosition;
        Vector2             mThisMouseScroll;
        Bitset<kMaxButtons> mLastButtons;
        Bitset<kMaxButtons> mThisButtons;
    };
}
