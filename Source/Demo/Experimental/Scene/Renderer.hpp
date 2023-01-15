// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2020 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Graphic/Camera.hpp>
#include <Graphic/Material.hpp>
#include <Graphic/Pipeline.hpp>
#include <Graphic/Service.hpp>
#include <Graphic/Texture.hpp>

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
        explicit Renderer(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        ~Renderer();

        // -=(Undocumented)=-
        void Begin(Ref<const Camera> Camera, Real32 Time);

        // -=(Undocumented)=-
        void Draw(
            Ref<const Rectf> Destination,
            Ref<const Rectf> Source,
            Real32 Depth,
            Real32 Angle,
            UInt32 Color,
            Ref<const SPtr<Pipeline>> Pipeline,
            Ref<const SPtr<Material>> Material);

        // -=(Undocumented)=-
        void End();

    private:

        inline constexpr static UInt k_MaxDrawables = 4096;
        inline constexpr static UInt k_MaxUniforms  = 4096;

        struct Layout
        {
            Vector3f Position;
            UInt32   Color;
            Vector2f Texture;
        };

        struct Quad
        {
            Layout V1;
            Layout V2;
            Layout V3;
            Layout V4;
        };

        struct Scene
        {
            Matrix4f ProjectionViewMatrix;
            Real32   Time;
        };

        struct Drawable
        {
            UInt64         ID;
            Rectf          Destination;
            Rectf          Source;
            Real32         Angle;
            Real32         Depth;
            UInt32         Color;
            SPtr<Pipeline> Pipeline;
            SPtr<Material> Material;
        };

        void Flush();
        Ptr<Quad> MapVertexBuffer(UInt Count, Ref<UInt> Offset);
        void WriteVertexBuffer(Ptr<Drawable> Drawable, Ptr<Quad> Buffer);
        void UnmapVertexBuffer();
        Ptr<Vector4f> MapUniformBuffer(UInt Count, Ref<UInt> Offset);
        void WriteUniformBuffer(CPtr<const Vector4f> Uniform, Ptr<Vector4f> Buffer);
        void UnmapUniformBuffer();

    private:


        SPtr<Graphic::Service> mService;

        Array<UInt, 3>         mBuffers;

        //Array<UInt,          3>              mSamplers;

        Scene                                      mScene;

        Stack<Drawable,      k_MaxDrawables>       mDrawables;
        Stack<Ptr<Drawable>, k_MaxDrawables>       mDrawablesPtr;
        Stack<Submission,    k_MaxDrawables>       mSubmissions;
        Stack<Ptr<const Material>, k_MaxDrawables> mMaterialsPtr;
    };
}