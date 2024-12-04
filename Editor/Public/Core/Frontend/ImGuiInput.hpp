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

#include <Aurora.Input/Service.hpp>
#include <imgui.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Editor
{
    // -=(Undocumented)=-
    class ImGuiInput final : public Input::Listener
    {
    private:

        // -=(Undocumented)=-
        Bool OnKeyType(UInt32 Codepoint) override;

        // -=(Undocumented)=-
        Bool OnKeyUp(Input::Key Key) override;

        // -=(Undocumented)=-
        Bool OnKeyDown(Input::Key Key) override;

        // -=(Undocumented)=-
        Bool OnMouseMove(Real32 X, Real32 Y, Real32 DeltaX, Real32 DeltaY) override;

        // -=(Undocumented)=-
        Bool OnMouseScroll(Real32 DeltaX, Real32 DeltaY) override;

        // -=(Undocumented)=-
        Bool OnMouseUp(Input::Button Button) override;

        // -=(Undocumented)=-
        Bool OnMouseDown(Input::Button Button)  override;

        // -=(Undocumented)=-
        Bool OnWindowResize(UInt32 Width, UInt32 Height) override;
    };
}