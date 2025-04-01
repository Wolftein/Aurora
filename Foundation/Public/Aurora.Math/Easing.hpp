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
    enum class Easing
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
    static constexpr Type Ease(Easing Function, Type Time)
    {
        switch (Function)
        {
        case Easing::Linear:     return Linear(Time);
        case Easing::InQuad:     return InQuad(Time);
        case Easing::OutQuad:    return OutQuad(Time);
        case Easing::InOutQuad:  return InOutQuad(Time);
        case Easing::InCubic:    return InCubic(Time);
        case Easing::OutCubic:   return OutCubic(Time);
        case Easing::InOutCubic: return InOutCubic(Time);
        case Easing::Smoothstep: return Smoothstep(Time);
        case Easing::InSine:     return InSine(Time);
        case Easing::OutSine:    return OutSine(Time);
        case Easing::InOutSine:  return InOutSine(Time);
        case Easing::InExpo:     return InExpo(Time);
        case Easing::OutExpo:    return OutExpo(Time);
        case Easing::InCirc:     return InCirc(Time);
        case Easing::OutCirc:    return OutCirc(Time);
        case Easing::InElastic:  return InElastic(Time);
        default:                 return Linear(Time);
        }
    }
}