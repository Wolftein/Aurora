// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Kernel.hpp"
#include <windows.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Initialize(Ref<const Properties> Properties)
    {
        // Creates the logging service
        Log::Service::GetSingleton().Initialise("Aurora.log");

        // Creates the platform service
        LOG_INFO("Initializing platform service");
        SPtr<Platform::Service> PlatformService = AddSubsystem<Platform::Service>();

        // Creates the input service
        LOG_INFO("Initializing input service");
        AddSubsystem<Input::Service>();

        // Create the game's window
        LOG_INFO("Initializing display");
        mDisplay = PlatformService->Initialise(
            Properties.GetWindowTitle(),
            Properties.GetWindowWidth(),
            Properties.GetWindowHeight());

        // Create the graphic service
        LOG_INFO("Initializing graphics service");
        SPtr<Graphic::Service> GraphicService = AddSubsystem<Graphic::Service>();
        GraphicService->Initialise(
            Graphic::Backend::Direct3D11, mDisplay->GetHandle(), Properties.GetWindowWidth(), Properties.GetWindowHeight());

        // Create the audio service
        LOG_INFO("Initializing audio service");
        SPtr<Audio::Service> AudioService = AddSubsystem<Audio::Service>();
        AudioService->Initialise(Audio::Backend::XAudio2, Audio::Driver::k_MaxSubmixes);

        // Create the content service
        LOG_INFO("Initializing content service");
        AddSubsystem<Content::Service>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Run()
    {
        while (mDisplay->Poll())
        {
            Execute([](Ref<Core::Subsystem> Service)
            {
                Service.OnTick();
            });
        }
    }
}