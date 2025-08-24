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

#include "Array.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    /// \brief Represents a section (table) node within a TOML document.
    class TOMLSection final
    {
    public:

        /// \brief Constructs an empty or wrapped TOML section.
        /// 
        /// \param Table Pointer to an existing TOML table, or `nullptr` to create an empty reference.
        TOMLSection(Ptr<toml::table> Table = nullptr);

        /// \brief Returns whether the section is null (uninitialized).
        /// 
        /// \return `true` if the section is null, `false` otherwise.
        Bool IsNull() const;

        /// \brief Returns whether the section is empty.
        /// 
        /// \return `true` if the section has no keys, `false` otherwise.
        Bool IsEmpty() const;

        /// \brief Returns the number of key-value pairs in the section.
        /// 
        /// \return The number of entries in the section.
        UInt32 GetSize() const;

        /// \brief Retrieves a nested section by key.
        /// 
        /// \param Key            The name of the section.
        /// \param CreateIfNeeded If `true`, creates the section if it does not exist.
        /// \return The requested section.
        TOMLSection GetSection(ConstText Key, Bool CreateIfNeeded = true) const;

        /// \brief Creates or replaces a nested section under the given key.
        /// 
        /// \param Key The name of the section.
        /// \return A handle to the new section.
        TOMLSection SetSection(ConstText Key);

        /// \brief Retrieves a nested array by key.
        /// 
        /// \param Key            The name of the array.
        /// \param CreateIfNeeded If `true`, creates the array if it does not exist.
        /// \return The requested array.
        TOMLArray GetArray(ConstText Key, Bool CreateIfNeeded = true) const;

        /// \brief Creates or replaces a nested array under the given key.
        /// 
        /// \param Key The name of the array.
        /// \return A handle to the new array.
        TOMLArray SetArray(ConstText Key);

        /// \brief Sets a boolean value under the specified key.
        /// 
        /// \param Key   The key name.
        /// \param Value The boolean value to store.
        void SetBool(ConstText Key, Bool Value);

        /// \brief Retrieves a boolean value from the section.
        /// 
        /// \param Key     The key name.
        /// \param Default Value to return if the key is not found.
        /// \return The boolean value stored at the key.
        Bool GetBool(ConstText Key, Bool Default = false) const;

        /// \brief Sets an integer value under the specified key.
        /// 
        /// \param Key   The key name.
        /// \param Value The integer value to store.
        void SetInteger(ConstText Key, SInt64 Value);

        /// \brief Retrieves an integer value from the section.
        /// 
        /// \param Key     The key name.
        /// \param Default Value to return if the key is not found.
        /// \return The integer value stored at the key.
        SInt64 GetInteger(ConstText Key, SInt64 Default = 0) const;

        /// \brief Sets a real (floating-point) value under the specified key.
        /// 
        /// \param Key   The key name.
        /// \param Value The real value to store.
        void SetReal(ConstText Key, Real64 Value);

        /// \brief Retrieves a real value from the section.
        /// 
        /// \param Key     The key name.
        /// \param Default Value to return if the key is not found.
        /// \return The real value stored at the key.
        Real64 GetReal(ConstText Key, Real64 Default = 0) const;

        /// \brief Sets a string value under the specified key.
        /// 
        /// \param Key   The key name.
        /// \param Value The string value to store.
        void SetString(ConstText Key, ConstText Value);

        /// \brief Retrieves a string value from the section.
        /// 
        /// \param Key     The key name.
        /// \param Default Value to return if the key is not found.
        /// \return The string value stored at the key.
        ConstText GetString(ConstText Key, ConstText Default = "") const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<toml::table> mTable;
    };
}