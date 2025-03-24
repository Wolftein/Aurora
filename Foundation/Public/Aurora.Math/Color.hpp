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

        static constexpr UInt32 k_Transparent = 0x00000000;
        static constexpr UInt32 k_White       = 0xFFFFFFFF;
        static constexpr UInt32 k_Black       = 0x000000FF;
        static constexpr UInt32 k_Red         = 0xFF0000FF;
        static constexpr UInt32 k_Green       = 0x00FF00FF;
        static constexpr UInt32 k_Blue        = 0x0000FFFF;
        static constexpr UInt32 k_Yellow      = 0xFFFF00FF;
        static constexpr UInt32 k_Cyan        = 0x00FFFFFF;
        static constexpr UInt32 k_Magenta     = 0xFF00FFFF;
        static constexpr UInt32 k_Gray        = 0x444444FF;
        static constexpr UInt32 k_Orange      = 0xFFA500FF;
        static constexpr UInt32 k_Purple      = 0x800080FF;
        static constexpr UInt32 k_Brown       = 0xA52A2AFF;
        static constexpr UInt32 k_Pink        = 0xFFC0CBFF;
        static constexpr UInt32 k_Lime        = 0x32CD32FF;
        static constexpr UInt32 k_Teal        = 0x008080FF;
        static constexpr UInt32 k_Navy        = 0x000080FF;
        static constexpr UInt32 k_Silver      = 0xC0C0C0FF;
        static constexpr UInt32 k_Olive       = 0x808000FF;
        static constexpr UInt32 k_Maroon      = 0x800000FF;
        static constexpr UInt32 k_Aqua        = 0x00FFFF7F;
        static constexpr UInt32 k_Violet      = 0xEE82EEFF;
        static constexpr UInt32 k_Indigo      = 0x4B0082FF;
        static constexpr UInt32 k_Gold        = 0xFFD700FF;
        static constexpr UInt32 k_Chocolate   = 0xD2691EFF;
        static constexpr UInt32 k_Tomato      = 0xFF6347FF;
        static constexpr UInt32 k_Coral       = 0xFF7F50FF;
        static constexpr UInt32 k_Azure       = 0x007FFF7F;
        static constexpr UInt32 k_Mint        = 0xF5FFFAFF;
        static constexpr UInt32 k_Lavender    = 0xE6E6FAFF;
        static constexpr UInt32 k_Beige       = 0xF5F5DCFF;
        static constexpr UInt32 k_Salmon      = 0xFA8072FF;
        static constexpr UInt32 k_Khaki       = 0xF0E68CFF;
        static constexpr UInt32 k_Peach       = 0xFFE5B4FF;
        static constexpr UInt32 k_Turquoise   = 0x40E0D0FF;
        static constexpr UInt32 k_Ivory       = 0xFFFFF0FF;
        static constexpr UInt32 k_Plum        = 0xDDA0DDFF;
        static constexpr UInt32 k_Honeydew    = 0xF0FFF0FF;
        static constexpr UInt32 k_Wheat       = 0xF5DEB3FF;

    public:

        // -=(Undocumented)=-
        constexpr Color()
            : mValue { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
            : mValue { Pack(Red, Green, Blue, Alpha) }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(UInt32 Color)
            : mValue { Color }
        {
        }

        // -=(Undocumented)=-
        void SetRed(UInt32 Value)
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                mValue = (mValue & ~0xFFu) | (Value & 0xFFu);
            }
            else
            {
                mValue = (mValue & ~(0xFFu << 24)) | ((Value & 0xFFu) << 24);
            }
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetRed() const
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return mValue & 0xFFu;
            }
            else
            {
                return (mValue >> 24) & 0xFFu;
            }
        }

        // -=(Undocumented)=-
        void SetGreen(UInt32 Value)
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                mValue = (mValue & ~(0xFFu << 8)) | ((Value & 0xFFu) << 8);
            }
            else
            {
                mValue = (mValue & ~(0xFFu << 16)) | ((Value & 0xFFu) << 16);
            }
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetGreen() const
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return (mValue >> 8) & 0xFFu;
            }
            else
            {
                return (mValue >> 16) & 0xFFu;
            }
        }

        // -=(Undocumented)=-
        void SetBlue(UInt32 Value)
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                mValue = (mValue & ~(0xFFu << 16)) | ((Value & 0xFFu) << 16);
            }
            else
            {
                mValue = (mValue & ~(0xFFu << 8)) | ((Value & 0xFFu) << 8);
            }
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetBlue() const
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return (mValue >> 16) & 0xFFu;
            }
            else
            {
                return (mValue >> 8) & 0xFFu;
            }
        }

        // -=(Undocumented)=-
        void SetAlpha(UInt32 Value)
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                mValue = (mValue & ~(0xFFu << 24)) | ((Value & 0xFFu) << 24);
            }
            else
            {
                mValue = (mValue & ~0xFFu) | (Value & 0xFFu);
            }
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetAlpha() const
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return (mValue >> 24) & 0xFFu;
            }
            else
            {
                return mValue & 0xFFu;
            }
        }

        // -=(Undocumented)=-
        constexpr UInt32 GetValue() const
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
        static constexpr UInt32 Pack(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return   (static_cast<UInt32>(Alpha * UINT8_MAX) << 24)
                       | (static_cast<UInt32>(Blue  * UINT8_MAX) << 16)
                       | (static_cast<UInt32>(Green * UINT8_MAX) << 8)
                       | (static_cast<UInt32>(Red   * UINT8_MAX));
            }
            else
            {
                return   (static_cast<UInt32>(Red   * UINT8_MAX) << 24)
                       | (static_cast<UInt32>(Green * UINT8_MAX) << 16)
                       | (static_cast<UInt32>(Blue  * UINT8_MAX) << 8)
                       | (static_cast<UInt32>(Alpha * UINT8_MAX));
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32 mValue;
    };
}