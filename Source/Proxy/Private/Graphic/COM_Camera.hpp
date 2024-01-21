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

#include "Core/COM_Common.hpp"
#include <Graphic/Camera.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("3CE05061-B168-41E3-B927-7C0B254AC3E1"), pointer_default(unique)]
    __interface Graphic_Camera_
    {
        HRESULT Compute();

        HRESULT SetPerspective([in] vbReal32 Eyes, [in] vbReal32 Aspect, [in] vbReal32 ZNear, [in] vbReal32 ZFar);

        HRESULT SetOrthographic([in] vbReal32 Left,
                                [in] vbReal32 Right,
                                [in] vbReal32 Bottom,
                                [in] vbReal32 Top,
                                [in] vbReal32 ZNear,
                                [in] vbReal32 ZFar);

        HRESULT SetPosition([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT SetScale([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT SetRotation([in] vbReal32 Angle, [in] vbReal32 AxisX, [in] vbReal32 AxisY, [in] vbReal32 AxisZ);

        HRESULT Translate([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT Scale([in] vbReal32 X, [in] vbReal32 Y, [in] vbReal32 Z);

        HRESULT Rotate([in] vbReal32 Angle, [in] vbReal32 AxisX, [in] vbReal32 AxisY, [in] vbReal32 AxisZ);
    };

    // -=(Undocumented)=-
    [coclass, uuid("7F01B9AB-EC2C-4986-9E94-BC896CE27B56")]
    class ATL_NO_VTABLE Graphic_Camera : public Graphic_Camera_, public CObjectWrapper<Graphic::Camera>
    {
    public:

        // \see Graphic_Camera_::Compute
        HRESULT Compute() override;

        // \see Graphic_Camera_::SetPerspective
        HRESULT SetPerspective(vbReal32 Eyes, vbReal32 Aspect, vbReal32 ZNear, vbReal32 ZFar) override;

        // \see Graphic_Camera_::SetOrthographic
        HRESULT SetOrthographic(vbReal32 Left,
                                vbReal32 Right,
                                vbReal32 Bottom,
                                vbReal32 Top,
                                vbReal32 ZNear,
                                vbReal32 ZFar) override;

        // \see Graphic_Camera_::SetPosition
        HRESULT SetPosition(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \see Graphic_Camera_::SetScale
        HRESULT SetScale(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \see Graphic_Camera_::SetRotation
        HRESULT SetRotation(vbReal32 Angle, vbReal32 AxisX, vbReal32 AxisY, vbReal32 AxisZ) override;

        // \see Graphic_Camera_::Translate
        HRESULT Translate(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \see Graphic_Camera_::Scale
        HRESULT Scale(vbReal32 X, vbReal32 Y, vbReal32 Z) override;

        // \see Graphic_Camera_::Rotate
        HRESULT Rotate(vbReal32 Angle, vbReal32 AxisX, vbReal32 AxisY, vbReal32 AxisZ) override;
    };
}