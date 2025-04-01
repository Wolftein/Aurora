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
    // -=(Undocumented)=-
    template<typename Type>
    class Tween final
    {
    public:

        // -=(Undocumented)=-
        Tween()
            : mTime        { 0 },
              mAccumulator { 0 },
              mEasing      { Easing::Linear }
        {
        }

        // -=(Undocumented)=-
        Tween(Type Start, Type End, Real64 Time, Easing Function = Easing::Linear)
            : mStart       { Start },
              mEnd         { End },
              mTime        { Time },
              mAccumulator { 0 },
              mEasing      { Function }
        {
        }

        // -=(Undocumented)=-
        Type Tick(Real64 Delta)
        {
            mAccumulator = Min(mAccumulator + Delta, mTime);

            if constexpr (std::is_arithmetic_v<Type>)
            {
                return Lerp<Type>(mStart, mEnd, Ease(mEasing, mAccumulator / mTime));
            }
            else
            {
                return Type::Lerp(mStart, mEnd, Ease(mEasing, mAccumulator / mTime));
            }
        }

        // -=(Undocumented)=-
        Bool HasFinish() const
        {
            return mAccumulator == mTime;
        }

        // -=(Undocumented)=-
        Bool HasStarted() const
        {
            return mAccumulator != mTime;
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