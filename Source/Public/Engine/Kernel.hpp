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

#include "Audio/Service.hpp"

#include "Content/Service.hpp"

#include "Properties.hpp"

#include "Graphic/Service.hpp"

#include "Input/Service.hpp"

#include "Network/Service.hpp"

#include "Platform/Service.hpp"

#include "UI/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=(Undocumented)=-
    class Kernel : public Subsystem::Context, public Input::Listener
    {
    public:

        // -=(Undocumented)=-
        Kernel();

        // -=(Undocumented)=-
        ~Kernel();

        // -=(Undocumented)=-
        void Initialize(Mode Mode, Ref<const Properties> Properties);

        // -=(Undocumented)=-
        void Run();

        // -=(Undocumented)=-
        void Exit();

    private:

        // -=(Undocumented)=-
        virtual Bool OnInitialize();

        // -=(Undocumented)=-
        virtual void OnDestroy();

        // -=(Undocumented)=-
        virtual void OnTick(Real64 Time);

    private:

        // -=(Undocumented)=-
        struct ProxyInputListener final : public Input::Listener
        {
            Ref<Kernel> Kernel;

            // -=(Undocumented)=-
            ProxyInputListener(Ref<Engine::Kernel> Kernel)
                : Kernel { Kernel }
            {
            }

            // \see Listener::OnEvent
            Bool OnEvent(Ref<const Input::Event> Event) override
            {
                return Kernel.OnEvent(Event);
            }
        };

        // \see Listener::OnWindowExit
        Bool OnWindowExit() override;

        // \see Listener::OnWindowResize
        Bool OnWindowResize(SInt Width, SInt Height) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool mActive;
    };
}