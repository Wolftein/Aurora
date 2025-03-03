// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Kernel.hpp"

#ifdef    __EMSCRIPTEN__
    #include <emscripten.h>
#endif // __EMSCRIPTEN__

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Kernel::Kernel()
        : mActive { false }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Kernel::~Kernel()
    {
        Log::Shutdown();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Initialize(Mode Mode, ConstRef<Properties> Properties)
    {
        // Creates the logging service
        Log::Initialize("Aurora.log");

        // Set the mode
        SetMode(Mode);

        // Creates multimedia services (if running in client mode)
        if (IsClientMode())
        {
            // Creates the input service and attach us
            Log::Info("Kernel: Creating input service");
            ConstSPtr<Input::Service> Input = AddSubsystem<Input::Service>();
            Input->Attach(NewPtr<ProxyInputListener>(* this));

            // Create or attach the game's device
            const Graphic::Backend GraphicBackend
                = CastEnum(Properties.GetVideoDriver(), Graphic::Backend::None);

            Log::Info("Kernel: Creating device ({}, {})", Properties.GetWindowWidth(), Properties.GetWindowHeight());
            mDevice = NewUniquePtr<Device>(
                Properties.GetWindowHandle(),
                Properties.GetWindowTitle(),
                Properties.GetWindowWidth(),
                Properties.GetWindowHeight(),
                GraphicBackend,
                Properties.IsWindowFullscreen(),
                Properties.IsWindowBorderless());

            // Create the graphic service
            Log::Info("Kernel: Creating graphics service");
            ConstSPtr<Graphic::Service> GraphicService = AddSubsystem<Graphic::Service>();
            if (! GraphicService->Initialize(
                GraphicBackend, mDevice->GetHandle(), mDevice->GetWidth(), mDevice->GetHeight(), Properties.GetWindowSamples()))
            {
                Log::Warn("Kernel: Failed to create graphics service, disabling service.");
                RemoveSubsystem<Graphic::Service>();
            }

            // Create the audio service
            Log::Info("Kernel: Creating audio service");
            ConstSPtr<Audio::Service> AudioService = AddSubsystem<Audio::Service>();
            if (! AudioService->Initialize(Audio::Backend::FAudio, Properties.GetAudioDevice()))
            {
                Log::Warn("Kernel: Failed to create audio service, disabling service.");
                RemoveSubsystem<Audio::Service>();
            }
        }

        // Create the content service
        Log::Info("Kernel: Creating content service");
        AddSubsystem<Content::Service>();

        // Create the network service
        Log::Info("Kernel: Creating network service");
        ConstSPtr<Network::Service> NetworkService = AddSubsystem<Network::Service>();
        if (! NetworkService->Initialize(IsServerMode() ? SDL_GetNumLogicalCPUCores() : 1))
        {
            Log::Warn("Kernel: Failed to create network service, disabling service.");
            RemoveSubsystem<Network::Service>();
        }

        // Create the scene service
        Log::Info("Kernel: Creating scene service");
        AddSubsystem<Scene::Service>();

        // Initialize the Host and then enable the platform's device
        mActive = OnInitialize();

        if (mDevice && mActive)
        {
            mDevice->SetVisible(true);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Poll()
    {
        // Updates the current time by calculating the absolute time
        mTime.SetAbsolute(static_cast<Real64>(SDL_GetTicksNS()) / static_cast<Real64>(SDL_NS_PER_SECOND));

        // Time subsystems (order matters)
        Tick(mTime);

        // Time application
        OnTick(mTime);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Run()
    {
#ifdef    __EMSCRIPTEN__

        const auto OnPoll = [](Ptr<void> Instance)
        {
            static_cast<Ptr<Kernel>>(Instance)->Poll();
        };

        emscripten_set_main_loop_arg(OnPoll, this, 0, true);

#else

        while (mActive)
        {
            Poll();
        }

#endif // __EMSCRIPTEN__

        OnDestroy();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Exit()
    {
        mActive = false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Kernel::OnWindowExit()
    {
        Exit();
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Kernel::OnWindowResize(UInt32 Width, UInt32 Height)
    {
        ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>();

        Graphics->Reset(Width, Height, Graphics->GetCapabilities().Samples);
        return false;
    }
}