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
    class Color final : public Serializable<Color>
    {
    public:

        // -=(Undocumented)=-
        constexpr Color()
            : mComponents { 0.0f, 0.0f, 0.0f, 0.0f }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
            : mComponents { Red, Green, Blue, Alpha }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(UInt32 Color)
            : mComponents { GetComponents(Color) }
        {
        }

        // -=(Undocumented)=-
        void SetRed(Real32 Value)
        {
            mComponents[0] = Value;
        }

        // -=(Undocumented)=-
        constexpr Real32 GetRed() const
        {
            return mComponents[0];
        }

        // -=(Undocumented)=-
        void SetGreen(Real32 Value)
        {
            mComponents[1] = Value;
        }

        // -=(Undocumented)=-
        constexpr Real32 GetGreen() const
        {
            return mComponents[1];
        }

        // -=(Undocumented)=-
        void SetBlue(Real32 Value)
        {
            mComponents[2] = Value;
        }

        // -=(Undocumented)=-
        constexpr Real32 GetBlue() const
        {
            return mComponents[2];
        }

        // -=(Undocumented)=-
        void SetAlpha(Real32 Value)
        {
            mComponents[3] = Value;
        }

        // -=(Undocumented)=-
        constexpr Real32 GetAlpha() const
        {
            return mComponents[3];
        }

        // -=(Undocumented)=-
        constexpr UInt32 AsPacked() const
        {
            return (static_cast<UInt32>(GetRed()   * UINT8_MAX) << 24
                  | static_cast<UInt32>(GetGreen() * UINT8_MAX) << 16
                  | static_cast<UInt32>(GetBlue()  * UINT8_MAX) << 8
                  | static_cast<UInt32>(GetAlpha() * UINT8_MAX));
        }

        // \see Serializable::OnSerialize
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeNumber(mComponents[0]);
            Archive.SerializeNumber(mComponents[1]);
            Archive.SerializeNumber(mComponents[2]);
            Archive.SerializeNumber(mComponents[3]);
        }

    public:

        // -=(Undocumented)=-
        static Color Lerp(Ref<const Color> Start, Ref<const Color> End, Real32 Percentage)
        {
            const Real32 Red   = Math::Lerp(Start.GetRed(),   End.GetRed(),   Percentage);
            const Real32 Green = Math::Lerp(Start.GetGreen(), End.GetGreen(), Percentage);
            const Real32 Blue  = Math::Lerp(Start.GetBlue(),  End.GetBlue(),  Percentage);
            const Real32 Alpha = Math::Lerp(Start.GetAlpha(), End.GetAlpha(), Percentage);
            return Color(Red, Green, Blue, Alpha);
        }

    private:

        // -=(Undocumented)=-
        static constexpr Array<Real32, 4> GetComponents(UInt32 Value)
        {
            const Real32 Red   = (Value >> 24) & 0xFF;
            const Real32 Green = (Value >> 16) & 0xFF;
            const Real32 Blue  = (Value >> 8 ) & 0xFF;
            const Real32 Alpha = (Value      ) & 0xFF;
            return Array<Real32, 4> { Red / UINT8_MAX, Green / UINT8_MAX, Blue / UINT8_MAX, Alpha / UINT8_MAX };
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Real32, 4> mComponents;
    };
}