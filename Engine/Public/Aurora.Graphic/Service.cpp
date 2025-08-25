// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Service.hpp"

#ifdef    AURORA_PLATFORM_WINDOWS
    #include <Aurora.Graphic/Driver/D3D11/D3D11Driver.hpp>
#endif // AURORA_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Host> Host)
        : AbstractService(Host)
    {
        mWorker = Thread(Capture(&Service::OnCommandThread, this));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTeardown()
    {
        AURORA_PROFILE;

        mWorker.request_stop();
        Finish(true);
        mWorker.join();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, Samples Samples)
    {
        AURORA_PROFILE;

        Bool Successful = true;

        if (!mDriver)
        {
            switch (Backend)
            {
#ifdef    AURORA_PLATFORM_WINDOWS
                case Backend::D3D11:
                    mDriver = NewUniquePtr<D3D11Driver>();
                    break;
#endif // AURORA_PLATFORM_WINDOWS
                default:
                    break;
            }

            if (mDriver)
            {
                WriteCommand<CommandList::Initialize>(GetProducerFrame(), Swapchain, Width, Height, Samples);
                Finish(false); // Handles the immediate encoding and begins the process of transferring data to the GPU.
                Finish(false); // Ensures that all data has been completely processed and synchronized.
            }

            Successful = mDriver != nullptr;

            if (Successful)
            {
                ConstRef<Capabilities> Capabilities = mDriver->GetCapabilities();
                LOG_INFO("Graphics: using {}", Enum::GetName(Capabilities.Backend));
                LOG_INFO("Graphics: Detected shader model {}", Enum::Cast(Capabilities.Language) + 1);

                for (ConstRef<Adapter> Adapter : Capabilities.Adapters)
                {
                    LOG_INFO("Graphics: Found GPU '{}'", Adapter.Description);
                    LOG_INFO("Graphics:     Memory {} (video), {} (system), {} (shared)",
                             Adapter.DedicatedMemoryInMBs,
                             Adapter.SystemMemoryInMBs,
                             Adapter.SharedMemoryInMBs);
                }
            }
        }
        return Successful;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Reset(UInt16 Width, UInt16 Height, Samples Samples)
    {
        WriteCommand<CommandList::Reset>(GetProducerFrame(), Width, Height, Samples);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateBuffer(Access Access, Usage Usage, AnyRef<Blob> Data)
    {
        return CreateBuffer(Access, Usage, Data.GetSize(), Move(Data));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateBuffer(Access Access, Usage Usage, UInt32 Length, AnyRef<Blob> Data)
    {
        const Object ID = mBuffers.Allocate();

        if (ID)
        {
            WriteCommand<CommandList::CreateBuffer>(GetProducerFrame(), ID, Access, Usage, Length, Move(Data));
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::UpdateBuffer(Object ID, UInt32 Offset, Bool Invalidate, AnyRef<Blob> Data)
    {
        WriteCommand<CommandList::UpdateBuffer>(GetProducerFrame(), ID, Offset, Invalidate, Move(Data));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeleteBuffer(Object ID)
    {
        if (mBuffers.Free(ID))
        {
            WriteCommand<CommandList::DeleteBuffer>(GetProducerFrame(), ID);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size)
    {
        WriteCommand<CommandList::CopyBuffer>(GetProducerFrame(), DstBuffer, DstOffset, SrcBuffer, SrcOffset, Size);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreatePass(ConstSpan<Attachment> Colors, ConstRef<Attachment> Auxiliary)
    {
        const Object ID = mPipelines.Allocate();

        if (ID)
        {
            Vector<Attachment, kMaxAttachments> Attachments(Colors.begin(), Colors.end());

            WriteCommand<CommandList::CreatePass>(GetProducerFrame(), ID, Move(Attachments), Move(Auxiliary));
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeletePass(Object ID)
    {
        if (mPasses.Free(ID))
        {
            WriteCommand<CommandList::DeletePass>(GetProducerFrame(), ID);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateMaterial()
    {
        return mMaterials.Allocate();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeleteMaterial(Object ID)
    {
        mMaterials.Free(ID);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreatePipeline(AnyRef<Blob> Vertex, AnyRef<Blob> Fragment, ConstRef<Descriptor> Descriptor)
    {
        const Object ID = mPipelines.Allocate();

        if (ID)
        {
            WriteCommand<CommandList::CreatePipeline>(GetProducerFrame(), ID, Move(Vertex), Move(Fragment), Descriptor);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeletePipeline(Object ID)
    {
        if (mPipelines.Free(ID))
        {
            WriteCommand<CommandList::DeletePipeline>(GetProducerFrame(), ID);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateTexture(Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Mipmaps, Samples Samples, AnyRef<Blob> Data)
    {
        const Object ID = mTextures.Allocate();

        if (ID)
        {
            WriteCommand<CommandList::CreateTexture>(GetProducerFrame(), ID, Access, Format, Layout, Width, Height, Mipmaps, Samples, Move(Data));
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::UpdateTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height, UInt32 Pitch, AnyRef<Blob> Data)
    {
        WriteCommand<CommandList::DeletePipeline>(GetProducerFrame(), mPipelines.Free(ID));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeleteTexture(Object ID)
    {
        if (mTextures.Free(ID))
        {
            WriteCommand<CommandList::DeleteTexture>(GetProducerFrame(), ID);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::CopyTexture(Object DstTexture, UInt8 DstLevel, UInt16 DstX, UInt16 DstY, Object SrcTexture, UInt8 SrcLevel, UInt16 SrcX, UInt16 SrcY, UInt16 Width, UInt16 Height)
    {
        WriteCommand<CommandList::CopyTexture>(GetProducerFrame(),
            DstTexture, DstLevel, DstX, DstY,
            SrcTexture, SrcLevel, SrcX, SrcY, Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Prepare(Object ID, ConstRef<Viewport> Viewport, Clear Target, ConstRef<Color> Tint, Real32 Depth, UInt8 Stencil)
    {
        WriteCommand<CommandList::Prepare>(GetProducerFrame(), ID, Viewport, Target, Tint, Depth, Stencil);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Submit(Ref<Encoder> Encoder)
    {
        Ref<Writer> Writer = GetProducerFrame().Stream;

        if (ConstSpan<Submission> Submissions = Encoder.GetSubmissions(); !Submissions.empty())
        {
            Writer.WriteEnum(CommandType::Submit);
            Writer.WriteUInt32(Submissions.size());
            Writer.Write(Submissions.data(), Submissions.size_bytes());
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Commit(Object ID)
    {
        WriteCommand<CommandList::Commit>(GetProducerFrame(), ID);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Finish(Bool Abort)
    {
        AURORA_PROFILE_SCOPE("Graphic::Service::Tick");

        FlushCommands(Abort);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::FlushCommands(Bool Abort)
    {
        if (Abort)
        {
            GetProducerFrame().Stream.Clear();
        }

        // Wait until the GPU thread signals that the current frame has finished processing.
        mBusy.wait(true);

        // Rotate the frame queue so that the next frame becomes writable for the CPU while the previous one
        // moves into the GPU submission pipeline.
        Swap(mProducer, mConsumer);

        // Mark the service as busy again and notify the GPU thread that a new frame is ready for processing.
        mBusy.test_and_set(std::memory_order_acq_rel);
        mBusy.notify_one();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnCommandThread(std::stop_token Token)
    {
        AURORA_THREAD_NAME("GPU Thread");

        while (!Token.stop_requested())
        {
            AURORA_PROFILE_SCOPE("Graphic::Service::Process");

            // Put the thread to sleep until the flag indicates there is more work to process.
            mBusy.wait(false);

            // Finalize the most recently recorded frame by uploading its transient resources to the GPU.
            Ref<InFlightFrame> Frame = mFrames[mConsumer];
            UpdateFrame(Frame);

            // Continuously process the data as long as there is available data in the decoder.
            Reader Decoder(Frame.Stream.GetData());
            while (Decoder.GetAvailable() > 0)
            {
                OnCommandExecute(Decoder.ReadEnum<CommandType>(), Decoder);
            }
            Frame.Stream.Clear();

            // Clear the busy flag to signal that the GPU has completed its current tasks.
            mBusy.clear(std::memory_order_release);
            mBusy.notify_one();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnCommandExecute(CommandType Type, Ref<Reader> Frame)
    {
        switch (Type)
        {
            case CommandType::Initialize:
            {
                const auto Data = Frame.ReadStruct<CommandList::Initialize>();

                const Ptr<SDL_Window> Window  = Fetch<0>(Data->Parameters);
                const UInt16          Width   = Fetch<1>(Data->Parameters);
                const UInt16          Height  = Fetch<2>(Data->Parameters);
                const Samples         Samples = Fetch<3>(Data->Parameters);

                if (const Bool Succeed = mDriver->Initialize(Window, Width, Height, Samples); Succeed)
                {
                    for (Ref<InFlightFrame> InFlightFrame : mFrames)
                    {
                        CreateFrame(InFlightFrame);
                    }
                }
                else
                {
                    mDriver = nullptr;
                }

                InPlaceDelete(* Data);
                break;
            }
            case CommandType::Reset:
            {
                ExecuteCommand<&Driver::Reset, CommandList::Reset>(Frame);
                break;
            }
            case CommandType::CreateBuffer:
            {
                ExecuteCommand<&Driver::CreateBuffer, CommandList::CreateBuffer>(Frame);
                break;
            }
            case CommandType::UpdateBuffer:
            {
                ExecuteCommand<&Driver::UpdateBuffer, CommandList::UpdateBuffer>(Frame);
                break;
            }
            case CommandType::DeleteBuffer:
            {
                ExecuteCommand<&Driver::DeleteBuffer, CommandList::DeleteBuffer>(Frame);
                break;
            }
            case CommandType::CopyBuffer:
            {
                ExecuteCommand<&Driver::CopyBuffer, CommandList::CopyBuffer>(Frame);
                break;
            }
            case CommandType::CreatePass:
            {
                ExecuteCommand<&Driver::CreatePass, CommandList::CreatePass>(Frame);
                break;
            }
            case CommandType::DeletePass:
            {
                ExecuteCommand<&Driver::DeletePass, CommandList::DeletePass>(Frame);
                break;
            }
            case CommandType::CreatePipeline:
            {
                ExecuteCommand<&Driver::CreatePipeline, CommandList::CreatePipeline>(Frame);
                break;
            }
            case CommandType::DeletePipeline:
            {
                ExecuteCommand<&Driver::DeletePipeline, CommandList::DeletePipeline>(Frame);
                break;
            }
            case CommandType::CreateTexture:
            {
                ExecuteCommand<&Driver::CreateTexture, CommandList::CreateTexture>(Frame);
                break;
            }
            case CommandType::UpdateTexture:
            {
                ExecuteCommand<&Driver::UpdateTexture, CommandList::UpdateTexture>(Frame);
                break;
            }
            case CommandType::DeleteTexture:
            {
                ExecuteCommand<&Driver::DeleteTexture, CommandList::DeleteTexture>(Frame);
                break;
            }
            case CommandType::CopyTexture:
            {
                ExecuteCommand<&Driver::CopyTexture, CommandList::CopyTexture>(Frame);
                break;
            }
            case CommandType::Prepare:
            {
                ExecuteCommand<&Driver::Prepare, CommandList::Prepare>(Frame);
                break;
            }
            case CommandType::Submit:
            {
                const auto Size = Frame.ReadUInt32();
                const auto Data = Frame.Read<ConstPtr<Submission>>(Size * sizeof(Submission));
                mDriver->Submit(ConstSpan<Submission>(Data, Size));
                break;
            }
            case CommandType::Commit:
            {
                ExecuteCommand<&Driver::Commit, CommandList::Commit>(Frame);
                break;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::CreateFrame(Ref<InFlightFrame> Frame)
    {
        static auto CreateInFlightBuffer = [&](Ref<InFlightArena> Arena, Usage Usage, UInt32 Length)
        {
            Arena.GpuBuffer = mBuffers.Allocate();
            Arena.CpuBuffer = mBuffers.Allocate();
            Arena.CpuLength = Length;
            mDriver->CreateBuffer(Arena.GpuBuffer, Access::Dual, Usage, Length, ConstSpan<Byte>());
            mDriver->CreateBuffer(Arena.CpuBuffer, Access::Host, Usage, Length, ConstSpan<Byte>());
            Arena.CpuMemory = mDriver->MapBuffer(Arena.CpuBuffer, 0, Length, false);
        };

        CreateInFlightBuffer(Frame.Heap[Enum::Cast(Usage::Vertex)],  Usage::Vertex,  kDefaultTransientVertexCapacity);
        CreateInFlightBuffer(Frame.Heap[Enum::Cast(Usage::Index)],   Usage::Index,   kDefaultTransientIndexCapacity);
        CreateInFlightBuffer(Frame.Heap[Enum::Cast(Usage::Uniform)], Usage::Uniform, kDefaultTransientUniformCapacity);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::UpdateFrame(Ref<InFlightFrame> Frame)
    {
        for (Ref<InFlightArena> Arena : Frame.Heap)
        {
            const UInt32 CpuOffsetBase = Exchange(Arena.CpuOffset, 0);
            const UInt32 CpuOffsetMore = Arena.CpuScratch.GetOffset();

            // If new data was appended, grow the GPU buffer and upload the additional contents
            // from the CPU scratch region.
            if (CpuOffsetMore > 0)
            {
                mDriver->ResizeBuffer(Arena.GpuBuffer, Arena.CpuLength + Arena.CpuScratch.GetCapacity());
                mDriver->UpdateBuffer(Arena.GpuBuffer, Arena.CpuLength, false, Arena.CpuScratch.GetData());
            }

            // If the base CPU staging region contains data, unmap it and copy its contents into the GPU buffer.
            if (CpuOffsetBase > 0)
            {
                mDriver->UnmapBuffer(Arena.CpuBuffer);
                mDriver->CopyBuffer(Arena.GpuBuffer, 0, Arena.CpuBuffer, 0, CpuOffsetBase);
            }

            // If new data was appended, reset the appender, extend the logical CPU buffer length,
            // resize the staging buffer, and remap it for the next frame’s writes.
            if (CpuOffsetMore > 0)
            {
                Arena.CpuLength += Arena.CpuScratch.GetCapacity();
                Arena.CpuScratch = Writer();

                mDriver->ResizeBuffer(Arena.CpuBuffer, Arena.CpuLength);

                Arena.CpuMemory = mDriver->MapBuffer(Arena.CpuBuffer, 0, Arena.CpuLength, false);
            }

            if (CpuOffsetBase > 0 && CpuOffsetMore == 0)
            {
                Arena.CpuMemory = mDriver->MapBuffer(Arena.CpuBuffer, 0, Arena.CpuLength, false);
            }
        }
    }
}