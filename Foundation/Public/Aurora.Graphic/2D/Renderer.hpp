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

#include "Aurora.Graphic/Camera.hpp"
#include "Aurora.Graphic/Font.hpp"
#include "Aurora.Graphic/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Renderer
    {
    public:

        // -=(Undocumented)=-
        Renderer();

        // -=(Undocumented)=-
        void Initialize(ConstSPtr<Graphic::Service> Graphics);

        // -=(Undocumented)=-
        void Begin(Ref<Encoder> Encoder, ConstRef<Camera> Camera);

        // -=(Undocumented)=-
        void SetPipeline(ConstSPtr<Pipeline> Pipeline);

        // -=(Undocumented)=-
        void SetMaterial(ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void Draw(ConstSPtr<Font> Font, CStr16 Text, UInt16 Size, ConstRef<Matrix4f> Transformation, ConstRef<Pivot> Pivot, Color Tint);

        // -=(Undocumented)=-
        void Draw(ConstRef<Rectf> Rectangle, ConstRef<Rectf> Source, ConstRef<Matrix4f> Transformation, ConstRef<Pivot> Pivot, Color Tint);

        // -=(Undocumented)=-
        void End();

    private:

        // -=(Undocumented)=-
        struct PosTexColorLayout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            Vector2f Texture;

            // -=(Undocumented)=-
            UInt32   Color;
        };

        // -=(Undocumented)=-
        void Push(ConstRef<Matrix4f> Transformation, ConstRef<Rectf> Rectangle, ConstRef<Rectf> Source, UInt32 Color);

        // -=(Undocumented)=-
        void Flush(Bool Restart);

    private:

        // -=(Undocumented)=-
        static constexpr UInt32 k_IndicesPerQuad  = 6;

        // -=(Undocumented)=-
        static constexpr UInt32 k_VerticesPerQuad = 4;

        // -=(Undocumented)=-
        static Rectf Calculate(ConstRef<Pivot> Pivot, ConstRef<Rectf> Rectangle)
        {
            static constexpr Rectf k_Multiplier[] = {
                Rectf( 0.0f,  0.0f, 1.0f, 1.0f),  // LeftTop
                Rectf( 0.0f, -0.5f, 1.0f, 0.5f),  // LeftMiddle
                Rectf( 0.0f, -1.0f, 1.0f, 0.0f),  // LeftBottom
                Rectf(-0.5f,  0.0f, 0.5f, 1.0f),  // CenterTop
                Rectf(-0.5f, -0.5f, 0.5f, 0.5f),  // CenterMiddle
                Rectf(-0.5f, -1.0f, 0.5f, 0.0f),  // CenterBottom
                Rectf(-1.0f,  0.0f, 0.0f, 1.0f),  // RightTop
                Rectf(-1.0f, -0.5f, 0.0f, 0.5f),  // RightMiddle
                Rectf(-1.0f, -1.0f, 0.0f, 0.0f),  // RightBottom
            };
            return k_Multiplier[CastEnum(Pivot.GetType())] *  Rectangle.GetSize() + Rectangle.GetPosition();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Graphic::Service> mGraphics;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32                 mInFlightBatch;
        Ptr<Encoder>           mInFlightEncoder;
        SPtr<Pipeline>         mInFlightPipeline;
        SPtr<Material>         mInFlightMaterial;
    };
}