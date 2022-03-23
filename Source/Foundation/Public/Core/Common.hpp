// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <EASTL/bitset.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/span.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/any.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    using Any    = eastl::any;

    // -=(Undocumented)=-
    using Bool   = bool8_t;

    // -=(Undocumented)=-
    using SInt   = ssize_t;

    // -=(Undocumented)=-
    using SInt08 = int8_t;

    // -=(Undocumented)=-
    using SInt16 = int16_t;

    // -=(Undocumented)=-
    using SInt32 = int32_t;

    // -=(Undocumented)=-
    using SInt64 = int64_t;

    // -=(Undocumented)=-
    using UInt   = size_t;

    // -=(Undocumented)=-
    using UInt08 = uint8_t;

    // -=(Undocumented)=-
    using UInt16 = uint16_t;

    // -=(Undocumented)=-
    using UInt32 = uint32_t;

    // -=(Undocumented)=-
    using UInt64 = uint64_t;

    // -=(Undocumented)=-
    using Real32 = float_t;

    // -=(Undocumented)=-
    using Real64 = double_t;

    // -=(Undocumented)=-
    using Char   = char;

    // -=(Undocumented)=-
    using SStr   = eastl::string;

    // -=(Undocumented)=-
    using CStr   = eastl::string_view;

    // -=(Undocumented)=-
    template<typename Type>
    using Ptr    = typename eastl::add_pointer<Type>::type;

    // -=(Undocumented)=-
    template<typename Type>
    using Ref    = typename eastl::add_reference<Type>::type;

    // -=(Undocumented)=-
    template<typename Type>
    using CPtr   = eastl::span<Type>;

    // -=(Undocumented)=-
    template<typename Type>
    using SPtr   = eastl::shared_ptr<Type>;

    // -=(Undocumented)=-
    template<typename Type, typename Deleter = eastl::default_delete<Type>>
    using UPtr   = eastl::unique_ptr<Type, Deleter>;

    // -=(Undocumented)=-
    template<typename Type>
    using FPtr   = eastl::function<Type>;

    // -=(Undocumented)=-
    template<typename Value, UInt Capacity>
    using Array  = eastl::fixed_vector<Value, Capacity>;

    // -=(Undocumented)=-
    template<typename Type, UInt Capacity>
    using Bitset = eastl::bitset<Capacity, Type>;

    // -=(Undocumented)=-
    template<typename Key, typename Value>
    using Table  = eastl::unordered_map<Key, Value>;

    // -=(Undocumented)=-
    template<typename Value>
    using Vector = eastl::vector<Value>;
}