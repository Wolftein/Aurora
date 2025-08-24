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

#include "Trait.hpp"
#include <cmath>
#include <smmintrin.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief The mathematical constant PI (π), templated for any real type.
    template<typename Type>
    inline constexpr Type kPI      = Type(3.141592653589793238462643383279502884e+00);

    /// \brief The smallest difference detectable between two values of type Type.
    template<typename Type>
    inline constexpr Type kEpsilon =
        IsEqual<Type, Real32> ? Type(1.19209290e-07F)        :
        IsEqual<Type, Real64> ? Type(2.2204460492503131e-16) :
        Type(0);

    /// \brief Relative epsilon for floating-point comparisons.
    template<typename Type>
    inline constexpr Type kRelativeEpsilon =
        IsEqual<Type, Real32> ? Type(1e-5f) :
        IsEqual<Type, Real64> ? Type(1e-12) :
        Type(0);

    /// \brief Absolute epsilon for floating-point comparisons.
    template<typename Type>
    inline constexpr Type kAbsoluteEpsilon =
        IsEqual<Type, Real32> ? Type(1e-6f) :
        IsEqual<Type, Real64> ? Type(1e-14) :
        Type(0);


    /// \brief Converts degrees to radians.
    /// 
    /// \param Degrees Angle in degrees.
    /// \return Angle in radians.
    template<typename Type>
    constexpr Type DegreesToRadians(Type Degrees)
    {
        return Degrees * (kPI<Type> / Type(180));
    }

    /// \brief Converts radians to degrees.
    /// 
    /// \param Radians Angle in radians.
    /// \return Angle in degrees.
    template<typename Type>
    constexpr Type RadiansToDegrees(Type Radians)
    {
        return Radians * (Type(180) / kPI<Type>);
    }

    /// \brief Returns the absolute value of a number.
    /// 
    /// \param Value Input number.
    /// \return The absolute (non-negative) value of the number.
    template<typename Type>
    constexpr Type Abs(Type Value)
    {
        return Value < Type(0) ? -Value : Value;
    }

    /// \brief Returns the sign of a numeric value as -1, 0, or +1.
    /// 
    /// \param Value Input number.
    /// \return The sign of the value.
    template<typename Type>
    constexpr Type Sign(Type Value)
    {
        return (Type(0) < Value) - (Value < Type(0));
    }

    /// \brief Computes the cosine of an angle in radians.
    /// 
    /// \param Angle Angle in radians.
    /// \return Cosine of the angle.
    template<typename Type>
    constexpr Type Cos(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::cos(Angle);
        }
        else
        {
            return std::cosf(Angle);
        }
    }

    /// \brief Computes the inverse cosine (arccos) of a value.
    /// 
    /// \param Angle Cosine value in the range [-1, 1].
    /// \return Angle in radians.
    template<typename Type>
    constexpr Type InvCos(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::acos(Angle);
        }
        else
        {
            return std::acosf(Angle);
        }
    }

    /// \brief Computes the sine of an angle in radians.
    /// 
    /// \param Angle Angle in radians.
    /// \return Sine of the angle.
    template<typename Type>
    constexpr Type Sin(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::sin(Angle);
        }
        else
        {
            return std::sinf(Angle);
        }
    }

    /// \brief Computes the inverse sine (arcsin) of a value.
    /// 
    /// \param Angle Sine value in the range [-1, 1].
    /// \return Angle in radians.
    template<typename Type>
    constexpr Type InvSin(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::asin(Angle);
        }
        else
        {
            return std::asinf(Angle);
        }
    }

    /// \brief Computes the tangent of an angle in radians.
    /// 
    /// \param Angle Angle in radians.
    /// \return Tangent of the angle.
    template<typename Type>
    constexpr Type Tan(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::tan(Angle);
        }
        else
        {
            return std::tanf(Angle);
        }
    }

    /// \brief Computes the inverse tangent (arctangent) of a value.
    /// 
    /// \param Angle Tangent value.
    /// \return Angle in radians.
    template<typename Type>
    constexpr Type InvTan(Type Angle)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::atan(Angle);
        }
        else
        {
            return std::atanf(Angle);
        }
    }

    /// \brief Computes the inverse tangent (atan2) given Y and X.
    /// 
    /// \param X X-coordinate.
    /// \param Y Y-coordinate.
    /// \return Angle in radians between the X-axis and the point (X, Y).
    template<typename Type>
    constexpr Type InvTan(Type X, Type Y)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::atan2(X, Y);
        }
        else
        {
            return std::atan2f(X, Y);
        }
    }

    /// \brief Computes the square root of a number.
    /// 
    /// \param Number The value to compute the square root of.
    /// \return The square root of the number.
    template<typename Type>
    constexpr Type Sqrt(Type Number)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::sqrt(Number);
        }
        else
        {
            return std::sqrtf(Number);
        }
    }

    /// \brief Computes the inv square root of a number.
    ///
    /// \param Number The value to compute the square root of.
    /// \return The inv square root of the number.
    template<typename Type>
    constexpr Type InvSqrt(Type Number)
    {
        return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(Number)));
    }

    /// \brief Raises a base to the given exponent.
    /// 
    /// \param Base     The base value.
    /// \param Exponent The exponent.
    /// \return Result of Base raised to the power of Exponent.
    template<typename Type>
    constexpr Type Pow(Type Base, Type Exponent)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::pow(Base, Exponent);
        }
        else
        {
            return std::powf(Base, Exponent);
        }
    }

    /// \brief Computes the largest integer less than or equal to the given value.
    /// 
    /// \param Value The value to floor.
    /// \return The largest integer less than or equal to Value.
    template<typename Type>
    constexpr Type Floor(Type Value)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::floor(Value);
        }
        else
        {
            return std::floorf(Value);
        }
    }

    /// \brief Computes the smallest integer greater than or equal to the given value.
    /// 
    /// \param Value The value to ceil.
    /// \return The smallest integer greater than or equal to Value.
    template<typename Type>
    constexpr Type Ceil(Type Value)
    {
        if constexpr (IsEqual<Real64, Type>)
        {
            return std::ceil(Value);
        }
        else
        {
            return std::ceilf(Value);
        }
    }

    /// \brief Linearly interpolates between two scalar.
    /// 
    /// \param Start      The starting scalar.
    /// \param End        The ending scalar.
    /// \param Percentage The interpolation percentage (range between 0 and 1).
    /// \return A scalar interpolated between the start and end scalar.
    template<typename Type>
    constexpr Type Lerp(Type Start, Type End, Real32 Percentage)
    {
        return Start + Percentage * (End - Start);
    }

    /// \brief Finds the smallest value among all provided arguments.
    /// 
    /// \param First  First value to compare.
    /// \param Second Second value to compare.
    /// \param Third  Additional values to compare (variadic).
    /// \return The smallest value among all inputs.
    template <typename Type, typename... Tail>
    constexpr auto Min(Type First, Type Second, Tail... Third)
    {
        if constexpr (sizeof...(Tail) == 0)
        {
            return First < Second ? First : Second;
        }
        else
        {
            return Min(Min(First, Second), Third...);
        }
    }

    /// \brief Finds the largest  value among all provided arguments.
    /// 
    /// \param First  First value to compare.
    /// \param Second Second value to compare.
    /// \param Third  Additional values to compare (variadic).
    /// \return The largest value among all inputs.
    template <typename Type, typename... Tail>
    constexpr auto Max(Type First, Type Second, Tail... Third)
    {
        if constexpr (sizeof...(Tail) == 0)
        {
            return First > Second ? First : Second;
        }
        else
        {
            return Max(Max(First, Second), Third...);
        }
    }

    /// \brief Restricts a value to be within a specified range.
    /// 
    /// \param Value   Input value to clamp.
    /// \param Minimum Lower bound of range.
    /// \param Maximum Upper bound of range.
    /// \return The restricted value within lower and upper bounds.
    template<typename Type>
    constexpr Type Clamp(Type Value, Type Minimum, Type Maximum)
    {
        return Min(Max(Value, Minimum), Maximum);
    }

    /// \brief Checks if a number is approximately zero, within the bounds of machine epsilon.
    /// 
    /// \param First Input value.
    /// \return `true` if the value is near zero, `false` otherwise.
    template<typename Type>
    constexpr Bool IsAlmostZero(Type First)
    {
        if constexpr (IsReal<Type>)
        {
            return Abs(First) <= kEpsilon<Type>;
        }
        else
        {
            return First == 0;
        }
    }

    /// \brief Near-equality test with relative and absolute tolerances.
    /// 
    /// \param First    Left-hand value.
    /// \param Second   Right-hand value.
    /// \param Relative Relative tolerance (defaults to `kRelativeEpsilon<Type>`; must be ≥ 0).
    /// \param Absolute Absolute tolerance (defaults to `kAbsoluteEpsilon<Type>`; must be ≥ 0).
    /// \return `true` if the values are considered almost equal; otherwise `false`.
    template<typename Type>
    constexpr Bool IsAlmostEqual(Type First, Type Second, Type Relative = kRelativeEpsilon<Type>, Type Absolute = kAbsoluteEpsilon<Type>)
    {
        if constexpr (IsReal<Type>)
        {
            const Type Difference = Abs(First - Second);
            const Type Scale      = Max(Abs(First), Abs(Second), Type(1));

            return Difference <= (Absolute > Relative * Scale ? Absolute : Relative * Scale);
        }
        else
        {
            return First == Second;
        }
    }
}