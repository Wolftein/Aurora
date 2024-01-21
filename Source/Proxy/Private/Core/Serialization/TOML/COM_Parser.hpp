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

#include "COM_Section.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("E863744A-6F62-11EE-ADD5-1418C3A8EDB8"), pointer_default(unique)]
    __interface TOMLParser_
    {
        HRESULT Load([in] vbStr16 Content);

        HRESULT Dump([out, retval] vbStr16 * Result);

        HRESULT GetRoot([out, retval] TOMLSection_ ** Result);

        HRESULT GetSection([in] vbStr8 Key, [in, defaultvalue(true)] vbBool CreateIfNecessary, [out, retval] TOMLSection_ ** Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("E88B2085-6F62-11EE-ADD5-1418C3A8EDB8")]
    class ATL_NO_VTABLE TOMLParser : public TOMLParser_, public CObjectWrapper<Core::TOMLParser>
    {
    public:

        // \see TOMLParser_::Load
        HRESULT Load(vbStr16 Content) override;

        // \see TOMLParser_::Dump
        HRESULT Dump(vbStr16 * Result) override;

        // \see TOMLParser_::GetRoot
        HRESULT GetRoot(TOMLSection_ ** Result) override;

        // \see TOMLParser_::GetSection
        HRESULT GetSection(vbStr8 Key, vbBool CreateIfNecessary, TOMLSection_ ** Result) override;
    };
}