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

#include "Primitive.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Bit
{
    /// \brief Checks whether all bits in the given mask are set in the field.
    /// 
    /// \param Field The field to check.
    /// \param Mask   The bitmask indicating which bits to compare.
    /// \return `true` if all bits in `Mask` are set in `Field`, `false` otherwise.
    template<typename Type, typename Value>
    constexpr Bool HasBit(Type Field, Value Mask)
    {
        return (Field & Mask) == Mask;
    }

    /// \brief Sets one or more bits in the field.
    /// 
    /// \param Field The original field value to modify.
    /// \param Mask  The bitmask indicating which bits to set.
    /// \return A new value with the specified bits set.
    template<typename Type, typename Value>
    constexpr Type SetBit(Type Field, Value Mask)
    {
        return Field | Mask;
    }

    /// \brief Clears one or more bits in the field.
    /// 
    /// \param Field The original field value to modify.
    /// \param Mask  The bitmask indicating which bits to clear.
    /// \return A new value with the specified bits cleared.
    template<typename Type, typename Value>
    constexpr Type ClearBit(Type Field, Value Mask)
    {
        return Field & ~Mask;
    }

    /// \brief Conditionally sets or clears bits in a field based on a boolean flag.
    ///
    /// \param Field  The original field value to modify.
    /// \param Mask   The bitmask indicating which bits to set or clear.
    /// \param Enable If `true`, the bits specified by the mask are set, otherwise are cleared.
    /// \return A new value with the specified bits either set or cleared based on the condition.
    template<typename Type, typename Value>
    constexpr Type SetOrClearBit(Type Field, Value Mask, Bool Enable)
    {
        return (Enable ? SetBit(Field, Mask) : ClearBit(Field, Mask));
    }

    /// \brief Packs two 16-bit values into a 32-bit integer with endian-aware layout.
    /// 
    /// \param Low  The lower 16 bits.
    /// \param High The upper 16 bits.
    /// \return A 32-bit packed integer.
    constexpr UInt32 PackInt16x2(UInt16 Low, UInt16 High)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return (static_cast<UInt32>(High) << 0x10) | Low;
        }
        else
        {
            return (static_cast<UInt32>(Low) << 0x10) | High;
        }
    }

    /// \brief Extracts the lower 16 bits from a packed 32-bit integer.
    /// 
    /// \param Value A packed 32-bit integer.
    /// \return The lower 16 bits.
    constexpr UInt16 UnpackLowInt16(UInt32 Value)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return static_cast<UInt16>(Value & 0xFFFF);
        }
        else
        {
            return static_cast<UInt16>(Value >> 0x10);
        }
    }

    /// \brief Extracts the upper 16 bits from a packed 32-bit integer.
    /// 
    /// \param Value A packed 32-bit integer.
    /// \return The upper 16 bits.
    constexpr UInt16 UnpackHighInt16(UInt32 Value)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return static_cast<UInt16>(Value >> 0x10);
        }
        else
        {
            return static_cast<UInt16>(Value & 0xFFFF);
        }
    }

    /// \brief Packs two 32-bit integers into a 64-bit integer with endian-aware layout.
    /// 
    /// \param Low  The lower 32 bits.
    /// \param High The upper 32 bits.
    /// \return A 64-bit packed integer.
    constexpr UInt64 PackInt32x2(UInt32 Low, UInt32 High)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return (static_cast<UInt64>(High) << 0x20) | Low;
        }
        else
        {
            return (static_cast<UInt64>(Low) << 0x20) | High;
        }
    }

    /// \brief Extracts the lower 32 bits from a packed 64-bit integer.
    /// 
    /// \param Value A packed 64-bit integer.
    /// \return The lower 32 bits.
    constexpr UInt32 UnpackLowInt32(UInt64 Value)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return static_cast<UInt32>(Value & 0xFFFFFFFF);
        }
        else
        {
            return static_cast<UInt32>(Value >> 0x20);
        }
    }

    /// \brief Extracts the upper 32 bits from a packed 64-bit integer.
    /// 
    /// \param Value A packed 64-bit integer.
    /// \return The upper 32 bits.
    constexpr UInt32 UnpackHighInt32(UInt64 Value)
    {
        if constexpr (AURORA_LITTLE_ENDIAN)
        {
            return static_cast<UInt32>(Value >> 0x20);
        }
        else
        {
            return static_cast<UInt32>(Value & 0xFFFFFFFF);
        }
    }
}