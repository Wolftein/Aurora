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
    constexpr void Sort(Ref<Type> Collection, Function Predicate)
    {
        std::sort(Collection.begin(), Collection.end(), Predicate);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type Min(Type First, Type Second)
    {
        return First < Second ? First : Second;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type Max(Type First, Type Second)
    {
        return First > Second ? First : Second;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type Clamp(Type Value, Type  Minimum, Type Maximum)
    {
        return Min(Max(Value, Minimum), Maximum);
    }

    // -=(Undocumented)=-
    constexpr UInt Align(UInt Value, UInt Alignment)
    {
        return Value + (Alignment - 1) & ~(Alignment - 1);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto Move(Any<Type> Object)
    {
        return std::move(Object);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr void Swap(Ref<Type> First, Ref<Type> Second)
    {
        std::swap(First, Second);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    SPtr<Type> NewPtr(Any<Args> ... Arguments)
    {
        return std::make_shared<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    UPtr<Type> NewUniquePtr(Any<Args> ... Arguments)
    {
        return std::make_unique<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename Base>
    SPtr<Type> CastPtr(ConstSPtr<Base> Pointer)
    {
        return std::static_pointer_cast<Type>(Pointer);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto CastSpan(Ref<Type> Value)
    {
        return CPtr<Type>(AddressOf(Value), 1);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto CastSpan(ConstRef<Type> Value)
    {
        return CPtr<const Type>(AddressOf(Value), 1);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto CastEnum(Type Value)
    {
        return static_cast<std::underlying_type_t<Type>>(Value);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto CastEnum(CStr Value, Type Default)
    {
        const auto Result = magic_enum::enum_cast<Type>(Value, magic_enum::case_insensitive);
        return Result.has_value() ? Result.value() : Default;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr auto CountEnum()
    {
        return magic_enum::enum_count<Type>();
    }

    // -=(Undocumented)=-
    template<typename... Args>
    constexpr auto Format(CStr Format, Any<Args> ... Arguments)
    {
        return std::vformat(Format, std::make_format_args(Arguments...));
    }

    // -=(Undocumented)=-
    template<typename... Args>
    constexpr auto Format(CStr16 Format, Any<Args> ... Arguments)
    {
        return std::vformat(Format, std::make_wformat_args(Arguments...));
    }

    // -=(Undocumented)=-
    template <class Type>
    constexpr auto AddressOf(Ref<Type> Value)
    {
        return std::addressof(Value);
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type GetMask(Type Count)
    {
        return static_cast<Type>(-(Count != 0)) & static_cast<Type>(-1) >> (sizeof(Type) * CHAR_BIT - Count);
    }

    // -=(Undocumented)=-
    template<typename Type, Type Mask, Type Shift>
    constexpr Type GetBit(Type Field)
    {
        return (Field & Mask) >> Shift;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Bool HasBit(Type Field, Type Value)
    {
        return (Field & Value) == Value;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type SetBit(Type Field, Type Value)
    {
        return Field | Value;
    }

    // -=(Undocumented)=-
    template<typename Type, Type Mask, Type Shift>
    constexpr Type SetBit(Type Field, Type Value)
    {
        return Field & ~Mask | Value << Shift & Mask;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr Type ClearBit(Type Field, Type Value)
    {
        return Field & ~Value;
    }

    // -=(Undocumented)=-
    template<typename Type>
    constexpr CStr Name()
    {
        return { __FUNCSIG__ };
    }

    // -=(Undocumented)=-
    constexpr UInt Hash(CStr Block, UInt Counter = 14695981039346656037ull)
    {
        return Block.size() > 0
            ? Hash(CStr(Block.data() + 1, Block.size() - 1), (Counter ^ Block[0]) * 1099511628211ull)
            : Counter;
    }

    // -=(Undocumented)=-
    template<typename Type>
    consteval UInt Hash()
    {
        return Hash(Name<Type>());
    }
}