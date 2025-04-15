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

        static constexpr UInt32 k_Transparent = CombineFloat32x4(0.0f, 0.0f, 0.0f, 0.0f);     // 0x00000000
        static constexpr UInt32 k_White       = CombineFloat32x4(1.0f, 1.0f, 1.0f, 1.0f);     // 0xFFFFFFFF
        static constexpr UInt32 k_Black       = CombineFloat32x4(0.0f, 0.0f, 0.0f, 1.0f);     // 0x000000FF
        static constexpr UInt32 k_Red         = CombineFloat32x4(1.0f, 0.0f, 0.0f, 1.0f);     // 0xFF0000FF
        static constexpr UInt32 k_Green       = CombineFloat32x4(0.0f, 1.0f, 0.0f, 1.0f);     // 0x00FF00FF
        static constexpr UInt32 k_Blue        = CombineFloat32x4(0.0f, 0.0f, 1.0f, 1.0f);     // 0x0000FFFF
        static constexpr UInt32 k_Yellow      = CombineFloat32x4(1.0f, 1.0f, 0.0f, 1.0f);     // 0xFFFF00FF
        static constexpr UInt32 k_Cyan        = CombineFloat32x4(0.0f, 1.0f, 1.0f, 1.0f);     // 0x00FFFFFF
        static constexpr UInt32 k_Magenta     = CombineFloat32x4(1.0f, 0.0f, 1.0f, 1.0f);     // 0xFF00FFFF
        static constexpr UInt32 k_Gray        = CombineFloat32x4(0.27f, 0.27f, 0.27f, 1.0f);  // 0x444444FF
        static constexpr UInt32 k_Orange      = CombineFloat32x4(1.0f, 0.65f, 0.0f, 1.0f);    // 0xFFA500FF
        static constexpr UInt32 k_Purple      = CombineFloat32x4(0.5f, 0.0f, 0.5f, 1.0f);     // 0x800080FF
        static constexpr UInt32 k_Brown       = CombineFloat32x4(0.65f, 0.16f, 0.16f, 1.0f);  // 0xA52A2AFF
        static constexpr UInt32 k_Pink        = CombineFloat32x4(1.0f, 0.75f, 0.8f, 1.0f);    // 0xFFC0CBFF
        static constexpr UInt32 k_Lime        = CombineFloat32x4(0.2f, 0.8f, 0.2f, 1.0f);     // 0x32CD32FF
        static constexpr UInt32 k_Teal        = CombineFloat32x4(0.0f, 0.5f, 0.5f, 1.0f);     // 0x008080FF
        static constexpr UInt32 k_Navy        = CombineFloat32x4(0.0f, 0.0f, 0.5f, 1.0f);     // 0x000080FF
        static constexpr UInt32 k_Silver      = CombineFloat32x4(0.75f, 0.75f, 0.75f, 1.0f);  // 0xC0C0C0FF
        static constexpr UInt32 k_Olive       = CombineFloat32x4(0.5f, 0.5f, 0.0f, 1.0f);     // 0x808000FF
        static constexpr UInt32 k_Maroon      = CombineFloat32x4(0.5f, 0.0f, 0.0f, 1.0f);     // 0x800000FF
        static constexpr UInt32 k_Aqua        = CombineFloat32x4(0.0f, 1.0f, 1.0f, 0.5f);     // 0x00FFFF7F
        static constexpr UInt32 k_Violet      = CombineFloat32x4(0.93f, 0.51f, 0.93f, 1.0f);  // 0xEE82EEFF
        static constexpr UInt32 k_Indigo      = CombineFloat32x4(0.29f, 0.0f, 0.51f, 1.0f);   // 0x4B0082FF
        static constexpr UInt32 k_Gold        = CombineFloat32x4(1.0f, 0.84f, 0.0f, 1.0f);    // 0xFFD700FF
        static constexpr UInt32 k_Chocolate   = CombineFloat32x4(0.82f, 0.41f, 0.12f, 1.0f);  // 0xD2691EFF
        static constexpr UInt32 k_Tomato      = CombineFloat32x4(1.0f, 0.39f, 0.28f, 1.0f);   // 0xFF6347FF
        static constexpr UInt32 k_Coral       = CombineFloat32x4(1.0f, 0.5f, 0.31f, 1.0f);    // 0xFF7F50FF
        static constexpr UInt32 k_Azure       = CombineFloat32x4(0.0f, 0.5f, 1.0f, 0.5f);     // 0x007FFF7F
        static constexpr UInt32 k_Mint        = CombineFloat32x4(0.96f, 1.0f, 0.98f, 1.0f);   // 0xF5FFFAFF
        static constexpr UInt32 k_Lavender    = CombineFloat32x4(0.9f, 0.9f, 0.98f, 1.0f);    // 0xE6E6FAFF
        static constexpr UInt32 k_Beige       = CombineFloat32x4(0.96f, 0.96f, 0.86f, 1.0f);  // 0xF5F5DCFF
        static constexpr UInt32 k_Salmon      = CombineFloat32x4(0.98f, 0.5f, 0.45f, 1.0f);   // 0xFA8072FF
        static constexpr UInt32 k_Khaki       = CombineFloat32x4(0.94f, 0.9f, 0.55f, 1.0f);   // 0xF0E68CFF
        static constexpr UInt32 k_Peach       = CombineFloat32x4(1.0f, 0.9f, 0.7f, 1.0f);     // 0xFFE5B4FF
        static constexpr UInt32 k_Turquoise   = CombineFloat32x4(0.25f, 0.88f, 0.82f, 1.0f);  // 0x40E0D0FF
        static constexpr UInt32 k_Ivory       = CombineFloat32x4(1.0f, 1.0f, 0.94f, 1.0f);    // 0xFFFFF0FF
        static constexpr UInt32 k_Plum        = CombineFloat32x4(0.87f, 0.63f, 0.87f, 1.0f);  // 0xDDA0DDFF
        static constexpr UInt32 k_Honeydew    = CombineFloat32x4(0.94f, 1.0f, 0.94f, 1.0f);   // 0xF0FFF0FF
        static constexpr UInt32 k_Wheat       = CombineFloat32x4(0.96f, 0.87f, 0.7f, 1.0f);   // 0xF5DEB3FF

    public:

        // -=(Undocumented)=-
        constexpr Color()
            : mValue { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha)
            : mValue { CombineFloat32x4(Red, Green, Blue, Alpha) }
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