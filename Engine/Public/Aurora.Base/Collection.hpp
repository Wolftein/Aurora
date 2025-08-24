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

#include "Trait.hpp"
#include <beman/inplace_vector/inplace_vector.hpp>
#include <bitset>
#include <unordered_dense.h>
#include <queue>
#include <vector>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A fixed-size array of elements with static capacity.
    /// 
    /// \tparam Value The type of each element.
    /// \tparam Capacity The number of elements in the array.
    template<typename Value, UInt32 Capacity>
    using Array  = std::array<Value, Capacity>;

    /// \brief A fixed-size sequence of bits.
    /// 
    /// \tparam Capacity The number of bits in the set.
    template<UInt32 Capacity>
    using Bitset = std::bitset<Capacity>;

    /// \brief Alias for a fixed list of values.
    /// 
    /// Provides a shorthand for \c std::initializer_list, typically used
    /// to supply a compile-time list of values for initialization.
    /// 
    /// \tparam Value The type of the elements in the list.
    template<typename Value>
    using List   = std::initializer_list<Value>;

    /// \brief A high-performance associative container.
    /// 
    /// \tparam Key       The key type.
    /// \tparam Value     The mapped value type.
    /// \tparam Hash      The hash function used for keys. Defaults to `ankerl::unordered_dense::hash<Key>`.
    /// \tparam Predicate The equality comparison function for keys. Defaults to `std::equal_to<>`.
    template<typename Key,
             typename Value,
             typename Hash = ankerl::unordered_dense::hash<Key>,
             typename Predicate = std::equal_to<>>
    using Table  = ankerl::unordered_dense::map<Key, Value, Hash, Predicate>;

    /// \brief FIFO (first-in, first-out) queue container.
    /// 
    /// This type alias wraps `std::queue` to represent a sequential container
    /// that allows insertion at the back and removal from the front.
    /// It is primarily used for task scheduling, event queues, and any
    /// structure requiring ordered processing.
    /// 
    /// \tparam Value The type of elements stored in the queue.
    template<typename Value>
    using Queue  = std::queue<Value>;

    /// \brief A dynamic array with optional inline storage based on capacity.
    /// 
    /// `Vector` acts as an alias for either `std::vector` or a small-vector-style container depending
    /// on the compile-time `Capacity` parameter. If `Capacity > 0`, it uses `beman::inplace_vector`
    /// to avoid heap allocation for small sizes; otherwise, it defaults to `std::vector`.
    /// 
    /// \tparam Value The type of elements stored in the vector.
    /// \tparam Capacity The number of inline elements to store before falling back to heap allocation.
    template<typename Value, UInt32 Capacity = 0>
    using Vector = Switch<(Capacity > 0), beman::inplace_vector<Value, Capacity>, std::vector<Value>>;

    /// \brief Hash functor for `Text`-like types supporting heterogeneous lookup.
    struct TextTableHash
    {
        /// \brief Allows heterogeneous lookup via transparent hashing.
        using is_transparent = void;

        /// \brief Marks this hash as avalanching for compatibility with unordered_dense.
        using is_avalanching = void;

        /// \brief Computes the hash of a UTF-8 character pointer.
        /// 
        /// \param Value A view into the character data.
        /// \return The hash of the string.
        [[nodiscard]] size_t operator()(ConstPtr<Char> Value) const
        {
            return ankerl::unordered_dense::hash<ConstText>{ }(Value);
        }

        /// \brief Computes the hash of a text view.
        /// 
        /// \param Value The view into the text.
        /// \return The hash of the string.
        [[nodiscard]] size_t operator()(ConstText Value) const
        {
            return ankerl::unordered_dense::hash<ConstText>{ }(Value);
        }

        /// \brief Computes the hash of a text reference.
        /// 
        /// \param Value The reference to a text object.
        /// \return The hash of the string.
        [[nodiscard]] size_t operator()(ConstRef<Text> Value) const
        {
            return ankerl::unordered_dense::hash<ConstText>{ }(Value);
        }
    };

    /// \brief Hash map with `Text` keys supporting fast UTF-8 string lookups.
    /// 
    /// \tparam Value The value type to associate with each key.
    template<typename Value>
    using TextTable = Table<Text, Value, TextTableHash>;
}