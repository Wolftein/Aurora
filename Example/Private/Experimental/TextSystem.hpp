// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Aurora.Math/Pivot.hpp>
#include <Aurora.Math/Color.hpp>
#include <Aurora.Render/Renderer.hpp>
#include <Aurora.Content/Service.hpp>
#include <Aurora.Scene/Service.hpp>
#include <Aurora.Graphic/Service.hpp>
#include "Sprite.hpp"
#include "Text.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class TEcsTextSystem final
    {

        using Query = Scene::Query<
                const TEcsText,
                const Matrix4f,
                ConstPtr<Color>,
                ConstPtr<Pivot>>;

        using Query2 = Scene::Query<
                const TEcsSprite,
                const Matrix4f,
                ConstPtr<Color>,
                ConstPtr<Pivot>>;

    public:

        // -=(Undocumented)=-
        TEcsTextSystem(Ref<Core::Subsystem::Context> Context)
        {
            mRenderer = NewUniquePtr<Graphic::Renderer>(Context);

            ConstSPtr<Content::Service> Resources = Context.GetSubsystem<Content::Service>();
            mPipeline2 = Resources->Load<Graphic::Pipeline>("Resources://Pipeline/Sprite.effect");

            ConstSPtr<Scene::Service> Scene = Context.GetSubsystem<Scene::Service>();
            Scene->Register<TEcsText>();
            Scene->Register<TEcsSprite>();

            mQuery = Scene->Match<Query>("QueryText").build();
            mQuery2 = Scene->Match<Query2>("QuerySprite").build();
        }

        // -=(Undocumented)=-
        void Draw(Ref<Graphic::Camera> Camera)
        {
            mRenderer->SetGlobalParameters(CastSpan(Camera.GetViewProjection()));
            mRenderer->SetPipeline(Graphic::Renderer::Type::Sprite, mPipeline2);

            mRenderer->DrawLine({0, 0}, {100, 100}, 0, Color::k_Violet, 1.0f);
            mRenderer->DrawLine({100, 100}, {200, 200}, 0, Color::k_Chocolate, 5.0f);
            mRenderer->DrawLine({100, 200}, {200, 200}, 0, Color::k_Red, 2.0f);
            mRenderer->DrawLine({500, 500}, {400, 400}, 0, Color::k_Red, 2.0f);
            mRenderer->DrawRect({256, 256, 318, 318}, 0, Color::k_Pink, 1);
            mRenderer->DrawRect({400, 256, 500, 312}, 0, Color::k_Ivory, 8);


            mQuery2.Each([&](ConstRef<TEcsSprite> Sprite, ConstRef<Matrix4f> Matrix, ConstPtr<Color> Tint, ConstPtr<Pivot> Pivot)
                        {
                            ConstSPtr<Graphic::Texture> Texture = Sprite.GetMaterial()->GetTexture(Graphic::TextureSlot::Diffuse);
                            const Rectf Source(
                                    Vector2f(Sprite.GetRectangle().GetLeft(), Sprite.GetRectangle().GetTop()) / Vector2f(Texture->GetWidth(), Texture->GetHeight()),
                                    Vector2f(Sprite.GetRectangle().GetRight(), Sprite.GetRectangle().GetBottom()) / Vector2f(Texture->GetWidth(), Texture->GetHeight()));


                            Math::Pivot OptPivot = (Pivot ? * Pivot : Math::Pivot());
                            Math::Color OptTint  = (Tint  ? * Tint  : Math::Color(1, 1, 1, 1));

                            auto Origin = Rectf::Transform(Rectf(0, 0, Sprite.GetRectangle().GetWidth(), Sprite.GetRectangle().GetHeight()), OptPivot);

                            mRenderer->DrawSprite(Matrix, Origin, 0.0f, Source, OptTint, Sprite.GetMaterial());
                        });

            mQuery.Each([&](ConstRef<TEcsText> Text, ConstRef<Matrix4f> Matrix, ConstPtr<Color> Tint, ConstPtr<Pivot> Pivot)
            {

                Math::Pivot OptPivot = (Pivot ? * Pivot : Math::Pivot());
                Math::Color OptTint  = (Tint  ? * Tint  : Math::Color());
                auto Origin = Text.GetFont()->Calculate(Text.GetWord(), Text.GetSize(), OptPivot);

                mRenderer->DrawFont(Matrix, Origin, 0.0f, Text.GetWord(), Text.GetSize(), OptTint, Text.GetFont());
            });

            mRenderer->Flush();
        }

        void DeleteQueries()
        {
            mQuery.~Query();
            mQuery2.~Query();

        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-= -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Graphic::Renderer>       mRenderer;
        Query mQuery;
        Query2 mQuery2;
        SPtr<Graphic::Pipeline> mPipeline;
        SPtr<Graphic::Pipeline> mPipeline2;
    };
}