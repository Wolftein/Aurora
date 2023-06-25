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
        HRESULT Reset([in] vbInt32 Width, [in] vbInt32 Height);

        // TODO: Resources?

        HRESULT Prepare([in] vbInt32 ID, [in] Math_Rectf * Viewport, [in] Graphic_Clear Clear, [in] Graphic_Color * Tint, [in] vbReal32 Depth, [in] vbInt8 Stencil);

        HRESULT Commit([in] vbInt32 ID, [in] vbBool Synchronised);
    };

    // -=(Undocumented)=-
    [coclass, uuid("DC1BD4B9-B5AD-45B3-AFDF-97631A4FA144")]
    class ATL_NO_VTABLE Graphic_Service : public Graphic_Service_, public CSmartPtrWrapper<Graphic::Service>
    {
    public:

        // \inheritDoc
        HRESULT FinalRelease();

        // \see Graphic_Service_::Reset
        HRESULT Reset(vbInt32 Width, vbInt32 Height) override;

        // \see Graphic_Service_::Prepare
        HRESULT Prepare(vbInt32 ID, Math_Rectf * Viewport, Graphic_Clear Clear, Graphic_Color * Tint, vbReal32 Depth, vbInt8 Stencil) override;

        // \see Graphic_Service_::Commit
        HRESULT Commit(vbInt32 ID, vbBool Synchronised) override;
    };
}