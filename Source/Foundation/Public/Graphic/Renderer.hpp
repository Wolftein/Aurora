// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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
        void Begin(Ref<const Camera> Camera);

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
        void Draw(
            Rectf Destination,
            Rectf Source,
            Real32 Depth,
            Real32 Angle,
            Color Tint,
            Order Order,
            ConstSPtr<Graphic::Pipeline> Pipeline,
            ConstSPtr<Graphic::Material> Material);

        // -=(Undocumented)=-
        void DrawFont(
            ConstSPtr<Font> Font,
            ConstSPtr<Graphic::Pipeline> Pipeline,
            CStr16 Text,
            Vector2f Position,
            Real32 Depth,
            Real32 Scale,
            Color Tint,
            Alignment Alignment);

        // -=(Undocumented)=-
        void End();

    private:

        // -=(Undocumented)=-
        struct VertexShaderLayout
        {
            Vector3f Position;
            UInt32   Color;
            Vector2f Texture;
        };

        // -=(Undocumented)=-
        struct VertexShaderGeometry
        {
            VertexShaderLayout V1;
            VertexShaderLayout V2;
            VertexShaderLayout V3;
            VertexShaderLayout V4;
        };

        // -=(Undocumented)=-
        struct Drawable
        {
            UInt64                  ID;
            Rectf                   Destination;
            Rectf                   Source;
            Real32                  Angle;
            Real32                  Depth;
            UInt32                  Color;
            SPtr<Graphic::Pipeline> Pipeline;
            SPtr<Graphic::Material> Material;
        };

    private:

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<VertexShaderGeometry> Buffer);

        // -=(Undocumented)=-
        void Flush();

        // -=(Undocumented)=-
        UInt64 GenerateUniqueID(Order Order, UInt Pipeline, UInt Material, Real32 Depth) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Encoder                              mEncoder;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Drawable,      k_MaxDrawables> mDrawables;
        Stack<Ptr<Drawable>, k_MaxDrawables> mDrawablesPtr;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Vector4f>                     mTechnique;
    };

}