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
    enum class Easing : UInt8
    {
        Linear,
        InQuad,
        OutQuad,
        InOutQuad,
        InCubic,
        OutCubic,
        InOutCubic,
        Smoothstep,
        InSine,
        OutSine,
        InOutSine,
        InExpo,
        OutExpo,
        InCirc,
        OutCirc,
        InElastic,
        OutElastic,
        InOutElastic,
        InBounce,
        OutBounce,
        InOutBounce,
        InBack,
        OutBack,
        InOutBack,
    };

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Linear(Type Time)
    {
        return Time;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InQuad(Type Time)
    {
        return Time * Time;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutQuad(Type Time)
    {
        return Time * (2 - Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutQuad(Type Time)
    {
        return Time < 0.5 ? 2 * Time * Time : -1 + (4 - 2 * Time) * Time;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InCubic(Type Time)
    {
        return Time * Time * Time;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutCubic(Type Time)
    {
        --Time;
        return Time * Time * Time + 1;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutCubic(Type Time)
    {
        return Time < 0.5 ? 4 * Time * Time * Time : (Time - 1) * (2 * Time - 2) * (2 * Time - 2) + 1;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Smoothstep(Type Time)
    {
        return Time * Time * (3 - 2 * Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InSine(Type Time)
    {
        return 1 - Cosine(Time * k_PI<Type> / 2);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutSine(Type Time)
    {
        return Sine(Time * k_PI<Type> / 2);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutSine(Type Time)
    {
        return -(Sine(k_PI<Type> * Time) - 1) / 2;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InExpo(Type Time)
    {
        return Time == 0 ? 0 : Pow(Type(2), 10 * (Time - 1));
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutExpo(Type Time)
    {
        return Time == 1 ? 1 : 1 - Pow(Type(2), -10 * Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InCirc(Type Time)
    {
        return 1 - Sqrt(1 - Time * Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutCirc(Type Time)
    {
        --Time;
        return Sqrt(1 - Time * Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InElastic(Type Time)
    {
        return Time == 0 ? 0 : Time == 1 ? 1 : -Pow(Type(2), 10 * Time - 10) * Sine((Time * 10 - 10.75) * (2 * k_PI<Type> / 3));
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutElastic(Type Time)
    {
        constexpr Type C4 = (2 * k_PI<Type>) / 3;
        return Time == 0 ? 0 : Time == 1 ? 1 : Pow(Type(2), -10 * Time) * Sine((Time * 10 - 0.75) * C4) + 1;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutElastic(Type Time)
    {
        constexpr Type C5 = (2 * k_PI<Type>) / 4.5;
        return Time == 0 ? 0 : Time == 1 ? 1 : Time < 0.5
            ? -(Pow(Type(2), 20 * Time - 10) * Sine((20 * Time - 11.125) * C5)) / 2
            : (Pow(Type(2), -20 * Time + 10) * Sine((20 * Time - 11.125) * C5 )) / 2 + 1;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutBounce(Type Time)
    {
        constexpr Type N1 = 7.5625;
        constexpr Type D1 = 2.75;

        if (Time < 1 / D1)
        {
            return N1 * Time * Time;
        }
        else if (Time < 2 / D1)
        {
            return N1 * (Time -= 1.5 / D1) * Time + 0.75;
        }
        else if (Time < 2.5 / D1)
        {
            return N1 * (Time -= 2.25 / D1) * Time + 0.9375;
        }
        else
        {
            return N1 * (Time -= 2.625 / D1) * Time + 0.984375;
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InBounce(Type Time)
    {
        return 1 - OutBounce(1 - Time);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutBounce(Type Time)
    {
        return Time < 0.5 ? (1 - OutBounce(1 - 2 * Time)) / 2 : (1 + OutBounce(2 * Time - 1)) / 2;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InBack(Type Time) 
    {
        constexpr Type C1 = 1.70158;
        constexpr Type C3 = C1 + 1;
        return C3 * Time * Time * Time - C1 * Time * Time;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type OutBack(Type Time) 
    {
        constexpr Type C1 = 1.70158;
        constexpr Type C3 = C1 + 1;
        return 1 + C3 * Pow(Time - 1, Type(3)) + C1 * Pow(Time - 1, Type(2));
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InOutBack(Type Time) 
    {
        constexpr Type C2 = 1.70158 * 1.525;
        return Time < 0.5
               ? (Pow(2 * Time, Type(2)) * ((C2 + 1) * 2 * Time - C2)) / 2
               : (Pow(2 * Time - 2, Type(2)) * ((C2 + 1) * (Time * 2 - 2) + C2) + 2) / 2;
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Ease(Easing Function, Type Time)
    {
        switch (Function)
        {
        case Easing::Linear:        return Linear(Time);
        case Easing::InQuad:        return InQuad(Time);
        case Easing::OutQuad:       return OutQuad(Time);
        case Easing::InOutQuad:     return InOutQuad(Time);
        case Easing::InCubic:       return InCubic(Time);
        case Easing::OutCubic:      return OutCubic(Time);
        case Easing::InOutCubic:    return InOutCubic(Time);
        case Easing::Smoothstep:    return Smoothstep(Time);
        case Easing::InSine:        return InSine(Time);
        case Easing::OutSine:       return OutSine(Time);
        case Easing::InOutSine:     return InOutSine(Time);
        case Easing::InExpo:        return InExpo(Time);
        case Easing::OutExpo:       return OutExpo(Time);
        case Easing::InCirc:        return InCirc(Time);
        case Easing::OutCirc:       return OutCirc(Time);
        case Easing::InElastic:     return InElastic(Time);
        case Easing::OutElastic:    return OutElastic(Time);
        case Easing::InOutElastic:  return InOutElastic(Time);
        case Easing::InBounce:      return InBounce(Time);
        case Easing::OutBounce:     return OutBounce(Time);
        case Easing::InOutBounce:   return InOutBounce(Time);
        case Easing::InBack:        return InBack(Time);
        case Easing::OutBack:       return OutBack(Time);
        case Easing::InOutBack:     return InOutBack(Time);
        }
    }
}