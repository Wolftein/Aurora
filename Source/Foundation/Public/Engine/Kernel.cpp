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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Kernel::~Kernel()
    {
        Log::Service::GetSingleton().Shutdown();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Initialize(Ref<const Properties> Properties)
    {
        // Creates the logging service
        Log::Service::GetSingleton().Initialise(Properties.GetLogFilename());

        // Creates the platform service
        LOG_INFO("Kernel: Creating platform service");
        SPtr<Platform::Service> PlatformService = AddSubsystem<Platform::Service>();

        // Creates the input service
        LOG_INFO("Kernel: Creating input service");
        AddSubsystem<Input::Service>();

        // Create the game's window
        Any      DisplayHandle = Properties.GetWindowHandle();
        Vector2i DisplaySize(Properties.GetWindowWidth(), Properties.GetWindowHeight());

        if (!DisplayHandle.has_value())
        {
            LOG_INFO("Kernel: Creating display ({}, {})", Properties.GetWindowWidth(), Properties.GetWindowHeight());
            mDisplay = PlatformService->Initialise(
                Properties.GetWindowTitle(),
                Properties.GetWindowWidth(),
                Properties.GetWindowHeight(),
                Properties.IsWindowFullscreen(),
                Properties.IsWindowBorderless());

            DisplaySize   = mDisplay->GetSize();
            DisplayHandle = mDisplay->GetHandle();
        }

        // Create the graphic service
        LOG_INFO("Kernel: Creating graphics service");
        SPtr<Graphic::Service> GraphicService = AddSubsystem<Graphic::Service>();
        GraphicService->Initialise(Graphic::Backend::Direct3D11, DisplayHandle, DisplaySize.GetX(), DisplaySize.GetY());

        // Create the audio service
        LOG_INFO("Kernel: Creating audio service");
        SPtr<Audio::Service> AudioService = AddSubsystem<Audio::Service>();
        AudioService->Initialise(Audio::Backend::XAudio2, Audio::k_MaxSubmixes);

        // Create the content service
        LOG_INFO("Kernel: Creating content service");
        AddSubsystem<Content::Service>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Tick()
    {
        Execute([](Ref<Core::Subsystem> Service)
        {
            Service.OnTick();
        });
    }
}