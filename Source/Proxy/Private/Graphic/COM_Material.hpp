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

#include "Graphic/COM_Common.hpp"
#include "Graphic/COM_Texture.hpp"
#include <Graphic/Material.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("03E55C9C-9624-45F2-A7D5-B98916070F0A"), pointer_default(unique)]
    __interface Graphic_Material_ : public Content_Resource_
    {
        HRESULT GetID([out, retval] vbInt32 * Result);

        HRESULT SetTexture([in] vbInt32 Slot, [in] Graphic_Texture_ * Texture);

        HRESULT SetSampler([in] vbInt32 Slot, [in] Graphic_Texture_Edge EdgeU, [in] Graphic_Texture_Edge EdgeV, [in] Graphic_Texture_Filter Filter);

        HRESULT SetParameter([in] vbInt32 Offset, [in] vbReal32 X, [in, defaultvalue(0)] vbReal32 Y, [in, defaultvalue(0)] vbReal32 Z, [in, defaultvalue(0)] vbReal32 W);
    };

    // -=(Undocumented)=-
    [coclass, uuid("3015D5E9-DEAB-41F7-8810-E963C69BADF9")]
    class ATL_NO_VTABLE Graphic_Material : public Graphic_Material_, public CSmartPtrWrapper<Graphic::Material>
    {
    public:

        // \see Graphic_Material_::GetID
        HRESULT GetID(vbInt32 * Result) override;

        // \see Graphic_Material_::SetTexture
        HRESULT SetTexture(vbInt32 Slot, Graphic_Texture_ * Texture) override;

        // \see Graphic_Material_::SetSampler
        HRESULT SetSampler(vbInt32 Slot, Graphic_Texture_Edge EdgeU, Graphic_Texture_Edge EdgeV, Graphic_Texture_Filter Filter) override;

        // \see Graphic_Material_::SetParameter
        HRESULT SetParameter(vbInt32 Offset, vbReal32 X, vbReal32 Y, vbReal32 Z, vbReal32 W) override;

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