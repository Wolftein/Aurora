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
        HRESULT Initialize([in] vbStr8 Name);

        HRESULT GetID([out, retval] vbInt32 * Result);

        HRESULT SetTexture([in] vbInt32 Slot, [in] Graphic_Texture_ * Texture);
    };

    // -=(Undocumented)=-
    [coclass, uuid("3015D5E9-DEAB-41F7-8810-E963C69BADF9")]
    class ATL_NO_VTABLE Graphic_Material : public Graphic_Material_, public CSmartPtrWrapper<Graphic::Material>
    {
    public:

        // \inheritDoc
        HRESULT FinalRelease();

        // \see Graphic_Material_::Initialize
        HRESULT Initialize(vbStr8 Name) override;

        // \see Graphic_Material_::GetDepth
        HRESULT GetID(vbInt32 * Result) override;

        // \see Graphic_Material_::SetTexture
        HRESULT SetTexture(vbInt32 Slot, Graphic_Texture_ * Texture) override;

        // \see Content_Resource_::GetKey
        HRESULT GetKey(vbStr16 * Result) override;

        // \see Content_Resource_::GetMemory
        HRESULT GetMemory(vbInt64 * Result) override;

        // \see Content_Resource_::GetCategory
        HRESULT GetCategory(Content_Resource_Type * Result) override;
    };
}