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

#include "Aurora.Graphic/Camera.hpp"
#include "Aurora.Graphic/Font.hpp"
#include "Aurora.Graphic/Encoder.hpp"
#include "Aurora.Graphic/Service.hpp"
#include "Aurora.Content/Service.hpp"
#include "Aurora.Math/Geometry/Line.hpp"
#include "Aurora.Math/Geometry/Rect.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    // TODO: Refactor Urgent!!
    class Render2D final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 kMaxDrawPerBatch = 20'480;

        // -=(Undocumented)=-
        enum class Type : UInt8
        {
            // -=(Undocumented)=-
            Primitive,

            // -=(Undocumented)=-
            Font,

            // -=(Undocumented)=-
            Sprite,
        };

        // -=(Undocumented)=-
        enum class Scope : UInt8
        {
            // -=(Undocumented)=-
            Global = 0,

            // -=(Undocumented)=-
            Effect = 1,

            // -=(Undocumented)=-
            Style  = 2,

            // -=(Undocumented)=-
            Object = 3,
        };

        // -=(Undocumented)=-
        struct AURORA_ALIGN(16) FontStyleSDF
        {
            Real32 uRounded              = 0;
            Real32 uInvThreshold         = 1.0f - 0.5f;
            Real32 uOutlineBias          = 1.0f/4.0f;
            Real32 uOutlineWidthAbsolute = 1.0f/3.0f;
            Real32 uOutlineWidthRelative = 1.0f/20.0f;
            Real32 uOutlineBlur          = 0.0f;
            Real32 Reserver[2];
            Color  uOuterColor           = Color(0, 0, 0, 1);

            // Hash function
            UInt64 Hash() const
            {
                UInt64 Seed = 0;

                Base::Hash(Seed, uRounded);
                Base::Hash(Seed, uInvThreshold);
                Base::Hash(Seed, uOutlineBias);
                Base::Hash(Seed, uOutlineWidthAbsolute);
                Base::Hash(Seed, uOutlineWidthRelative);
                Base::Hash(Seed, uOutlineBlur);
                Base::Hash(Seed, uOuterColor.GetRed());
                Base::Hash(Seed, uOuterColor.GetGreen());
                Base::Hash(Seed, uOuterColor.GetBlue());
                Base::Hash(Seed, uOuterColor.GetAlpha());
                return Seed;
            }
        };

    public:

        // -=(Undocumented)=-
        Render2D(Ref<Service::Host> Host);

        // -=(Undocumented)=-
        template<typename Format>
        AURORA_INLINE void SetGlobalParameters(ConstSpan<Format> Parameters)
        {
            mParameters[Enum::Cast(Scope::Global)] = mGraphics->Allocate(Usage::Uniform, Parameters);
        }

        // -=(Undocumented)=-
        template<typename Format>
        AURORA_INLINE void SetEffectParameters(ConstSpan<Format> Parameters)
        {
            mParameters[Enum::Cast(Scope::Effect)] = mGraphics->Allocate(Usage::Uniform, Parameters);
        }

        // -=(Undocumented)=-
        void SetPipeline(Type Type, ConstTracker<Pipeline> Pipeline);

        // -=(Undocumented)=-
        void SetFontStyle(ConstRef<FontStyleSDF> Style);

        // -=(Undocumented)=-
        void DrawLine(ConstRef<Line> Origin, Real32 Depth, UInt32 Tint, float Thickness = 1.0f);

        // -=(Undocumented)=-
        void DrawRect(ConstRef<Rect> Origin, Real32 Depth, UInt32 Tint, float Thickness = 1.0f);

        // -=(Undocumented)=-
        void DrawRectFilled(ConstRef<Rect> Origin, Real32 Depth, UInt32 Tint);

        // -=(Undocumented)=-
        void DrawSprite(ConstRef<Rect> Origin, Real32 Depth, ConstRef<Rect> Uv, UInt32 Tint, ConstTracker<Material> Material);

        // -=(Undocumented)=-
        void DrawSprite(ConstRef<Matrix4x4> Transform, ConstRef<Rect> Origin, Real32 Depth, ConstRef<Rect> Uv, UInt32 Tint, ConstTracker<Material> Material);

        // -=(Undocumented)=-
        void DrawFont(ConstRef<Rect> Origin, Real32 Depth, ConstText Text, Real32 Size, UInt32 Tint, ConstTracker<Font> Font);

        // -=(Undocumented)=-
        void DrawFont(ConstRef<Matrix4x4> Transform, ConstRef<Rect> Origin, Real32 Depth, ConstText Text, Real32 Size, UInt32 Tint, ConstTracker<Font> Font);

        // -=(Undocumented)=-
        void Flush();

    private:

        // -=(Undocumented)=-
        void CreateDefaultResources(ConstTracker<Content::Service> Content);

        // -=(Undocumented)=-
        struct Command
        {
            // -=(Undocumented)=-
            UInt64             Key;

            // -=(Undocumented)=-
            Type               Kind;

            // -=(Undocumented)=-
            UInt32             Tint;

            // -=(Undocumented)=-
            Tracker<Pipeline>  Pipeline;

            // -=(Undocumented)=-
            Tracker<Material>  Material;

            // -=(Undocumented)=-
            Array<Vector3, 4>  Edges;

            // -=(Undocumented)=-
            Rect               Uv;

            // -=(Undocumented)=-
            UInt32             Instance;
        };

        // -=(Undocumented)=-
        AURORA_INLINE Ref<Command> Create(Type Kind, UInt64 Key)
        {
            if (mCommandTracker.size() == kMaxDrawPerBatch)
            {
                Flush();
            }

            Ref<Command> Command = mCommands[mCommandTracker.size()];
            Command.Key      = Key;
            Command.Kind     = Kind;
            Command.Pipeline = mPipelines[Enum::Cast(Kind)];
            mCommandTracker.push_back(&Command);
            return Command;
        }

        // -=(Undocumented)=-
        AURORA_INLINE UInt64 GetUniqueKey(Material::Kind Order, Type Type, Real32 Depth, Object Material)
        {
            Object Pipeline = mPipelines[Enum::Cast(Type)]->GetID();

            if (Order == Material::Kind::Opaque)
            {
                const UInt32 DepthBits = std::bit_cast<UInt32>(1.0f + Depth);

                // TODO: Consider Byte Endianness
                return   (static_cast<UInt64>(DepthBits)                  << 32)
                       | (static_cast<UInt64>(Material          & 0xFFFF) << 16)
                       | (static_cast<UInt64>(Pipeline          & 0xFF)   << 8)
                       | (static_cast<UInt64>(Enum::Cast(Order) & 0xFF));
            }
            else
            {
                const UInt32 DepthBits = std::bit_cast<UInt32>(Depth);

                // TODO: Consider Byte Endianness
                return   (static_cast<UInt64>(Material          & 0xFFFF) << 48)
                       | (static_cast<UInt64>(Pipeline          & 0xFF)   << 40)
                       | (static_cast<UInt64>(DepthBits)                  << 8)
                       | (static_cast<UInt64>(Enum::Cast(Order) & 0xFF));
            }
        }

        // -=(Undocumented)=-
        struct Layout
        {
            // -=(Undocumented)=-
            Vector3 Position;

            // -=(Undocumented)=-
            Vector2 Texture;

            // -=(Undocumented)=-
            UInt32  Color;

            // -=(Undocumented)=-
            UInt32  Instance;
        };

        // -=(Undocumented)=-
        void WritePrimitives(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline);

        // -=(Undocumented)=-
        void WriteFonts(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material);

        // -=(Undocumented)=-
        void WriteSprites(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material);

        void PushFontData();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Tracker<Graphic::Service>                     mGraphics;
        Encoder                                       mEncoder;
        Array<Stream, Enum::Count<Scope>()>           mParameters;
        Array<Tracker<Pipeline>, Enum::Count<Type>()> mPipelines;
        Array<Command, kMaxDrawPerBatch>              mCommands;
        Vector<Ptr<Command>, kMaxDrawPerBatch>        mCommandTracker;

        Table<UInt64, UInt32>                         mFontStylesToUniform;
        Vector<FontStyleSDF, 128>                     mFontStyles;
        UInt32                                        mFontStylesSelected = 0;
        Stream                                        mFontStream;
    };
}