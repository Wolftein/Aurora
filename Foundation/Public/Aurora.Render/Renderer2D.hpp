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

#include "Heap.hpp"
#include "Aurora.Graphic/Camera.hpp"
#include "Aurora.Graphic/Font.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Renderer2D final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxDrawables = 10240;

        // -=(Undocumented)=-
        enum class Block
        {
            // -=(Undocumented)=-
            Scene,

            // -=(Undocumented)=-
            Technique,

            // -=(Undocumented)=-
            Material,

            // -=(Undocumented)=-
            Instance,
        };

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
        Renderer2D(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        ~Renderer2D();

        // -=(Undocumented)=-
        template<typename Format>
        void SetScene(CPtr<const Format> Scene)
        {
            mBlocks[CastEnum(Block::Scene)] = mAllocator.Allocate(Usage::Uniform, Scene);
        }

        // -=(Undocumented)=-
        template<typename Format>
        void SetTechnique(CPtr<const Format> Technique)
        {
            mBlocks[CastEnum(Block::Technique)] = mAllocator.Allocate(Usage::Uniform, Technique);
        }

        // -=(Undocumented)=-
        void Draw(ConstRef<Vector3f> Position, ConstRef<Vector2f> Size, ConstRef<Rectf> Source, Color Tint, ConstRef<Pivot> Pivot, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void Draw(ConstRef<Matrix4f> Transform, ConstRef<Rectf> Origin, ConstRef<Rectf> Source, Color Tint, ConstRef<Pivot> Pivot, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void Draw(ConstRef<Matrix4f> Transform, CStr16 Text, UInt16 Size, Color Tint, ConstRef<Pivot> Pivot, Order Order, ConstSPtr<Font> Font);

        // -=(Undocumented)=-
        void Flush(Bool Copy = false);

    private:

        // -=(Undocumented)=-
        struct Buffer
        {
            Object        ID;
            UPtr<UInt8[]> Memory;
            UInt          Length;
            UInt          Reader;
            UInt          Writer;
        };

        // -=(Undocumented)=-
        struct Drawable
        {
            // -=(Undocumented)=-
            UInt64                  ID;

            // -=(Undocumented)=-
            Array<Vector3f, 4>      Coordinates;

            // -=(Undocumented)=-
            Rectf                   Source;

            // -=(Undocumented)=-
            Color                   Tint;

            // -=(Undocumented)=-
            Ptr<Graphic::Pipeline>  Pipeline;

            // -=(Undocumented)=-
            Ptr<Graphic::Material>  Material;
        };

        // -=(Undocumented)=-
        struct Layout
        {
            // -=(Undocumented)=-
            Vector3f Position;

            // -=(Undocumented)=-
            Vector2f Texture;

            // -=(Undocumented)=-
            UInt32   Color;
        };

        // -=(Undocumented)=-
        void CreateDefaultResources(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        void PushBatch(UInt32 Offset, UInt32 Count, ConstRef<Drawable> Drawable);

        // -=(Undocumented)=-
        void PushDrawable(ConstRef<Matrix4f> Transformation, ConstRef<Rectf> Destination, ConstRef<Rectf> Source,
            Color Tint, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void PushDrawable(ConstRef<Rectf> Destination, Real32 Depth, ConstRef<Rectf> Source,
            Color Tint, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void PushGeometry(ConstPtr<Drawable> Drawable, Ptr<Layout> Buffer);

        // -=(Undocumented)=-
        UInt64 GenerateUniqueId(Order Order, Object Pipeline, Object Material, Real32 Depth) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Service>                      mService;
        Heap                               mAllocator;
        Array<Binding, CountEnum<Block>()> mBlocks;
        Encoder                            mEncoder;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<SPtr<Pipeline>, 1>           mPipelines;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Drawable, k_MaxDrawables>    mDrawables;
        Vector<Ptr<Drawable>>              mDrawablesPtr;
    };
}