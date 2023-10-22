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

#include <any>
#include <array>
#include <bitset>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <EASTL/span.h>
#include <EASTL/fixed_vector.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    using Any    = std::any;

    // -=(Undocumented)=-
    using Bool   = bool;

    // -=(Undocumented)=-
    using SInt   = std::intptr_t;

    // -=(Undocumented)=-
    using SInt08 = std::int8_t;

    // -=(Undocumented)=-
    using SInt16 = std::int16_t;

    // -=(Undocumented)=-
    using SInt32 = std::int32_t;

    // -=(Undocumented)=-
    using SInt64 = std::int64_t;

    // -=(Undocumented)=-
    using UInt   = std::uintptr_t;

    // -=(Undocumented)=-
    using UInt08 = std::uint8_t;

    // -=(Undocumented)=-
    using UInt16 = std::uint16_t;

    // -=(Undocumented)=-
    using UInt32 = std::uint32_t;

    // -=(Undocumented)=-
    using UInt64 = std::uint64_t;

    // -=(Undocumented)=-
    using Real   = double_t;

    // -=(Undocumented)=-
    using Real32 = float_t;

    // -=(Undocumented)=-
    using Real64 = double_t;

    // -=(Undocumented)=-
    using SStr   = std::string;

    // -=(Undocumented)=-
    using CStr   = std::string_view;

    // -=(Undocumented)=-
    using Char   = SStr::value_type;

    // -=(Undocumented)=-
    using SStr16 = std::u16string;

    // -=(Undocumented)=-
    using CStr16 = std::u16string_view;

    // -=(Undocumented)=-
    using Char16 = SStr16::value_type;

    // -=(Undocumented)=-
    using SStr32 = std::u32string;

    // -=(Undocumented)=-
    using CStr32 = std::u32string_view;

    // -=(Undocumented)=-
    using Char32 = SStr32::value_type;

    // -=(Undocumented)=-
#define STRING_FIX(Value) SStr(Value)  // TODO: Remove this until heterogeous lookup is fixed

    // -=(Undocumented)=-
    template<typename Type>
    using Ptr    = typename std::add_pointer<Type>::type;

    // -=(Undocumented)=-
    template<typename Type>
    using Ref    = typename std::add_lvalue_reference<Type>::type;

    // -=(Undocumented)=-
    template<typename Type>
    using CPtr   = eastl::span<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using SPtr   = std::shared_ptr<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstSPtr = Ref<const SPtr<Type>>;

    // -=(Undocumented)=-
    template<typename Type, typename Deleter = std::default_delete<Type>>
    using UPtr   = std::unique_ptr<Type, Deleter>;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstUPtr = Ref<const UPtr<Type>>;

    // -=(Undocumented)=-
    template<typename Type>
    using FPtr   = std::function<Type>;

    // -=(Undocumented)=-
    template<typename Value, UInt Capacity>
    using Array  = std::array<Value, Capacity>;

    // -=(Undocumented)=-
    template<UInt Capacity>
    using Bitset = std::bitset<Capacity>;

    // -=(Undocumented)=-
    template<typename Value>
    using List   = std::initializer_list<Value>;

    // -=(Undocumented)=-
    template<typename Value, UInt Capacity>
    using Stack  = eastl::fixed_vector<Value, Capacity>;

    // -=(Undocumented)=-
    template<typename Value>
    using Vector = std::vector<Value>;

    // -=(Undocumented)=-
    template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Predicate = std::equal_to<>>
    using Table  = std::unordered_map<Key, Value, Hash, Predicate>;

    // -=(Undocumented)=-
    template<typename Class>
    using EnableSmartPointer = std::enable_shared_from_this<Class>;
}