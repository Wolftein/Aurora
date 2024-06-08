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

#include "Kernel.hpp"
#include "Host.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Kernel::Kernel()
        : mState { State::Idle }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Kernel::~Kernel()
    {
        Log::Service::GetSingleton().Shutdown();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Initialize(Mode Mode, Ref<const Properties> Properties, SPtr<Host> Host)
    {
        // Creates the logging service
        Log::Service::GetSingleton().Initialize(Properties.GetLogFilename());

        // Set the mode
        SetMode(Mode);

        // Creates the platform service
        LOG_INFO("Kernel: Creating platform service");
        ConstSPtr<Platform::Service> Platform = AddSubsystem<Platform::Service>();

        // Creates multimedia services (if running in client mode)
        if (IsClientMode())
        {
            // Creates the input service
            LOG_INFO("Kernel: Creating input service");
            ConstSPtr<Input::Service> Input = AddSubsystem<Input::Service>();
            Input->AddListener(Host);

            // Create the game's window
            Any DisplayHandle = Properties.GetWindowHandle();

            if (!DisplayHandle.has_value())
            {
                LOG_INFO("Kernel: Creating display ({}, {})", Properties.GetWindowWidth(), Properties.GetWindowHeight());
                Platform->Initialize(
                    Properties.GetWindowTitle(),
                    Properties.GetWindowWidth(),
                    Properties.GetWindowHeight(),
                    Properties.IsWindowFullscreen(),
                    Properties.IsWindowBorderless());
                DisplayHandle = Platform->GetWindow()->GetHandle();
            }

            // Create the graphic service
            LOG_INFO("Kernel: Creating graphics service");
            ConstSPtr<Graphic::Service> GraphicService = AddSubsystem<Graphic::Service>();
            if (! GraphicService->Initialize(
                Graphic::Backend::Direct3D11, DisplayHandle, Properties.GetWindowWidth(), Properties.GetWindowHeight()))
            {
                LOG_WARNING("Kernel: Failed to create graphics service, disabling service.");
                GraphicService->Initialize(Graphic::Backend::None, nullptr, 0, 0);
            }

            // Create the audio service
            LOG_INFO("Kernel: Creating audio service");
            ConstSPtr<Audio::Service> AudioService = AddSubsystem<Audio::Service>();
            if (! AudioService->Initialize(Audio::Backend::XAudio2, Audio::k_MaxSubmixes))
            {
                LOG_WARNING("Kernel: Failed to create audio service, disabling service.");
                AudioService->Initialize(Audio::Backend::None, Audio::k_MaxSubmixes);
            }
        }

        // Create the content service
        LOG_INFO("Kernel: Creating content service");
        AddSubsystem<Content::Service>();

        // Create the network service
        LOG_INFO("Kernel: Creating network service");
        ConstSPtr<Network::Service> NetworkService = AddSubsystem<Network::Service>();
        if (! NetworkService->Initialize())
        {
            LOG_WARNING("Kernel: Failed to create network service, disabling service.");
            RemoveSubsystem<Network::Service>();
        }

        // Create the user interface service
        if (IsClientMode())
        {
            LOG_INFO("Kernel: Creating user interface service");
            ConstSPtr<UI::Service> UIService = AddSubsystem<UI::Service>();
            if (! UIService->Initialize(Platform->GetWindow()))
            {
                LOG_WARNING("Kernel: Failed to create user interface service, disabling service.");
                RemoveSubsystem<UI::Service>();
            }
        }

        // Initialize the Host and then enable the platform's window
        mHost = Host;
        mHost->OnStart();

        if (ConstSPtr<Platform::Window> Window = Platform->GetWindow())
        {
            Window->SetVisible(true);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Run()
    {
        mState = State::Running;

        while (mState == State::Running)
        {
            const Real64 Time = GetSubsystem<Platform::Service>()->GetTime();

            Execute([Time](Ref<Core::Subsystem> Service)
            {
                Service.OnTick(Time);
            });

            ConstSPtr<Engine::Activity> Foreground = (mActivities.empty() ? nullptr : mActivities.back());
            if (Foreground)
            {
                Foreground->OnTick(Time);
            }

            mHost->OnTick(Time);
        }

        mHost->OnStop();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Exit()
    {
        mState = State::Exiting;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Goto(ConstSPtr<Activity> Foreground)
    {
        ConstSPtr<Activity> Current = GetForeground();
        if (Current)
        {
            Current->OnPause();
        }

        mActivities.emplace_back(Foreground);
        Foreground->OnAttach();
        Foreground->OnResume();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Back()
    {
        ConstSPtr<Engine::Activity> Current = GetForeground();
        if (Current)
        {
            Current->OnPause();
            Current->OnDetach();
            mActivities.pop_back();
        }

        ConstSPtr<Engine::Activity> Newest = GetForeground();
        if (Newest)
        {
            Newest->OnResume();
        }
    }
}