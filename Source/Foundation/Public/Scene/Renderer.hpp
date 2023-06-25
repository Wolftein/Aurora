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
#include "Graphic/Material.hpp"
#include "Graphic/Pipeline.hpp"
#include "Graphic/Service.hpp"
#include "Graphic/Texture.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
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
        void Begin(Ref<const Graphic::Camera> Camera);

        // -=(Undocumented)=-
        void Draw(
            Ref<const Rectf> Destination,
            Ref<const Rectf> Source,
            Real32 Depth,
            Real32 Angle,
            UInt32 Color,
            Ref<const SPtr<Graphic::Pipeline>> Pipeline,
            Ref<const SPtr<Graphic::Material>> Material);

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

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Graphic::Submission,    k_MaxBuckets>   mSubmissions;
        Stack<Drawable,               k_MaxDrawables> mDrawablesRef;
        Stack<Ptr<Drawable>,          k_MaxDrawables> mDrawablesPtr;
        Stack<Ptr<Graphic::Material>, k_MaxBuckets>   mMaterialsPtr;
    };
}