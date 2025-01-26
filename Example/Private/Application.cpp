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

#include "Application.hpp"
#include <Aurora.Content/Locator/SystemLocator.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Example
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        ConstSPtr<Content::Service> Content = GetSubsystem<Content::Service>();
        Content->AddLocator("Root", NewPtr<Content::SystemLocator>("Resources"));

        ConstSPtr<Scene::Service> Scene = GetSubsystem<Scene::Service>();

        mEntities[0] = Scene->Create();
        mEntities[0].SetName("Grand Master");
        mEntities[0].Attach(Component::TEcsTransform());

        mEntities[1] = Scene->Create();
        mEntities[1].SetName("Master");
        mEntities[1].SetParent(mEntities[0]);
        mEntities[1].Attach(Component::TEcsTransform(Vector3f(10, 0, 0)));

        mEntities[2] = Scene->Create();
        mEntities[2].SetName("Child");
        mEntities[2].SetParent(mEntities[1]);
        mEntities[2].Attach(Component::TEcsTransform(Vector3f(0, 0, 10)));

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(ConstRef<Time> Time)
    {
        ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>();

        Rectf Viewport(0, 0, GetDevice().GetWidth(), GetDevice().GetHeight());
        Graphics->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, Color(0, 0, 0, 1), 1, 0);
        {

        }
        Graphics->Commit(Graphic::k_Default, false);
        Graphics->Flush();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDestroy()
    {

    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   MAIN   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef    SDL_PLATFORM_WINDOWS
    #include <Windows.h>
#endif // SDL_PLATFORM_WINDOWS

int main([[maybe_unused]] int Argc, [[maybe_unused]] Ptr<Char> Argv[])
{
#ifdef    SDL_PLATFORM_WINDOWS
    ::CoInitialize(nullptr);
#endif // SDL_PLATFORM_WINDOWS

    Engine::Properties Properties;
    Properties.SetWindowTitle("Aurora Example");
    Properties.SetWindowWidth(1024);
    Properties.SetWindowHeight(768);
    Properties.SetWindowSamples(4);
    Properties.SetWindowFullscreen(false);
    Properties.SetWindowBorderless(false);
    Properties.SetVideoDriver("D3D11");

    // Initialize 'Aurora Engine' and enter main loop
    UPtr<Example::Application> Application = NewUniquePtr<Example::Application>();
    Application->Initialize(System<Subsystem>::Mode::Client, Properties);
    Application->Run();

    return 0;
}