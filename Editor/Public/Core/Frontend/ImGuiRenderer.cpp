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

        // Create texture atlas.
        CreateTextureFontAtlas(Context);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void ImGuiRenderer::Submit(Ref<Graphic::Encoder> Encoder, ConstRef<ImDrawData> Commands)
    {
        auto VtxTransientPtr = mGraphics->Allocate<ImDrawVert>(Graphic::Usage::Vertex, Commands.TotalVtxCount);
        auto IdxTransientPtr = mGraphics->Allocate<ImDrawIdx>(Graphic::Usage::Index, Commands.TotalIdxCount);

        const Matrix4f Projection = Matrix4f::CreateOrthographic(
                Commands.DisplayPos.x,
                Commands.DisplayPos.x + Commands.DisplaySize.x,
                Commands.DisplayPos.y + Commands.DisplaySize.y,
                Commands.DisplayPos.y,
                -1.0f,
                +1.0f);
        const Graphic::Binding UfmTransient = mGraphics->Allocate<Matrix4f>(Graphic::Usage::Uniform, CastSpan(Projection));

        UInt32 VtxOffset = 0;
        UInt32 IdxOffset = 0;

        for (ConstPtr<ImDrawList> CommandList : Commands.CmdLists)
        {
            std::memcpy(VtxTransientPtr.Pointer, CommandList->VtxBuffer.Data, CommandList->VtxBuffer.Size * sizeof(ImDrawVert));
            std::memcpy(IdxTransientPtr.Pointer, CommandList->IdxBuffer.Data, CommandList->IdxBuffer.Size * sizeof(ImDrawIdx));
            VtxTransientPtr.Pointer += CommandList->VtxBuffer.Size;
            IdxTransientPtr.Pointer += CommandList->IdxBuffer.Size;

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

                    Encoder.SetVertices(0, VtxTransientPtr.Binding);
                    Encoder.SetIndices(IdxTransientPtr.Binding);
                    Encoder.SetUniforms(0, UfmTransient);
                    Encoder.SetPipeline(* mPipeline);
                    Encoder.SetTexture(0, Command.TextureId);
                    Encoder.SetSampler(0, Graphic::Sampler(Graphic::TextureEdge::Clamp, Graphic::TextureEdge::Clamp, Graphic::TextureFilter::Trilinear));
                    Encoder.Draw(Command.ElemCount, Command.VtxOffset + VtxOffset, Command.IdxOffset + IdxOffset);
                }
            }

            VtxOffset += CommandList->VtxBuffer.Size;
            IdxOffset += CommandList->IdxBuffer.Size;
        }
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