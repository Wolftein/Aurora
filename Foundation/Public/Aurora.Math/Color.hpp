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

#include "Trigonometry.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    class Color final
    {
    public:

        // -=(Undocumented)=-
        constexpr Color()
            : mValue { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
            : mValue { PackFloatColor(Red, Green, Blue, Alpha) }
        {
        }

        // -=(Undocumented)=-
        explicit constexpr Color(UInt32 Color)
            : mValue { Color }
        {
        }

        // -=(Undocumented)=-
        void SetRed(UInt32 Value)
        {
            mValue = (mValue & ~(0xFF << 24)) | ((Value & 0xFF) << 24);
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetRed() const
        {
            return mValue >> 24 & 0xFF;
        }

        // -=(Undocumented)=-
        void SetGreen(UInt32 Value)
        {
            mValue = mValue & ~(0xFF << 16) | (Value & 0xFF) << 16;
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetGreen() const
        {
            return mValue >> 16 & 0xFF;
        }

        // -=(Undocumented)=-
        void SetBlue(UInt32 Value)
        {
            mValue = mValue & ~(0xFF << 8) | (Value & 0xFF) << 8;
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetBlue() const
        {
            return ((mValue >> 8) & 0xFF);
        }

        // -=(Undocumented)=-
        void SetAlpha(UInt32 Value)
        {
            mValue = mValue & ~0xFF | Value & 0xFF;
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetAlpha() const
        {
            return mValue & 0xFF;
        }

        // -=(Undocumented)=-
        constexpr UInt32 AsPacked() const
        {
            return mValue;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeNumber(mValue);
        }

    public:

        // -=(Undocumented)=-
        static Color Lerp(ConstRef<Color> Start, ConstRef<Color> End, Real32 Percentage)
        {
            const Real32 Red   = Math::Lerp(Start.GetRed(),   End.GetRed(),   Percentage);
            const Real32 Green = Math::Lerp(Start.GetGreen(), End.GetGreen(), Percentage);
            const Real32 Blue  = Math::Lerp(Start.GetBlue(),  End.GetBlue(),  Percentage);
            const Real32 Alpha = Math::Lerp(Start.GetAlpha(), End.GetAlpha(), Percentage);
            return Color(Red, Green, Blue, Alpha);
        }

    private:

        // -=(Undocumented)=-
        static constexpr UInt32 PackFloatColor(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
        {
            if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                return (static_cast<UInt32>(Red   * UINT8_MAX) << 24)
                     | (static_cast<UInt32>(Green * UINT8_MAX) << 16)
                     | (static_cast<UInt32>(Blue  * UINT8_MAX) << 8)
                     | (static_cast<UInt32>(Alpha * UINT8_MAX));
            }
            else
            {
                return (static_cast<UInt32>(Alpha * UINT8_MAX) << 24)
                    | (static_cast<UInt32>(Blue   * UINT8_MAX) << 16)
                    | (static_cast<UInt32>(Green  * UINT8_MAX) << 8)
                    | (static_cast<UInt32>(Red    * UINT8_MAX));
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32 mValue;
    };
}