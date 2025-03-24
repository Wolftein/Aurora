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
    constexpr UInt32 Float4ToUInt32(Real32 X, Real32 Y, Real32 Z, Real32 W)
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
    constexpr UInt32 UInt16x2ToUInt32(UInt16 X, UInt16 Y)
    {
        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            return (static_cast<UInt32>(Y) << 16) | static_cast<UInt32>(X);
        }
        else
        {
            return (static_cast<UInt32>(X) << 16) | static_cast<UInt32>(Y);
        }
    }
}