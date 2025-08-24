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

#include "Driver.hpp"
#include "Encoder.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Provides high-level management of the graphic subsystem.
    class Service final : public AbstractService<Service>
    {
    public:

        /// \brief Default capacity (in bytes) for transient vertex buffers.
        static constexpr UInt32 kDefaultTransientVertexCapacity  = 8 * 1024 * 1024;

        /// \brief Default capacity (in bytes) for transient index buffers.
        static constexpr UInt32 kDefaultTransientIndexCapacity   = 4 * 1024 * 1024;

        /// \brief Default capacity (in bytes) for transient uniform buffers.
        static constexpr UInt32 kDefaultTransientUniformCapacity = 2 * 1024 * 1024;

    public:

        /// \brief Constructs the graphic service and attaches it to the system context.
        ///
        /// \param Host The parent system context managing this subsystem.
        explicit Service(Ref<Host> Host);

        /// \brief Shuts down the service and releases its active resources.
        void OnTeardown() override;

        /// \brief Initializes the rendering backend.
        ///
        /// \param Backend   Rendering backend to initialize.
        /// \param Window    Native window handle for presentation.
        /// \param Width     Initial surface width in pixels.
        /// \param Height    Initial surface height in pixels.
        /// \param Samples   Initial multisampling level.
        /// \return `true` if the initialization was successful, `false` otherwise.
        Bool Initialize(Backend Backend, Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, Samples Samples);

        /// \brief Resizes the rendering surface and recreates swapchain.
        ///
        /// \param Width   New surface width in pixels.
        /// \param Height  New surface height in pixels.
        /// \param Samples New multisampling level.
        void Reset(UInt16 Width, UInt16 Height, Samples Samples);

        /// \brief Gets graphics hardware capabilities.
        ///
        /// \return Immutable capabilities structure.
        AURORA_INLINE ConstRef<Capabilities> GetCapabilities() const
        {
            return mDriver->GetCapabilities();
        }

        /// \brief Allocates transient per-frame buffer memory.
        ///
        /// \param Usage  Intended buffer usage.
        /// \param Length Number of elements to allocate.
        /// \param Stride Element stride in bytes.
        /// \return Pair containing a writable CPU pointer to the allocated memory and the stream descriptor.
        template<typename Format>
        AURORA_INLINE auto Allocate(Usage Usage, UInt32 Length, UInt32 Stride = sizeof(Format)) -> Tuple<Ptr<Format>, Stream>
        {
            if (Usage == Usage::Uniform)
            {
                Stride = kBlockAlignment;
                Length = Align<kBlockAlignment>(Length) / kBlockAlignment;
            }
            return AllocateFrame<Format>(GetProducerFrame().Heap[Enum::Cast(Usage)], Length, Stride);
        }

        /// \brief Allocates and uploads data into a transient per-frame buffer.
        ///
        /// \param Usage Intended buffer usage.
        /// \param Data  Span of elements to upload.
        /// \return Stream descriptor for the allocated range.
        template<typename Format>
        AURORA_INLINE auto Allocate(Usage Usage, ConstSpan<Format> Data) -> Stream
        {
            const auto [Pointer, Description] = Allocate<Format>(Usage, Data.size());
            std::memcpy(Pointer, Data.data(), Data.size_bytes());
            return Description;
        }

        /// \brief Creates a buffer with the specified usage and access pattern.
        ///
        /// \param Access Specifies the CPU/GPU access mode.
        /// \param Usage  Specifies how the buffer will be used.
        /// \param Data   Optional pointer to initial data (can be `nullptr`).
        /// \return Handle of the new object (0 indicates failure).
        Object CreateBuffer(Access Access, Usage Usage, AnyRef<Blob> Data);

        /// \brief Creates a buffer with the specified usage and access pattern.
        ///
        /// \param Access Specifies the CPU/GPU access mode.
        /// \param Usage  Specifies how the buffer will be used.
        /// \param Length Size of the buffer in bytes.
        /// \param Data   Optional pointer to initial data (can be `nullptr`).
        /// \return Handle of the new object (0 indicates failure).
        Object CreateBuffer(Access Access, Usage Usage, UInt32 Length, AnyRef<Blob> Data);

        /// \brief Updates buffer contents.
        ///
        /// \param ID         Buffer identifier.
        /// \param Offset     Starting byte offset for update.
        /// \param Invalidate If `true`, discards previous content before update.
        /// \param Data       New data to upload.
        void UpdateBuffer(Object ID, UInt32 Offset, Bool Invalidate, AnyRef<Blob> Data);

        /// \brief Deletes a buffer.
        ///
        /// \param ID Buffer identifier to delete.
        void DeleteBuffer(Object ID);

        /// \brief Copies data between buffers.
        ///
        /// \param DstBuffer  Destination buffer.
        /// \param DstOffset  Destination starting byte offset.
        /// \param SrcBuffer  Source buffer.
        /// \param SrcOffset  Source starting byte offset.
        /// \param Size       Bytes to copy.
        void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size);

        /// \brief Creates a material.
        ///
        /// \return Handle of the new object (0 indicates failure).
        Object CreateMaterial();

        /// \brief Deletes a material.
        ///
        /// \param ID Material identifier to delete.
        void DeleteMaterial(Object ID);

        /// \brief Creates a render pass.
        ///
        /// \param Colors    Color attachments.
        /// \param Auxiliary Optional Depth/stencil attachment.
        /// \return Handle of the new object (0 indicates failure).
        Object CreatePass(ConstSpan<Attachment> Colors, ConstRef<Attachment> Auxiliary);

        /// \brief Deletes a render pass.
        ///
        /// \param ID Pass identifier to delete.
        void DeletePass(Object ID);

        /// \brief Creates a rendering pipeline.
        ///
        /// \param Vertex     Vertex shader bytecode.
        /// \param Fragment   Fragment shader bytecode.
        /// \param Descriptor Fixed-function state (copied internally).
        /// \return Handle of the new object (0 indicates failure).
        Object CreatePipeline(AnyRef<Blob> Vertex, AnyRef<Blob> Fragment, ConstRef<Descriptor> Descriptor);

        /// \brief Deletes a pipeline.
        ///
        /// \param ID Pipeline identifier to delete.
        void DeletePipeline(Object ID);

        /// \brief Creates a texture.
        ///
        /// \param Access  Specifies the CPU/GPU access mode.
        /// \param Format  Pixel data format.
        /// \param Layout  Resource usage pattern.
        /// \param Width   Texture width in texels.
        /// \param Height  Texture height in texels.
        /// \param Mipmaps Number of mipmap levels.
        /// \param Samples Multisampling level.
        /// \param Data    Optional initialization data.
        /// \return Handle of the new object (0 indicates failure).
        Object CreateTexture(Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Mipmaps, Samples Samples, AnyRef<Blob> Data);

        /// \brief Updates a texture subregion.
        ///
        /// \param ID     Texture identifier.
        /// \param Level  Mipmap level (0 = base).
        /// \param X      Region X offset in texels.
        /// \param Y      Region Y offset in texels.
        /// \param Width  Region width in texels.
        /// \param Height Region height in texels.
        /// \param Pitch  Byte stride between rows in input data.
        /// \param Data   Pixel data to upload.
        void UpdateTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height, UInt32 Pitch, AnyRef<Blob> Data);

        /// \brief Deletes a texture.
        ///
        /// \param ID Texture identifier to delete.
        void DeleteTexture(Object ID);

        /// \brief Copies a region from one texture to another.
        ///
        /// \param DstTexture Destination texture.
        /// \param DstLevel   Destination mip level.
        /// \param DstX       Destination X offset.
        /// \param DstY       Destination Y offset.
        /// \param SrcTexture Source texture.
        /// \param SrcLevel   Source mip level.
        /// \param SrcX       Source X offset.
        /// \param SrcY       Source Y offset.
        /// \param Width      Copy width in texels.
        /// \param Height     Copy height in texels.
        void CopyTexture(Object DstTexture, UInt8 DstLevel, UInt16 DstX, UInt16 DstY, Object SrcTexture, UInt8 SrcLevel, UInt16 SrcX, UInt16 SrcY, UInt16 Width, UInt16 Height);

        /// \brief Prepares render target for drawing.
        ///
        /// \param ID       Render pass identifier.
        /// \param Viewport Rendering viewport.
        /// \param Target   Which buffers to clear (color, depth/stencil, or both).
        /// \param Tint     Color clear value (used if clearing color).
        /// \param Depth    Depth clear value (used if clearing depth).
        /// \param Stencil  Stencil clear value (used if clearing stencil).
        void Prepare(Object ID, ConstRef<Viewport> Viewport, Clear Target, ConstRef<Color> Tint, Real32 Depth, UInt8 Stencil);

        /// \brief Submits recorded rendering commands for execution.
        ///
        /// \param Encoder Command encoder that contains the recorded draw submissions.
        void Submit(Ref<Encoder> Encoder);

        /// \brief Resolves and finalizes render target.
        ///
        /// \param ID Render pass identifier to commit.
        void Commit(Object ID);

        /// \brief Finalizes the frame and submits its command buffer for execution.
        ///
        /// \param Abort `true` If `true`, cancels command execution and discards all recorded commands.
        void Finish(Bool Abort);

    private:

        /// \brief Transient memory arena used by a single in-flight frame.
        struct InFlightArena final
        {
            /// Handle to the GPU buffer associated with this arena.
            Object    GpuBuffer = 0;

            /// Handle to the CPU staging buffer.
            Object    CpuBuffer = 0;

            /// Pointer to the mapped CPU memory of the staging buffer.
            Ptr<Byte> CpuMemory = nullptr;

            /// Total capacity of the CPU buffer in bytes.
            UInt32    CpuLength = 0;

            /// Current allocation offset within the CPU buffer.
            UInt32    CpuOffset = 0;

            /// Buffer used to extend the buffer with new data.
            Writer    CpuScratch;
        };

        /// \brief Per–frame container for all GPU submission data.
        struct InFlightFrame final
        {
            /// Serialized command stream for GPU execution.
            Writer                                     Stream;

            /// Transient heaps indexed by buffer usage type.
            Array<InFlightArena, Enum::Count<Usage>()> Heap;
        };

        /// \brief Enumerates all driver command functions.
        enum class CommandType : Byte
        {
            Initialize,     ///< \see Driver::Initialize
            Reset,          ///< \see Driver::Reset
            CreateBuffer,   ///< \see Driver::CreateBuffer
            UpdateBuffer,   ///< \see Driver::UpdateBuffer
            DeleteBuffer,   ///< \see Driver::DeleteBuffer
            CopyBuffer,     ///< \see Driver::CopyBuffer
            CreatePass,     ///< \see Driver::CreatePass
            DeletePass,     ///< \see Driver::DeletePass
            CreatePipeline, ///< \see Driver::CreatePipeline
            DeletePipeline, ///< \see Driver::DeletePipeline
            CreateTexture,  ///< \see Driver::CreateTexture
            UpdateTexture,  ///< \see Driver::UpdateTexture
            DeleteTexture,  ///< \see Driver::DeleteTexture
            CopyTexture,    ///< \see Driver::CopyTexture
            Prepare,        ///< \see Driver::Prepare
            Submit,         ///< \see Driver::Submit
            Commit,         ///< \see Driver::Commit
        };

        /// \brief Stores type-safe data for a deferred driver command.
        ///
        /// \tparam Arguments Types of the stored command arguments.
        template<CommandType Type, typename ...Arguments>
        struct Command final
        {
            /// \brief Command type identifier bound at compile time.
            static constexpr CommandType kType = Type;

            /// \brief Constructs command from argument values.
            ///
            /// \param Parameters Argument values to store.
            AURORA_INLINE Command(Arguments... Parameters)
                : Parameters { Move(Parameters)... }
            {
            }

            /// \brief Executes the stored command on a Driver instance.
            ///
            /// \tparam Method Pointer to a member function of Driver.
            /// \param Driver Reference to the Driver instance.
            template<auto Method>
            AURORA_INLINE void Execute(Ref<Driver> Driver) const
            {
                std::apply([& Driver](auto&&... Parameters) {
                    (Driver.*Method)(Forward<decltype(Parameters)>(Parameters)...);
                }, Parameters);
            }

            /// Argument values of the command.
            Tuple<Arguments...> Parameters;
        };

        /// \brief Collection of type aliases for all supported driver commands.
        struct CommandList final
        {
#define DEFINE(Name, ...) using Name = Command<CommandType::Name, __VA_ARGS__>;

            DEFINE(Initialize,     Ptr<SDL_Window>, UInt16, UInt16, Samples)
            DEFINE(Reset,          UInt16, UInt16, Samples)
            DEFINE(CreateBuffer,   Object, Access, Usage, UInt32, Blob)
            DEFINE(UpdateBuffer,   Object, UInt32, Bool, Blob)
            DEFINE(DeleteBuffer,   Object)
            DEFINE(CopyBuffer,     Object, UInt32, Object, UInt32, UInt32)
            DEFINE(CreatePass,     Object, Vector<Attachment, kMaxAttachments>, Attachment)
            DEFINE(DeletePass,     Object)
            DEFINE(CreatePipeline, Object, Blob, Blob, Descriptor)
            DEFINE(DeletePipeline, Object)
            DEFINE(CreateTexture,  Object, Access, TextureFormat, TextureLayout, UInt16, UInt16, UInt8, Samples, Blob)
            DEFINE(UpdateTexture,  Object, UInt8, UInt16, UInt16, UInt16, UInt16, UInt32, Blob)
            DEFINE(DeleteTexture,  Object)
            DEFINE(CopyTexture,    Object, UInt8, UInt16, UInt16, Object, UInt8, UInt16, UInt16, UInt16, UInt16)
            DEFINE(Prepare,        Object, Viewport, Clear, Color, float, UInt8)
            DEFINE(Commit,         Object)
        };

        /// \brief Encodes a GPU command and its payload into a frame's command stream.
        ///
        /// \tparam Command   The command struct type.
        /// \tparam Arguments Argument types that define the command payload.
        /// \param Frame      The frame whose command stream will receive the data.
        /// \param Parameters Values used to construct and record the command payload.
        template<typename Command, typename... Arguments>
        AURORA_INLINE void WriteCommand(Ref<InFlightFrame> Frame, AnyRef<Arguments>... Parameters)
        {
            /// Write the command type identifier.
            Frame.Stream.WriteEnum(Command::kType);

            /// Write the command arguments.
            Frame.Stream.template WriteStruct<Command>(Forward<Arguments>(Parameters)...);
        }

        /// \brief Reconstructs and executes a GPU command from a command stream.
        ///
        /// \tparam Method  Pointer-to-member function of the driver invoked by the command.
        /// \tparam Command The command struct type.
        /// \param Stream   Source stream containing the encoded command data.
        template<auto Method, typename Command>
        AURORA_INLINE void ExecuteCommand(Ref<Reader> Stream)
        {
            const ConstPtr<Command> Instance = Stream.template ReadStruct<Command>();
            Instance->template Execute<Method>(* mDriver);
            InPlaceDelete(* Instance);
        }

        /// \brief Returns the frame currently open for CPU recording (producer).
        ///
        /// \return Reference to the producer (CPU) frame.
        AURORA_INLINE Ref<InFlightFrame> GetProducerFrame()
        {
            return mFrames[mProducer];
        }

        /// \brief Returns the frame currently being consumed by the GPU.
        ///
        /// \return Reference to the consumer (GPU) frame.
        AURORA_INLINE Ref<InFlightFrame> GetConsumerFrame()
        {
            return mFrames[mConsumer];
        }

        /// \brief Main GPU thread loop for processing and executing queued commands.
        ///
        /// \param Token Stop token used to request thread termination.
        void OnCommandThread(std::stop_token Token);

        /// \brief Dispatches execution of a single GPU command from the frame stream.
        ///
        /// \param Type  Type of command to run.
        /// \param Frame Source stream containing the encoded command data.
        void OnCommandExecute(CommandType Type, Ref<Reader> Frame);

        /// \brief Initializes a new in-flight frame.
        ///
        /// Prepares the frame’s command stream and transient heaps so it can be used for recording GPU work.
        ///
        /// \param Frame In-flight frame to initialize.
        void CreateFrame(Ref<InFlightFrame> Frame);

        /// \brief Finalizes and refreshes an in-flight frame.
        ///
        /// Pushes the frame’s transient heap data to the GPU.
        ///
        /// \param Frame In-flight frame to update.
        void UpdateFrame(Ref<InFlightFrame> Frame);

        /// \brief Allocates memory from a frame-local transient arena.
        ///
        /// \param Arena  In-flight arena from which to allocate memory.
        /// \param Length Number of elements to allocate.
        /// \param Stride Element stride in bytes.
        /// \return Pair containing a writable CPU pointer to the allocated memory and the stream descriptor.
        template<typename Format>
        AURORA_INLINE auto AllocateFrame(Ref<InFlightArena> Arena, UInt32 Length, UInt32 Stride)
        {
            const UInt32 Bytes  = Length * Stride;
            UInt32 Offset = Align(Arena.CpuOffset, Stride);

            if (Offset + Bytes < Arena.CpuLength)
            {
                Arena.CpuOffset = Offset + Bytes;

                const Ptr<Format> Address = reinterpret_cast<Ptr<Format>>(&Arena.CpuMemory[Offset]);
                return Tuple(Address, Stream(Arena.GpuBuffer, Stride, Offset));
            }
            else
            {
                if (Arena.CpuScratch.GetCapacity() == 0)
                {
                    Arena.CpuScratch.Ensure(Arena.CpuLength);
                }

                const UInt32 ScratchOffset     = Align(Arena.CpuLength + Arena.CpuScratch.GetOffset(), Stride);
                const UInt32 ScratchBaseOffset = ScratchOffset - Arena.CpuLength - Arena.CpuScratch.GetOffset();

                const Ptr<Format> Address = Arena.CpuScratch.Reserve<Format>(Bytes, ScratchBaseOffset);
                return Tuple(Address, Stream(Arena.GpuBuffer, Stride, ScratchOffset));
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Unique<Driver>                   mDriver;
        Thread                           mWorker;
        Array<InFlightFrame, kMaxFrames> mFrames;   // TODO: Implement Triple Buffer
        Flag                             mBusy;     // TODO: Implement Spinlock (Uncapped) / Wait (Capped)
        UInt32                           mProducer = 0;
        UInt32                           mConsumer = 1;


        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<kMaxBuffers>              mBuffers;
        Handle<kMaxMaterials>            mMaterials;
        Handle<kMaxPasses>               mPasses;
        Handle<kMaxPipelines>            mPipelines;
        Handle<kMaxTextures>             mTextures;
    };
}