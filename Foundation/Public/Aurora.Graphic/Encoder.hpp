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
        static constexpr UInt32 k_MaxCommands = 256;

    public:

        // -=(Undocumented)=-
        void Attach()
        {
            mInFlySubmissions.Clear(false);
            mInFlyCommand = mInFlySubmissions.Allocate();
        }

        // -=(Undocumented)=-
        void Detach()
        {
            if (mInFlyCommand)
            {
                mInFlyCommand = nullptr;
                mInFlySubmissions.Free();
            }
        }

        // -=(Undocumented)=-
        void SetIndices(ConstRef<Binding> Binding)
        {
            mInFlyCommand->Indices = Binding;
        }

        // -=(Undocumented)=-
        void SetIndices(Object Buffer, UInt32 Offset, UInt32 Length, UInt32 Stride)
        {
            Ref<Binding> Binding = mInFlyCommand->Indices;
            Binding.Buffer = Buffer;
            Binding.Offset = Offset;
            Binding.Length = Length;
            Binding.Stride = Stride;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetIndices(Object Buffer, UInt32 Offset, UInt32 Length)
        {
            SetIndices(Buffer, Offset, Length * sizeof(Type), sizeof(Type));
        }

        // -=(Undocumented)=-
        void SetVertices(UInt8 Slot, ConstRef<Binding> Binding)
        {
            mInFlyCommand->Vertices[Slot] = Binding;
        }

        // -=(Undocumented)=-
        void SetVertices(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Length, UInt32 Stride)
        {
            Ref<Binding> Binding = mInFlyCommand->Vertices[Slot];
            Binding.Buffer = Buffer;
            Binding.Offset = Offset;
            Binding.Length = Length;
            Binding.Stride = Stride;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetVertices(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Length)
        {
            SetVertices(Slot, Buffer, Offset, Length * sizeof(Type), sizeof(Type));
        }

        // -=(Undocumented)=-
        void SetUniforms(UInt8 Slot, ConstRef<Binding> Binding)
        {
            mInFlyCommand->Uniforms[Slot] = Binding;
        }

        // -=(Undocumented)=-
        void SetUniforms(UInt8 Slot, Object Buffer, UInt32 Offset, UInt32 Length)
        {
            Ref<Binding> Binding = mInFlyCommand->Uniforms[Slot];
            Binding.Buffer = Buffer;
            Binding.Offset = Offset;
            Binding.Length = Length;
        }

        // -=(Undocumented)=-
        void SetScissor(ConstRef<Recti> Scissor)
        {
            mInFlyCommand->Scissor = Scissor;
        }

        // -=(Undocumented)=-
        void SetStencil(UInt8 Stencil)
        {
            mInFlyCommand->Stencil = Stencil;
        }

        // -=(Undocumented)=-
        void SetPipeline(Object Technique)
        {
            mInFlyCommand->Pipeline = Technique;
        }

        // -=(Undocumented)=-
        void SetPipeline(ConstRef<Pipeline> Technique)
        {
            SetPipeline(Technique.GetID());
        }

        // -=(Undocumented)=-
        void SetSampler(UInt8 Slot, ConstRef<Sampler> Sampler)
        {
            mInFlyCommand->Samplers[Slot] = Sampler;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt8 Slot, Object Texture)
        {
            mInFlyCommand->Textures[Slot] = Texture;
        }

        // -=(Undocumented)=-
        void SetTexture(UInt8 Slot, ConstRef<Texture> Texture)
        {
            SetTexture(Slot, Texture.GetID());
        }

        // -=(Undocumented)=-
        void Draw(UInt32 Count, UInt32 Base, UInt32 Offset, UInt32 Instances = 0)
        {
            Ref<Instance> Primitive = mInFlyCommand->Primitive;
            Primitive.Count     = Count;
            Primitive.Base      = Base;
            Primitive.Offset    = Offset;
            Primitive.Instances = Instances;

            // Continue with the next command (if there is one)
            mInFlyCommand = mInFlySubmissions.IsFull() ? nullptr : mInFlySubmissions.Allocate();
        }

        // -=(Undocumented)=-
        Bool HasSubmission() const
        {
            return !mInFlySubmissions.IsEmpty();
        }

        // -=(Undocumented)=-
        CPtr<const Submission> GetSubmission() const
        {
            return mInFlySubmissions.GetData();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Stack<Submission, k_MaxCommands> mInFlySubmissions; // @TODO: Replace with linear/page allocator.
        Ptr<Submission>                  mInFlyCommand;
    };
}