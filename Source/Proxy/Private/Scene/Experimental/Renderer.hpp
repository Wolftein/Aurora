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
#include "Graphic/Font.hpp"
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
    class Renderer // TODO: Rework
    {
    public:

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxBuckets   = 2048;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxDrawables = 16384;

        // -=(Undocumented)=-
        inline constexpr static UInt k_MaxUniforms  = 16384;

        enum class Order {
            Opaque, Transparent
        };

        enum  Alignment {
            FONS_ALIGN_LEFT 	= 1<<0,	// Default
            FONS_ALIGN_CENTER 	= 1<<1,
            FONS_ALIGN_RIGHT 	= 1<<2,
            FONS_ALIGN_TOP 		= 1<<3,
            FONS_ALIGN_MIDDLE	= 1<<4,
            FONS_ALIGN_BOTTOM	= 1<<5,
            FONS_ALIGN_BASELINE	= 1<<6, // Default
        };

    public:

        // -=(Undocumented)=-
        explicit Renderer(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        ~Renderer();

        void SetDefaultFontPipeline(SPtr<Graphic::Pipeline> FontPipeline)
        {
            mFontPipeline = FontPipeline;
        }

        // -=(Undocumented)=-
        auto GetGraphics() { return mGraphics; }

        // -=(Undocumented)=-
        void Begin(Ref<const Graphic::Camera> Camera);

        // -=(Undocumented)=-
        template<typename Type>
        void SetParameter(UInt Offset, Type Parameter)
        {
            constexpr UInt Size = sizeof(Type) / sizeof(Vector4f) + 1;

            if (Offset + Size > mSceneParameters.size())
            {
                mSceneParameters.resize(mSceneParameters.size() + Align<16>(Size));
            }

            * (reinterpret_cast<Ptr<Type>>(reinterpret_cast<UInt>(mSceneParameters.data()) + Offset)) = Parameter;
        }

        // -=(Undocumented)=-
        void Draw(
            Rectf Destination,
            Rectf Source,
            Real32 Depth,
            Real32 Angle,
            Color Tint,
            Order Order,
            Ref<const SPtr<const Graphic::Pipeline>> Pipeline,
            Ref<const SPtr<const Graphic::Material>> Material);

        // -=(Undocumented)=-
        void DrawFont(SPtr<const Graphic::Font> Font, CStr Text, Alignment Alignment, Vector2f Position, Real32 Depth, Real32 Scale, Color Tint);

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
        struct DrawableID
        {
            union
            {
                UInt64 Key;

                struct
                {
                    UInt64 bDepth     : 32;
                    UInt64 bMaterial  : 16;
                    UInt64 bTechnique : 8;
                    UInt64 bType      : 8;

                } Opaque;

                struct
                {
                    UInt64 bMaterial  : 16;
                    UInt64 bTechnique : 8;
                    UInt64 bDepth     : 32;
                    UInt64 bType      : 8;
                } Transparent;

            } ;
        };

        // -=(Undocumented)=-
        struct Drawable
        {
            DrawableID              ID;
            Rectf                   Destination;
            Rectf                   Source;
            Real32                  Angle;
            Real32                  Depth;
            UInt32                  Color;
            SPtr<const Graphic::Pipeline> Pipeline;
            SPtr<const Graphic::Material> Material;
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

        SPtr<Graphic::Service>               mGraphics;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Graphic::Object                      mArrayBuffer;
        Graphic::Object                      mIndexBuffer;
        Graphic::Object                      mSceneBuffer;
        VertexShaderData                     mSceneData;
        Vector<Vector4f>                     mSceneParameters;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Graphic::Submission,    k_MaxBuckets>   mSubmissions;
        Stack<Drawable,               k_MaxDrawables> mDrawablesRef;
        Stack<Ptr<Drawable>,          k_MaxDrawables> mDrawablesPtr;
        Stack<Ptr<Graphic::Material>, k_MaxBuckets>   mMaterialsPtr;

        SPtr<Graphic::Pipeline> mFontPipeline;
    };
}