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

#include "Graphic/Material.hpp"
#include "Graphic/Pipeline.hpp"
#include "Graphic/Service.hpp"

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
        static constexpr UInt k_UniformBlockPerScene     = 0;

        // -=(Undocumented)=-
        static constexpr UInt k_UniformBlockPerTechnique = 1;

        // -=(Undocumented)=-
        static constexpr UInt k_UniformBlockPerMaterial  = 2;

    public:

        // -=(Undocumented)=-
        Encoder(ConstSPtr<Service> Graphics);

        // -=(Undocumented)=-
        ~Encoder();

        // -=(Undocumented)=-
        template<typename Type>
        Ptr<Type> AllocateTransientVertices(UInt Length)
        {
            return reinterpret_cast<Ptr<Type>>(AllocateTransientBuffer(mInFlyBuffers[0], Length * sizeof(Type)));
        }

        // -=(Undocumented)=-
        void SetVertices(Graphic::Object Buffer, UInt Offset, UInt Length, UInt Stride)
        {
            mInFlyBatch.Vertices = { Buffer, Length, Offset, Stride };
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ptr<Type> AllocateTransientIndices(UInt Length)
        {
            return reinterpret_cast<Ptr<Type>>(AllocateTransientBuffer(mInFlyBuffers[1], Length * sizeof(Type)));
        }

        // -=(Undocumented)=-
        void SetIndices(Graphic::Object Buffer, UInt Offset, UInt Length, UInt Stride)
        {
            mInFlyBatch.Indices = { Buffer, Length, Offset, Stride };
        }

        // -=(Undocumented)=-
        void SetUniforms(UInt Location, Graphic::Object Buffer, UInt Offset, UInt Length)
        {
            mInFlyBatch.Uniforms[Location] = { Buffer, Length, Offset, sizeof(Vector4f) };
        }

        // -=(Undocumented)=-
        void SetScissor(Recti Scissor)
        {
            mInFlyBatch.Scissor = Scissor;
        }

        // -=(Undocumented)=-
        void SetStencil(UInt Stencil)
        {
            mInFlyBatch.Stencil = Stencil;
        }

        // -=(Undocumented)=-
        void SetPipeline(ConstSPtr<const Pipeline> Pipeline)
        {
            mInFlyBatch.Pipeline = Pipeline->GetID();
        }

        // -=(Undocumented)=-
        void SetMaterial(ConstSPtr<const Material> Material)
        {
            // Apply material's sources(s)
            for (UInt Slot = 0; Slot < Graphic::k_MaxSources; ++Slot)
            {
                if (ConstSPtr<Texture> Texture = Material->GetTexture(Slot))
                {
                    mInFlyBatch.Textures[Slot] = Texture->GetID();
                }
                else
                {
                    mInFlyBatch.Textures[Slot] = 0;
                }

                mInFlyBatch.Samplers[Slot] = Material->GetSampler(Slot);
            }

            // Apply material's parameter(s)
            const CPtr<const Vector4f> Parameters = Material->GetParameters();
            if (Parameters.empty())
            {
                mInFlyRanges[k_UniformBlockPerMaterial] = TransientRange();
            }
            else
            {
                Ptr<void> MaterialDataPtr
                    = AllocateTransientUniforms(mInFlyRanges[k_UniformBlockPerMaterial], Parameters.size_bytes());
                FastCopyMemory(MaterialDataPtr, Parameters.data(), Parameters.size_bytes());
            }
        }

        template<typename Type>
        void SetScene(Ref<const Type> Scene)
        {
            Ptr<void> SceneDataPtr
                = AllocateTransientUniforms(mInFlyRanges[k_UniformBlockPerScene], sizeof(Scene));
            FastCopyMemory(SceneDataPtr, & Scene, sizeof(Scene));
        }

        // -=(Undocumented)=-
        void SetTechnique(CPtr<Vector4f> Technique)
        {
            Ptr<void> TechniqueDataPtr
                = AllocateTransientUniforms(mInFlyRanges[k_UniformBlockPerTechnique], Technique.size_bytes());
            FastCopyMemory(TechniqueDataPtr, Technique.data(), Technique.size_bytes());
        }

        // -=(Undocumented)=-
        template<typename Vertex, typename Index>
        Bool Ensure(UInt Vertices, UInt Indices, SPtr<const Material> Material)
        {
            const Bool FitsVertices
                = mInFlyBuffers[0].Writer + Vertices * sizeof(Vertex) <= mInFlyBuffers[0].Capacity;
            const Bool FitsIndices
                = mInFlyBuffers[1].Writer + Indices * sizeof(Index)  <= mInFlyBuffers[1].Capacity;
            const Bool FitsUniforms
                = mInFlyBuffers[2].Writer + Align<256>(Material->GetParameters().size_bytes()) <= mInFlyBuffers[2].Capacity;

            if (! FitsVertices || ! FitsIndices || ! FitsUniforms)
            {
                Flush();

                if (! FitsVertices)
                {
                    mInFlyBuffers[0].Reader = mInFlyBuffers[0].Writer = mInFlyBuffers[0].Marker = 0;
                }

                if (! FitsIndices)
                {
                    mInFlyBuffers[1].Reader = mInFlyBuffers[1].Writer = mInFlyBuffers[1].Marker = 0;
                }

                if (! FitsUniforms)
                {
                    mInFlyBuffers[2].Reader = mInFlyBuffers[2].Writer = mInFlyBuffers[2].Marker = 0;

                    ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerScene]);
                    ReallocateTransientUniforms(mInFlyRanges[k_UniformBlockPerTechnique]);
                }

                return false;
            }
            return true;
        }

        // -=(Undocumented)=-
        void Draw(UInt VertexStrideBytes, UInt IndexStrideBytes);

        // -=(Undocumented)=-
        void Flush();

    private:

        // -=(Undocumented)=-
        struct TransientBuffer
        {
            Object         ID;
            UPtr<UInt08[]> Memory;
            UInt           Capacity;
            UInt           Reader = 0;
            UInt           Writer = 0;
            UInt           Marker = 0;
        };

        // -=(Undocumented)=-
        struct TransientRange
        {
            UInt Offset = 0;
            UInt Length = 0;
        };

    private:

        // -=(Undocumented)=-
        Ptr<void> AllocateTransientBuffer(Ref<TransientBuffer> Buffer, UInt Size);

        // -=(Undocumented)=-
        Ptr<void> AllocateTransientUniforms(Ref<TransientRange> Range, UInt Size);

        // -=(Undocumented)=-
        void ReallocateTransientUniforms(Ref<TransientRange> Range);

        // -=(Undocumented)=-
        void CreateTransientBuffer(Ref<TransientBuffer> Buffer, Usage Type, UInt Capacity);

        // -=(Undocumented)=-
        void UpdateTransientBuffer(Ref<TransientBuffer> Buffer);

        // -=(Undocumented)=-
        void DeleteTransientBuffer(Ref<TransientBuffer> Buffer);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Service>             mGraphics;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Submission>        mInFlySubmission;
        Submission                mInFlyBatch;
        Array<TransientBuffer, 3> mInFlyBuffers;
        Array<TransientRange,  3> mInFlyRanges;
    };
}