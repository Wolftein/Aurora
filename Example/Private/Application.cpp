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
#include "Aurora.Math/Matrix4x4.hpp"
#include "Aurora.Math/Tween.hpp"
#include "Aurora.Graphic/Experimental/Render2D.hpp"
#include <Aurora.Content/Mount/DiskMount.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Example
{
    Unique<Graphic::Render2D> Renderer;
    Tracker<Graphic::Service>  Graphics;
    Graphic::Camera Camera;
    Tracker<Graphic::Material> Material;
    Tracker<Graphic::Pipeline> Pipeline;

    Tracker<Graphic::Font> Font;
    Graphic::Object            VBO;
    Graphic::Object            IBO;

    struct Layout
    {
        Vector2 Position;
        Vector2 Texture;
    };

    struct Instance
    {
        Vector2 Position;
        Vector2 Size;
        UInt32  Color;
        Real32  Depth;
    };


    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        auto Content = GetService<Content::Service>();
        Content->AddMount("Resources", Tracker<Content::DiskMount>::Create("Resources"));

        Material = Content->Load<Graphic::Material>("Resources://Entities.material");
        Pipeline  = Content->Load<Graphic::Pipeline>("Resources://Sprite.effect");
        Font  = Content->Load<Graphic::Font>("Resources://test.arfont");

        Renderer = NewUniquePtr<Graphic::Render2D>(* this);
        Graphics = GetService<Graphic::Service>();

        static constexpr std::initializer_list<Layout> VBO_DATA = {
            { {0, 1}, {0, 1}},
            { {1, 1}, {1, 1}},
            { {1, 0}, {1, 0}},
            { {0, 0}, {0, 0}}
        };
        VBO = Graphics->CreateBuffer(Graphic::Access::Device,
                                     Graphic::Usage::Vertex,
                                     Blob::FromImmutable(VBO_DATA));


        static constexpr std::initializer_list<UInt16> IBO_DATA = {
            0, 1, 2, 0, 2, 3
        };
        IBO = Graphics->CreateBuffer(Graphic::Access::Device,
                                     Graphic::Usage::Index,
                                     Blob::FromImmutable(IBO_DATA));


        Camera.SetOrthographic(1920, 1024, 0, 1);
        Camera.Compute();

        return true;
    }

    static constexpr UInt32 kMaxInstances = 10'000;

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Graphic::Render2D::FontStyleSDF GetFontOutline()
    {
        Graphic::Render2D::FontStyleSDF EffectFont;
        EffectFont.uInvThreshold =  1.0f - 0.5f;
        EffectFont.uOutlineBias = 1.0f/4.0f;
        EffectFont.uOutlineWidthAbsolute = 1.0f/3.0f;
        EffectFont.uOutlineWidthRelative = 1.0f/20.0f;
        EffectFont.uOutlineBlur = 0;
        return EffectFont;
    }

    Graphic::Render2D::FontStyleSDF GetFontThick()
    {
        Graphic::Render2D::FontStyleSDF EffectFont;
        EffectFont.uInvThreshold = 1.0f - 0.5f;
        EffectFont.uOutlineBias = 0;
        EffectFont.uOutlineWidthAbsolute = 1.0f/16.0f;
        EffectFont.uOutlineWidthRelative = 1.0f/50.0f;
        EffectFont.uOutlineBlur = 0;
        return EffectFont;
    }

    Graphic::Render2D::FontStyleSDF GetFontThicker()
    {
        Graphic::Render2D::FontStyleSDF EffectFont;
        EffectFont.uInvThreshold = 1.0f-0.6f;
        EffectFont.uOutlineBias = 0;
        EffectFont.uOutlineWidthAbsolute = 1.0f/16.0f;
        EffectFont.uOutlineWidthRelative = 1.0f/7.0f;
        EffectFont.uOutlineBlur = 0;
        return EffectFont;
    }

    Graphic::Render2D::FontStyleSDF GetFontShadow()
    {
        Graphic::Render2D::FontStyleSDF EffectFont;
        EffectFont.uInvThreshold = 1.0f-0.5f;
        EffectFont.uOutlineBias = 0;
        EffectFont.uOutlineWidthAbsolute = 1.0f/3.0f;
        EffectFont.uOutlineWidthRelative = 1.0f/5.0f;
        EffectFont.uOutlineBlur = 1;
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

    void Application::OnTick(ConstRef<Time> Time)
    {
        Graphics->Prepare(Graphic::kDisplay, {0, 0, 1920, 1024}, Graphic::Clear::All, Color(0.65, 0.50, 0.65, 1), 1, 0);

        Renderer->SetGlobalParameters(ConstSpan<Matrix4x4>(& Camera.GetViewProjection(), 1));

        /*
        for (auto i = 0; i < kMaxInstances; ++i)
        Renderer->DrawSprite({100, 100, 300, 300}, 0, {0, 0, 1, 1}, Color::PackRGBA32F(1, 0, 0), Material);
        Renderer->DrawLine({0, 0}, {100, 100}, 0.0f, 0xFF0000FF, 5.0f);*/
        /*
        const Text Word = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";

        float maxHeight = 1024.0f;
        float currentY = 0.0f;

        for (float fontSize = 32.0f; fontSize >= 6.0f; fontSize--)
        {
            float lineHeight = fontSize * 1.5f;

            if (currentY + lineHeight > maxHeight) {
                break;
            }

            Renderer->DrawFont({0.0f, currentY, 0.0f, 0.0f}, 0.0f, Format("{}px", fontSize), 20.0f, Color::PackRGBA32F(1, 1, 1), 0, Font);

            Renderer->DrawFont(
                {64.0f, currentY, 0.0f, 0.0f},
                0.0f,
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890.,:;!@#$%^&*",
                fontSize,
                Color::PackRGBA32F(1, 1, 1),
                0,
                Font
            );

            currentY += lineHeight;
        }*/

        Renderer->SetFontStyle(GetFontOutline());
        Renderer->DrawFont({ 100, 50, 0, 0}, 0.0f, "We met again, GoDKeR ~!", 8.0f, Color::PackRGBA32F(0, 0, 1), Font);
        Renderer->DrawFont({ 100, 100, 0, 0}, 0.0f, "We met again, GoDKeR ~@", 12.0f, Color::PackRGBA32F(0, 0, 1), Font);
        Renderer->DrawFont({ 100, 200, 0, 0}, 0.0f, "We met again, GoDKeR ~#", 24.0f, Color::PackRGBA32F(0, 0, 1), Font);
        Renderer->DrawFont({ 100, 300, 0, 0}, 0.0f, "We met again, GoDKeR :D", 36.0f, Color::PackRGBA32F(0, 1, 0), Font);
        Renderer->DrawFont({ 100, 400, 0, 0}, 0.0f, "We met again, GoDKeR :P", 48.0f, Color::PackRGBA32F(0, 1, 1), Font);
        Renderer->DrawFont({ 100, 500, 0, 0}, 0.0f, Format("Delta: {}", Time.GetDelta()), 48.0f, Color::PackRGBA32F(1, 1, 1), Font);


        Renderer->SetFontStyle(GetFontOutline());
        Renderer->DrawFont({ 900, 50, 0, 0}, 0.0f, "GetFontOutline", 32.0f, Color::PackRGBA32F(0, 1, 1), Font);

        Renderer->SetFontStyle(GetFontThick());
        Renderer->DrawFont({ 900, 150, 0, 0}, 0.0f, "GetFontThick", 32.0f, Color::PackRGBA32F(0, 1, 1), Font);

        Renderer->SetFontStyle(GetFontThicker());
        Renderer->DrawFont({ 900, 250, 0, 0}, 0.0f, "GetFontThicker", 32.0f, Color::PackRGBA32F(0, 1, 1), Font);

        Renderer->SetFontStyle(GetFontShadow());
        Renderer->DrawFont({ 900, 350, 0, 0}, 0.0f, "GetFontShadow", 32.0f, Color::PackRGBA32F(0, 1, 1), Font);

        Renderer->SetFontStyle(GetFontShadow(Color(1, 0, 0, 1)));
        Renderer->DrawFont({ 900, 450, 0, 0}, 0.0f, "GetFontShadowWithColor", 32.0f, Color::PackRGBA32F(0, 1, 1), Font);




        Renderer->Flush();

        Graphics->Commit(Graphic::kDisplay);
        Graphics->Finish(false);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTeardown()
    {
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