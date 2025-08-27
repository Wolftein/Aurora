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
#include "Component/TextComponent.hpp"
#include "Aurora.Math/Matrix4x4.hpp"
#include "Aurora.Graphic/Experimental/Render2D.hpp"
#include <Aurora.Content/Mount/DiskMount.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Example
{
    Graphic::Camera           AppCamera;
    Unique<Graphic::Render2D> AppRenderer;
    Tracker<Graphic::Font>    Font;

    Scene::Query<const TextComponent, const Matrix4x4, ConstPtr<Color>, ConstPtr<Pivot>> TextQuery;
    Scene::Query<> TransformQuery;
    Scene::Entity GrandMasterEntity;

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        auto Content = GetService<Content::Service>();
        Content->AddMount("Resources", Tracker<Content::DiskMount>::Create("Resources"));

        AppRenderer = NewUniquePtr<Graphic::Render2D>(* this);

        AppCamera.SetOrthographic(GetDevice().GetWidth(), GetDevice().GetHeight(), 0, 1);
        AppCamera.Compute();

        // Initialize the scene.
        ConstTracker<Scene::Service> Scene = GetService<Scene::Service>();
        Scene->RegisterComponent<Color>();
        Scene->RegisterComponent<Pivot>();
        Scene->RegisterComponent<Matrix4x4>();
        Scene->RegisterComponent<Transform>().With<Matrix4x4>();

        auto FontPipeline = Content->Load<Graphic::Pipeline>("Engine://Pipeline/MSDF.effect");
        Font  = Content->Load<Graphic::Font>("Engine://Font/Roboto.arfont");
        while (!Font->HasFinished() || !FontPipeline->HasFinished()) {
            Content->OnTick({});
        }

        GrandMasterEntity = Scene->CreateEntity<false>();
        GrandMasterEntity.Attach(Transform(Vector3(256, 256, 0)));
        GrandMasterEntity.Attach(TextComponent(Font, 32.0f, "[Lord Fernando]"));
        GrandMasterEntity.Attach(Color(1, 0, 0, 1));
        GrandMasterEntity.Attach(Pivot::CenterMiddle);

        auto MasterEntity = Scene->CreateEntity<false>();
        MasterEntity.Attach(Transform(Vector3(128, 0, 0)));
        MasterEntity.Attach(TextComponent(Font, 32.0f, "[ES PETE]"));
        MasterEntity.Attach(Color(0, 1, 0, 1));
        MasterEntity.Attach(Pivot::LeftMiddle);
        MasterEntity.SetParent(GrandMasterEntity);


        TextQuery = Scene->CreateQuery<const TextComponent, const Matrix4x4, ConstPtr<Color>, ConstPtr<Pivot>>().build();

        TransformQuery = Scene->CreateQuery<>("OnTransformUpdateQuery")
            .with<const Transform>()
            .with<const Matrix4x4>().optional().parent().cascade()
            .with<Matrix4x4>().out().build();
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

     Graphic::Render2D::FontStyleSDF GetFontOutline()
    {
        static Graphic::Render2D::FontStyleSDF EffectFont {
            .uInvThreshold =  1.0f - 0.5f,
            .uOutlineBias = 1.0f/4.0f,
            .uOutlineWidthAbsolute = 1.0f/3.0f,
            .uOutlineWidthRelative = 1.0f/20.0f,
            .uOutlineBlur = 0
        };
        return EffectFont;
    }

     Graphic::Render2D::FontStyleSDF GetFontThick()
    {
        static  Graphic::Render2D::FontStyleSDF EffectFont {
            .uInvThreshold =  1.0f - 0.5f,
            .uOutlineBias = 0.0f,
            .uOutlineWidthAbsolute = 1.0f/16.0f,
            .uOutlineWidthRelative = 1.0f/50.0f,
            .uOutlineBlur = 0
        };
        return EffectFont;
    }

     Graphic::Render2D::FontStyleSDF GetFontThicker()
    {
        static  Graphic::Render2D::FontStyleSDF EffectFont {
            .uInvThreshold =  1.0f - 0.6f,
            .uOutlineBias = 0.0f,
            .uOutlineWidthAbsolute = 1.0f/16.0f,
            .uOutlineWidthRelative = 1.0f/7.0f,
            .uOutlineBlur = 0
        };
        return EffectFont;
    }

     Graphic::Render2D::FontStyleSDF GetFontShadow()
    {
        static  Graphic::Render2D::FontStyleSDF EffectFont {
            .uInvThreshold =  1.0f - 0.5f,
            .uOutlineBias = 0.0f,
            .uOutlineWidthAbsolute = 1.0f/3.0f,
            .uOutlineWidthRelative = 1.0f/5.0f,
            .uOutlineBlur = 1
        };
        return EffectFont;
    }

     Graphic::Render2D::FontStyleSDF GetFontShadow(Color Shadow)
    {
        Graphic::Render2D::FontStyleSDF EffectFont;
        EffectFont.uInvThreshold = 1.0f-0.5f;
        EffectFont.uOutlineBias = 0;
        EffectFont.uOutlineWidthAbsolute = 1.0f/3.0f;
        EffectFont.uOutlineWidthRelative = 1.0f/5.0f;
        EffectFont.uOutlineBlur = 1;
        EffectFont.uOuterColor = Shadow;
        return EffectFont;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(ConstRef<Time> Time)
    {
        AURORA_PROFILE;
  
        static Real32 Angles = 0.0f;
        Angles += (128.0f * Time.GetDelta());

        Ptr<Transform> Transformation = GrandMasterEntity.Lookup<Transform>();
        Transformation->SetRotation(Quaternion::FromAngles(DegreesToRadians(Angles), Vector3(0, 0, 1)));
        GrandMasterEntity.Notify<Transform>();
        {
            AURORA_PROFILE_SCOPE("TransformQuery");

            TransformQuery.Run([](Ref<flecs::iter> Iterator)
                               {
                                   AURORA_PROFILE;

                                   while (Iterator.next())
                                   {
                                       if (Iterator.changed())
                                       {
                                           const auto LocalTransformTable = Iterator.field<const Transform>(0);
                                           const auto ParentTransform = Iterator.field<const Matrix4x4>(1);
                                           const auto WorldTransformTable = Iterator.field<Matrix4x4>(2);

                                           if (Iterator.is_set(1))
                                           {
                                               for (const UInt64 Index: Iterator)
                                               {
                                                   WorldTransformTable[Index] =
                                                       ParentTransform[0] * LocalTransformTable[Index].Compute();
                                               }
                                           } else
                                           {
                                               for (const UInt64 Index: Iterator)
                                               {
                                                   WorldTransformTable[Index] = LocalTransformTable[Index].Compute();
                                               }
                                           }
                                       } else
                                       {
                                           Iterator.skip();
                                       }
                                   }
                               });
        }

        ConstTracker<Graphic::Service> Graphics = GetService<Graphic::Service>();

        Graphic::Viewport Viewport(0, 0, GetDevice().GetWidth(), GetDevice().GetHeight());
        Graphics->Prepare(Graphic::kDisplay, Viewport, Graphic::Clear::All, Color(0.65f, 0.50f, 0.55f), 1, 0);
        {
            AppRenderer->SetGlobalParameters(ConstSpan<Matrix4x4>(&AppCamera.GetViewProjection(), 1));

            {
                AppRenderer->SetFontStyle(GetFontOutline());

                auto Text = Format("Time: {}", Time.GetAbsolute());
                auto Where = Font->Layout(Text, 24.0f, Pivot::LeftTop);
                AppRenderer->DrawFont(Where,0.0f, Text, 24.0f, Color::PackRGBA32F(0, 0, 1), Font);

                {
                    AURORA_PROFILE_SCOPE("TextQuery");
                    TextQuery.Each([&](ConstRef<TextComponent> Text, ConstRef<Matrix4x4> Matrix, ConstPtr<Color> Tint, ConstPtr<Pivot> Pivot)
                                   {
                                       auto Color  = (Tint ? Tint->PackRGBA32F(Tint->GetRed(), Tint->GetGreen(), Tint->GetBlue(), Tint->GetAlpha()) : 0xFFFFFFFF);
                                       auto Anchor = (Pivot ? *Pivot : Math::Pivot::LeftTop);
                                       auto Origin = Text.GetFont()->Layout(Text.GetWord(), Text.GetSize(), Anchor);
                                       AppRenderer->DrawFont(Matrix, Origin, 0.0f, Text.GetWord(), Text.GetSize(), Color, Text.GetFont());
                                   });
                }
            }

            AppRenderer->Flush();
        }
        Graphics->Commit(Graphic::kDisplay);
        Graphics->Finish(false);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTeardown()
    {
        TextQuery.Destruct();
        TransformQuery.Destruct();
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   MAIN   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int main([[maybe_unused]] int Argc, [[maybe_unused]] Ptr<Char> Argv[])
{
    AURORA_PROFILE;

    Engine::Properties Properties;
    Properties.SetWindowTitle("Aurora Example");
    Properties.SetWindowWidth(1920);
    Properties.SetWindowHeight(1024);
    Properties.SetWindowSamples(1);
    Properties.SetWindowFullscreen(false);
    Properties.SetWindowBorderless(false);
    Properties.SetVideoDriver("D3D11");

    // Initialize 'Aurora Engine' and enter main loop
    Tracker<Example::Application> Application = Tracker<Example::Application>::Create();
    Application->Initialize(Service::Host::Mode::Client, Properties);
    Application->Run();

    return 0;
}