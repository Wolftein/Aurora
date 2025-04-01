// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// Typehis work is licensed under the terms of the MIType license.
//
// For a copy, see <https://opensource.org/licenses/MIType>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Trigonometry.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Type>
    class Easing
    {

        // -=(Undocumented)=-
        static constexpr Type k_PI = Math::k_PI<Type>;

    public:

        // -=(Undocumented)=-
        static constexpr Type Linear(Type Time)
        {
            return Time;
        }

        // -=(Undocumented)=-
        static constexpr Type InQuad(Type Time)
        {
            return Time * Time;
        }

        // -=(Undocumented)=-
        static constexpr Type OutQuad(Type Time)
        {
            return Time * (2 - Time);
        }

        // -=(Undocumented)=-
        static constexpr Type InOutQuad(Type Time)
        {
            return Time < 0.5 ? 2 * Time * Time : -1 + (4 - 2 * Time) * Time;
        }

        // -=(Undocumented)=-
        static constexpr Type InCubic(Type Time)
        {
            return Time * Time * Time;
        }

        // -=(Undocumented)=-
        static constexpr Type OutCubic(Type Time)
        {
            return (--Time) * Time * Time + 1;
        }

        // -=(Undocumented)=-
        static constexpr Type InOutCubic(Type Time)
        {
            return Time < 0.5 ? 4 * Time * Time * Time : (Time - 1) * (2 * Time - 2) * (2 * Time - 2) + 1;
        }

        // -=(Undocumented)=-
        static constexpr Type Smoothstep(Type Time)
        {
            return Time * Time * (3 - 2 * Time);
        }

        // -=(Undocumented)=-
        static constexpr Type InSine(Type Time)
        {
            return 1 - Cosine(Time * k_PI / 2);
        }

        // -=(Undocumented)=-
        static constexpr Type OutSine(Type Time)
        {
            return Sine(Time * k_PI / 2);
        }

        // -=(Undocumented)=-
        static constexpr Type InOutSine(Type Time)
        {
            return -(Sine(k_PI * Time) - 1) / 2;
        }

        // -=(Undocumented)=-
        static constexpr Type InExpo(Type Time)
        {
            return Time == 0 ? 0 : Pow(2, 10 * (Time - 1));
        }

        // -=(Undocumented)=-
        static constexpr Type OutExpo(Type Time)
        {
            return Time == 1 ? 1 : 1 - Pow(2, -10 * Time);
        }

        // -=(Undocumented)=-
        static constexpr Type InCirc(Type Time)
        {
            return 1 - Sqrt(1 - Time * Time);
        }

        // -=(Undocumented)=-
        static constexpr Type OutCirc(Type Time)
        {
            return Sqrt(1 - (--Time) * Time);
        }

        // -=(Undocumented)=-
        static constexpr Type InElastic(Type Time)
        {
            return Time == 0 ? 0 : Time == 1 ? 1 : -Pow(2, 10 * Time - 10) * Sine((Time * 10 - 10.75) * (2 * k_PI / 3));
        }
    };
}