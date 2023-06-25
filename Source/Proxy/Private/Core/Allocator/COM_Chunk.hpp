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

#include "Core/COM_Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("47E58AF8-584B-48F7-BB7C-F8E35A671DED"), pointer_default(unique)]
    __interface Memory_Chunk_
    {
        HRESULT Allocate([in] vbInt32 Bytes);

        HRESULT Free();

        HRESULT GetData([in, satype(vbInt8)] SAFEARRAY ** Result);

        HRESULT GetText([out, retval] vbStr16 * Result);

        HRESULT GetSize([out, retval] vbInt32 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("AD08ECDD-D7B4-4EFB-98CE-CABDF9407FF2")]
    class Memory_Chunk : public Memory_Chunk_, public CObjectWrapper<Chunk>
    {
    public:

        // \see Memory_Chunk_::Allocate
        HRESULT Allocate(vbInt32 Bytes);

        // \see Memory_Chunk_::Free
        HRESULT Free();

        // \see Memory_Chunk_::GetData
        HRESULT GetData(SAFEARRAY ** Result);

        // \see Memory_Chunk_::GetText
        HRESULT GetText(vbStr16 * Result);

        // \see Memory_Chunk_::GetSize
        HRESULT GetSize(vbInt32 * Result);
    };
}