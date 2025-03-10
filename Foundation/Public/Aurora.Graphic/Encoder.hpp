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
        void SetIndices(ConstRef<Binding> Binding)
        {
            mInFlightCommand.Indices = Binding;
        }

        // -=(Undocumented)=-
        void SetIndices(Object Buffer, UInt32 Offset, UInt32 Stride)
        {
            SetIndices(Binding(Buffer, Stride, Offset));
        }

        // -=(Undocumented)=-
        void SetVertices(UInt8 Slot, ConstRef<Binding> Binding)
        {
            mInFlightCommand.Vertices[Slot] = Binding;
        }

        // -=(Undocumented)=-
        void SetVertices(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Stride)
        {
            SetVertices(Slot, Binding(Buffer, Stride, Offset));
        }

        // -=(Undocumented)=-
        void SetUniforms(UInt8 Slot, ConstRef<Binding> Binding)
        {
            mInFlightCommand.Uniforms[Slot] = Binding;
        }

        // -=(Undocumented)=-
        void SetUniforms(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Stride)
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
        void SetPipeline(Object Pipeline)
        {
            mInFlightCommand.Pipeline = Pipeline;
        }

        // -=(Undocumented)=-
        void SetPipeline(ConstRef<Pipeline> Pipeline)
        {
            SetPipeline(Pipeline.GetID());
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
        void Bind(ConstRef<Pipeline> Pipeline, ConstRef<Material> Material)
        {
            for (UInt32 Slot = 0; Slot < Graphic::k_MaxSlots; ++Slot)
            {
                const TextureSlot Source = Pipeline.GetSlot(Slot);
                if (Source == TextureSlot::None)
                {
                    continue;
                }

                if (ConstSPtr<Texture> Texture = Material.GetTexture(Source))
                {
                    SetTexture(Slot, Texture->GetID());
                }
                else
                {
                    SetTexture(Slot, 0);
                }
                SetSampler(Slot, Material.GetSampler(Source));
            }
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
            // After queuing, reset structure to prepare for the next command.
            mInFlightSubmission.push_back(mInFlightCommand);
            mInFlightCommand = Submission();
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