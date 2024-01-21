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
#include <Graphic/Texture.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("FE87C56C-02C9-41C9-89E6-C03257DBE992"), pointer_default(unique)]
    __interface Graphic_Texture_ : public Content_Resource_
    {
        HRESULT GetID([out, retval] vbInt32 * Result);

        HRESULT GetWidth([out, retval] vbInt32 * Result);

        HRESULT GetHeight([out, retval] vbInt32 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("5918B7A3-6578-4007-A34C-9636BD705F80")]
    class ATL_NO_VTABLE Graphic_Texture : public Graphic_Texture_, public CSmartPtrWrapper<Graphic::Texture>
    {
    public:

        // \see Graphic_Texture_::GetDepth
        HRESULT GetID(vbInt32 * Result) override;

        // \see Graphic_Texture_::GetWidth
        HRESULT GetWidth(vbInt32 * Result) override;

        // \see Graphic_Texture_::GetHeight
        HRESULT GetHeight(vbInt32 * Result) override;

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