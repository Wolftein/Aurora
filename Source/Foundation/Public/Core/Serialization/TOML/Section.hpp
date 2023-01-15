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

#include "Array.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class TOMLSection final
    {
    public:

        // -=(Undocumented)=-
        TOMLSection(Ptr<toml::table> Table);

        // -=(Undocumented)=-
        Bool IsNull() const;

        // -=(Undocumented)=-
        Bool IsEmpty() const;

        // -=(Undocumented)=-
        UInt GetSize() const;

        // -=(Undocumented)=-
        TOMLSection GetSection(CStr Key, Bool CreateIfNeeded = true) const;

        // -=(Undocumented)=-
        TOMLSection SetSection(CStr Key);

        // -=(Undocumented)=-
        TOMLArray GetArray(CStr Key, Bool CreateIfNeeded = true) const;

        // -=(Undocumented)=-
        TOMLArray SetArray(CStr Key);

        // -=(Undocumented)=-
        void SetBool(CStr Key, Bool Value);

        // -=(Undocumented)=-
        Bool GetBool(CStr Key, Bool Default = false) const;

        // -=(Undocumented)=-
        void SetString(CStr Key, CStr Value);

        // -=(Undocumented)=-
        CStr GetString(CStr Key, CStr Default = "") const;

        // -=(Undocumented)=-
        void SetNumber(CStr Key, SInt Value);

        // -=(Undocumented)=-
        SInt GetNumber(CStr Key, SInt Default = 0) const;

        // -=(Undocumented)=-
        void SetReal(CStr Key, Real Value);

        // -=(Undocumented)=-
        Real GetReal(CStr Key, Real Default = 0) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<toml::table> mTable;
    };
}