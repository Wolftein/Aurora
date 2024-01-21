// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Graphic/Camera.hpp"
#include "Graphic/Encoder.hpp"
#include "Graphic/Font.hpp"

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
        inline constexpr static UInt k_MaxDrawables = 16384;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxVertices  = 64 * 1024 * 1024; // 64mb

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxIndices   =  8 * 1024 * 1024; // 8mb

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxUniform   = 16 * 1024 * 1024; // 16mb

        // -=(Undocumented)=-
        enum class Order
        {
            // -=(Undocumented)=-
            Subtractive,

            // -=(Undocumented)=-
            Additive,

            // -=(Undocumented)=-
            Normal,

            // -=(Undocumented)=-
            Opaque,
        };

        // -=(Undocumented)=-
        enum class Alignment
        {
            // -=(Undocumented)=-
            LeftTop        = (1 << 0) | (1 << 3),

            // -=(Undocumented)=-
            LeftMiddle     = (1 << 0) | (1 << 4),

            // -=(Undocumented)=-
            LeftBottom     = (1 << 0) | (1 << 5),

            // -=(Undocumented)=-
            LeftBaseline   = (1 << 0) | (1 << 6),

            // -=(Undocumented)=-
            CenterTop      = (1 << 1) | (1 << 3),

            // -=(Undocumented)=-
            CenterMiddle   = (1 << 1) | (1 << 4),

            // -=(Undocumented)=-
            CenterBottom   = (1 << 1) | (1 << 5),

            // -=(Undocumented)=-
            CenterBaseline = (1 << 1) | (1 << 6),

            // -=(Undocumented)=-
            RightTop       = (1 << 2) | (1 << 3),

            // -=(Undocumented)=-
            RightMiddle    = (1 << 2) | (1 << 4),

            // -=(Undocumented)=-
            RightBottom    = (1 << 2) | (1 << 5),

            // -=(Undocumented)=-
            RightBaseline  = (1 << 2) | (1 << 6),
        };

    public:

        // -=(Undocumented)=-
        Renderer(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        void Begin(Ref<const Matrix4f> Projection);

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt Offset, Type Parameter)
        {
            constexpr UInt Size = sizeof(Type) / sizeof(Vector4f) + 1;

            if (Offset + Size > mTechnique.size())
            {
                mTechnique.resize(mTechnique.size() + Align<16>(Size));
            }

            * (reinterpret_cast<Ptr<Type>>(reinterpret_cast<UInt>(mTechnique.data()) + Offset)) = Parameter;
        }

        // -=(Undocumented)=-
        void DrawLine(Ref<const Array<Vector2f, 2>> Points, Real32 Depth, Ref<const Array<Color, 2>> Tint, Real32 Thickness = 1.0f);

        // -=(Undocumented)=-
        void DrawLine(Ref<const Array<Vector2f, 2>> Points, Real32 Depth, Ref<const Color> Tint, Real32 Thickness = 1.0f)
        {
            DrawLine(Points, Depth, Array<Color, 2> { Tint, Tint }, Thickness);
        }

        // -=(Undocumented)=-b
        void DrawRectangle(Ref<const Array<Vector2f, 4>> Points, Real32 Depth, Ref<const Array<Color, 4>> Tint);

        // -=(Undocumented)=-
        void DrawRectangle(Ref<const Rectf> Rectangle, Real32 Depth, Real32 Angle, Ref<const Array<Color, 4>> Tint);

        // -=(Undocumented)=-
        void DrawRectangle(Ref<const Rectf> Rectangle, Real32 Depth, Real32 Angle, Ref<const Color> Tint)
        {
            DrawRectangle(Rectangle, Depth, Angle, Array<Color, 4> { Tint, Tint, Tint, Tint });
        }

        // -=(Undocumented)=-
        void DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Real32 Depth, Real32 Angle,
            Order Order, Ref<const Array<Color, 4>> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Real32 Depth, Real32 Angle,
            Order Order, Ref<const Color> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
        {
            DrawTexture(Rectangle, Source, Depth, Angle, Order, Array<Color, 4> { Tint, Tint, Tint, Tint }, Pipeline, Material);
        }

        // -=(Undocumented)=-
        // TODO: Don't sort multiple rectangles and allow rotated text
        void DrawFont(ConstSPtr<Font> Font, CStr16 Text, Vector2f Position, Real32 Depth, Real32 Scale, Color Tint, Alignment Alignment);

        // -=(Undocumented)=-
        void End();

    private:

        // -=(Undocumented)=-
        struct PosColorLayout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            UInt32   Color;
        };

        // -=(Undocumented)=-
        struct PosColorTextureLayout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            UInt32   Color;

            // -=(Undocumented)=-
            Vector2f Texture;
        };

        // -=(Undocumented)=-
        struct Drawable
        {
            // -=(Undocumented)=-
            UInt64                  ID;

            // -=(Undocumented)=-
            Real                    Depth;

            // -=(Undocumented)=-
            Array<Vector2f, 4>      Position;

            // -=(Undocumented)=-
            Array<Color, 4>         Color;

            // -=(Undocumented)=-
            Array<Vector2f, 4>      Texture;

            // -=(Undocumented)=-
            SPtr<Graphic::Pipeline> Pipeline;

            // -=(Undocumented)=-
            SPtr<Graphic::Material> Material;
        };

    private:

        // -=(Undocumented)=-
        void CreateDefaultResources(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        void Flush();

        // -=(Undocumented)=-
        void Push(
            Ref<const Array<Vector2f, 4>> Position,
            Ref<const Array<Color, 4>> Tint,
            Ref<const Array<Vector2f, 4>> Texture,
            Order Order, Real32 Depth, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<PosColorLayout> Buffer);

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<PosColorTextureLayout> Buffer);

        // -=(Undocumented)=-
        UInt64 GenerateUniqueID(Order Order, UInt Pipeline, UInt Material, Real32 Depth) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Encoder                              mEncoder;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Vector4f>                     mTechnique;
        Array<SPtr<Pipeline>, 2>             mPipelines;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Drawable, k_MaxDrawables>      mDrawables;
        Stack<Ptr<Drawable>, k_MaxDrawables> mDrawablesPtr;
    };

}