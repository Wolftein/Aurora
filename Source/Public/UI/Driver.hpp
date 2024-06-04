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

#include "Common.hpp"
#include "Platform/Window.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=(Undocumented)=-
    class Driver
    {
    public:

        // -=(Undocumented)=-
        virtual ~Driver() = default;

        // -=(Undocumented)=-
        virtual Bool Initialise(ConstSPtr<Platform::Window> Window) = 0;

        // -=(Undocumented)=-
        virtual void Advance(Real64 Time) = 0;

        // -=(Undocumented)=-
        virtual void Present() = 0;

        // -=(Undocumented)=-
        virtual Bool Load(CStr Uri) = 0;

        // -=(Undocumented)=-
        virtual Value Call(CStr Function, CPtr<const Value> Parameters) = 0;

        // -=(Undocumented)=-
        virtual Value Eval(CStr Script) = 0;

        // -=(Undocumented)=-
        virtual void Register(CStr Function, Callback OnExecute) = 0;

        // -=(Undocumented)=-
        virtual void Unregister(CStr Function) = 0;
    };
}