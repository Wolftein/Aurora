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

#include "Renderer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
        : mGraphics { Context.GetSubsystem<Graphic::Service>() }
    {
        // Initialize heap
        mHeap = NewUniquePtr<Heap>(mGraphics);

        // Initialize default resources
        CreateDefaultResources(Context.GetSubsystem<Content::Service>());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawLine(ConstRef<Vector2f> Origin, ConstRef<Vector2f> Target, Real32 Depth, Color Tint, float Thickness)
    {
        // Calculate the direction vector of the line
        const Real32 DirX = Target.GetX() - Origin.GetX();
        const Real32 DirY = Target.GetY() - Origin.GetY();

        const Real32 InvLength = InvSqrt(DirX * DirX + DirY * DirY);
        const Real32 UnitX     = DirX * InvLength;
        const Real32 UnitY     = DirY * InvLength;

        // Calculate the perpendicular vector (normal) to the line and apply thickness
        const Real32 HalfThickness = Thickness * 0.5f;
        const Real32 NormalX       = -UnitY * HalfThickness;
        const Real32 NormalY       =  UnitX * HalfThickness;

        Ref<Command> Command = Create(Type::Primitive, GetUniqueKey(Material::Kind::Opaque, Type::Primitive, Depth, 0));
        Command.Material = nullptr;
        Command.Tint     = Tint;
        Command.Edges[0] = { Origin.GetX() + NormalX, Origin.GetY() + NormalY, Depth };
        Command.Edges[1] = { Target.GetX() + NormalX, Target.GetY() + NormalY, Depth };
        Command.Edges[2] = { Target.GetX() - NormalX, Target.GetY() - NormalY, Depth };
        Command.Edges[3] = { Origin.GetX() - NormalX, Origin.GetY() - NormalY, Depth };
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawRect(ConstRef<Rectf> Origin, Real32 Depth, Color Tint, float Thickness)
    {
        const Vector2f EdgeLeftBottom(Origin.GetLeft(), Origin.GetBottom());
        const Vector2f EdgeLeftTop(Origin.GetLeft(), Origin.GetTop());
        const Vector2f EdgeRightTop(Origin.GetRight(), Origin.GetTop());
        const Vector2f EdgeRightBottom(Origin.GetRight(), Origin.GetBottom());

        DrawLine(EdgeLeftBottom, EdgeLeftTop, Depth, Tint, Thickness);
        DrawLine(EdgeLeftTop, EdgeRightTop, Depth, Tint, Thickness);
        DrawLine(EdgeRightTop, EdgeRightBottom, Depth, Tint, Thickness);
        DrawLine(EdgeRightBottom, EdgeLeftBottom, Depth, Tint, Thickness);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawQuad(ConstRef<Rectf> Origin, Real32 Depth, Color Tint)
    {
        Ref<Command> Command  = Create(Type::Primitive, GetUniqueKey(Material::Kind::Opaque, Type::Primitive, Depth, 0));
        Command.Material = nullptr;
        Command.Tint     = Tint;
        Command.Edges[0] = { Origin.GetLeft(),  Origin.GetBottom(), Depth };
        Command.Edges[1] = { Origin.GetRight(), Origin.GetBottom(), Depth };
        Command.Edges[2] = { Origin.GetRight(), Origin.GetTop(),    Depth };
        Command.Edges[3] = { Origin.GetLeft(),  Origin.GetTop(),    Depth };
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawSprite(ConstRef<Rectf> Origin, Real32 Depth, ConstRef<Rectf> UV, Color Tint, ConstSPtr<Material> Material)
    {
        Ref<Command> Command = Create(Type::Sprite, GetUniqueKey(Material->GetKind(), Type::Sprite, Depth, Material->GetID()));
        Command.Material = Material.get();
        Command.Tint     = Tint;
        Command.Edges[0] = { Origin.GetLeft(),  Origin.GetBottom(), Depth };
        Command.Edges[1] = { Origin.GetRight(), Origin.GetBottom(), Depth };
        Command.Edges[2] = { Origin.GetRight(), Origin.GetTop(),    Depth };
        Command.Edges[3] = { Origin.GetLeft(),  Origin.GetTop(),    Depth };
        Command.UV       = UV;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawSprite(ConstRef<Matrix4f> Transform, ConstRef<Rectf> Origin, Real32 Depth, ConstRef<Rectf> UV, Color Tint, ConstSPtr<Material> Material)
    {
        Ref<Command> Command = Create(Type::Sprite, GetUniqueKey(Material->GetKind(), Type::Sprite, Depth, Material->GetID()));
        Command.Material = Material.get();
        Command.Tint     = Tint;
        Command.Edges[0] = Transform * Vector3f(Origin.GetLeft(),  Origin.GetBottom(), 0.0f);
        Command.Edges[1] = Transform * Vector3f(Origin.GetRight(), Origin.GetBottom(), 0.0f);
        Command.Edges[2] = Transform * Vector3f(Origin.GetRight(), Origin.GetTop(),    0.0f);
        Command.Edges[3] = Transform * Vector3f(Origin.GetLeft(),  Origin.GetTop(),    0.0f);
        Command.UV       = UV;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawFont(ConstRef<Rectf> Origin, Real32 Depth, CStr16 Text, UInt16 Size, Color Tint, ConstSPtr<Font> Font)
    {
        const UInt32 ID = Font->GetMaterial()->GetID();

        // TODO: Automatically wrapping
        Real32 CurrentX = Origin.GetX() + Origin.GetX();
        Real32 CurrentY = Origin.GetY() + Origin.GetY();

        for (Char16 Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char16 Codepoint = Text[Symbol];

            switch (Codepoint)
            {
            case '\r':
                CurrentX = Origin.GetX();
                break;
            case '\n':
                CurrentY += Font->GetMetrics().UnderlineHeight * Size;
                break;
            default:
                const ConstPtr<Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                if (Glyph->PlaneBounds.GetWidth() > 0 && Glyph->PlaneBounds.GetHeight() > 0)
                {
                    const Rectf Boundaries = (Glyph->PlaneBounds * Size) + Vector2f(CurrentX, CurrentY);

                    // TODO: Create a command for all font, not each letter
                    // TODO: Calculate Transformation over the origin, not individual
                    Ref<Command> Command = Create(Type::Font, GetUniqueKey(Font->GetMaterial()->GetKind(), Type::Font, Depth, ID));
                    Command.Material = Font->GetMaterial().get();
                    Command.Tint     = Tint;
                    Command.Edges[0] = Vector3f(Boundaries.GetLeft(),  Boundaries.GetBottom(), Depth);
                    Command.Edges[1] = Vector3f(Boundaries.GetRight(), Boundaries.GetBottom(), Depth);
                    Command.Edges[2] = Vector3f(Boundaries.GetRight(), Boundaries.GetTop(),    Depth);
                    Command.Edges[3] = Vector3f(Boundaries.GetLeft(),  Boundaries.GetTop(),    Depth);
                    Command.UV       = Glyph->ImageBounds;
                }
                CurrentX += (Font->GetKerning(Previous, Codepoint) + Glyph->Advance) * Size;
                break;
            }
            Previous = Codepoint;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawFont(ConstRef<Matrix4f> Transform, ConstRef<Rectf> Origin, Real32 Depth, CStr16 Text, UInt16 Size, Color Tint, ConstSPtr<Font> Font)
    {
        const UInt32 ID = Font->GetMaterial()->GetID();

        // TODO: Automatically wrapping
        Real32 CurrentX = Origin.GetX() + Origin.GetX();
        Real32 CurrentY = Origin.GetY() + Origin.GetY();

        for (Char16 Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char16 Codepoint = Text[Symbol];

            switch (Codepoint)
            {
            case '\r':
                CurrentX = Origin.GetX();
                break;
            case '\n':
                CurrentY += Font->GetMetrics().UnderlineHeight * Size;
                break;
            default:
                const ConstPtr<Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                if (Glyph->PlaneBounds.GetWidth() > 0 && Glyph->PlaneBounds.GetHeight() > 0)
                {
                    const Rectf Boundaries = (Glyph->PlaneBounds * Size) + Vector2f(CurrentX, CurrentY);

                    // TODO: Create a command for all font, not each letter
                    // TODO: Calculate Transformation over the origin, not individual
                    Ref<Command> Command = Create(Type::Font, GetUniqueKey(Font->GetMaterial()->GetKind(), Type::Font, Depth, ID));
                    Command.Material = Font->GetMaterial().get();
                    Command.Tint     = Tint;
                    Command.Edges[0] = Transform * Vector3f(Boundaries.GetLeft(),  Boundaries.GetBottom(), Depth);
                    Command.Edges[1] = Transform * Vector3f(Boundaries.GetRight(), Boundaries.GetBottom(), Depth);
                    Command.Edges[2] = Transform * Vector3f(Boundaries.GetRight(), Boundaries.GetTop(),    Depth);
                    Command.Edges[3] = Transform * Vector3f(Boundaries.GetLeft(),  Boundaries.GetTop(),    Depth);
                    Command.UV       = Glyph->ImageBounds;
                }
                CurrentX += (Font->GetKerning(Previous, Codepoint) + Glyph->Advance) * Size;
                break;
            }
            Previous = Codepoint;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush(Bool Copy)
    {
        if (mCommandsPtr.empty())
        {
            return;
        }

        // Sort all command(s) back to front and by material/pipeline
        Sort(mCommandsPtr, [](ConstPtr<Command> Left, ConstPtr<Command> Right)
        {
            return Left->Key > Right->Key;
        });

        // Render all command(s) in group of batches
        for (UInt32 Element = 0, Start = 0, Max = mCommandsPtr.size() - 1; Element <= Max; ++Element)
        {
            const ConstPtr<Command> Current = mCommandsPtr[Element];
            const ConstPtr<Command> Other   = (Element == Max ? Current : mCommandsPtr[Element + 1]);

            // Break batch if material and pipeline differ or we are handling the last element
            if (Current->Pipeline != Other->Pipeline || Current->Material != Other->Material || Current == Other)
            {
                const UInt32 Count = Element - Start + 1;

                // Ensure we have enough space for the batch otherwise flush previous batches
                switch (Current->Kind)
                {
                case Type::Primitive:
                    WritePrimitives(Start, Count, * Current->Pipeline);
                    break;
                case Type::Font:        // TODO Efficient hot path
                case Type::Sprite:
                    WriteSprites(Start, Count, * Current->Pipeline, * Current->Material);
                    break;
                }

                // Continue with the next batch
                Start = Element + 1;
            }
        }

        // Flush all data to the GPU
        mHeap->Commit(Copy);

        // Flush all batches to the GPU
        mGraphics->Submit(mEncoder.GetSubmissions());
        mEncoder.Clear();

        // Reset all stack(s) back to original states
        mCommandsPtr.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::WritePrimitives(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline)
    {
        const Bool FitsVertices = mHeap->Fits<Layout>(Usage::Vertex, Count * 4);
        const Bool FitsIndices  = mHeap->Fits<UInt16>(Usage::Index,  Count * 6);

        if (!FitsVertices || !FitsIndices)
        {
            // Flush all data to the GPU
            mHeap->Commit(true);

            // Flush all batches to the GPU
            mGraphics->Submit(mEncoder.GetSubmissions());
            mEncoder.Clear();

            // Reallocate scopes
            mHeap->Reallocate(mParameters[CastEnum(Scope::Global)]);
            mHeap->Reallocate(mParameters[CastEnum(Scope::Effect)]);
        }

        auto [VtxPointer, VtxDescription] = mHeap->Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription] = mHeap->Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            Ptr<Command> CommandPtr = mCommandsPtr[Element];
            VtxPointer[0].Position = CommandPtr->Edges[0];
            VtxPointer[0].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[1].Position = CommandPtr->Edges[1];
            VtxPointer[1].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[2].Position = CommandPtr->Edges[2];
            VtxPointer[2].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[3].Position = CommandPtr->Edges[3];
            VtxPointer[3].Color    = CommandPtr->Tint.GetValue();

            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniforms(CastEnum(Scope::Global), mParameters[CastEnum(Scope::Global)]);
        mEncoder.SetUniforms(CastEnum(Scope::Effect), mParameters[CastEnum(Scope::Effect)]);
        mEncoder.SetVertices(0, Binding(VtxDescription.Buffer, VtxDescription.Stride, 0));
        mEncoder.SetIndices(Binding(IdxDescription.Buffer, IdxDescription.Stride, 0));
        mEncoder.SetPipeline(Pipeline);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::WriteSprites(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material)
    {
        const Bool FitsVertices = mHeap->Fits<Layout>(Usage::Vertex, Count * 4);
        const Bool FitsIndices  = mHeap->Fits<UInt16>(Usage::Index,  Count * 6);
        const Bool FitsUniforms = mHeap->Fits<UInt8>(Usage::Uniform, Material.GetParameters().size_bytes());

        if (!FitsVertices || !FitsIndices || !FitsUniforms)
        {
            // Flush all data to the GPU
            mHeap->Commit(true);

            // Flush all batches to the GPU
            mGraphics->Submit(mEncoder.GetSubmissions());
            mEncoder.Clear();

            // Reallocate scopes
            mHeap->Reallocate(mParameters[CastEnum(Scope::Global)]);
            mHeap->Reallocate(mParameters[CastEnum(Scope::Effect)]);
        }

        auto [VtxPointer, VtxDescription] = mHeap->Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription] = mHeap->Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            Ptr<Command> CommandPtr = mCommandsPtr[Element];
            VtxPointer[0].Position = CommandPtr->Edges[0];
            VtxPointer[0].Texture.Set(CommandPtr->UV.GetLeft(), CommandPtr->UV.GetBottom());
            VtxPointer[0].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[1].Position = CommandPtr->Edges[1];
            VtxPointer[1].Texture.Set(CommandPtr->UV.GetRight(), CommandPtr->UV.GetBottom());
            VtxPointer[1].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[2].Position = CommandPtr->Edges[2];
            VtxPointer[2].Texture.Set(CommandPtr->UV.GetRight(), CommandPtr->UV.GetTop());
            VtxPointer[2].Color    = CommandPtr->Tint.GetValue();
            VtxPointer[3].Position = CommandPtr->Edges[3];
            VtxPointer[3].Texture.Set(CommandPtr->UV.GetLeft(),  CommandPtr->UV.GetTop());
            VtxPointer[3].Color    = CommandPtr->Tint.GetValue();

            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniforms(CastEnum(Scope::Global), mParameters[CastEnum(Scope::Global)]);
        mEncoder.SetUniforms(CastEnum(Scope::Effect), mParameters[CastEnum(Scope::Effect)]);

        if (CPtr<const UInt8> Parameters = Material.GetParameters(); !Parameters.empty())
        {
            const auto Binding = mHeap->Allocate(Usage::Uniform, Parameters);
            mEncoder.SetUniforms(CastEnum(Scope::Style), Binding);
        }

        mEncoder.SetVertices(0, Binding(VtxDescription.Buffer, VtxDescription.Stride, 0));
        mEncoder.SetIndices(Binding(IdxDescription.Buffer, IdxDescription.Stride, 0));
        mEncoder.SetPipeline(Pipeline);
        mEncoder.Bind(Pipeline, Material);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::CreateDefaultResources(ConstSPtr<Content::Service> Content)
    {
        // Initialize the default pipelines
        mPipelines[CastEnum(Type::Primitive)] = Content->Load<Pipeline>("Engine://Pipeline/Primitive.effect");
        mPipelines[CastEnum(Type::Font)]      = Content->Load<Pipeline>("Engine://Pipeline/MSDF.effect");

        // Initialize the default material
        SPtr<Material> Material = Material::GetFactory().GetOrCreate(k_DefaultMaterialUri, true);
        Material->SetSampler(TextureSlot::Diffuse, Sampler(TextureEdge::Repeat, TextureEdge::Repeat, TextureFilter::Bilinear));
        Material->SetTexture(TextureSlot::Diffuse, Content->Load<Texture>("Engine://Texture/Unknown.png"));
        Content->Process(Material, true);
    }
}