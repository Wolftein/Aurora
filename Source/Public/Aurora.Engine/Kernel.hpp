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

#include "Aurora.Audio/Service.hpp"

#include "Aurora.Content/Service.hpp"

#include "Aurora.Graphic/Service.hpp"

#include "Aurora.Input/Service.hpp"

#include "Aurora.Network/Service.hpp"

#include "Device.hpp"

#include "Properties.hpp"

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

        // -=(Undocumented)=-
        Ref<Device> GetDevice()
        {
            return (* mDevice);
        }

    private:

        // -=(Undocumented)=-
        virtual Bool OnInitialize()
        {
            return true;
        }

        // -=(Undocumented)=-
        virtual void OnDestroy()
        {

        }

        // -=(Undocumented)=-
        virtual void OnTick(UInt64 Time)
        {
        }

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
        Bool OnWindowResize(UInt16 Width, UInt16 Height) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool         mActive;
        UPtr<Device> mDevice;
    };
}