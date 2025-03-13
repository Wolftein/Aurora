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
    Scene::Entity GrandMaster;
    Scene::Entity Master;
    Scene::Entity Child;
    Scene::Entity Random;
    Scene::Entity MySprite;

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    
    Bool Application::OnInitialize()
    {
        //
        ConstSPtr<Content::Service> Content = GetSubsystem<Content::Service>();
        Content->AddLocator("Root", NewPtr<Content::SystemLocator>("Resources"));

        const auto Font = Content->Load<Graphic::Font>("Resources://Font/Primary.arfont");

        // Initialize the scene.
        ConstSPtr<Scene::Service> Scene = GetSubsystem<Scene::Service>();

        // Transform ->
        // Matrix    ->
        // Pivot
        // Animation



        // Matrix
        // Pivot
        // ???????????
        // XxX_Wolftein_XxX
        //


        Scene->Observe<Scene::TEcsMatrix>()
                .event(flecs::OnSet)
                .self()
                .each([](flecs::entity Entity, Ref<Scene::TEcsMatrix> _1)
                {
                    Log::Info("Entity  with TEcsMatrix Changed");
                });


        GrandMaster = Scene->Create();
        GrandMaster.SetName("Grand Master");
        GrandMaster.Attach(Scene::TEcsTransform(Vector3f(256, 256, 0)));
        GrandMaster.Attach(Scene::TEcsText(Font, 32, L"[Fers]"));
        GrandMaster.Attach(Scene::TEcsTint(0, 1, 0, 1));

        Master = Scene->Create();
        Master.SetName("Master");
        Master.SetParent(GrandMaster);
        Master.Attach(Scene::TEcsTransform(
                Vector3f(0, 32, 0),
                Vector3f(1),
                Quaternionf::FromAngles(DegreesToRadians(90), Vector3f(0, 0, 1))));
        Master.Attach(Scene::TEcsPivot(Pivot::Type::CenterMiddle));
        Master.Attach(Scene::TEcsText(Font, 32, L"[Es Puto]"));
        Master.Attach(Scene::TEcsTint(1, 0, 0, 1));

        Child = Scene->Create();
        Child.SetParent(Master);
        Child.SetName("Child");
        Child.Attach(Scene::TEcsTransform(
                Vector3f(12, 24, 0),
                Vector3f(1),
                Quaternionf::FromAngles(DegreesToRadians(90), Vector3f(0, 0, 1))));
        Child.Attach(Scene::TEcsText(Font, 12, L"[CHILD_OF_MASTER]"));
        Child.Attach(Scene::TEcsTint(0, 0, 1, 1));

        Random = Scene->Create();
        Random.SetParent(GrandMaster);
        Random.Attach(Scene::TEcsTransform(
                Vector3f(-100, -100, 0.5f),
                Vector3f(0.5),
                Quaternionf::FromAngles(DegreesToRadians(125), Vector3f(1, 1, 0))));
        Random.Attach(Scene::TEcsText(Font, 16, L"[Hello SIR]"));
        Random.Attach(Scene::TEcsTint(0, 1, 1, 1));



        ConstSPtr<Graphic::Material> Material = Graphic::Material::GetFactory().GetOrCreate(
                Content::Uri(Format("Memory://Material/{}", 1)), true);
        ConstSPtr<Graphic::Texture> Texture = Content->Load<Graphic::Texture>(
                Content::Uri(Format("Resources://Texture/{}.png", 8165)));
        const Graphic::Sampler Sampler(
                Graphic::TextureEdge::Repeat, Graphic::TextureEdge::Repeat, Graphic::TextureFilter::Nearest);
        Material->SetTexture(Graphic::TextureSlot::Diffuse, Texture);
        Material->SetSampler(Graphic::TextureSlot::Diffuse, Sampler);
        Content->Process(Material, true);

        MySprite = Scene->Create();
        MySprite.Attach(Scene::TEcsTransform(Vector3f(333, 333, 0.7f), Vector3f(0.5), Quaternionf()));
        MySprite.Attach(Scene::TEcsSprite(Material, Rectf(0, 0, 500, 400)));
        MySprite.Attach(Scene::TEcsPivot(Pivot::Type::CenterMiddle));
        MySprite.Attach(Scene::TEcsTint(0xFF0000FF));


        /*
        auto Night1 = Scene->Create();
        Night1.Attach(Scene::TEcsTransform(Vector3f(300, 100, 0)));
        Night1.Attach(Scene::TEcsText(Font, 32, L"Nightw"));
        Night1.Attach(Scene::TEcsTint(1, 0, 1, 1));
        Night1.Attach(Scene::TEcsPivot(Pivot::Type::CenterMiddle));

        Scene::Entity LastEntity = Night1;

        for (UInt32 I = 31; I >= 10; --I)
        {
            SStr16 TEXT = Format(L"PT - {}px", I);

            auto Night2 = Scene->Create();
            Night2.SetParent(LastEntity);
            Night2.Attach(Scene::TEcsTransform(Vector3f(0, I, 0)));
            Night2.Attach(Scene::TEcsText(Font, I, TEXT));
            Night2.Attach(Scene::TEcsTint(1, 1, 0, 1));
            Night2.Attach(Scene::TEcsPivot(Pivot::Type::CenterMiddle));
            LastEntity = Night2;
        }*/

        // Initialize Camera.
        mCamera.SetOrthographic(GetDevice().GetWidth(), GetDevice().GetHeight(), 0, 1);
        mCamera.Compute();

        // Initial test
        mTexts = NewUniquePtr<Scene::TEcsTextSystem>(* this);

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(ConstRef<Time> Time)
    {
        ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>();

        Rectf Viewport(0, 0, GetDevice().GetWidth(), GetDevice().GetHeight());
        Graphics->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, Color(0, 0, 0, 0), 1, 0);
        {
            static Graphic::Encoder Encoder;
            {
                Encoder.Clear();
                mTexts->Draw(Encoder, mCamera);
            }
            Graphics->Submit(Encoder, true);
        }
        Graphics->Commit(Graphic::k_Default, false);
        Graphics->Flush();

        static Real32 Angles = 0;
        Angles += 0.01f;
        if (Angles >= 360.0f) { Angles -= 360.0f; }

        Random.Attach(Scene::TEcsTransform(
                Vector3f(-100, -100, 0.5f),
                Vector3f(4),
                Quaternionf::FromAngles(DegreesToRadians(Angles), Vector3f(0, 0, 1))));

        GrandMaster.Obtain<Scene::TEcsTransform>()->SetRotation(Quaternionf::FromAngles(DegreesToRadians(Angles), Vector3f(0, 0, 1)));
        GrandMaster.Notify<Scene::TEcsTransform>();

        MySprite.Obtain<Scene::TEcsTransform>()->SetRotation(Quaternionf::FromAngles(DegreesToRadians(Angles), Vector3f(0, 0, 1)));
        MySprite.Notify<Scene::TEcsTransform>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDestroy()
    {

    }

    Bool Application::OnKeyDown(Input::Key Key)
    {
        return true;
    }

    Bool Application::OnMouseMove(Real32 X, Real32 Y, Real32 DeltaX, Real32 DeltaY)
    {
        GrandMaster.Obtain<Scene::TEcsTransform>()->SetPosition(Vector2f { X, Y});
        GrandMaster.Notify<Scene::TEcsTransform>();
        return true;
    }

    Bool Application::OnWindowResize(UInt32 Width, UInt32 Height)
    {
        return true;
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
    Properties.SetWindowWidth(1920);
    Properties.SetWindowHeight(1024);
    Properties.SetWindowSamples(1);
    Properties.SetWindowFullscreen(false);
    Properties.SetWindowBorderless(false);
    Properties.SetVideoDriver("D3D11");

    // Initialize 'Aurora Engine' and enter main loop
    UPtr<Example::Application> Application = NewUniquePtr<Example::Application>();
    Application->Initialize(System<Subsystem>::Mode::Client, Properties);
    Application->Run();

    return 0;
}