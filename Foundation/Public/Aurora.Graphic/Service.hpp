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

#include "Frame.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Service final : public AbstractSubsystem<Service>
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_InFlightFrames = 2;

    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // -=(Undocumented)=-
        ~Service();

        // -=(Undocumented)=-
        Bool Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        ConstRef<Capabilities> GetCapabilities() const
        {
            return mDriver->GetCapabilities();
        }

        // -=(Undocumented)=-
        void Reset(UInt16 Width, UInt16 Height, UInt8 Samples);

        // -=(Undocumented)=-
        Ref<Encoder> GetEncoder()
        {
            return mFrames[k_CpuFrame].GetEncoder();
        }

        // -=(Undocumented)=-
        template<typename Format>
        Frame::Allocation<Format> Allocate(Usage Type, UInt32 Length, UInt32 Stride = sizeof(Format))
        {
            return mFrames[k_CpuFrame].Allocate<Format>(Type, Length, Stride);
        }

        // -=(Undocumented)=-
        template<typename Format>
        Binding Allocate(Usage Type, CPtr<const Format> Data)
        {
            const Frame::Allocation<Format> Allocation = Allocate<Format>(Type, Data.size_bytes());
            std::memcpy(Allocation.Pointer, Data.data(), Data.size_bytes());
            return Allocation.Binding;
        }

        // -=(Undocumented)=-
        Object CreateBuffer(Usage Type, UInt32 Capacity)
        {
            return CreateBuffer(Type, Capacity, Data());
        }

        // -=(Undocumented)=-
        Object CreateBuffer(Usage Type, UInt32 Capacity, Any<Data> Data);

        // -=(Undocumented)=-
        void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size);

        // -=(Undocumented)=-
        void UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, Any<Data> Data);

        // -=(Undocumented)=-
        void DeleteBuffer(Object ID);

        // -=(Undocumented)=-
        Object CreateMaterial();

        // -=(Undocumented)=-
        void DeleteMaterial(Object ID);

        // -=(Undocumented)=-
        Object CreatePass(CPtr<const Attachment> Colors, CPtr<const Attachment> Resolves, ConstRef<Attachment> Auxiliary);

        // -=(Undocumented)=-
        void DeletePass(Object ID);

        // -=(Undocumented)=-
        Object CreatePipeline(Any<Data> Vertex, Any<Data> Fragment, Any<Data> Geometry, ConstRef<Descriptor> Properties);

        // -=(Undocumented)=-
        void DeletePipeline(Object ID);

        // -=(Undocumented)=-
        Object CreateTexture(TextureFormat Format, TextureLayout Layout, UInt16 Width ,UInt16 Height, UInt8 Level, UInt8 Samples, Any<Data> Data);

        // -=(Undocumented)=-
        void CopyTexture(Object DstTexture, UInt8 DstLevel, ConstRef<Vector2i> DstOffset, Object SrcTexture, UInt8 SrcLevel, ConstRef<Recti> SrcOffset);

        // -=(Undocumented)=-
        void UpdateTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset, UInt32 Pitch, Any<Data> Data);

        // -=(Undocumented)=-
        Data ReadTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset);

        // -=(Undocumented)=-
        void DeleteTexture(Object ID);

        // -=(Undocumented)=-
        void Prepare(Object ID, ConstRef<Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil);

        // -=(Undocumented)=-
        void Submit(Ref<Encoder> Encoder, Bool Temporally);

        // -=(Undocumented)=-
        void Commit(Object ID, Bool Synchronised);

        // -=(Undocumented)=-
        void Flush();

    private:

        // -=(Undocumented)=-
        static constexpr UInt32 k_CpuFrame = 0;

        // -=(Undocumented)=-
        static constexpr UInt32 k_GpuFrame = 1;

        // -=(Undocumented)=-
        enum class Command
        {
            Initialize,
            Reset,
            CreateBuffer,
            CopyBuffer,
            UpdateBuffer,
            DeleteBuffer,
            CreatePass,
            DeletePass,
            CreatePipeline,
            DeletePipeline,
            CreateTexture,
            CopyTexture,
            UpdateTexture,
            DeleteTexture,
            Prepare,
            Submit,
            Commit,
        };

        // -=(Undocumented)=-
        void OnConsume(std::stop_token Token);

        // -=(Undocumented)=-
        void OnExecute(Command Type, Ref<Reader> Reader);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<Driver>                   mDriver;
        Thread                         mWorker;
        Writer                         mEncoder;
        Writer                         mDecoder;
        Atomic_Flag                    mBusy;
        Array<Frame, k_InFlightFrames> mFrames;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<k_MaxBuffers>           mBuffers;
        Handle<k_MaxMaterials>         mMaterials;
        Handle<k_MaxPasses>            mPasses;
        Handle<k_MaxPipelines>         mPipelines;
        Handle<k_MaxTextures>          mTextures;
    };
}