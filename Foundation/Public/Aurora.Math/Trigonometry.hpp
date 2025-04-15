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

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type k_PI = Type(3.141592653589793238462643383279502884e+00);

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type k_Epsilon = std::numeric_limits<Type>::epsilon();

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type DegreesToRadians(Type Degrees)
    {
        return Degrees * (k_PI<Type> / 180);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type RadiansToDegrees(Type Radians)
    {
        return Radians * (180 / k_PI<Type>);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Abs(Type Value)
    {
        return std::abs(Value);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Bool AlmostZero(Type First)
    {
        if constexpr (std::is_floating_point_v<Type>)
        {
            return Abs(First) <= k_Epsilon<Type>;
        }
        else
        {
            return First == 0;
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Bool AlmostEqual(Type First, Type Second)
    {
        if constexpr (std::is_floating_point_v<Type>)
        {
            const Type Difference = Abs(First - Second);

            return Difference <= k_Epsilon<Type>
                || Difference <= ((Abs(First) < Abs(Second) ? Abs(Second) : Abs(First)) * k_Epsilon<Type>);
        }
        else
        {
            return First == Second;
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Cosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::cos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::cosf(Angle);
        }
        else
        {
            return std::cosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvCosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::acos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::acosf(Angle);
        }
        else
        {
            return std::acosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Sine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::sin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::sinf(Angle);
        }
        else
        {
            return std::sinl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvSine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::asin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::asinf(Angle);
        }
        else
        {
            return std::asinl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Tangent(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::tan(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::tanf(Angle);
        }
        else
        {
            return std::tanl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvTangent(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::atan(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::atanf(Angle);
        }
        else
        {
            return std::atanl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Sqrt(Type Number)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::sqrt(Number);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::sqrtf(Number);
        }
        else
        {
            return std::sqrtl(Number);
        }
    }

    // -=(Undocumented)=-
    static constexpr Real32 InvSqrt(Real32 Number)
    {
        UInt32 Magic = std::bit_cast<UInt32>(Number);
        Magic = 0x5F375A86 - (Magic >> 1);

        const Real32 Y = std::bit_cast<Real32>(Magic);
        return Y * (1.5f - (Number * 0.5f * Y * Y));
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Pow(Type Base, Type Exponent)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::pow(Base, Exponent);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::powf(Base, Exponent);
        }
        else
        {
            return std::powl(Base, Exponent);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Floor(Type Value)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::floor(Value);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::floorf(Value);
        }
        else
        {
            return std::floorl(Value);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Ceil(Type Value)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return std::ceil(Value);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return std::ceilf(Value);
        }
        else
        {
            return std::ceill(Value);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Lerp(Type Start, Type End, Real32 Percentage)
    {
        return Start + Percentage * (End - Start);
    }
}