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

#include "Font.hpp"
#include "Encoder.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    // TODO: Need Rework
    class Renderer final
    {
    public:

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxDrawables = 16384;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxVertices  = 24 * 1024 * 1024; // 24mb

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxIndices   =  4 * 1024 * 1024; // 4mb

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxUniform   =  8 * 1024 * 1024; // 8mb

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

    public:

        // -=(Undocumented)=-
        Renderer(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        void SetScene(Ref<const Matrix4f> Projection, Real32 Time);

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt Offset, Type Parameter)
        {
            constexpr UInt32 Size = sizeof(Type) / sizeof(Vector4f) + 1;

            if (Offset + Size > mParameters.size())
            {
                mParameters.resize(mParameters.size() + Align(Size, 16));
            }

            * (reinterpret_cast<Ptr<Type>>(reinterpret_cast<UInt>(mParameters.data()) + Offset)) = Parameter;
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
        void DrawRectangle(Ref<const Rectf> Rectangle, Vector2f Origin, Real32 Depth, Real32 Angle, Ref<const Array<Color, 4>> Tint);

        // -=(Undocumented)=-
        void DrawRectangle(Ref<const Rectf> Rectangle, Vector2f Origin, Real32 Depth, Real32 Angle, Ref<const Color> Tint)
        {
            DrawRectangle(Rectangle, Origin, Depth, Angle, Array<Color, 4> { Tint, Tint, Tint, Tint });
        }

        // -=(Undocumented)=-
        void DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Vector2f Origin, Real32 Depth, Real32 Angle, Order Order, Ref<const Array<Color, 4>> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Vector2f Origin, Real32 Depth, Real32 Angle, Order Order, Ref<const Color> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
        {
            DrawTexture(Rectangle, Source, Origin, Depth, Angle, Order, Array<Color, 4> { Tint, Tint, Tint, Tint }, Pipeline, Material);
        }

        // -=(Undocumented)=-
        void DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Real32 Depth, Real32 Angle, Order Order, Ref<const Color> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
        {
            DrawTexture(Rectangle, Source, Rectangle.GetCenter(), Depth, Angle, Order, Array<Color, 4> { Tint, Tint, Tint, Tint }, Pipeline, Material);
        }

        // -=(Undocumented)=-
        void DrawFont(ConstSPtr<Font> Font, CStr16 Text, Real32 Size, Vector2f Position, Real32 Depth, Real32 Angle, Color Tint, Font::Alignment Alignment);

        // -=(Undocumented)=-
        void Flush();

    private:

        // -=(Undocumented)=-
        struct Layout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            UInt32   Color;

            // -=(Undocumented)=-
            Vector2f Texture;
        };

        // -=(Undocumented)=-
        struct NonTextureLayout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            UInt32   Color;
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

        // -=(Undocumented)=-
        void EncodeWithMaterial(UInt Offset, UInt Count, Ref<const Drawable> Drawable);

        // -=(Undocumented)=-
        void EncodeWithoutMaterial(UInt Offset, UInt Count, Ref<const Drawable> Drawable);

    private:

        // -=(Undocumented)=-
        void CreateDefaultResources(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        void Push(
            Ref<const Array<Vector2f, 4>> Position,
            Ref<const Array<Color, 4>> Tint,
            Ref<const Array<Vector2f, 4>> Texture,
            Order Order, Real32 Depth, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<Layout> Buffer);

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<NonTextureLayout> Buffer);

        // -=(Undocumented)=-
        UInt64 GenerateUniqueID(Order Order, UInt Pipeline, UInt Material, Real32 Depth) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Encoder                        mEncoder;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<SPtr<Pipeline>, 2>        mPipelines;
        Vector<Vector4f>                mParameters;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Drawable, k_MaxDrawables> mDrawables;
        Vector<Ptr<Drawable>>           mDrawablesPtr;
    };

}