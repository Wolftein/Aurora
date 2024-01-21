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

#include "Content/COM_Resource.hpp"
#include <Graphic/Pipeline.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("A2AC7EA9-6884-428E-B526-B7B965E9C494"), pointer_default(unique)]
    __interface Graphic_Pipeline_ : public Content_Resource_
    {
        HRESULT GetID([out, retval] vbInt32 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("790311CB-EE0C-47F9-A32A-F64205CA6398")]
    class ATL_NO_VTABLE Graphic_Pipeline : public Graphic_Pipeline_, public CSmartPtrWrapper<Graphic::Pipeline>
    {
    public:

        // \see Graphic_Pipeline_::GetDepth
        HRESULT GetID(vbInt32 * Result) override;

        // \see Content_Resource_::GetKey
        HRESULT GetKey(vbStr16 * Result) override;

        // \see Content_Resource_::GetMemory
        HRESULT GetMemory(vbInt64 * Result) override;

        // \see Content_Resource_::GetCategory
        HRESULT GetCategory(Content_Resource_Type * Result) override;

        // \see Content_Resource_::GetStatus
        HRESULT GetStatus(Content_Resource_Status * Result) override;
    };
}