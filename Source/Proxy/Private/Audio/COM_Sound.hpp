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
#include <Audio/Sound.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("9640AFE4-6645-431F-92AE-52110B5C7A6C"), pointer_default(unique)]
    __interface Audio_Sound_ : public Content_Resource_
    {
        HRESULT GetDepth([out, retval] vbReal32 * Result);

        HRESULT GetChannel([out, retval] vbReal32 * Result);

        HRESULT GetFrequency([out, retval] vbReal32 * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("BE1689D8-95D1-4510-B5EE-71A236335B48")]
    class ATL_NO_VTABLE Audio_Sound : public Audio_Sound_, public CSmartPtrWrapper<Audio::Sound>
    {
    public:

        // \see Audio_Sound_::GetDepth
        HRESULT GetDepth(vbReal32 * Result) override;

        // \see Audio_Sound_::GetChannel
        HRESULT GetChannel(vbReal32 * Result) override;

        // \see Audio_Sound_::GetFrequency
        HRESULT GetFrequency(vbReal32 * Result) override;

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