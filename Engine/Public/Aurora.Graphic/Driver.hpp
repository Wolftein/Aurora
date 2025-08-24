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

#include "Color.hpp"
#include "Common.hpp"
#include <SDL3/SDL_video.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Abstract interface for a low-level graphics driver.
    class Driver
    {
    public:

        /// \brief Destructor.
        virtual ~Driver() = default;

        /// \brief Initializes the rendering backend.
        ///
        /// \param Window  Native window handle for presentation.
        /// \param Width   Initial surface width in pixels.
        /// \param Height  Initial surface height in pixels.
        /// \param Samples Initial multisampling level.
        /// \return `true` if the initialization was successful, `false` otherwise.
        virtual Bool Initialize(Ptr<SDL_Window> Swapchain, UInt16 Width, UInt16 Height, Samples Samples) = 0;

        /// \brief Resizes the rendering surface and recreates swapchain.
        ///
        /// \param Width   New surface width in pixels.
        /// \param Height  New surface height in pixels.
        /// \param Samples New multisampling level.
        virtual void Reset(UInt16 Width, UInt16 Height, Samples Samples) = 0;

        /// \brief Gets graphics hardware capabilities.
        /// 
        /// \return Immutable capabilities structure.
        virtual ConstRef<Capabilities> GetCapabilities() const = 0;

        /// \brief Creates a buffer with the specified usage and access pattern.
        /// 
        /// \param ID     Buffer identifier.
        /// \param Access Specifies the CPU/GPU access mode.
        /// \param Usage  Specifies how the buffer will be used.
        /// \param Length Size of the buffer in bytes.
        /// \param Data   Optional pointer to initial data (can be `nullptr`).
        virtual void CreateBuffer(Object ID, Access Access, Usage Usage, UInt32 Length, ConstSpan<Byte> Data) = 0;

        /// \brief Updates buffer contents.
        ///
        /// \param ID         Buffer identifier.
        /// \param Offset     Starting byte offset for update.
        /// \param Invalidate If `true`, discards previous content before update.
        /// \param Data       New data to upload.
        virtual void UpdateBuffer(Object ID, UInt32 Offset, Bool Invalidate, ConstSpan<Byte> Data) = 0;

        /// \brief Deletes a buffer.
        /// 
        /// \param ID Buffer identifier to delete.
        virtual void DeleteBuffer(Object ID) = 0;

        /// \brief Resizes a buffer.
        ///
        /// \param ID   Buffer identifier to resize.
        /// \param Size The new size of the buffer in bytes.
        virtual void ResizeBuffer(Object ID, UInt32 Size) = 0;

        /// \brief Copies data between buffers.
        /// 
        /// \param DstBuffer Destination buffer.
        /// \param DstOffset Destination starting byte offset.
        /// \param SrcBuffer Source buffer.
        /// \param SrcOffset Source starting byte offset.
        /// \param Size      Bytes to copy.
        virtual void CopyBuffer(Object DstBuffer, UInt32 DstOffset, Object SrcBuffer, UInt32 SrcOffset, UInt32 Size) = 0;

        /// \brief Reads buffer data to CPU memory.
        /// 
        /// \param ID     Buffer identifier.
        /// \param Offset Offset in bytes from the start of the buffer.
        /// \param Size   Number of bytes to read.
        /// \return The requested buffer data.
        virtual Blob ReadBuffer(Object ID, UInt32 Offset, UInt32 Size) = 0;

        /// \brief Maps a buffer into the CPU address space for direct access.
        ///
        /// \param ID         Identifier of the buffer to map.
        /// \param Offset     Byte offset from the beginning of the buffer where the mapping starts.
        /// \param Size       Number of bytes to map.
        /// \param Invalidate If `true`, discards the buffer’s previous contents before mapping.
        ///
        /// \return Pointer to the mapped memory region, `nullptr` if mapping fails.
        virtual Ptr<Byte> MapBuffer(Object ID, UInt32 Offset, UInt32 Size, Bool Invalidate) = 0;

        /// \brief Unmaps a previously mapped buffer.
        ///
        /// \param ID Identifier of the buffer to unmap.
        virtual void UnmapBuffer(Object ID) = 0;

        /// \brief Creates a render pass.
        ///
        /// \param ID        Pass identifier (must be unique).
        /// \param Colors    Color attachments.
        /// \param Auxiliary Optional Depth/stencil attachment.
        virtual void CreatePass(Object ID, ConstSpan<Attachment> Colors, ConstRef<Attachment> Auxiliary) = 0;

        /// \brief Deletes a render pass.
        /// 
        /// \param ID Pass identifier to delete.
        virtual void DeletePass(Object ID) = 0;

        /// \brief Creates a rendering pipeline.
        ///
        /// \param ID         Pipeline identifier (must be unique).
        /// \param Vertex     Vertex shader bytecode.
        /// \param Fragment   Fragment shader bytecode.
        /// \param Descriptor Fixed-function state (copied internally).
        virtual void CreatePipeline(Object ID, ConstSpan<Byte> Vertex, ConstSpan<Byte> Fragment, ConstRef<Descriptor> Descriptor) = 0;

        /// \brief Deletes a pipeline.
        /// 
        /// \param ID Pipeline identifier to delete.
        virtual void DeletePipeline(Object ID) = 0;

        /// \brief Creates a texture.
        ///
        /// \param ID      Texture identifier (must be unique).
        /// \param Access  Specifies the CPU/GPU access mode.
        /// \param Format  Pixel data format.
        /// \param Layout  Resource usage pattern.
        /// \param Width   Texture width in texels.
        /// \param Height  Texture height in texels.
        /// \param Mipmaps Number of mipmap levels.
        /// \param Samples Multisampling level.
        /// \param Data    Optional initialization data.
        virtual void CreateTexture(Object ID, Access Access, TextureFormat Format, TextureLayout Layout, UInt16 Width, UInt16 Height, UInt8 Mipmaps, Samples Samples, ConstSpan<Byte> Data) = 0;

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
        virtual void UpdateTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height, UInt32 Pitch, ConstSpan<Byte> Data) = 0;

        /// \brief Deletes a texture.
        /// 
        /// \param ID Texture identifier to delete.
        virtual void DeleteTexture(Object ID) = 0;

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
        virtual void CopyTexture(Object DstTexture, UInt8 DstLevel, UInt16 DstX, UInt16 DstY, Object SrcTexture, UInt8 SrcLevel, UInt16 SrcX, UInt16 SrcY, UInt16 Width, UInt16 Height) = 0;

        /// \brief Reads texture data to CPU memory.
        ///
        /// \param ID     Texture identifier.
        /// \param Level  Mipmap level to read.
        /// \param X      Region X offset.
        /// \param Y      Region Y offset.
        /// \param Width  Region width.
        /// \param Height Region height.
        /// \return Blob containing pixel data.
        virtual Blob ReadTexture(Object ID, UInt8 Level, UInt16 X, UInt16 Y, UInt16 Width, UInt16 Height) = 0;

        /// \brief Prepares render target for drawing.
        ///
        /// \param ID       Render pass identifier.
        /// \param Viewport Rendering viewport.
        /// \param Target   Which buffers to clear (color, depth/stencil, or both).
        /// \param Tint     Color clear value (used if clearing color).
        /// \param Depth    Depth clear value (used if clearing depth).
        /// \param Stencil  Stencil clear value (used if clearing stencil).
        virtual void Prepare(Object ID, ConstRef<Viewport> Viewport, Clear Target, ConstRef<Color> Tint, Real32 Depth, UInt8 Stencil) = 0;

        /// \brief Submits rendering commands.
        /// 
        /// \param Submissions List of draw submissions to execute.
        virtual void Submit(ConstSpan<Submission> Submissions) = 0;

        /// \brief Resolves and finalizes render target.
        /// 
        /// \param ID Render pass identifier to commit.
        virtual void Commit(Object ID) = 0;
    };
}