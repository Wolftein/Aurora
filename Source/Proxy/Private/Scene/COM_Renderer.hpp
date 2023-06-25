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

#include "Graphic/COM_Camera.hpp"
#include "Graphic/COM_Material.hpp"
#include "Graphic/COM_Pipeline.hpp"
#include "Kernel/COM_Engine.hpp"
#include <Scene/Renderer.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("FB866167-F639-4982-85AC-280255832156"), pointer_default(unique)]
    __interface Scene_Renderer_
    {
        HRESULT Initialize([in] Engine_ * System);

        HRESULT Begin([in] Graphic_Camera_ * Camera);

        HRESULT Draw([in] Math_Rectf * Destination,
                     [in] Math_Rectf * Source,
                     [in] vbReal32 Depth,
                     [in] vbReal32 Angle,
                     [in] vbInt32 Color,
                     [in] Graphic_Pipeline_ * Pipeline,
                     [in] Graphic_Material_ * Material);

        HRESULT End();
    };

    // -=(Undocumented)=-
    [coclass, uuid("42407F72-E795-40E0-B6D6-06C949932CD7")]
    class ATL_NO_VTABLE Scene_Renderer : public Scene_Renderer_, public CSmartPtrWrapper<Scene::Renderer>
    {
    public:

        // \inheritDoc
        HRESULT FinalRelease();

        // \see Scene_Renderer_::Initialize
        HRESULT Initialize(Engine_ * System) override;

        // \see Scene_Renderer_::Begin
        HRESULT Begin(Graphic_Camera_ * Camera) override;

        // \see Scene_Renderer_::Draw
        HRESULT Draw(Math_Rectf * Destination,
                     Math_Rectf * Source,
                     vbReal32 Depth,
                     vbReal32 Angle,
                     vbInt32 Color,
                     Graphic_Pipeline_ * Pipeline,
                     Graphic_Material_ * Material) override;

        // \see Scene_Renderer_::End
        HRESULT End() override;
    };
}