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

#include "Aurora.Base/Scalar.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Enumerates easing functions for smooth interpolation.
    enum class Easing : UInt8
    {
        Linear,        ///< Linear (no easing)
        InQuad,        ///< Quadratic acceleration
        OutQuad,       ///< Quadratic deceleration
        InOutQuad,     ///< Quadratic acceleration and deceleration
        InCubic,       ///< Cubic acceleration
        OutCubic,      ///< Cubic deceleration
        InOutCubic,    ///< Cubic acceleration and deceleration
        Smoothstep,    ///< Smooth acceleration and deceleration
        InSine,        ///< Sinusoidal acceleration
        OutSine,       ///< Sinusoidal deceleration
        InOutSine,     ///< Sinusoidal acceleration and deceleration
        InExpo,        ///< Exponential acceleration
        OutExpo,       ///< Exponential deceleration
        InCirc,        ///< Circular acceleration
        OutCirc,       ///< Circular deceleration
        InElastic,     ///< Elastic spring effect (acceleration)
        OutElastic,    ///< Elastic spring effect (deceleration)
        InOutElastic,  ///< Elastic spring effect (both directions)
        InBounce,      ///< Bounce effect (acceleration)
        OutBounce,     ///< Bounce effect (deceleration)
        InOutBounce,   ///< Bounce effect (both directions)
        InBack,        ///< Overshoot (acceleration)
        OutBack,       ///< Overshoot (deceleration)
        InOutBack      ///< Overshoot (both directions)
    };

    /// \brief Linear easing function (no acceleration).
    ///
    /// \param Time Normalized time value [0, 1].
    /// \return The unmodified time value.
    template<typename Type>
    AURORA_INLINE constexpr static Type Linear(Type Time)
    {
        return Time;
    }

    /// \brief Quadratic easing function (accelerating from zero velocity).
    ///
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following quadratic acceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InQuad(Type Time)
    {
        return Time * Time;
    }

    /// \brief Quadratic easing function (decelerating to zero velocity).
    ///
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following quadratic deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type OutQuad(Type Time)
    {
        return Time * (2 - Time);
    }

    /// \brief Quadratic easing function (acceleration until halfway, then deceleration).
    ///
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with quadratic acceleration/deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutQuad(Type Time)
    {
        return Time < 0.5 ? 2 * Time * Time : -1 + (4 - 2 * Time) * Time;
    }

    /// \brief Cubic easing function (accelerating from zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following cubic acceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InCubic(Type Time)
    {
        return Time * Time * Time;
    }

    /// \brief Cubic easing function (decelerating to zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following cubic deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type OutCubic(Type Time)
    {
        --Time;
        return Time * Time * Time + 1;
    }

    /// \brief Cubic easing function (acceleration until halfway, then deceleration).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with cubic acceleration/deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutCubic(Type Time)
    {
        return Time < 0.5 ? 4 * Time * Time * Time : (Time - 1) * (2 * Time - 2) * (2 * Time - 2) + 1;
    }

    /// \brief Smoothstep easing function (smooth acceleration and deceleration).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Smoothed eased value.
    template<typename Type>
    AURORA_INLINE constexpr static Type Smoothstep(Type Time)
    {
        return Time * Time * (3 - 2 * Time);
    }

    /// \brief Sinusoidal easing function (accelerating from zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following sinusoidal acceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InSine(Type Time)
    {
        return 1 - Cos(Time * kPI<Type> / 2);
    }

    /// \brief Sinusoidal easing function (decelerating to zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following sinusoidal deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type OutSine(Type Time)
    {
        return Sin(Time * kPI<Type> / 2);
    }

    /// \brief Sinusoidal easing function (acceleration until halfway, then deceleration).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with sinusoidal acceleration/deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutSine(Type Time)
    {
        return -(Sin(kPI<Type> * Time) - 1) / 2;
    }

    /// \brief Exponential easing function (accelerating from zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following exponential acceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InExpo(Type Time)
    {
        return Time == 0 ? 0 : Pow(Type(2), 10 * (Time - 1));
    }

    /// \brief Exponential easing function (decelerating to zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following exponential deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type OutExpo(Type Time)
    {
        return Time == 1 ? 1 : 1 - Pow(Type(2), -10 * Time);
    }

    /// \brief Circular easing function (accelerating from zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following circular acceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type InCirc(Type Time)
    {
        return 1 - Sqrt(1 - Time * Time);
    }

    /// \brief Circular easing function (decelerating to zero velocity).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value following circular deceleration.
    template<typename Type>
    AURORA_INLINE constexpr static Type OutCirc(Type Time)
    {
        --Time;
        return Sqrt(1 - Time * Time);
    }

    /// \brief Elastic easing function (accelerating from zero velocity with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with elastic spring effect (acceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type InElastic(Type Time)
    {
        return Time == 0 ? 0 : Time == 1 ? 1 : -Pow(Type(2), 10 * Time - 10) * Sin((Time * 10 - 10.75) * (2 * kPI<Type> / 3));
    }

    /// \brief Elastic easing function (decelerating to zero velocity with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with elastic spring effect (deceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type OutElastic(Type Time)
    {
        constexpr Type C4 = (2 * kPI<Type>) / 3;
        return Time == 0 ? 0 : Time == 1 ? 1 : Pow(Type(2), -10 * Time) * Sin((Time * 10 - 0.75) * C4) + 1;
    }

    /// \brief Elastic easing function (acceleration and deceleration with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with elastic spring effect (both directions).
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutElastic(Type Time)
    {
        constexpr Type C5 = (2 * kPI<Type>) / 4.5;
        return Time == 0 ? 0 : Time == 1 ? 1 : Time < 0.5
            ? -(Pow(Type(2), 20 * Time - 10) * Sin((20 * Time - 11.125) * C5)) / 2
            : (Pow(Type(2), -20 * Time + 10) * Sin((20 * Time - 11.125) * C5 )) / 2 + 1;
    }

    /// \brief Bounce easing function (decelerating with bounce effect).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with bounce effect (deceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type OutBounce(Type Time)
    {
        constexpr Type N1 = 7.5625;
        constexpr Type D1 = 2.75;

        if (Time < 1 / D1)
        {
            return N1 * Time * Time;
        }
        if (Time < 2 / D1)
        {
            Time -= 1.5 / D1;
            return N1 * Time * Time + 0.75;
        }
        if (Time < 2.5 / D1)
        {
            Time -= 2.25 / D1;
            return N1 * Time * Time + 0.9375;
        }

        Time -= 2.625 / D1;
        return N1 * Time * Time + 0.984375;
    }

    /// \brief Bounce easing function (accelerating with bounce effect).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with bounce effect (acceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type InBounce(Type Time)
    {
        return 1 - OutBounce(1 - Time);
    }

    /// \brief Bounce easing function (acceleration and deceleration with bounce effect).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with bounce effect (both directions).
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutBounce(Type Time)
    {
        return Time < 0.5 ? (1 - OutBounce(1 - 2 * Time)) / 2 : (1 + OutBounce(2 * Time - 1)) / 2;
    }

    /// \brief Back easing function (accelerating with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with overshoot (acceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type InBack(Type Time) 
    {
        constexpr Type C1 = 1.70158;
        constexpr Type C3 = C1 + 1;
        return C3 * Time * Time * Time - C1 * Time * Time;
    }

    /// \brief Back easing function (decelerating with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with overshoot (deceleration).
    template<typename Type>
    AURORA_INLINE constexpr static Type OutBack(Type Time) 
    {
        constexpr Type C1 = 1.70158;
        constexpr Type C3 = C1 + 1;
        return 1 + C3 * Pow(Time - 1, Type(3)) + C1 * Pow(Time - 1, Type(2));
    }

    /// \brief Back easing function (acceleration and deceleration with overshoot).
    /// 
    /// \param Time Normalized time value [0, 1].
    /// \return Eased value with overshoot (both directions).
    template<typename Type>
    AURORA_INLINE constexpr static Type InOutBack(Type Time) 
    {
        constexpr Type C2 = 1.70158 * 1.525;
        return Time < 0.5
               ? (Pow(2 * Time, Type(2)) * ((C2 + 1) * 2 * Time - C2)) / 2
               : (Pow(2 * Time - 2, Type(2)) * ((C2 + 1) * (Time * 2 - 2) + C2) + 2) / 2;
    }

    /// \brief Applies the specified easing function to a time value.
    /// 
    /// \param Function The easing function to apply.
    /// \param Time     Normalized time value [0, 1].
    /// \return The eased value according to the specified function.
    template<typename Type>
    AURORA_INLINE constexpr static Type Ease(Easing Function, Type Time)
    {
        switch (Function)
        {
            case Easing::Linear:
                return Linear(Time);
            case Easing::InQuad:
                return InQuad(Time);
            case Easing::OutQuad:
                return OutQuad(Time);
            case Easing::InOutQuad:
                return InOutQuad(Time);
            case Easing::InCubic:
                return InCubic(Time);
            case Easing::OutCubic:
                return OutCubic(Time);
            case Easing::InOutCubic:
                return InOutCubic(Time);
            case Easing::Smoothstep:
                return Smoothstep(Time);
            case Easing::InSine:
                return InSine(Time);
            case Easing::OutSine:
                return OutSine(Time);
            case Easing::InOutSine:
                return InOutSine(Time);
            case Easing::InExpo:
                return InExpo(Time);
            case Easing::OutExpo:
                return OutExpo(Time);
            case Easing::InCirc:
                return InCirc(Time);
            case Easing::OutCirc:
                return OutCirc(Time);
            case Easing::InElastic:
                return InElastic(Time);
            case Easing::OutElastic:
                return OutElastic(Time);
            case Easing::InOutElastic:
                return InOutElastic(Time);
            case Easing::InBounce:
                return InBounce(Time);
            case Easing::OutBounce:
                return OutBounce(Time);
            case Easing::InOutBounce:
                return InOutBounce(Time);
            case Easing::InBack:
                return InBack(Time);
            case Easing::OutBack:
                return OutBack(Time);
            case Easing::InOutBack:
                return InOutBack(Time);
            default:
                return Linear(Time);
        }
    }
}