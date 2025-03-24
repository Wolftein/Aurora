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

#include "Bit.hpp"
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

        static constexpr UInt32 k_Transparent = Float4ToUInt32(0.0f, 0.0f, 0.0f, 0.0f);     // 0x00000000
        static constexpr UInt32 k_White       = Float4ToUInt32(1.0f, 1.0f, 1.0f, 1.0f);     // 0xFFFFFFFF
        static constexpr UInt32 k_Black       = Float4ToUInt32(0.0f, 0.0f, 0.0f, 1.0f);     // 0x000000FF
        static constexpr UInt32 k_Red         = Float4ToUInt32(1.0f, 0.0f, 0.0f, 1.0f);     // 0xFF0000FF
        static constexpr UInt32 k_Green       = Float4ToUInt32(0.0f, 1.0f, 0.0f, 1.0f);     // 0x00FF00FF
        static constexpr UInt32 k_Blue        = Float4ToUInt32(0.0f, 0.0f, 1.0f, 1.0f);     // 0x0000FFFF
        static constexpr UInt32 k_Yellow      = Float4ToUInt32(1.0f, 1.0f, 0.0f, 1.0f);     // 0xFFFF00FF
        static constexpr UInt32 k_Cyan        = Float4ToUInt32(0.0f, 1.0f, 1.0f, 1.0f);     // 0x00FFFFFF
        static constexpr UInt32 k_Magenta     = Float4ToUInt32(1.0f, 0.0f, 1.0f, 1.0f);     // 0xFF00FFFF
        static constexpr UInt32 k_Gray        = Float4ToUInt32(0.27f, 0.27f, 0.27f, 1.0f);  // 0x444444FF
        static constexpr UInt32 k_Orange      = Float4ToUInt32(1.0f, 0.65f, 0.0f, 1.0f);    // 0xFFA500FF
        static constexpr UInt32 k_Purple      = Float4ToUInt32(0.5f, 0.0f, 0.5f, 1.0f);     // 0x800080FF
        static constexpr UInt32 k_Brown       = Float4ToUInt32(0.65f, 0.16f, 0.16f, 1.0f);  // 0xA52A2AFF
        static constexpr UInt32 k_Pink        = Float4ToUInt32(1.0f, 0.75f, 0.8f, 1.0f);    // 0xFFC0CBFF
        static constexpr UInt32 k_Lime        = Float4ToUInt32(0.2f, 0.8f, 0.2f, 1.0f);     // 0x32CD32FF
        static constexpr UInt32 k_Teal        = Float4ToUInt32(0.0f, 0.5f, 0.5f, 1.0f);     // 0x008080FF
        static constexpr UInt32 k_Navy        = Float4ToUInt32(0.0f, 0.0f, 0.5f, 1.0f);     // 0x000080FF
        static constexpr UInt32 k_Silver      = Float4ToUInt32(0.75f, 0.75f, 0.75f, 1.0f);  // 0xC0C0C0FF
        static constexpr UInt32 k_Olive       = Float4ToUInt32(0.5f, 0.5f, 0.0f, 1.0f);     // 0x808000FF
        static constexpr UInt32 k_Maroon      = Float4ToUInt32(0.5f, 0.0f, 0.0f, 1.0f);     // 0x800000FF
        static constexpr UInt32 k_Aqua        = Float4ToUInt32(0.0f, 1.0f, 1.0f, 0.5f);     // 0x00FFFF7F
        static constexpr UInt32 k_Violet      = Float4ToUInt32(0.93f, 0.51f, 0.93f, 1.0f);  // 0xEE82EEFF
        static constexpr UInt32 k_Indigo      = Float4ToUInt32(0.29f, 0.0f, 0.51f, 1.0f);   // 0x4B0082FF
        static constexpr UInt32 k_Gold        = Float4ToUInt32(1.0f, 0.84f, 0.0f, 1.0f);    // 0xFFD700FF
        static constexpr UInt32 k_Chocolate   = Float4ToUInt32(0.82f, 0.41f, 0.12f, 1.0f);  // 0xD2691EFF
        static constexpr UInt32 k_Tomato      = Float4ToUInt32(1.0f, 0.39f, 0.28f, 1.0f);   // 0xFF6347FF
        static constexpr UInt32 k_Coral       = Float4ToUInt32(1.0f, 0.5f, 0.31f, 1.0f);    // 0xFF7F50FF
        static constexpr UInt32 k_Azure       = Float4ToUInt32(0.0f, 0.5f, 1.0f, 0.5f);     // 0x007FFF7F
        static constexpr UInt32 k_Mint        = Float4ToUInt32(0.96f, 1.0f, 0.98f, 1.0f);   // 0xF5FFFAFF
        static constexpr UInt32 k_Lavender    = Float4ToUInt32(0.9f, 0.9f, 0.98f, 1.0f);    // 0xE6E6FAFF
        static constexpr UInt32 k_Beige       = Float4ToUInt32(0.96f, 0.96f, 0.86f, 1.0f);  // 0xF5F5DCFF
        static constexpr UInt32 k_Salmon      = Float4ToUInt32(0.98f, 0.5f, 0.45f, 1.0f);   // 0xFA8072FF
        static constexpr UInt32 k_Khaki       = Float4ToUInt32(0.94f, 0.9f, 0.55f, 1.0f);   // 0xF0E68CFF
        static constexpr UInt32 k_Peach       = Float4ToUInt32(1.0f, 0.9f, 0.7f, 1.0f);     // 0xFFE5B4FF
        static constexpr UInt32 k_Turquoise   = Float4ToUInt32(0.25f, 0.88f, 0.82f, 1.0f);  // 0x40E0D0FF
        static constexpr UInt32 k_Ivory       = Float4ToUInt32(1.0f, 1.0f, 0.94f, 1.0f);    // 0xFFFFF0FF
        static constexpr UInt32 k_Plum        = Float4ToUInt32(0.87f, 0.63f, 0.87f, 1.0f);  // 0xDDA0DDFF
        static constexpr UInt32 k_Honeydew    = Float4ToUInt32(0.94f, 1.0f, 0.94f, 1.0f);   // 0xF0FFF0FF
        static constexpr UInt32 k_Wheat       = Float4ToUInt32(0.96f, 0.87f, 0.7f, 1.0f);   // 0xF5DEB3FF

    public:

        // -=(Undocumented)=-
        constexpr Color()
            : mValue { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
            : mValue { Float4ToUInt32(Red, Green, Blue, Alpha) }
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

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32 mValue;
    };
}