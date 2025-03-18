// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
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

#include "Aurora.Scene/Service.hpp"

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
        ~Kernel() override;

        // -=(Undocumented)=-
        void Initialize(Mode Mode, ConstRef<Properties> Properties);

        // -=(Undocumented)=-
        void Poll();

        // -=(Undocumented)=-
        void Run();

        // -=(Undocumented)=-
        void Exit();

        // -=(Undocumented)=-
        Ref<Device> GetDevice()
        {
            return * mDevice;
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
        virtual void OnTick(ConstRef<Time> Tick)
        {
        }

    protected:

        // -=(Undocumented)=-
        struct ProxyInputListener final : Listener
        {
            Ref<Kernel> Kernel;

            // -=(Undocumented)=-
            explicit ProxyInputListener(Ref<Engine::Kernel> Kernel)
                : Kernel { Kernel }
            {
            }

            // \see Listener::OnEvent
            Bool OnEvent(ConstRef<Input::Event> Event) override
            {
                return Kernel.OnEvent(Event);
            }
        };

        // \see Listener::OnWindowExit
        Bool OnWindowExit() override;

        // \see Listener::OnWindowResize
        Bool OnWindowResize(UInt32 Width, UInt32 Height) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool         mActive;
        Time         mTime;
        UPtr<Device> mDevice;
    };
}