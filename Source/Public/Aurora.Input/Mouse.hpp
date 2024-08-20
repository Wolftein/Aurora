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

#include "Common.hpp"
#include "Aurora.Math/Vector2.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=(Undocumented)=-
    class Mouse final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt k_MaxButtons = 6;

    public:

        // -=(Undocumented)=-
        void Poll(CPtr<const Event> Events);

        // -=(Undocumented)=-
        void Reset();

        // -=(Undocumented)=-
        Vector2i GetPosition() const
        {
            return mThisMousePosition;
        }

        // -=(Undocumented)=-
        Vector2i GetDelta() const
        {
            return mThisMousePosition - mLastMousePosition;
        }

        // -=(Undocumented)=-
        Vector2i GetScroll() const
        {
            return mThisMouseScroll;
        }

        // -=(Undocumented)=-
        bool IsButtonPressed(Button Button) const
        {
            return !mLastButtons.test(CastEnum(Button)) && mThisButtons.test(CastEnum(Button));
        }

        // -=(Undocumented)=-
        bool IsButtonHeld(Button Button) const
        {
            return mThisButtons.test(CastEnum(Button));
        }

        // -=(Undocumented)=-
        Bool IsButtonReleased(Button Button) const
        {
            return mLastButtons.test(CastEnum(Button)) && !mThisButtons.test(CastEnum(Button));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2i             mLastMousePosition;
        Vector2i             mThisMousePosition;
        Vector2i             mThisMouseScroll;
        Bitset<k_MaxButtons> mLastButtons;
        Bitset<k_MaxButtons> mThisButtons;
    };
}