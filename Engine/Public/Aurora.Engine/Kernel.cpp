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
        Log::Initialize("Aurora.log");
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
        AURORA_THREAD_NAME("Main Thread");

        SetMode(Mode);

        // Initializes client services.
        if (IsClientMode())
        {
            InitializeClientServices(Properties);
        }

        // Initializes server services.
        if (IsServerMode())
        {
            InitializeServerServices(Properties);
        }

        // Initializes common services.
        InitializeCommonServices(Properties);

        // Initializes the host and then enable the platform device.
        mActive = OnInitialize();

        if (mDevice.GetHandle() != nullptr)
        {
            mDevice.SetVisible(mActive);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Poll()
    {
        // Updates the current time by calculating the absolute time.
        mTime.SetAbsolute(static_cast<Real64>(SDL_GetTicksNS()) * (1.0 / SDL_NS_PER_SECOND));

        // Tick services (order matters).
        Tick(mTime);

        // Tick application.
        OnTick(mTime);

        AURORA_PROFILE_FRAME;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Run()
    {
        // Updates the simulation time by calculating the absolute time.
        mTime.SetAbsolute(static_cast<Real64>(SDL_GetTicksNS()) * (1.0 / SDL_NS_PER_SECOND));

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

        // Performs application-level teardown.
        OnTeardown();

        // Performs subsystem-level teardown.
        Teardown();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::Exit()
    {
        mActive = false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::InitializeClientServices(ConstRef<Properties> Properties)
    {
        // Initializes input service.
        LOG_INFO("Kernel: Creating input service");
        ConstTracker<Input::Service> Input = AddService<Input::Service>();
        Input->Attach(Tracker<Input::Listener>(this));

        // Initializes device service.
        LOG_INFO("Kernel: Creating device ({}, {})", Properties.GetWindowWidth(), Properties.GetWindowHeight());
        InPlaceConstruct<Device>(mDevice,
            Properties.GetWindowHandle(),
            Properties.GetWindowTitle(),
            Properties.GetWindowWidth(),
            Properties.GetWindowHeight(),
            Properties.IsWindowFullscreen(),
            Properties.IsWindowBorderless());

        if (mDevice.GetHandle() == nullptr)
        {
            LOG_ERROR("Failed to create device: '{}'", SDL_GetError());
        }

        // Initializes graphic service.
        const Graphic::Backend Backend = Enum::Cast(Properties.GetVideoDriver(), Graphic::Backend::D3D11);

        Graphic::Samples Samples = Graphic::Samples::X1;
        switch (Properties.GetWindowSamples())
        {
            case 1:
                Samples = Graphic::Samples::X1;
                break;
            case 2:
                Samples = Graphic::Samples::X2;
                break;
            case 4:
                Samples = Graphic::Samples::X4;
                break;
            case 8:
                Samples = Graphic::Samples::X8;
                break;
            default:
                LOG_WARNING("{} invalid samples count, default to 1", Properties.GetWindowSamples());
                break;
        }

        LOG_INFO("Kernel: Creating graphics service");
        ConstTracker<Graphic::Service> Graphics = AddService<Graphic::Service>();

        if (!Graphics->Initialize(Backend, mDevice.GetHandle(), mDevice.GetWidth(), mDevice.GetHeight(), Samples))
        {
            LOG_WARNING("Kernel: Failed to create graphics service.");
        }

        // Initializes audio service.
        LOG_INFO("Kernel: Creating audio service");
        ConstTracker<Audio::Service> AudioService = AddService<Audio::Service>();
        if (! AudioService->Initialize(Audio::Backend::FAudio, Properties.GetAudioDevice()))
        {
            LOG_WARNING("Kernel: Failed to create audio service.");
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::InitializeServerServices(ConstRef<Properties> Properties)
    {
        // TODO: Implement server-specific services initialization.
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Kernel::InitializeCommonServices(ConstRef<Properties> Properties)
    {
        // Initializes resources service.
        LOG_INFO("Kernel: Creating content service");
        AddService<Content::Service>();

        // Initializes scene service.
        LOG_INFO("Kernel: Creating scene service");
        AddService<Scene::Service>();
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
        if (ConstTracker<Graphic::Service> Graphics = GetService<Graphic::Service>())
        {
            Graphics->Reset(Width, Height, Graphics->GetCapabilities().Samples);
        }
        return false;
    }
}