// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Graphic/Material.hpp"
#include "Aurora.Graphic/Pipeline.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Encoder final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_DefaultCapacity = 64;

        // -=(Undocumented)=-
        enum class Uniforms
        {
            // -=(Undocumented)=-
            Scene    = 0,

            // -=(Undocumented)=-
            Pipeline = 1,

            // -=(Undocumented)=-
            Material = 2,

            // -=(Undocumented)=-
            Instance = 3,
        };

    public:

        // -=(Undocumented)=-
        Encoder(UInt32 Capacity = k_DefaultCapacity)
        {
            mInFlightSubmission.reserve(Capacity);
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mInFlightSubmission.clear();
            mInFlightCommand = Submission();
        }

        // -=(Undocumented)=-
        void Reset()
        {
            mInFlightCommand = Submission();    // @TODO Allow to reset partially
        }

        // -=(Undocumented)=-
        void SetIndices(ConstRef<Binding> Binding)
        {
            mInFlightCommand.Indices = Binding;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetIndices(Object Buffer, UInt32 Offset, UInt32 Stride = sizeof(Type))
        {
            SetIndices(Binding(Buffer, Stride, Offset));
        }

        // -=(Undocumented)=-
        void SetVertices(UInt8 Slot, ConstRef<Binding> Binding)
        {
            mInFlightCommand.Vertices[Slot] = Binding;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetVertices(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Stride = sizeof(Type))
        {
            SetVertices(Slot, Binding(Buffer, Stride, Offset));
        }

        // -=(Undocumented)=-
        void SetUniforms(Uniforms Slot, ConstRef<Binding> Binding)
        {
            mInFlightCommand.Uniforms[CastEnum(Slot)] = Binding;
        }

        // -=(Undocumented)=-
        void SetUniforms(Uniforms Slot, Object Buffer, UInt32 Offset, UInt32 Stride)
        {
            SetUniforms(Slot, Binding(Buffer, Stride, Offset));
        }

        // -=(Undocumented)=-
        void SetScissor(ConstRef<Recti> Scissor)
        {
            mInFlightCommand.Scissor = Scissor;
        }

        // -=(Undocumented)=-
        void SetStencil(UInt8 Stencil)
        {
            mInFlightCommand.Stencil = Stencil;
        }

        // -=(Undocumented)=-
        void SetPipeline(Object Technique)
        {
            mInFlightCommand.Pipeline = Technique;
        }

        // -=(Undocumented)=-
        void SetPipeline(ConstRef<Pipeline> Technique)
        {
            SetPipeline(Technique.GetID());
        }

        // -=(Undocumented)=-
        void SetSampler(UInt8 Slot, ConstRef<Sampler> Sampler)
        {
            mInFlightCommand.Samplers[Slot] = Sampler;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt8 Slot, Object Texture)
        {
            mInFlightCommand.Textures[Slot] = Texture;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt8 Slot, ConstRef<Texture> Texture)
        {
            SetTexture(Slot, Texture.GetID());
        }

        // -=(Undocumented)=-
        void Draw(UInt32 Count, UInt32 Base, UInt32 Offset, UInt32 Instances = 0)
        {
            Ref<Primitive> Primitive = mInFlightCommand.Primitive;
            Primitive.Count     = Count;
            Primitive.Base      = Base;
            Primitive.Offset    = Offset;
            Primitive.Instances = Instances;

            // Add the current in-flight command to the list of submissions to be processed.
            mInFlightSubmission.push_back(mInFlightCommand);
        }

        // -=(Undocumented)=-
        CPtr<const Submission> GetSubmissions() const
        {
            return mInFlightSubmission;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Submission> mInFlightSubmission; // @TODO: Replace with linear/page allocator.
        Submission         mInFlightCommand;
    };
}