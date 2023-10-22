// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
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
    template<typename Type>
    inline constexpr UInt Hash(Type String, UInt Length = UINT_MAX)
    {
        return (Length && (* String)) ? (Hash(String + 1u, Length - 1u) * 33u) ^ (* String & ~32u) : 5381u;
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline SPtr<Type> NewPtr(Args && ... Arguments)
    {
        return std::make_shared<Type>(Arguments...);
    }

    // -=(Undocumented)=-
    template<typename Type, typename... Args>
    inline UPtr<Type> NewUniquePtr(Args && ... Arguments)
    {
        return std::make_unique<Type>(Arguments...);
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
    inline auto FastCopyMemory(Ptr<void> Destination, Ptr<const void> Source, UInt Size)
    {
        memcpy(Destination, Source, Size);
    }
}