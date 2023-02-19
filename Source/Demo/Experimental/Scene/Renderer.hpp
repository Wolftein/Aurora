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
        inline constexpr static UInt k_MaxBuckets   = 2048;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxDrawables = 16384;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxUniforms  = 16384;

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

        // -=(Undocumented)=-
        void Flush();

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
        struct VertexShaderData
        {
            Matrix4f Camera;
            Real32   Time;
        };

        // -=(Undocumented)=-
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

        // -=(Undocumented)=-
        inline constexpr static UInt k_IndicesPerQuad = 6;

        // -=(Undocumented)=-
        inline constexpr static UInt k_SceneSize = Align<16>(sizeof(VertexShaderData) / sizeof(Vector4f));

    private:

        // -=(Undocumented)=-
        void CreateResources();

        // -=(Undocumented)=-
        void DeleteResources();

        // -=(Undocumented)=-
        void WriteGeometry(Ptr<const Drawable> Drawable, Ptr<VertexShaderGeometry> Buffer);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Graphic::Service>               mService;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Graphic::Object                      mArrayBuffer;
        Graphic::Object                      mIndexBuffer;
        Graphic::Object                      mSceneBuffer;
        VertexShaderData                     mSceneData;

        Graphic::Object                      mSampler;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Submission,    k_MaxBuckets>   mSubmissions;
        Stack<Drawable,      k_MaxDrawables> mDrawablesRef;
        Stack<Ptr<Drawable>, k_MaxDrawables> mDrawablesPtr;
        Stack<Ptr<Material>, k_MaxBuckets>   mMaterialsPtr;
    };
}