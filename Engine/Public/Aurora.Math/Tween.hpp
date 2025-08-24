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

#include "Easing.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Provides smooth interpolation between two values over time.
    template<typename Type>
    class Tween final
    {
    public:

        /// \brief Creates an uninitialized tween.
        AURORA_INLINE Tween()
            : mTime        { 0 },
              mAccumulator { 0 },
              mEasing      { Easing::Linear }
        {
        }

        /// \brief Constructs a tween with specified parameters.
        ///
        /// \param Start    The starting value of the tween.
        /// \param End      The target value of the tween.
        /// \param Time     The total duration of the tween in seconds.
        /// \param Function The easing function to use (default: Linear).
        AURORA_INLINE Tween(Type Start, Type End, Real64 Time, Easing Function = Easing::Linear)
            : mStart       { Start },
              mEnd         { End },
              mTime        { Time },
              mAccumulator { 0 },
              mEasing      { Function }
        {
        }

        /// \brief Advances the tween by the specified time delta.
        /// 
        /// \param Delta The time elapsed since last update in seconds.
        /// \return The current interpolated value.
        AURORA_INLINE Type Tick(Real64 Delta)
        {
            mAccumulator = Min(mAccumulator + Delta, mTime);

            if constexpr (IsInteger<Type> || IsReal<Type>)
            {
                return Lerp<Type>(mStart, mEnd, Ease(mEasing, mAccumulator / mTime));
            }
            else
            {
                return Type::Lerp(mStart, mEnd, Ease(mEasing, mAccumulator / mTime));
            }
        }

        /// \brief Checks if the tween hasn't started yet.
        /// 
        /// \return `true` if the tween is idling, `false` otherwise.
        AURORA_INLINE Bool IsIdle() const
        {
            return IsAlmostZero(mTime);
        }

        /// \brief Checks if the tween has completed.
        /// 
        /// \return `true` if the tween has reached its end value, `false` otherwise.
        AURORA_INLINE Bool IsComplete() const
        {
            return IsAlmostEqual(mAccumulator, mTime);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Type   mStart;
        Type   mEnd;
        Real64 mTime;
        Real64 mAccumulator;
        Easing mEasing;
    };
}