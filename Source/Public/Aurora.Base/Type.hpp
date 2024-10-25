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

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <format>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <unordered_dense.h>
#include <vector>
#include <SDL3/SDL.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    using Bool      = bool;

    // -=(Undocumented)=-
    using Char      = char;

    // -=(Undocumented)=-
    using Char16    = char16_t;

    // -=(Undocumented)=-
    using SInt      = intptr_t;

    // -=(Undocumented)=-
    using SInt8     = int8_t;

    // -=(Undocumented)=-
    using SInt16    = int16_t;

    // -=(Undocumented)=-
    using SInt32    = int32_t;

    // -=(Undocumented)=-
    using SInt64    = int64_t;

    // -=(Undocumented)=-
    using UInt      = uintptr_t;

    // -=(Undocumented)=-
    using UInt8     = uint8_t;

    // -=(Undocumented)=-
    using UInt16    = uint16_t;

    // -=(Undocumented)=-
    using UInt32    = uint32_t;

    // -=(Undocumented)=-
    using UInt64    = uint64_t;

    // -=(Undocumented)=-
    using Real      = double_t;

    // -=(Undocumented)=-
    using Real32    = float_t;

    // -=(Undocumented)=-
    using Real64    = double_t;

    // -=(Undocumented)=-
    using SStr      = std::string;

    // -=(Undocumented)=-
    using CStr      = std::string_view;

    // -=(Undocumented)=-
    using SStr16    = std::wstring;

    // -=(Undocumented)=-
    using CStr16    = std::wstring_view;

    // -=(Undocumented)=-
    template<typename Type>
    using Ptr       = Type *;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstPtr  = Ptr<const Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using Ref       = Type &;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstRef  = Ref<const Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using Any       = Type &&;

    // -=(Undocumented)=-
    template<typename Type>
    using CPtr      = std::span<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using SPtr      = std::shared_ptr<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstSPtr = Ref<const SPtr<Type>>;

    // -=(Undocumented)=-
    template<typename Type, typename Deleter = std::default_delete<Type>>
    using UPtr      = std::unique_ptr<Type, Deleter>;

    // -=(Undocumented)=-
    template<typename Type>
    using ConstUPtr = Ref<const UPtr<Type>>;

    // -=(Undocumented)=-
    template<typename Type>
    using FPtr      = std::function<Type>;

    // -=(Undocumented)=-
    template<typename Value>
    using List      = std::initializer_list<Value>;

    // -=(Undocumented)=-
    template<typename Value, UInt Capacity>
    using Array     = std::array<Value, Capacity>;

    // -=(Undocumented)=-
    template<UInt Capacity>
    using Bitset    = std::bitset<Capacity>;

    // -=(Undocumented)=-
    template<typename Key, typename Hash = ankerl::unordered_dense::hash<Key>, typename Predicate = std::equal_to<>>
    using Set       = ankerl::unordered_dense::set<Key, Hash, Predicate>;

    // -=(Undocumented)=-
    template<typename Key, typename Value, typename Hash = ankerl::unordered_dense::hash<Key>, typename Predicate = std::equal_to<>>
    using Table     = ankerl::unordered_dense::map<Key, Value, Hash, Predicate>;

    // -=(Undocumented)=-
    template<typename Value>
    using Vector    = std::vector<Value>;

    // -=(Undocumented)=-
    struct StringHash
    {
        using is_transparent = void;
        using is_avalanching = void;

        [[nodiscard]] size_t operator()(Ptr<const Char> Value) const
        {
            return ankerl::unordered_dense::hash<CStr>{ }(Value);
        }

        [[nodiscard]] size_t operator()(CStr Value) const
        {
            return ankerl::unordered_dense::hash<CStr>{ }(Value);
        }

        [[nodiscard]] size_t operator()(Ref<const SStr> Value) const
        {
            return ankerl::unordered_dense::hash<SStr>{ }(Value);
        }
    };

    // -=(Undocumented)=-
    template<typename Value>
    using StringTable = Table<SStr, Value, StringHash>;

    // -=(Undocumented)=-
    template<typename Type>
    using EnableSmartPointer = std::enable_shared_from_this<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using Atomic      = std::atomic<Type>;
}