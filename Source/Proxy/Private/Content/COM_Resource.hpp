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

#include "Core/COM_Core.hpp"
#include <Content/Resource.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [export, uuid("33D42F44-023F-4F82-B61D-B56BF967AC56"), v1_enum]
    typedef enum Content_Resource_Type
    {
        eResourceTypeMaterial = Hash("Material"),
        eResourceTypePipeline = Hash("Pipeline"),
        eResourceTypeSound    = Hash("Sound"),
        eResourceTypeTexture  = Hash("Texture"),
    } Content_Resource_Type;

    // -=(Undocumented)=-
    [object, hidden, uuid("EAA9EDD8-22AE-4623-BF2A-90C6F7DA5CF7"), pointer_default(unique)]
    __interface Content_Resource_
    {
        HRESULT GetKey([out, retval] vbStr16 * Result);

        HRESULT GetMemory([out, retval] vbInt64 * Result);

        HRESULT GetCategory([out, retval] Content_Resource_Type * Result);
    };
}