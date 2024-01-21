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

#include "Core/COM_Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("82B22FC2-6F50-11EE-ADD5-1418C3A8EDB8"), pointer_default(unique)]
    __interface TOMLArray_
    {
        HRESULT IsNull([out, retval] vbBool * Result);

        HRESULT IsEmpty([out, retval] vbBool * Result);

        HRESULT GetSize([out, retval] vbInt32 * Result);

        HRESULT AddArray([out, retval] TOMLArray_ ** Result);

        HRESULT GetArray([in] vbInt32 Index, [out, retval] TOMLArray_ ** Result);

        HRESULT AddBool([in] vbBool Value);

        HRESULT GetBool([in] vbInt32 Index, [out, retval] vbBool * Result);

        HRESULT AddString([in] vbStr16 Value);

        HRESULT GetString([in] vbInt32 Index, [out, retval] vbStr16 * Result);

        HRESULT AddInt8([in] vbInt8 Value);

        HRESULT GetInt8([in] vbInt32 Index, [out, retval] vbInt8 * Result);

        HRESULT AddInt16([in] vbInt16 Value);

        HRESULT GetInt16([in] vbInt32 Index, [out, retval] vbInt16 * Result);

        HRESULT AddInt32([in] vbInt32 Value);

        HRESULT GetInt32([in] vbInt32 Index, [out, retval] vbInt32 * Result);

        HRESULT AddInt64([in] vbInt64 Value);

        HRESULT GetInt64([in] vbInt32 Index, [out, retval] vbInt64 * Result);

        HRESULT AddReal32([in] vbReal32 Value);

        HRESULT GetReal32([in] vbInt32 Index, [out, retval] vbReal32 * Result);

        HRESULT AddReal64([in] vbReal64 Value);

        HRESULT GetReal64([in] vbInt32 Index, [out, retval] vbReal64 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("84F49507-6F50-11EE-ADD5-1418C3A8EDB8")]
    class ATL_NO_VTABLE TOMLArray : public TOMLArray_, public CObjectWrapper<Core::TOMLArray>
    {
    public:

        // \see TOMLArray_::IsNull
        HRESULT IsNull(vbBool * Result) override;

        // \see TOMLArray_::IsEmpty
        HRESULT IsEmpty(vbBool * Result) override;

        // \see TOMLArray_::GetSize
        HRESULT GetSize(vbInt32 * Result) override;

        // \see TOMLArray_::AddArray
        HRESULT AddArray(TOMLArray_ ** Result) override;

        // \see TOMLArray_::GetArray
        HRESULT GetArray(vbInt32 Index, TOMLArray_ ** Result) override;

        // \see TOMLArray_::AddBool
        HRESULT AddBool(vbBool Value) override;

        // \see TOMLArray_::GetBool
        HRESULT GetBool(vbInt32 Index, vbBool * Result) override;

        // \see TOMLArray_::AddString
        HRESULT AddString(vbStr16 Value) override;

        // \see TOMLArray_::GetString
        HRESULT GetString(vbInt32 Index, vbStr16 * Result) override;

        // \see TOMLArray_::AddInt8
        HRESULT AddInt8(vbInt8 Value) override;

        // \see TOMLArray_::GetInt8
        HRESULT GetInt8(vbInt32 Index, vbInt8 * Result) override;

        // \see TOMLArray_::AddInt16
        HRESULT AddInt16(vbInt16 Value) override;

        // \see TOMLArray_::GetInt16
        HRESULT GetInt16(vbInt32 Index, vbInt16 * Result) override;

        // \see TOMLArray_::AddInt32
        HRESULT AddInt32(vbInt32 Value) override;

        // \see TOMLArray_::GetInt32
        HRESULT GetInt32(vbInt32 Index, vbInt32 * Result) override;

        // \see TOMLArray_::AddInt64
        HRESULT AddInt64(vbInt64 Value) override;

        // \see TOMLArray_::GetInt64
        HRESULT GetInt64(vbInt32 Index, vbInt64 * Result) override;

        // \see TOMLArray_::AddReal32
        HRESULT AddReal32(vbReal32 Value) override;

        // \see TOMLArray_::GetReal32
        HRESULT GetReal32(vbInt32 Index, vbReal32 * Result) override;

        // \see TOMLArray_::AddReal64
        HRESULT AddReal64(vbReal64 Value) override;

        // \see TOMLArray_::GetReal64
        HRESULT GetReal64(vbInt32 Index, vbReal64 * Result) override;
    };
}