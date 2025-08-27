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
#include <enchantum/enchantum.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Enum
{
    /// \brief Gets the string name of an enum value.
    /// 
    /// \param Value The enum value.
    /// \return The string representation of the enum value.
    template<typename Type>
    constexpr auto GetName(Type Value)
    {
        return enchantum::to_string(Value);
    }

    /// \brief Gets a list of all values of the enum type.
    /// 
    /// \return A constexpr array of all enum values.
    template<typename Enum>
    constexpr auto GetValues()
    {
        return enchantum::values<Enum>;
    }

    /// \brief Gets the underlying integer value of an enum.
    /// 
    /// \param Value The enum value.
    /// \return The integral representation of the enum.
    template<typename Enum>
    constexpr auto Cast(Enum Value)
    {
        return static_cast<__underlying_type(Enum)>(Value);
    }

    /// \brief Attempts to convert a string to an enum value.
    /// 
    /// \param Name    The string representation of the enum.
    /// \param Default The fallback enum value if no match is found.
    /// \return The matched enum value or the provided default.

    template<typename Enum>
    constexpr Enum Cast(ConstText Name, Enum Default)
    {
        return enchantum::cast<Enum>(Name, [](Char X, Char Y)
        {
            return std::tolower(X) == std::tolower(Y);
        }).value_or(Default);
    }

    /// \brief Gets the number of enumerators defined in the enum.
    /// 
    /// \return The number of enum values.
    template<typename Enum>
    constexpr UInt Count()
    {
        return enchantum::count<Enum>;
    }
}