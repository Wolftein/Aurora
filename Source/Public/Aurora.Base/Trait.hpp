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

#include "Type.hpp"
#include <magic_enum/magic_enum.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Type, typename Function>
    inline constexpr void Sort(Ref<Type> Collection, Function Predicate)
    {
        std::sort(Collection.begin(), Collection.end(), Predicate);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr Type Min(Type First, Type Second)
    {
        return First < Second ? First : Second;
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr Type Max(Type First, Type Second)
    {
        return First > Second ? First : Second;
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr Type Clamp(Type Value, Type  Minimum, Type Maximum)
    {
        return Min(Max(Value, Minimum), Maximum);
    }

    // -=(Undocumented)=-
    inline constexpr UInt Align(UInt Value, UInt Alignment)
    {
        return (Value + (Alignment - 1)) & ~(Alignment - 1);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr auto Move(Any<Type> Object)
    {
        return std::move(Object);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline SPtr<Type> NewPtr(Any<Args> ... Arguments)
    {
        return std::make_shared<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline UPtr<Type> NewUniquePtr(Any<Args> ... Arguments)
    {
        return std::make_unique<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename Base>
    inline SPtr<Type> CastPtr(ConstSPtr<Base> Pointer)
    {
        return std::static_pointer_cast<Type>(Pointer);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr auto CastEnum(Type Value)
    {
        return static_cast<std::underlying_type_t<Type>>(Value);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr auto CastEnum(CStr Value, Type Default)
    {
        const auto Result = magic_enum::enum_cast<Type>(Value, magic_enum::case_insensitive);
        return (Result.has_value() ? Result.value() : Default);
    }

    // -=(Undocumented)=-
    template<typename... Args>
    inline constexpr auto Format(CStr Format, Any<Args> ... Arguments)
    {
        return std::vformat(Format, std::make_format_args(Arguments...));
    }

    // -=(Undocumented)=-
    template<typename... Args>
    inline constexpr auto Format(CStr16 Format, Any<Args> ... Arguments)
    {
        return std::vformat(Format, std::make_wformat_args(Arguments...));
    }

    // -=(Undocumented)=-
    template <class Type>
    inline constexpr auto AddressOf(Ref<Type> Value)
    {
        return std::addressof(Value);
    }

    // -=(Undocumented)=-
    template<typename Type, Type Mask, Type Shift>
    inline constexpr Type GetBit(Type Field)
    {
        return (Field & Mask) >> Shift;
    }

    // -=(Undocumented)=-
    template<typename Type, Type Mask, Type Shift>
    inline constexpr Type SetBit(Type Field, Type Value)
    {
        return (Field & ~Mask) | ((Value << Shift) & Mask);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr Type GetMask(Type Count)
    {
        return static_cast<Type>(-(Count != 0)) & (static_cast<Type>(-1) >> ((sizeof(Type) * CHAR_BIT) - Count));
    }
}