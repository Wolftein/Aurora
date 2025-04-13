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
    Scene::Proxy<Transformf> MyCacheLocalscape;
    Scene::Proxy<Transformf> MyCacheLocalscape2;

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        //
        ConstSPtr<Content::Service> Content = GetSubsystem<Content::Service>();
        Content->AddLocator("Resources", NewPtr<Content::SystemLocator>("Resources"));

        const auto Font = Content->Load<Graphic::Font>("Resources://Font/Primary.arfont");

        // Initialize the scene.
        ConstSPtr<Scene::Service> Scene = GetSubsystem<Scene::Service>();
        Scene->Component<Pivot>().Attach<Scene::Trait::Serializable>();
        Scene->Component<Color>();
        Scene->Component<Matrix4f>().Attach<Scene::Trait::Inheritable | Scene::Trait::Sparse>();
        Scene->Component<Transformf>().With<Matrix4f>();

        // Initial test
        mTexts = NewUniquePtr<Scene::TEcsTextSystem>(* this);

        auto MyArchetype = Scene->Spawn<true>();
        MyArchetype.SetName("Wacho");
        MyArchetype.Attach(Color(0, 1, 0, 1));
        MyArchetype.Attach(Pivot());

        GrandMaster = Scene->Spawn();
        GrandMaster.SetArchetype(MyArchetype);
        GrandMaster.SetName("Grand Master");
        GrandMaster.Attach(Transformf(Vector3f(256, 256, 0)));
        GrandMaster.Attach(Scene::TEcsText(Font, 32, L"[Fers]"));

        MyCacheLocalscape = GrandMaster.Link<Transformf>();

        Master = Scene->Spawn();
        Master.SetArchetype(MyArchetype);
        Master.SetName("Master");
        Master.SetParent(GrandMaster);
        Master.Attach(Transformf(
                Vector3f(0, 32, 0),
                Vector3f(1),
                Quaternionf::FromAngles(DegreesToRadians(90), Vector3f(0, 0, 1))));
        Master.Attach(Pivot::CenterMiddle);
        Master.Attach(Scene::TEcsText(Font, 32, L"[Es Puto]"));
        Master.Attach(Color(1, 0, 0, 1));

        Child = Scene->Spawn();
        Child.SetParent(Master);
        Child.SetName("Child");
        Child.Attach(Transformf(
                Vector3f(12, 24, 0),
                Vector3f(1),
                Quaternionf::FromAngles(DegreesToRadians(90), Vector3f(0, 0, 1))));
        Child.Attach(Scene::TEcsText(Font, 12, L"[CHILD_OF_MASTER]"));
        Child.Attach(Color(0, 0, 1, 1));

        Random = Scene->Spawn();
        Random.SetParent(GrandMaster);
        Random.Attach(Transformf(
                Vector3f(-100, -100, 0.5f),
                Vector3f(4),
                Quaternionf()));
        Random.Attach(Scene::TEcsText(Font, 16, L"[Hello SIR!!!!!!]"));
        Random.Attach(Color(0, 1, 1, 1));

        MyCacheLocalscape2 = Random.Link<Transformf>();

        ConstSPtr<Graphic::Material> Material = Content->Load<Graphic::Material>("Resources://Material/Test.material");

        MySprite = Scene->Spawn();
        MySprite.Attach(Transformf(Vector3f(633, 333, 0.7f), Vector3f(0.5), Quaternionf()));
        MySprite.Attach(Scene::TEcsSprite(Material, Rectf(0, 0, 500, 400)));
        MySprite.Attach(Pivot(Pivot::CenterMiddle));
        MySprite.Attach(Color(0xFF0000FF));

        // Initialize Camera.
        mCamera.SetOrthographic(GetDevice().GetWidth(), GetDevice().GetHeight(), 0, 1);
        mCamera.Compute();

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
            mTexts->Draw(mCamera);
        }
        Graphics->Commit(Graphic::k_Default, false);
        Graphics->Flush();

        static Real32 Angles = 0;
        Angles += 0.01f;
        if (Angles >= 360.0f) { Angles -= 360.0f; }

        MyCacheLocalscape
                .Modify<& Transformf::SetRotation>(Quaternionf::FromAngles(DegreesToRadians(Angles), Vector3f(0, 0, 1)));
        MyCacheLocalscape2
            .Modify<& Transformf::SetRotation>(Quaternionf::FromAngles(DegreesToRadians(Angles), Vector3f(0, 0, 1)));


        // Register default system(s).
        using Query = Scene::Query<>;
        GetSubsystem<Scene::Service>()->Match<Query>("OnTransformUpdateQuery")
            .with<const Transformf>()
            .with<const Matrix4f>().optional().parent().cascade()
            .with<Matrix4f>().out()
            .build()
            .run([](Ref<flecs::iter> Iterator)
                 {
                     while (Iterator.next())
                     {
                         if (Iterator.changed())
                         {
                             const auto LocalTransformTable  = Iterator.field<const Transformf>(0);
                             const auto ParentTransform      = Iterator.field<const Matrix4f>(1);
                             const auto WorldTransformTable  = Iterator.field<Matrix4f>(2);

                             if (Iterator.is_set(1))
                             {
                                 for (const UInt64 Index : Iterator)
                                 {
                                     WorldTransformTable[Index] = ParentTransform[0] * LocalTransformTable[Index].Compute();
                                 }
                             }
                             else
                             {
                                 for (const UInt64 Index : Iterator)
                                 {
                                     WorldTransformTable[Index] = LocalTransformTable[Index].Compute();
                                 }
                             }
                         }
                         else
                         {
                             Iterator.skip();
                         }
                     }
                 });

    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDestroy()
    {
        mTexts->DeleteQueries();
    }

    Bool Application::OnKeyDown(Input::Key Key)
    {
        return true;
    }

    Bool Application::OnMouseMove(Real32 X, Real32 Y, Real32 DeltaX, Real32 DeltaY)
    {
        MyCacheLocalscape->SetPosition(Vector2f { X, Y});
        MyCacheLocalscape.Notify();
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

int main([[maybe_unused]] int Argc, [[maybe_unused]] Ptr<Char> Argv[])
{
    Engine::Properties Properties;
    Properties.SetWindowTitle("Aurora Example");
    Properties.SetWindowWidth(1920);
    Properties.SetWindowHeight(1024);
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