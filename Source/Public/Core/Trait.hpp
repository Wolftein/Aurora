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

#include "Types.hpp"
#include <algorithm>
#include <format>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
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
    inline constexpr Type Within(Type Value, Type  Minimum, Type Maximum)
    {
        return Min(Max(Value, Minimum), Maximum);
    }
    
    // -=(Undocumented)=-
    template<typename Enum>
    inline constexpr auto CastEnum(Enum Value)
    {
        return static_cast<std::underlying_type_t<Enum>>(Value);
    }

    // -=(Undocumented)=-
    template<UInt Alignment>
    inline constexpr UInt Align(UInt Value)
    {
        return (Value + (Alignment - 1)) & ~(Alignment - 1);
    }

    // -=(Undocumented)=-
    inline constexpr UInt Align(UInt Value, UInt Alignment)
    {
        return (Value + (Alignment - 1)) & ~(Alignment - 1);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline constexpr UInt Hash(Type String, UInt Length = UINT_MAX)
    {
        return (Length && (* String)) ? (Hash(String + 1u, Length - 1u) * 33u) ^ (* String & ~32u) : 5381u;
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline SPtr<Type> NewPtr(Args && ... Arguments)
    {
        return std::make_shared<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline UPtr<Type> NewUniquePtr(Args && ... Arguments)
    {
        return std::make_unique<Type>(std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template<typename Destination, typename Source>
    inline SPtr<Destination> CastPtr(SPtr<Source> Pointer)
    {
        return std::static_pointer_cast<Destination>(Pointer);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline auto Move(Type && Object)
    {
        return std::move(Object);
    }

    // -=(Undocumented)=-
    template<typename Type, typename Primitive = Type>
    inline auto Exchange(Type & Object, Primitive && Value)
    {
        return std::exchange(Object, Value);
    }

    // -=(Undocumented)=-
    template<Bool Cacheable = true>
    inline void FastCopyMemory(Ptr<void> Destination, Ptr<const void> Source, UInt Size)
    {
        memcpy(Destination, Source, Size);
    }

    // -=(Undocumented)=-
    template<typename... Args>
    inline auto Format(CStr Format, Args&&... Arguments)
    {
        return std::vformat(Format, std::make_format_args(Arguments...));
    }

    // -=(Undocumented)=-
    template<typename... Args>
    inline auto Format(CStr16 Format, Args&&... Arguments)
    {
        return std::vformat(Format, std::make_wformat_args(Arguments...));
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
        return static_cast<Type>(-(Count != 0))
            & (static_cast<Type>(-1) >> ((sizeof(Type) * CHAR_BIT) - Count));
    }
}