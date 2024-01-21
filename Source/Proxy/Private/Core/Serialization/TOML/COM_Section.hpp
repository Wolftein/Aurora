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

#include "COM_Array.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("7365E4AA-6F55-11EE-ADD5-1418C3A8EDB8"), pointer_default(unique)]
    __interface TOMLSection_
    {
        HRESULT IsNull([out, retval] vbBool * Result);

        HRESULT IsEmpty([out, retval] vbBool * Result);

        HRESULT GetSize([out, retval] vbInt32 * Result);

        HRESULT SetSection([in] vbStr8 Key, [out, retval] TOMLSection_ ** Result);

        HRESULT GetSection([in] vbStr8 Key, [in, defaultvalue(true)] vbBool CreateIfNecessary, [out, retval] TOMLSection_ ** Result);

        HRESULT SetArray([in] vbStr8 Key, [out, retval] TOMLArray_ ** Result);

        HRESULT GetArray([in] vbStr8 Key, [in, defaultvalue(true)] vbBool CreateIfNecessary, [out, retval] TOMLArray_ ** Result);

        HRESULT SetBool([in] vbStr8 Key, [in] vbBool Value);

        HRESULT GetBool([in] vbStr8 Key, [in, defaultvalue(false)] vbBool Default, [out, retval] vbBool * Result);

        HRESULT SetString([in] vbStr8 Key, [in] vbStr16 Value);

        HRESULT GetString([in] vbStr8 Key, [in, defaultvalue(0)] vbStr16 Default, [out, retval] vbStr16 * Result);

        HRESULT SetInt8([in] vbStr8 Key, [in] vbInt8 Value);

        HRESULT GetInt8([in] vbStr8 Key, [in, defaultvalue(0)] vbInt8 Default, [out, retval] vbInt8 * Result);

        HRESULT SetInt16([in] vbStr8 Key, [in] vbInt16 Value);

        HRESULT GetInt16([in] vbStr8 Key, [in, defaultvalue(0)] vbInt16 Default, [out, retval] vbInt16 * Result);

        HRESULT SetInt32([in] vbStr8 Key, [in] vbInt32 Value);

        HRESULT GetInt32([in] vbStr8 Key, [in, defaultvalue(0)] vbInt32 Default, [out, retval] vbInt32 * Result);

        HRESULT SetInt64([in] vbStr8 Key, [in] vbInt64 Value);

        HRESULT GetInt64([in] vbStr8 Key, [in, defaultvalue(0)] vbInt64 Default, [out, retval] vbInt64 * Result);

        HRESULT SetReal32([in] vbStr8 Key, [in] vbReal32 Value);

        HRESULT GetReal32([in] vbStr8 Key, [in, defaultvalue(0)] vbReal32 Default, [out, retval] vbReal32 * Result);

        HRESULT SetReal64([in] vbStr8 Key, [in] vbReal64 Value);

        HRESULT GetReal64([in] vbStr8 Key, [in, defaultvalue(0)] vbReal64 Default, [out, retval] vbReal64 * Result);

    };

    // -=(Undocumented)=-
    [coclass, uuid("738C7F87-6F55-11EE-ADD5-1418C3A8EDB8")]
    class ATL_NO_VTABLE TOMLSection : public TOMLSection_, public CObjectWrapper<Core::TOMLSection>
    {
    public:

        // \see TOMLSection_::IsNull
        HRESULT IsNull(vbBool * Result) override;

        // \see TOMLSection_::IsEmpty
        HRESULT IsEmpty(vbBool * Result) override;

        // \see TOMLSection_::GetSize
        HRESULT GetSize(vbInt32 * Result) override;

        // \see TOMLSection_::SetSection
        HRESULT SetSection(vbStr8 Key, TOMLSection_ ** Result) override;

        // \see TOMLSection_::GetSection
        HRESULT GetSection(vbStr8 Key, vbBool CreateIfNecessary, TOMLSection_ ** Result) override;

        // \see TOMLSection_::SetArray
        HRESULT SetArray(vbStr8 Key, TOMLArray_ ** Result) override;

        // \see TOMLSection_::GetArray
        HRESULT GetArray(vbStr8 Key, vbBool CreateIfNecessary, TOMLArray_ ** Result) override;

        // \see TOMLSection_::SetBool
        HRESULT SetBool(vbStr8 Key, vbBool Value) override;

        // \see TOMLSection_::GetBool
        HRESULT GetBool(vbStr8 Key, vbBool Default, vbBool * Result) override;

        // \see TOMLSection_::SetString
        HRESULT SetString(vbStr8 Key, vbStr16 Value) override;

        // \see TOMLSection_::GetString
        HRESULT GetString(vbStr8 Key, vbStr16 Default, vbStr16 * Result) override;

        // \see TOMLSection_::SetInt8
        HRESULT SetInt8(vbStr8 Key, vbInt8 Value) override;

        // \see TOMLSection_::GetInt8
        HRESULT GetInt8(vbStr8 Key, vbInt8 Default, vbInt8 * Result) override;

        // \see TOMLSection_::SetInt16
        HRESULT SetInt16(vbStr8 Key, vbInt16 Value) override;

        // \see TOMLSection_::GetInt16
        HRESULT GetInt16(vbStr8 Key, vbInt16 Default, vbInt16 * Result) override;

        // \see TOMLSection_::SetInt32
        HRESULT SetInt32(vbStr8 Key, vbInt32 Value) override;

        // \see TOMLSection_::GetInt32
        HRESULT GetInt32(vbStr8 Key, vbInt32 Default, vbInt32 * Result) override;

        // \see TOMLSection_::SetInt64
        HRESULT SetInt64(vbStr8 Key, vbInt64 Value) override;

        // \see TOMLSection_::GetInt64
        HRESULT GetInt64(vbStr8 Key, vbInt64 Default, vbInt64 * Result) override;

        // \see TOMLSection_::SetReal32
        HRESULT SetReal32(vbStr8 Key, vbReal32 Value) override;

        // \see TOMLSection_::GetReal32
        HRESULT GetReal32(vbStr8 Key, vbReal32 Default, vbReal32 * Result) override;

        // \see TOMLSection_::SetReal64
        HRESULT SetReal64(vbStr8 Key, vbReal64 Value) override;

        // \see TOMLSection_::GetReal64
        HRESULT GetReal64(vbStr8 Key, vbReal64 Default, vbReal64 * Result) override;
    };
}