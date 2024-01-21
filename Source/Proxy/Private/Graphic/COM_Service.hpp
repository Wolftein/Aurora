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

#include "Core/Math/COM_Rect.hpp"
#include "COM_Common.hpp"
#include <Graphic/Service.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("DA47A358-9469-40AE-B056-C6F2E8CC6BC2"), pointer_default(unique)]
    __interface Graphic_Service_
    {
        // TODO: Remove this class

        HRESULT Reset([in] vbInt32 Width, [in] vbInt32 Height);

        HRESULT CreatePassFromDisplay([in] vbInt32 Handle, [in] vbInt32 Width, [in] vbInt32 Height, [out, retval] vbInt32 * Result);

        HRESULT DeletePass([in] vbInt32 * ID);

        HRESULT Prepare([in] vbInt32 ID, [in] Math_Rectf * Viewport, [in] Graphic_Clear Clear, [in] vbInt32 Tint, [in] vbReal32 Depth, [in] vbInt8 Stencil);

        HRESULT Commit([in] vbInt32 ID, [in] vbBool Synchronised);
    };

    // -=(Undocumented)=-
    [coclass, uuid("DC1BD4B9-B5AD-45B3-AFDF-97631A4FA144")]
    class ATL_NO_VTABLE Graphic_Service : public Graphic_Service_, public CSmartPtrWrapper<Graphic::Service>
    {
    public:

        // \see Graphic_Service_::Reset
        HRESULT Reset(vbInt32 Width, vbInt32 Height) override;

        // \see Graphic_Service_::CreatePassFromDisplay
        HRESULT CreatePassFromDisplay(vbInt32 Handle, vbInt32 Width, vbInt32 Height, vbInt32 * Result);

        // \see Graphic_Service_::DeletePass
        HRESULT DeletePass(vbInt32 * ID);

        // \see Graphic_Service_::Prepare
        HRESULT Prepare(vbInt32 ID, Math_Rectf * Viewport, Graphic_Clear Clear, vbInt32 Tint, vbReal32 Depth, vbInt8 Stencil) override;

        // \see Graphic_Service_::Commit
        HRESULT Commit(vbInt32 ID, vbBool Synchronised) override;
    };
}