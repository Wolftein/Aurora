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

#include "Aurora.Base/Type.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    constexpr UInt32 CombineFloat32x4(Real32 X, Real32 Y, Real32 Z, Real32 W)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return (static_cast<UInt32>(W * UINT8_MAX) << 24)
                 | (static_cast<UInt32>(Z * UINT8_MAX) << 16)
                 | (static_cast<UInt32>(Y * UINT8_MAX) << 8)
                 | (static_cast<UInt32>(X * UINT8_MAX));
        }
        else
        {
            return (static_cast<UInt32>(X * UINT8_MAX) << 24)
                 | (static_cast<UInt32>(Y * UINT8_MAX) << 16)
                 | (static_cast<UInt32>(Z * UINT8_MAX) << 8)
                 | (static_cast<UInt32>(W * UINT8_MAX));
        }
    }

    // -=(Undocumented)=-
    constexpr UInt32 CombineUInt16x2(UInt16 Lower, UInt16 Upper)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return (static_cast<UInt32>(Upper) << 16) | static_cast<UInt32>(Lower);
        }
        else
        {
            return (static_cast<UInt32>(Lower) << 16) | static_cast<UInt32>(Upper);
        }
    }

    // -=(Undocumented)=-
    constexpr UInt16 GetLowerUInt16(UInt32 Value)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return static_cast<UInt16>(Value & 0xFFFF);
        }
        else
        {
            return static_cast<UInt16>(Value >> 16);
        }
    }

    // -=(Undocumented)=-
    constexpr UInt16 GetUpperUInt16(UInt32 Value)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return static_cast<UInt16>(Value >> 16);
        }
        else
        {
            return static_cast<UInt16>(Value & 0xFFFF);
        }
    }

    // -=(Undocumented)=-
    constexpr UInt64 CombineUInt32x2(UInt32 Lower, UInt32 Upper)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return (static_cast<UInt64>(Upper) << 32) | Lower;
        }
        else
        {
            return (static_cast<UInt64>(Lower) << 32) | Upper;
        }
    }

    // -=(Undocumented)=-
    constexpr UInt32 GetLowerUInt32(UInt64 Value)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return static_cast<UInt32>(Value & 0xFFFFFFFF);
        }
        else
        {
            return static_cast<UInt32>(Value >> 32);
        }
    }

    // -=(Undocumented)=-
    constexpr UInt32 GetUpperUInt32(UInt64 Value)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return static_cast<UInt32>(Value >> 32);
        }
        else
        {
            return static_cast<UInt32>(Value & 0xFFFFFFFF);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type Flatten2DTo1D(Type X, Type Y, Type Width)
    {
        return Y * Width + X;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type Flatten3DTo1D(Type X, Type Y, Type Z, Type Width, Type Height)
    {
        return Z * (Width * Height) + Y * Width + X;
    }
}