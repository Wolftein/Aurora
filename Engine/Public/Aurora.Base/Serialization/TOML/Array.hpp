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

#include "Aurora.Base/Primitive.hpp"
#include <toml.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    /// \brief Forward declaration of the TOML section representation.
    class TOMLSection;

    /// \brief Represents an array node within a TOML document.
    class TOMLArray final
    {
    public:

        /// \brief Constructs an empty or wrapped TOML array.
        /// 
        /// \param Array Pointer to an existing TOML array, or `nullptr` to create an empty reference.
        TOMLArray(Ptr<toml::array> Array = nullptr);

        /// \brief Returns whether the array is null (uninitialized).
        /// 
        /// \return `true` if the array is null, `false` otherwise.
        Bool IsNull() const;

        /// \brief Returns whether the array is empty.
        /// 
        /// \return `true` if the array has no elements, `false` otherwise.
        Bool IsEmpty() const;

        /// \brief Returns the number of elements in the array.
        /// 
        /// \return The total number of array elements.
        UInt32 GetSize() const;

        /// \brief Appends a new array to the current array.
        /// 
        /// \return A reference to the newly added array.
        TOMLArray AddArray();

        /// \brief Retrieves the array at the specified index.
        /// 
        /// \param Index The zero-based index of the array element.
        /// \return A reference to the specified array.
        TOMLArray GetArray(UInt32 Index) const;

        /// \brief Appends a new section to the array.
        /// 
        /// \return A reference to the newly added section.
        TOMLSection AddSection();

        /// \brief Retrieves the section at the specified index.
        /// 
        /// \param Index The zero-based index of the section element.
        /// \return A reference to the specified section.
        TOMLSection GetSection(UInt32 Index) const;

        /// \brief Appends a boolean value to the array.
        /// 
        /// \param Value The boolean value to add.
        void AddBool(Bool Value);

        /// \brief Retrieves a boolean value from the array.
        /// 
        /// \param Index The zero-based index.
        /// \return The boolean value at the given index.
        Bool GetBool(UInt32 Index) const;

        /// \brief Appends an integer to the array.
        /// 
        /// \param Value The integer value to add.
        void AddInteger(SInt64 Value);

        /// \brief Retrieves an integer from the array.
        /// 
        /// \param Index The zero-based index.
        /// \return The integer value at the specified index.
        SInt64 GetInteger(UInt32 Index) const;

        /// \brief Appends a floating-point value to the array.
        /// 
        /// \param Value The real value to add.
        void AddReal(Real64 Value);

        /// \brief Retrieves a floating-point value from the array.
        /// 
        /// \param Index The zero-based index.
        /// \return The real value at the specified index.
        Real64 GetReal(UInt32 Index) const;

        /// \brief Appends a string to the array.
        /// 
        /// \param Value The string to add.
        void AddString(ConstText Value);

        /// \brief Retrieves a string from the array.
        /// 
        /// \param Index The zero-based index.
        /// \return The string at the specified index.
        ConstText GetString(UInt32 Index) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<toml::array> mArray;
    };
}