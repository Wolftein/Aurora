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

#include "Section.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Parses and manipulates TOML configuration data.
    /// 
    /// Provides an interface to parse a TOML text source and access its sections and arrays
    /// for structured data manipulation.
    class TOMLParser final
    {
    public:

        /// \brief Constructs an empty TOML parser.
        TOMLParser();

        /// \brief Constructs a parser from a TOML-formatted text source.
        /// 
        /// \param Text The TOML text to parse.
        TOMLParser(ConstText Text);

        /// \brief Returns the root section of the parsed TOML document.
        /// 
        /// \return A `TOMLSection` representing the root of the TOML tree.
        TOMLSection GetRoot();

        /// \brief Retrieves or creates a child section from the root.
        /// 
        /// \param Key            The section name.
        /// \param CreateIfNeeded If `true`, creates the section if it does not exist.
        /// \return A handle to the specified section.
        TOMLSection GetSection(ConstText Key, Bool CreateIfNeeded = true);

        /// \brief Retrieves or creates an array from the root.
        /// 
        /// \param Key            The array name.
        /// \param CreateIfNeeded If `true`, creates the array if it does not exist.
        /// \return A handle to the specified array.
        TOMLArray GetArray(ConstText Key, Bool CreateIfNeeded = true);

        /// \brief Serializes the current TOML tree back to text.
        /// 
        /// \return A textual representation of the current TOML structure.
        Text Dump() const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        toml::parse_result mResult;
    };
}