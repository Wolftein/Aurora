// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "COM_Renderer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::FinalRelease()
    {
        mWrapper = nullptr;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::Begin(Graphic_Camera_ * Camera)
    {
        mWrapper->Begin(CCast<Graphic_Camera>(Camera).GetWorld());
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::SetParameter(vbInt32 Offset, vbReal32 X, vbReal32 Y, vbReal32 Z, vbReal32 W)
    {
        mWrapper->SetParameter(Offset, Vector4f(X, Y, Z, W));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::DrawLine(vbReal32 StartX, vbReal32 StartY, vbReal32 EndX, vbReal32 EndY, vbReal32 Depth, vbInt32 Tint, vbReal32 Thickness)
    {
        mWrapper->DrawLine(
            Array<Vector2f, 2> { Vector2f(StartX, StartY), Vector2f(EndX, EndY) }, Depth, Color(Tint), Thickness);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::DrawRectangle(Math_Rectf * Rectangle, vbReal32 Depth, vbReal32 Angle, vbInt32 Tint)
    {
        mWrapper->DrawRectangle(Ref<Rectf>(*Rectangle), Depth, Angle, Color(Tint));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::DrawTexture(Math_Rectf * Rectangle,
                                          Math_Rectf * Source,
                                          vbReal32 Depth,
                                          vbReal32 Angle,
                                          Renderer_Order Order,
                                          vbInt32  Tint,
                                          Graphic_Pipeline_ * Pipeline,
                                          Graphic_Material_ * Material)
    {
        Graphic::Renderer::Order     COrder    = static_cast<Graphic::Renderer::Order>(Order);
        ConstSPtr<Graphic::Pipeline> CPipeline = CCast<Graphic_Pipeline>(Pipeline);
        ConstSPtr<Graphic::Material> CMaterial = CCast<Graphic_Material>(Material);

        mWrapper->DrawTexture(
            Ref<Rectf>(*Rectangle), Ref<Rectf>(*Source), Depth, Angle, COrder, Color(Tint), CPipeline, CMaterial);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::DrawFont(Graphic_Font_ * Font,
                                       vbStr16 Text,
                                       vbReal32 X,
                                       vbReal32 Y,
                                       vbReal32 Depth,
                                       vbReal32 Scale,
                                       vbInt32 Tint,
                                       Renderer_Alignment Alignment)
    {
        ConstSPtr<Graphic::Font>     CFont      = CCast<Graphic_Font>(Font);
        Graphic::Renderer::Alignment CAlignment = static_cast<Graphic::Renderer::Alignment>(Alignment);

        mWrapper->DrawFont(CFont, VBString16ToString16(Text), Vector2f(X, Y), Depth, Scale, Color(Tint), CAlignment);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Graphic_Renderer::End()
    {
        mWrapper->End();
        return S_OK;
    }
}