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
#include <Aurora.Graphic/GLES3/GLES3Driver.hpp>

#ifdef    SDL_PLATFORM_WINDOWS
    #include <Aurora.Graphic/D3D11/D3D11Driver.hpp>
#endif // SDL_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Context> Context)
        : AbstractSubsystem(Context)
    {
        // Initialize the worker thread allowing the service to handle
        // GPU commands concurrently with other tasks.
        mWorker = Thread(std::bind_front(&Service::OnConsume, this));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnDispose()
    {
        // Gracefully signals the thread to finish its current tasks and terminate.
        // Ensures an orderly shutdown of the worker thread, preventing abrupt
        // terminations that could lead to resource leaks or inconsistent states.
        mWorker.request_stop();
        Flush();
        mWorker.join();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        Bool Successful = true;

        if (!mDriver)
        {
            switch (Backend)
            {
#ifdef    SDL_PLATFORM_WINDOWS
                case Backend::D3D11:
                    mDriver = NewUniquePtr<D3D11Driver>();
                    break;
#endif // SDL_PLATFORM_WINDOWS
                case Backend::GLES3:
                    mDriver = NewUniquePtr<GLES3Driver>();
                    break;
                default:
                    break;
            }

            if (mDriver)
            {
                // Perform a double flush to ensure synchronization between the CPU and GPU.
                // The first Flush call handles the immediate encoding and begins the process of
                // transferring data to the GPU. The second Flush ensures that all data has been
                // completely processed and synchronized, eliminating any potential latency or
                // incomplete transfer issues that could arise from a single flush operation.
                mFrames[k_Default].WriteEnum(Command::Initialize);
                mFrames[k_Default].WriteInt(reinterpret_cast<UInt>(Swapchain));
                mFrames[k_Default].WriteUInt16(Width);
                mFrames[k_Default].WriteUInt16(Height);
                mFrames[k_Default].WriteUInt8(Samples);
                Flush();
                Flush();
            }

            Successful = mDriver != nullptr;

            if (Successful)
            {
                ConstRef<Capabilities> Capabilities = mDriver->GetCapabilities();
                Log::Info("Graphics: using {}", magic_enum::enum_name(Capabilities.Backend));
                Log::Info("Graphics: Detected shader model {}", CastEnum(Capabilities.Language) + 1);

                for (ConstRef<Adapter> Adapter : Capabilities.Adapters)
                {
                    Log::Info("Graphics: Found GPU '{}'", Adapter.Description);
                    Log::Info("Graphics:     Memory {} (video), {} (system), {} (shared)",
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

    void Service::Reset(UInt16 Width, UInt16 Height, UInt8 Samples)
    {
        mFrames[k_Default].WriteEnum(Command::Reset);
        mFrames[k_Default].WriteUInt16(Width);
        mFrames[k_Default].WriteUInt16(Height);
        mFrames[k_Default].WriteUInt8(Samples);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateBuffer(Usage Type, Bool Immutable, Any<Data> Data)
    {
        const Object ID = mBuffers.Allocate();

        if (ID)
        {
            mFrames[k_Default].WriteEnum(Command::CreateBuffer);
            mFrames[k_Default].WriteUInt16(ID);
            mFrames[k_Default].WriteEnum(Type);
            mFrames[k_Default].WriteBool(Immutable);
            mFrames[k_Default].WriteObject(Data);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size)
    {
        mFrames[k_Default].WriteEnum(Command::CopyBuffer);
        mFrames[k_Default].WriteUInt16(DstBuffer);
        mFrames[k_Default].WriteUInt32(DstOffset);
        mFrames[k_Default].WriteUInt16(SrcBuffer);
        mFrames[k_Default].WriteUInt32(SrcOffset);
        mFrames[k_Default].WriteUInt32(Size);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::UpdateBuffer(Object ID, Bool Discard, UInt32 Offset, Any<Data> Data)
    {
        mFrames[k_Default].WriteEnum(Command::UpdateBuffer);
        mFrames[k_Default].WriteUInt16(ID);
        mFrames[k_Default].WriteBool(Discard);
        mFrames[k_Default].WriteUInt32(Offset);
        mFrames[k_Default].WriteObject(Data);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeleteBuffer(Object ID)
    {
        mFrames[k_Default].WriteEnum(Command::DeleteBuffer);
        mFrames[k_Default].WriteUInt16(mBuffers.Free(ID));
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

    Object Service::CreatePass(CPtr<const Attachment> Colors, CPtr<const Attachment> Resolves, ConstRef<Attachment> Auxiliary)
    {
        const Object ID = mPasses.Allocate();

        if (ID)
        {
            mFrames[k_Default].WriteEnum(Command::CreatePass);
            mFrames[k_Default].WriteUInt16(ID);
            mFrames[k_Default].WriteBlock(Colors);
            mFrames[k_Default].WriteBlock(Resolves);
            mFrames[k_Default].Write(Auxiliary);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeletePass(Object ID)
    {
        mFrames[k_Default].WriteEnum(Command::DeletePass);
        mFrames[k_Default].WriteUInt16(mPasses.Free(ID));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreatePipeline(Any<Data> Vertex, Any<Data> Fragment, Any<Data> Geometry, ConstRef<Descriptor> Properties)
    {
        const Object ID = mPipelines.Allocate();

        if (ID)
        {
            mFrames[k_Default].WriteEnum(Command::CreatePipeline);
            mFrames[k_Default].WriteUInt16(ID);
            mFrames[k_Default].WriteObject(Vertex);
            mFrames[k_Default].WriteObject(Fragment);
            mFrames[k_Default].WriteObject(Geometry);
            mFrames[k_Default].Write(Properties);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeletePipeline(Object ID)
    {
        mFrames[k_Default].WriteEnum(Command::DeletePipeline);
        mFrames[k_Default].WriteUInt16(mPipelines.Free(ID));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object Service::CreateTexture(TextureFormat Format, TextureLayout Layout, UInt16 Width ,UInt16 Height, UInt8 Level, UInt8 Samples, Any<Data> Data)
    {
        const Object ID = mTextures.Allocate();

        if (ID)
        {
            mFrames[k_Default].WriteEnum(Command::CreateTexture);
            mFrames[k_Default].WriteUInt16(ID);
            mFrames[k_Default].WriteEnum(Format);
            mFrames[k_Default].WriteEnum(Layout);
            mFrames[k_Default].WriteUInt16(Width);
            mFrames[k_Default].WriteUInt16(Height);
            mFrames[k_Default].WriteUInt8(Level);
            mFrames[k_Default].WriteUInt8(Samples);
            mFrames[k_Default].WriteObject(Data);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::CopyTexture(Object DstTexture, UInt8 DstLevel, ConstRef<Vector2i> DstOffset, Object SrcTexture, UInt8 SrcLevel, ConstRef<Recti> SrcOffset)
    {
        mFrames[k_Default].WriteEnum(Command::CopyTexture);
        mFrames[k_Default].WriteUInt16(DstTexture);
        mFrames[k_Default].WriteUInt8(DstLevel);
        mFrames[k_Default].WriteObject(DstOffset);
        mFrames[k_Default].WriteUInt16(SrcTexture);
        mFrames[k_Default].WriteUInt8(SrcLevel);
        mFrames[k_Default].WriteObject(SrcOffset);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::UpdateTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset, UInt32 Pitch, Any<Data> Data)
    {
        mFrames[k_Default].WriteEnum(Command::UpdateTexture);
        mFrames[k_Default].WriteUInt16(ID);
        mFrames[k_Default].WriteUInt8(Level);
        mFrames[k_Default].WriteObject(Offset);
        mFrames[k_Default].WriteUInt32(Pitch);
        mFrames[k_Default].WriteObject(Data);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data Service::ReadTexture(Object ID, UInt8 Level, ConstRef<Recti> Offset)
    {
        Log::Info("ReadTexture is not supported yet.");
        return Data();  // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::DeleteTexture(Object ID)
    {
        mFrames[k_Default].WriteEnum(Command::DeleteTexture);
        mFrames[k_Default].WriteUInt16(mTextures.Free(ID));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Prepare(Object ID, ConstRef<Rectf> Viewport, Clear Target, Color Tint, Real32 Depth, UInt8 Stencil)
    {
        mFrames[k_Default].WriteEnum(Command::Prepare);
        mFrames[k_Default].WriteUInt16(ID);
        mFrames[k_Default].WriteObject(Viewport);
        mFrames[k_Default].WriteEnum(Target);
        mFrames[k_Default].WriteObject(Tint);
        mFrames[k_Default].WriteReal32(Depth);
        mFrames[k_Default].WriteUInt8(Stencil);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Submit(CPtr<const Submission> Submissions, Bool Copy)
    {
        if (!Submissions.empty())
        {
            Data EncoderDataPtr;

            if (Copy)
            {
                EncoderDataPtr = Data(Submissions.size_bytes());
                EncoderDataPtr.Copy(Submissions.data(), Submissions.size_bytes());
            }
            else
            {
                const Ptr<Submission> Memory = const_cast<Ptr<Submission>>(Submissions.data());
                EncoderDataPtr = Data(Memory, Submissions.size_bytes(), Data::EMPTY_DELETER);
            }

            mFrames[k_Default].WriteEnum(Command::Submit);
            mFrames[k_Default].WriteObject(EncoderDataPtr);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Commit(Object ID, Bool Synchronised)
    {
        mFrames[k_Default].WriteEnum(Command::Commit);
        mFrames[k_Default].WriteUInt16(ID);
        mFrames[k_Default].WriteBool(Synchronised);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Flush()
    {
        // Wait until the GPU has finished processing any current tasks.
        // This ensures that the buffer swap occurs only when the GPU is idle.
        mBusy.wait(true);

        // Exchange the buffers so that the CPU can write new commands into the
        // encoder buffer while the GPU processes the commands in the decoder buffer.
        std::rotate(mFrames.rbegin(), mFrames.rbegin() + 1, mFrames.rend());

        // Clear the encoder buffer to prepare it for new data.
        // This is necessary to avoid processing stale or incorrect data in subsequent operations.
        mFrames[k_Default].Clear();

        // Notify the GPU has new work to process by setting the busy flag.
        // This flag ensures synchronization between the CPU and GPU tasks.
        mBusy.test_and_set();
        mBusy.notify_one();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnConsume(std::stop_token Token)
    {
        while (! Token.stop_requested())
        {
            // Put the thread to sleep until the flag indicates there is more work to process.
            // This ensures that the CPU remains idle, conserving resources, until new tasks
            // are available for execution.
            mBusy.wait(false);

            // Exit the loop if a stop request has been issued.
            if (! Token.stop_requested())
            {
                // Continuously process the data as long as there is available data in the decoder.
                // This involves reading commands from the decoder and executing them to update the state
                // or perform necessary actions.
                Reader Decoder(mFrames[k_InFlightFrames - 1].GetData());
                while (Decoder.GetAvailable() > 0)
                {
                    OnExecute(Decoder.ReadEnum<Command>(), Decoder);
                }
            }

            // Clear the busy flag to signal that the GPU has completed its current tasks.
            // This action informs the main thread that the system is no longer busy, ensuring
            // that the main thread can proceed with issuing new commands.
            mBusy.clear(std::memory_order_release);
            mBusy.notify_one();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnExecute(Command Type, Ref<Reader> Reader)
    {
        switch (Type)
        {
        case Command::Initialize:
        {
            const auto Swapchain = reinterpret_cast<Ptr<SDL_Window>>(Reader.ReadInt<UInt>());
            const auto Width     = Reader.ReadUInt16();
            const auto Height    = Reader.ReadUInt16();
            const auto Samples   = Reader.ReadUInt8();

            if (const Bool Succeed = mDriver->Initialize(Swapchain, Width, Height, Samples); !Succeed)
            {
                mDriver = nullptr;
            }
            break;
        }
        case Command::Reset:
        {
            const auto Width   = Reader.ReadUInt16();
            const auto Height  = Reader.ReadUInt16();
            const auto Samples = Reader.ReadUInt8();

            mDriver->Reset(Width, Height, Samples);
            break;
        }
        case Command::CreateBuffer:
        {
            const auto ID        = Reader.ReadUInt16();
            const auto Kind      = Reader.ReadEnum<Usage>();
            const auto Immutable = Reader.ReadBool();
            const auto Bytes     = Reader.ReadObject<Data>();

            mDriver->CreateBuffer(ID, Kind, Immutable, Bytes.GetData<const UInt8>(), Bytes.GetSize());
            break;
        }
        case Command::CopyBuffer:
        {
            const auto DstBuffer = Reader.ReadUInt16();
            const auto DstOffset = Reader.ReadUInt32();
            const auto SrcBuffer = Reader.ReadUInt16();
            const auto SrcOffset = Reader.ReadUInt32();
            const auto Size      = Reader.ReadUInt32();

            mDriver->CopyBuffer(DstBuffer, DstOffset, SrcBuffer, SrcOffset, Size);
            break;
        }
        case Command::UpdateBuffer:
        {
            const auto ID      = Reader.ReadUInt16();
            const auto Discard = Reader.ReadBool();
            const auto Offset  = Reader.ReadUInt32();
            const auto Bytes   = Reader.ReadObject<Data>();

            mDriver->UpdateBuffer(ID, Discard, Offset, Bytes);
            break;
        }
        case Command::DeleteBuffer:
        {
            mDriver->DeleteBuffer(Reader.ReadUInt16());
            break;
        }
        case Command::CreatePass:
        {
            const auto ID        = Reader.ReadUInt16();
            const auto Colors    = Reader.ReadBlock<Attachment>();
            const auto Resolves  = Reader.ReadBlock<Attachment>();
            const auto Auxiliary = Reader.Read<Attachment>();

            mDriver->CreatePass(ID, Colors, Resolves, Auxiliary);
            break;
        }
        case Command::DeletePass:
        {
            mDriver->DeletePass(Reader.ReadUInt16());
            break;
        }
        case Command::CreatePipeline:
        {
            const auto ID         = Reader.ReadUInt16();
            const auto Vertex     = Reader.ReadObject<Data>();
            const auto Fragment   = Reader.ReadObject<Data>();
            const auto Geometry   = Reader.ReadObject<Data>();
            const auto Properties = Reader.Read<Descriptor>();

            mDriver->CreatePipeline(ID, Vertex, Fragment, Geometry, Properties);
            break;
        }
        case Command::DeletePipeline:
        {
            mDriver->DeletePipeline(Reader.ReadUInt16());
            break;
        }
        case Command::CreateTexture:
        {
            const auto ID      = Reader.ReadUInt16();
            const auto Format  = Reader.ReadEnum<TextureFormat>();
            const auto Layout  = Reader.ReadEnum<TextureLayout>();
            const auto Width   = Reader.ReadUInt16();
            const auto Height  = Reader.ReadUInt16();
            const auto Level   = Reader.ReadUInt8();
            const auto Samples = Reader.ReadUInt8();
            const auto Bytes   = Reader.ReadObject<Data>();

            mDriver->CreateTexture(ID, Format, Layout, Width, Height, Level, Samples, Bytes);
            break;
        }
        case Command::CopyTexture:
        {
            const auto DstTexture = Reader.ReadUInt16();
            const auto DstLevel   = Reader.ReadUInt8();
            const auto DstOffset  = Reader.ReadObject<Vector2i>();
            const auto SrcTexture = Reader.ReadUInt16();
            const auto SrcLevel   = Reader.ReadUInt8();
            const auto SrcOffset  = Reader.ReadObject<Recti>();

            mDriver->CopyTexture(DstTexture, DstLevel, DstOffset, SrcTexture, SrcLevel, SrcOffset);
            break;
        }
        case Command::UpdateTexture:
        {
            const auto ID     = Reader.ReadUInt16();
            const auto Level  = Reader.ReadUInt8();
            const auto Offset = Reader.ReadObject<Recti>();
            const auto Pitch  = Reader.ReadUInt32();
            const auto Bytes  = Reader.ReadObject<Data>();

            mDriver->UpdateTexture(ID, Level, Offset, Pitch, Bytes);
            break;
        }
        case Command::DeleteTexture:
        {
            mDriver->DeleteTexture(Reader.ReadUInt16());
            break;
        }
        case Command::Prepare:
        {
            const auto ID       = Reader.ReadUInt16();
            const auto Viewport = Reader.ReadObject<Rectf>();
            const auto Target   = Reader.ReadEnum<Clear>();
            const auto Tint     = Reader.ReadObject<Color>();
            const auto Depth    = Reader.ReadReal32();
            const auto Stencil  = Reader.ReadUInt8();

            mDriver->Prepare(ID, Viewport, Target, Tint, Depth, Stencil);
            break;
        }
        case Command::Submit:
        {
            const auto Bytes = Reader.ReadObject<Data>();

            mDriver->Submit(Bytes.GetSpan<Submission>());
            break;
        }
        case Command::Commit:
        {
            const auto ID           = Reader.ReadUInt16();
            const auto Synchronised = Reader.ReadBool();

            mDriver->Commit(ID, Synchronised);
            break;
        }
        }
    }
}