// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "ImGuiRenderer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Editor
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void ImGuiRenderer::Initialize(Ref<Subsystem::Context> Context)
    {
        // Create technique.
        ConstSPtr<Content::Service> Content = Context.GetSubsystem<Content::Service>();
        mPipeline = Content->Load<Graphic::Pipeline>("Engine://Pipeline/UI.effect");
        mGraphics = Context.GetSubsystem<Graphic::Service>();

        // Create buffers and textures
        ConstSPtr<Graphic::Service> Graphics = Context.GetSubsystem<Graphic::Service>();
        mBuffers[0] = Graphics->CreateBuffer(Graphic::Usage::Vertex,  4 * 1024 * 1024);
        mBuffers[1] = Graphics->CreateBuffer(Graphic::Usage::Index,   1 * 1024 * 1024);
        mBuffers[2] = Graphics->CreateBuffer(Graphic::Usage::Uniform, 1024);
        CreateTextureFontAtlas(Context);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void ImGuiRenderer::Submit(Ref<Graphic::Encoder> Encoder, ConstRef<ImDrawData> Commands)
    {
        // @TODO: Transient buffer should remove all this stale and temporally data.
        Data VtxTransientPtr(Commands.TotalVtxCount * sizeof(ImDrawVert));
        Data IdxTransientPtr(Commands.TotalIdxCount * sizeof(ImDrawIdx));
        Data UfmTransientPtr(sizeof(Matrix4f));

        auto VtxData = VtxTransientPtr.GetData<ImDrawVert>();
        auto IdxData = IdxTransientPtr.GetData<ImDrawIdx>();

        const Matrix4f Projection = Matrix4f::CreateOrthographic(
                Commands.DisplayPos.x,
                Commands.DisplayPos.x + Commands.DisplaySize.x,
                Commands.DisplayPos.y + Commands.DisplaySize.y,
                Commands.DisplayPos.y,
                -1.0f,
                +1.0f);
        UfmTransientPtr.Copy(AddressOf(Projection));

        UInt32 VtxOffset = 0;
        UInt32 IdxOffset = 0;
        for (ConstPtr<ImDrawList> CommandList : Commands.CmdLists)
        {
            std::memcpy(VtxData, CommandList->VtxBuffer.Data, CommandList->VtxBuffer.Size * sizeof(ImDrawVert));
            std::memcpy(IdxData, CommandList->IdxBuffer.Data, CommandList->IdxBuffer.Size * sizeof(ImDrawIdx));
            VtxData += CommandList->VtxBuffer.Size;
            IdxData += CommandList->IdxBuffer.Size;

            for (ConstRef<ImDrawCmd> Command : CommandList->CmdBuffer)
            {
                if (Command.UserCallback)
                {
                    Command.UserCallback(CommandList, AddressOf(Command));
                }
                else
                {
                    // Apply scissor/clipping rectangle
                    const Recti Scissor(
                            Command.ClipRect.x - Commands.DisplayPos.x, Command.ClipRect.y - Commands.DisplayPos.y,
                            Command.ClipRect.z - Commands.DisplayPos.x, Command.ClipRect.w - Commands.DisplayPos.y);
                    if (Scissor.GetRight() <= Scissor.GetLeft() || Scissor.GetBottom() <= Scissor.GetTop())
                    {
                        continue;
                    }
                    Encoder.SetScissor(Scissor);

                    Encoder.SetVertices(0, Graphic::Binding { mBuffers[0], sizeof(ImDrawVert), 0 });
                    Encoder.SetIndices(Graphic::Binding { mBuffers[1], sizeof(ImDrawIdx), 0 });
                    Encoder.SetUniforms(0, mBuffers[2], 0, 16);
                    Encoder.SetPipeline(* mPipeline);
                    Encoder.SetTexture(0, Command.TextureId);
                    Encoder.SetSampler(0, Graphic::Sampler(Graphic::TextureEdge::Clamp, Graphic::TextureFilter::Trilinear));
                    Encoder.Draw(Command.ElemCount, Command.VtxOffset + VtxOffset, Command.IdxOffset + IdxOffset);
                }
            }

            VtxOffset += CommandList->VtxBuffer.Size;
            IdxOffset += CommandList->IdxBuffer.Size;
        }

        mGraphics->UpdateBuffer(mBuffers[0], true, 0, Move(VtxTransientPtr));
        mGraphics->UpdateBuffer(mBuffers[1], true, 0, Move(IdxTransientPtr));
        mGraphics->UpdateBuffer(mBuffers[2], true, 0, Move(UfmTransientPtr));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void ImGuiRenderer::CreateTextureFontAtlas(Ref<Subsystem::Context> Context)
    {
        Ptr<UInt8> Pixels;
        SInt32     Width;
        SInt32     Height;
        ImGui::GetIO().Fonts->GetTexDataAsRGBA32(AddressOf(Pixels), AddressOf(Width), AddressOf(Height));

        SPtr<Graphic::Texture> Texture = NewPtr<Graphic::Texture>("ImGUI_FontAtlas");
        Data Data(Pixels, Width * Height * 4, Data::BASIC_DELETER<UInt8>);

        constexpr UInt8 k_DefaultMipmaps = 1;
        constexpr UInt8 k_DefaultSamples = 1;
        Texture->Load(Graphic::TextureFormat::RGBA8UIntNorm,
                      Graphic::TextureLayout::Source, Width, Height, k_DefaultMipmaps, k_DefaultSamples, Move(Data));
        Texture->Create(Context);

        ImGui::GetIO().Fonts->SetTexID(Texture->GetID());
    }
}