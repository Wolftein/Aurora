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
    /// \brief Represents a standard keyboard input device and its state.
    class Keyboard final
    {
    public:

        /// \brief The total number of supported keys.
        constexpr static UInt32 kMaxKeys = Enum::Count<Key>();

    public:

        /// \brief Updates the internal key state by processing a list of input events.
        ///
        /// \param Events A span containing the input events to process.
        void Poll(ConstSpan<Event> Events);

        /// \brief Clears all stored key states for both the current and previous frames.
        void Reset();

        /// \brief Checks if a key was pressed during the current frame.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key was pressed this frame, otherwise `false`.
        AURORA_INLINE Bool IsKeyPressed(Key Key) const
        {
            return !mLastKeys.test(Enum::Cast(Key)) && mThisKeys.test(Enum::Cast(Key));
        }

        /// \brief Checks if a key is currently held down.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key is held, otherwise `false`.
        AURORA_INLINE Bool IsKeyHeld(Key Key) const
        {
            return mThisKeys.test(Enum::Cast(Key));
        }

        /// \brief Checks if a key was released during the current frame.
        ///
        /// A key is considered "released" if it was held in the previous frame,
        /// but is not held in the current frame.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key was released this frame, otherwise `false`.
        AURORA_INLINE Bool IsKeyReleased(Key Key) const
        {
            return mLastKeys.test(Enum::Cast(Key)) && !mThisKeys.test(Enum::Cast(Key));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bitset<kMaxKeys> mLastKeys;
        Bitset<kMaxKeys> mThisKeys;
    };
}
