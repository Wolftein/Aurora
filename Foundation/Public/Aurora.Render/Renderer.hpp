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
#include "Aurora.Content/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Renderer final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxDrawCalls     = 10'240;

        // -=(Undocumented)=-
        static constexpr CStr k_DefaultMaterialUri = "Memory://Material/Default";

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
        enum class Order : UInt8
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

    public:

        // -=(Undocumented)=-
        Renderer(Ref<Core::Subsystem::Context> Context);

        // -=(Undocumented)=-
        template<typename Format>
        void SetGlobalParameters(CPtr<const Format> Parameters)
        {
            // TODO: Check if Fits
            mParameters[CastEnum(Scope::Global)] = mHeap->Allocate(Usage::Uniform, Parameters);
        }

        // -=(Undocumented)=-
        template<typename Format>
        void SetEffectParameters(CPtr<const Format> Parameters)
        {
            // TODO: Check if Fits
            mParameters[CastEnum(Scope::Effect)] = mHeap->Allocate(Usage::Uniform, Parameters);
        }

        // -=(Undocumented)=-
        void SetPipeline(Type Type, ConstSPtr<Pipeline> Pipeline)
        {
            mPipelines[CastEnum(Type)] = Pipeline;
        }

        // -=(Undocumented)=-
        void DrawLine(ConstRef<Vector2f> Origin, ConstRef<Vector2f> Target, Real32 Depth, Color Tint, float Thickness = 1.0f);

        // -=(Undocumented)=-
        void DrawRect(ConstRef<Rectf> Origin, Real32 Depth, Color Tint, float Thickness = 1.0f);

        // -=(Undocumented)=-
        void DrawQuad(ConstRef<Rectf> Origin, Real32 Depth, Color Tint);

        // -=(Undocumented)=-
        void DrawSprite(ConstRef<Rectf> Origin, Real32 Depth, ConstRef<Rectf> UV, Color Tint, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void DrawSprite(ConstRef<Matrix4f> Transform, ConstRef<Rectf> Origin, Real32 Depth, ConstRef<Rectf> UV, Color Tint, ConstSPtr<Material> Material);

        // -=(Undocumented)=-
        void DrawFont(ConstRef<Rectf> Origin, Real32 Depth, CStr16 Text, UInt16 Size, Color Tint, ConstSPtr<Font> Font);

        // -=(Undocumented)=-
        void DrawFont(ConstRef<Matrix4f> Transform, ConstRef<Rectf> Origin, Real32 Depth, CStr16 Text, UInt16 Size, Color Tint, ConstSPtr<Font> Font);

        // -=(Undocumented)=-
        void Flush(Bool Copy = false);

    private:

        // -=(Undocumented)=-
        void CreateDefaultResources(ConstSPtr<Content::Service> Content);

        // -=(Undocumented)=-
        struct Command
        {
            // -=(Undocumented)=-
            UInt64             Key;

            // -=(Undocumented)=-
            Type               Kind;

            // -=(Undocumented)=-
            Color              Tint;

            // -=(Undocumented)=-
            Ptr<Pipeline>      Pipeline;

            // -=(Undocumented)=-
            Ptr<Material>      Material;

            // -=(Undocumented)=-
            Array<Vector3f, 4> Edges;

            // -=(Undocumented)=-
            Rectf              UV;
        };

        // -=(Undocumented)=-
        Ref<Command> Create(Type Kind, UInt64 Key)
        {
            if (mCommandsPtr.size() == k_MaxDrawCalls)
            {
                Flush(true);
            }

            Ref<Command> Command = mCommands[mCommandsPtr.size()];
            Command.Key      = Key;
            Command.Kind     = Kind;
            Command.Pipeline = mPipelines[CastEnum(Kind)].get();
            mCommandsPtr.push_back(AddressOf(Command));
            return Command;
        }

        // -=(Undocumented)=-
        UInt64 GetUniqueKey(Order Order, Type Type, Real32 Depth, Object Material)
        {
            Object Pipeline = mPipelines[CastEnum(Type)]->GetID();

            if (Order == Order::Opaque)
            {
                const UInt32 DepthBits = std::bit_cast<UInt32>(1.0f + Depth);

                // TODO: Consider Byte Endianness
                return (static_cast<UInt64>(DepthBits)                << 32)
                     | (static_cast<UInt64>(Material        & 0xFFFF) << 16)
                     | (static_cast<UInt64>(Pipeline        & 0xFF)   << 8)
                     | (static_cast<UInt64>(CastEnum(Order) & 0xFF));
            }
            else
            {
                const UInt32 DepthBits = std::bit_cast<UInt32>(Depth);

                // TODO: Consider Byte Endianness
                return (static_cast<UInt64>(Material        & 0xFFFF) << 48)
                     | (static_cast<UInt64>(Pipeline        & 0xFF)   << 40)
                     | (static_cast<UInt64>(DepthBits)                << 8)
                     | (static_cast<UInt64>(CastEnum(Order) & 0xFF));
            }
        }

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
        void WritePrimitives(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline);

        // -=(Undocumented)=-
        void WriteSprites(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Service>                            mGraphics;
        UPtr<Heap>                               mHeap;
        Encoder                                  mEncoder;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Binding, CountEnum<Scope>()>       mParameters;
        Array<SPtr<Pipeline>, CountEnum<Type>()> mPipelines;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Command, k_MaxDrawCalls>           mCommands;
        Vector<Ptr<Command>>                     mCommandsPtr;
    };
}