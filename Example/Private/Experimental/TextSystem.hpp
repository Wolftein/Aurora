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

#include <Aurora.Render/Renderer2D.hpp>
#include <Aurora.Content/Service.hpp>
#include <Aurora.Scene/Service.hpp>
#include <Aurora.Graphic/Service.hpp>
#include "Sprite.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class TEcsTextSystem final
    {

        using Query = Query<
                const TEcsText,
                const TEcsMatrix ,
                Ptr<const TEcsTint>,
                Ptr<const TEcsPivot>>;

        using Query2 = Scene::Query<
                const TEcsSprite,
                const TEcsMatrix,
                Ptr<const TEcsTint>,
                Ptr<const TEcsPivot>>;

    public:

        // -=(Undocumented)=-
        TEcsTextSystem(Ref<Core::Subsystem::Context> Context)
        {
            mRenderer = NewUniquePtr<Graphic::Renderer2D>(Context);

            ConstSPtr<Content::Service> Resources = Context.GetSubsystem<Content::Service>();
            mPipeline2 = Resources->Load<Graphic::Pipeline>("Resources://Pipeline/Sprite.effect");

            ConstSPtr<Scene::Service> Scene = Context.GetSubsystem<Scene::Service>();
            mQuery = Scene->Query<const TEcsText, const TEcsMatrix, Ptr<const TEcsTint>, Ptr<const TEcsPivot>>()
                    .build();

            mQuery2 = Scene->Query<const TEcsSprite, const TEcsMatrix, Ptr<const TEcsTint>, Ptr<const TEcsPivot>>()
                    .build();
        }

        // -=(Undocumented)=-
        void Draw(Ref<Graphic::Encoder> Encoder, Ref<Graphic::Camera> Camera)
        {
            mRenderer->SetScene(CastSpan(Camera.GetWorld()));

            mQuery2.Each([&](ConstRef<TEcsSprite> Sprite, ConstRef<TEcsMatrix> Matrix, Ptr<const TEcsTint> Tint, Ptr<const TEcsPivot> Pivot)
                        {
                            ConstSPtr<Graphic::Texture> Texture = Sprite.GetMaterial()->GetTexture(Graphic::TextureSlot::Diffuse);
                            const Rectf Source(
                                    Vector2f(Sprite.GetRectangle().GetLeft(), Sprite.GetRectangle().GetTop()) / Vector2f(Texture->GetWidth(), Texture->GetHeight()),
                                    Vector2f(Sprite.GetRectangle().GetRight(), Sprite.GetRectangle().GetBottom()) / Vector2f(Texture->GetWidth(), Texture->GetHeight()));
                            const Rectf Destination(0, 0, Sprite.GetRectangle().GetWidth(), Sprite.GetRectangle().GetHeight());

                            Math::Pivot OptPivot = (Pivot ? * Pivot : Math::Pivot());
                            Math::Color OptTint  = (Tint  ? * Tint  : Math::Color(1, 1, 1, 1));
                            mRenderer->Draw(Matrix, Destination, Source, OptTint, OptPivot, Graphic::Renderer2D::Order::Opaque, mPipeline2, Sprite.GetMaterial() );
                        });


            mQuery.Each([&](ConstRef<TEcsText> Text, ConstRef<TEcsMatrix> Matrix, Ptr<const TEcsTint> Tint, Ptr<const TEcsPivot> Pivot)
            {
                Math::Pivot OptPivot = (Pivot ? * Pivot : Math::Pivot());
                Math::Color OptTint  = (Tint  ? * Tint  : Math::Color());
                mRenderer->Draw(Matrix, Text.GetWord(), Text.GetSize(), OptTint, OptPivot, Graphic::Renderer2D::Order::Opaque, Text.GetFont());
            });

            mRenderer->Flush();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-= -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Graphic::Renderer2D>       mRenderer;
        Query mQuery;
        Query2 mQuery2;
        SPtr<Graphic::Pipeline> mPipeline;
        SPtr<Graphic::Pipeline> mPipeline2;
    };
}