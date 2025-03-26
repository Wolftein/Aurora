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
    static constexpr Real32 k_PI = 3.141592653589793238462643383279502884e+00f;

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type DegreesToRadians(Type Degrees)
    {
        return Degrees * (k_PI / 180);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type RadiansToDegrees(Type Radians)
    {
        return Radians * (180 / k_PI);
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Cosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return cos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return cosf(Angle);
        }
        else
        {
            return cosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvCosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return acos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return acosf(Angle);
        }
        else
        {
            return acosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Sine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return sin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return sinf(Angle);
        }
        else
        {
            return sinl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvSine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return asin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return asinf(Angle);
        }
        else
        {
            return asinl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Tangent(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return tan(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return tanf(Angle);
        }
        else
        {
            return tanl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type InvTangent(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return atan(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return atanf(Angle);
        }
        else
        {
            return atanl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    static constexpr Type Sqrt(Type Number)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return sqrt(Number);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return sqrtf(Number);
        }
        else
        {
            return sqrtl(Number);
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
    static constexpr Type Lerp(Type Start, Type End, Real32 Percentage)
    {
        return Start + Percentage * (End - Start);
    }
}