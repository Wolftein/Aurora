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

#include "Compiler.hpp"
#include <functional>
#include <memory>
#include <span>
#include <string>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Boolean value (`true` or `false`).
    using Bool      = bool;

    /// \brief Char value (0 to 255).
    using Char      = char;

    /// \brief 8-bit unsigned integer (0 to 255).
    using Byte      = std::uint8_t;

    /// \brief Signed integer type with the same size as a pointer.
    using SInt      = std::intptr_t;

    /// \brief Unsigned integer type with the same size as a pointer.
    using UInt      = std::uintptr_t;

    /// \brief 8-bit signed integer (-128 to 127).
    using SInt8     = std::int8_t;

    /// \brief 8-bit unsigned integer (0 to 255).
    using UInt8     = std::uint8_t;

    /// \brief 16-bit signed integer (-32,768 to 32,767).
    using SInt16    = std::int16_t;

    /// \brief 16-bit unsigned integer (0 to 65,535).
    using UInt16    = std::uint16_t;

    /// \brief 32-bit signed integer (-2,147,483,648 to 2,147,483,647).
    using SInt32    = std::int32_t;

    /// \brief 32-bit unsigned integer (0 to 4,294,967,295).
    using UInt32    = std::uint32_t;

    /// \brief 64-bit signed integer (-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807).
    using SInt64    = std::int64_t;

    /// \brief 64-bit unsigned integer (0 to 18,446,744,073,709,551,615).
    using UInt64    = std::uint64_t;

    /// \brief Single-precision floating point number (typically 32-bit).
    using Real32    = float;

    /// \brief Double-precision floating point number (typically 64-bit).
    using Real64    = double;

    /// \brief Raw pointer to a mutable object of type `Type`.
    template<typename Type>
    using Ptr       = Type *;

    /// \brief Raw pointer to a const object of type `Type`.
    template<typename Type>
    using ConstPtr  = Ptr<const Type>;

    /// \brief Non-owning reference to a mutable object of type `Type`.
    template<typename Type>
    using Ref       = Type &;

    /// \brief Non-owning reference to a const object of type `Type`.
    template<typename Type>
    using ConstRef  = Ref<const Type>;

    /// \brief Universal (forwarding) reference for perfect forwarding.
    template<typename Type>
    using AnyRef    = Type &&;

    /// \brief A lightweight view over a contiguous sequence of elements.
    template<typename Type>
    using Span      = std::span<Type>;

    /// \brief A lightweight view over a contiguous sequence of immutable elements.
    template<typename Type>
    using ConstSpan = Span<const Type>;

    /// \brief An owning UTF-8 encoded string used for textual storage and manipulation.
    using Text      = std::string;

    /// \brief A non-owning, read-only view of a UTF-8 encoded string.
    using ConstText = std::string_view;

    /// \brief Alias for a fixed-size heterogeneous collection of values.
    template<typename... Type>
    using Tuple     = std::tuple<Type...>;

    /// \brief Alias for a unique ownership pointer.
    /// 
    /// \tparam Type    The type of the object to own.
    /// \tparam Deleter The type of the deleter used to destroy the object.
    template<typename Type, typename Deleter = std::default_delete<Type>>
    using Unique    = std::unique_ptr<Type, Deleter>;

    /// \brief Alias for a general-purpose callable object.
    /// 
    /// This type represents a type-erased function or lambda that can be stored,
    /// copied, and invoked later. It supports capturing state and can be used
    /// to pass callbacks or deferred execution blocks.
    /// 
    /// \tparam Type The function signature (e.g., `void()` or `int(float, int)`).
    template<typename Type>
    using Function  = std::function<Type>;
}