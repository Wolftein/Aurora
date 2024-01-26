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
        TOMLSection(Ptr<toml::table> Table = nullptr);

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
        Vector<Bool> GetBoolArray(CStr Key) const;

        // -=(Undocumented)=-
        void SetString(CStr Key, CStr Value);

        // -=(Undocumented)=-
        CStr GetString(CStr Key, CStr Default = "") const;

        // -=(Undocumented)=-
        Vector<CStr> GetStringArray(CStr Key) const;

        // -=(Undocumented)=-
        void SetNumber(CStr Key, SInt Value);

        // -=(Undocumented)=-
        SInt GetNumber(CStr Key, SInt Default = 0) const;

        // -=(Undocumented)=-
        template<typename Type>
        void SetEnum(CStr Key, Type Value)
        {
            SetNumber(Key, static_cast<SInt>(Value));
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type GetEnum(CStr Key, Type Default = static_cast<Type>(0)) const
        {
            return static_cast<Type>(GetNumber(Key, static_cast<SInt>(Default)));
        }

        // -=(Undocumented)=-
        Vector<SInt> GetNumberArray(CStr Key) const;

        // -=(Undocumented)=-
        void SetReal(CStr Key, Real Value);

        // -=(Undocumented)=-
        Real GetReal(CStr Key, Real Default = 0) const;

        // -=(Undocumented)=-
        Vector<Real> GetRealArray(CStr Key) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<toml::table> mTable;
    };
}