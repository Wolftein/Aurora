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

#include "Core/Math/COM_Rect.hpp"
#include "Graphic/COM_Common.hpp"
#include "Graphic/COM_Camera.hpp"
#include "Graphic/COM_Material.hpp"
#include "Graphic/COM_Pipeline.hpp"
#include "Graphic/COM_Font.hpp"
#include <Graphic/Renderer.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [export, uuid("D260AA57-1CFF-11EE-ADCD-1418C3A8EDB8"), v1_enum]
    typedef enum Renderer_Order
    {
        eRendererOrderOpaque,
        eRendererOrderTransparent,
    } Renderer_Order_Draw;

    // -=(Undocumented)=-
    [export, uuid("D260AA57-1CFF-11EE-ADCD-1418C3A8EDB9"), v1_enum]
    typedef enum Renderer_Alignment
    {
        eRendererAlignmentLeftTop        = (1 << 0) | (1 << 3),
        eRendererAlignmentLeftMiddle     = (1 << 0) | (1 << 4),
        eRendererAlignmentLeftBottom     = (1 << 0) | (1 << 5),
        eRendererAlignmentLeftBaseline   = (1 << 0) | (1 << 6),
        eRendererAlignmentCenterTop      = (1 << 1) | (1 << 3),
        eRendererAlignmentCenterMiddle   = (1 << 1) | (1 << 4),
        eRendererAlignmentCenterBottom   = (1 << 1) | (1 << 5),
        eRendererAlignmentCenterBaseline = (1 << 1) | (1 << 6),
        eRendererAlignmentRightTop       = (1 << 2) | (1 << 3),
        eRendererAlignmentRightMiddle    = (1 << 2) | (1 << 4),
        eRendererAlignmentRightBottom    = (1 << 2) | (1 << 5),
        eRendererAlignmentRightBaseline  = (1 << 2) | (1 << 6),
    } Renderer_Alignment;

    // -=(Undocumented)=-
    [object, uuid("FB866167-F639-4982-85AC-280255832156"), pointer_default(unique)]
    __interface Graphic_Renderer_
    {
        HRESULT Begin([in] Graphic_Camera_ * Camera);

        HRESULT SetParameter([in] vbInt32 Offset, [in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z, [in] vbReal32 W);

        HRESULT Draw([in] Math_Rectf * Destination,
                     [in] Math_Rectf * Source,
                     [in] vbReal32 Depth,
                     [in] vbReal32 Angle,
                     [in] vbInt32 Tint,
                     [in] Renderer_Order Order,
                     [in] Graphic_Pipeline_ * Pipeline,
                     [in] Graphic_Material_ * Material);

        HRESULT DrawFont([in] Graphic_Font_ * Font,
                         [in] Graphic_Pipeline_ * Pipeline,
                         [in] vbStr16 Text,
                         [in] vbReal32 X,
                         [in] vbReal32 Y,
                         [in] vbReal32 Depth,
                         [in] vbReal32 Scale,
                         [in] vbInt32 Tint,
                         [in] Renderer_Alignment Alignment);

        HRESULT End();
    };

    // -=(Undocumented)=-
    [coclass, uuid("42407F72-E795-40E0-B6D6-06C949932CD7")]
    class ATL_NO_VTABLE Graphic_Renderer : public Graphic_Renderer_, public CSmartPtrWrapper<Graphic::Renderer>
    {
    public:

        // \inheritDoc
        HRESULT FinalRelease();

        // \see Graphic_Renderer_::Begin
        HRESULT Begin(Graphic_Camera_ * Camera) override;

        // \see Graphic_Renderer_::SetParameter
        HRESULT SetParameter(vbInt32 Offset, vbReal32 X, vbReal32 Y, vbReal32 Z, vbReal32 W);

        // \see Graphic_Renderer_::Draw
        HRESULT Draw(Math_Rectf * Destination,
                     Math_Rectf * Source,
                     vbReal32 Depth,
                     vbReal32 Angle,
                     vbInt32 Tint,
                     Renderer_Order Order,
                     Graphic_Pipeline_ * Pipeline,
                     Graphic_Material_ * Material) override;

        // \see Graphic_Renderer_::DrawFont
        HRESULT DrawFont(Graphic_Font_ * Font,
                         Graphic_Pipeline_ * Pipeline,
                         vbStr16 Text,
                         vbReal32 X,
                         vbReal32 Y,
                         vbReal32 Depth,
                         vbReal32 Scale,
                         vbInt32 Tint,
                         Renderer_Alignment Alignment) override;

        // \see Scene_Renderer_::End
        HRESULT End() override;
    };
}