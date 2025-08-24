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

#include "Render2D.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Render2D::Render2D(Ref<Service::Host> Host)
        : mGraphics { Host.GetService<Graphic::Service>() }
    {
        CreateDefaultResources(Host.GetService<Content::Service>());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::SetFontStyle(ConstRef<FontStyleSDF> Style)
    {
        const UInt64 HashID = Style.Hash();

        if (const auto Iterator = mFontStylesToUniform.find(HashID); Iterator != mFontStylesToUniform.end())
        {
            mFontStylesSelected = Iterator->second;
        }
        else
        {
            mFontStyles.push_back(Style);
            mFontStylesSelected = mFontStyles.size() - 1;
            mFontStylesToUniform.try_emplace(HashID, mFontStylesSelected);
        }

        if (mFontStyles.size() == mFontStyles.max_size())
        {
            Flush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawLine(ConstRef<Line> Origin, Real32 Depth, UInt32 Tint, float Thickness)
    {
        ConstRef<Vector2> Start = Origin.GetStart();
        ConstRef<Vector2> End   = Origin.GetEnd();
        const Vector2 Normal    = Origin.GetNormal() * (Thickness * 0.5f);

        Ref<Command> Command = Create(Type::Primitive, GetUniqueKey(Material::Kind::Opaque, Type::Primitive, Depth, 0));
        Command.Material = nullptr;
        Command.Tint     = Tint;
        Command.Edges[0] = { Start.GetX() + Normal.GetX(), Start.GetY() + Normal.GetY(), Depth };
        Command.Edges[1] = {   End.GetX() + Normal.GetX(),   End.GetY() + Normal.GetY(), Depth };
        Command.Edges[2] = {   End.GetX() - Normal.GetX(),   End.GetY() - Normal.GetY(), Depth };
        Command.Edges[3] = { Start.GetX() - Normal.GetX(), Start.GetY() - Normal.GetY(), Depth };
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawRect(ConstRef<Rect> Origin, Real32 Depth, UInt32 Tint, float Thickness)
    {
        const Vector2 EdgeLeftBottom(Origin.GetLeft(), Origin.GetBottom());
        const Vector2 EdgeLeftTop(Origin.GetLeft(), Origin.GetTop());
        const Vector2 EdgeRightTop(Origin.GetRight(), Origin.GetTop());
        const Vector2 EdgeRightBottom(Origin.GetRight(), Origin.GetBottom());

        DrawLine(Line(EdgeLeftBottom, EdgeLeftTop), Depth, Tint, Thickness);
        DrawLine(Line(EdgeLeftTop, EdgeRightTop), Depth, Tint, Thickness);
        DrawLine(Line(EdgeRightTop, EdgeRightBottom), Depth, Tint, Thickness);
        DrawLine(Line(EdgeRightBottom, EdgeLeftBottom), Depth, Tint, Thickness);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawRectFilled(ConstRef<Rect> Origin, Real32 Depth, UInt32 Tint)
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

    void Render2D::DrawSprite(ConstRef<Rect> Origin, Real32 Depth, ConstRef<Rect> Uv, UInt32 Tint, ConstTracker<Material> Material)
    {
        Ref<Command> Command = Create(Type::Sprite, GetUniqueKey(Material->GetKind(), Type::Sprite, Depth, Material->GetID()));
        Command.Material = Material;
        Command.Tint     = Tint;
        Command.Edges[0] = { Origin.GetLeft(),  Origin.GetBottom(), Depth };
        Command.Edges[1] = { Origin.GetRight(), Origin.GetBottom(), Depth };
        Command.Edges[2] = { Origin.GetRight(), Origin.GetTop(),    Depth };
        Command.Edges[3] = { Origin.GetLeft(),  Origin.GetTop(),    Depth };
        Command.Uv       = Uv;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawSprite(ConstRef<Matrix4x4> Transform, ConstRef<Rect> Origin, Real32 Depth, ConstRef<Rect> Uv, UInt32 Tint, ConstTracker<Material> Material)
    {
        Ref<Command> Command = Create(Type::Sprite, GetUniqueKey(Material->GetKind(), Type::Sprite, Depth, Material->GetID()));
        Command.Material = Material;
        Command.Tint     = Tint;
        Command.Edges[0] = Matrix4x4::Project(Transform, Vector3(Origin.GetLeft(),  Origin.GetBottom(), Depth));
        Command.Edges[1] = Matrix4x4::Project(Transform, Vector3(Origin.GetRight(), Origin.GetBottom(), Depth));
        Command.Edges[2] = Matrix4x4::Project(Transform, Vector3(Origin.GetRight(), Origin.GetTop(),    Depth));
        Command.Edges[3] = Matrix4x4::Project(Transform, Vector3(Origin.GetLeft(),  Origin.GetTop(),    Depth));
        Command.Uv       = Uv;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawFont(ConstRef<Rect> Origin, Real32 Depth, ConstText Text, Real32 Size, UInt32 Tint, ConstTracker<Font> Font)
    {
        if (!Font->HasCompleted())   // TODO: Prevent checking this per frame.
        {
            return;
        }

        const UInt32 ID = Font->GetMaterial()->GetID();

        // TODO: Automatically wrapping
        Real32 CurrentX = Origin.GetX();
        Real32 CurrentY = Origin.GetY();

        for (Char Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char Codepoint = Text[Symbol];

            switch (Codepoint)
            {
                case '\r':
                    CurrentX = Origin.GetX();
                    break;
                case '\n':
                    CurrentY += Font->GetMetrics().UnderlineSize * Size;
                    break;
                default:
                    const ConstPtr<Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                    if (Glyph->LocalBounds.GetWidth() > 0 && Glyph->LocalBounds.GetHeight() > 0)
                    {
                        Rect Boundaries = (Glyph->LocalBounds * Size) + Vector2(CurrentX, CurrentY);

                        // TODO: Create a command for all font, not each letter
                        // TODO: Calculate Transformation over the origin, not individual
                        Ref<Command> Command = Create(Type::Font, GetUniqueKey(Font->GetMaterial()->GetKind(), Type::Font, Depth, ID));
                        Command.Material = Font->GetMaterial();
                        Command.Tint     = Tint;
                        Command.Edges[0] = Vector3(Boundaries.GetLeft(),  Boundaries.GetBottom(), Depth);
                        Command.Edges[1] = Vector3(Boundaries.GetRight(), Boundaries.GetBottom(), Depth);
                        Command.Edges[2] = Vector3(Boundaries.GetRight(), Boundaries.GetTop(),    Depth);
                        Command.Edges[3] = Vector3(Boundaries.GetLeft(),  Boundaries.GetTop(),    Depth);
                        Command.Uv       = Glyph->AtlasBounds;
                        Command.Instance = mFontStylesSelected;
                    }
                    CurrentX += (Font->GetKerning(Previous, Codepoint) + Glyph->Advance) * Size;
                    break;
            }
            Previous = Codepoint;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::DrawFont(ConstRef<Matrix4x4> Transform, ConstRef<Rect> Origin, Real32 Depth, ConstText Text, Real32 Size, UInt32 Tint, ConstTracker<Font> Font)
    {
        if (!Font->HasCompleted())   // TODO: Prevent checking this per frame.
        {
            return;
        }

        const UInt32 ID = Font->GetMaterial()->GetID();

        // TODO: Automatically wrapping
        Real32 CurrentX = Origin.GetX();
        Real32 CurrentY = Origin.GetY();

        for (Char Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char Codepoint = Text[Symbol];

            switch (Codepoint)
            {
                case '\r':
                    CurrentX = Origin.GetX();
                    break;
                case '\n':
                    CurrentY += Font->GetMetrics().UnderlineSize * Size;
                    break;
                default:
                    const ConstPtr<Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                    if (Glyph->LocalBounds.GetWidth() > 0 && Glyph->LocalBounds.GetHeight() > 0)
                    {
                        const Rect Boundaries = (Glyph->LocalBounds * Size) + Vector2(CurrentX, CurrentY);

                        // TODO: Create a command for all font, not each letter
                        // TODO: Calculate Transformation over the origin, not individual
                        Ref<Command> Command = Create(Type::Font, GetUniqueKey(Font->GetMaterial()->GetKind(), Type::Font, Depth, ID));
                        Command.Material = Font->GetMaterial();
                        Command.Tint     = Tint;
                        Command.Edges[0] = Matrix4x4::Project(Transform, Vector3(Boundaries.GetLeft(),  Boundaries.GetBottom(), Depth));
                        Command.Edges[1] = Matrix4x4::Project(Transform, Vector3(Boundaries.GetRight(), Boundaries.GetBottom(), Depth));
                        Command.Edges[2] = Matrix4x4::Project(Transform, Vector3(Boundaries.GetRight(), Boundaries.GetTop(),    Depth));
                        Command.Edges[3] = Matrix4x4::Project(Transform, Vector3(Boundaries.GetLeft(),  Boundaries.GetTop(),    Depth));
                        Command.Uv       = Glyph->AtlasBounds;
                        Command.Instance = mFontStylesSelected;
                    }
                    CurrentX += (Font->GetKerning(Previous, Codepoint) + Glyph->Advance) * Size;
                    break;
            }
            Previous = Codepoint;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::Flush()
    {
        if (mCommandTracker.empty())
        {
            return;
        }

        // Sort all command(s) back to front and by material/pipeline
        std::sort(mCommandTracker.begin(), mCommandTracker.end(), [](ConstPtr<Command> Left, ConstPtr<Command> Right)
        {
            return Left->Key > Right->Key;
        });

        PushFontData();

        // Render all command(s) in group of batches
        for (UInt32 Element = 0, Start = 0, Max = mCommandTracker.size() - 1; Element <= Max; ++Element)
        {
            const ConstPtr<Command> Current = mCommandTracker[Element];
            const ConstPtr<Command> Other   = (Element == Max ? Current : mCommandTracker[Element + 1]);

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
                        WriteFonts(Start, Count, * Current->Pipeline, * Current->Material);
                        break;
                    case Type::Sprite:
                        WriteSprites(Start, Count, * Current->Pipeline, * Current->Material);
                        break;
                }

                // Continue with the next batch
                Start = Element + 1;
            }
        }

        // Flush all batches to the GPU
        mGraphics->Submit(mEncoder);
        mEncoder.Clear();

        // Reset all stack(s) back to original states
        mCommandTracker.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::WritePrimitives(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline)
    {
        if (!Pipeline.HasCompleted())   // TODO: Prevent checking this per frame.
        {
            return;
        }

        auto [VtxPointer, VtxDescription] = mGraphics->Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription] = mGraphics->Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            Ptr<Command> CommandPtr = mCommandTracker[Element];
            VtxPointer[0].Position = CommandPtr->Edges[0];
            VtxPointer[0].Color    = CommandPtr->Tint;
            VtxPointer[1].Position = CommandPtr->Edges[1];
            VtxPointer[1].Color    = CommandPtr->Tint;
            VtxPointer[2].Position = CommandPtr->Edges[2];
            VtxPointer[2].Color    = CommandPtr->Tint;
            VtxPointer[3].Position = CommandPtr->Edges[3];
            VtxPointer[3].Color    = CommandPtr->Tint;

            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniform(Enum::Cast(Scope::Global), mParameters[Enum::Cast(Scope::Global)]);
        mEncoder.SetUniform(Enum::Cast(Scope::Effect), mParameters[Enum::Cast(Scope::Effect)]);

        mEncoder.SetVertices(0, VtxDescription.Buffer, 0, VtxDescription.Stride);
        mEncoder.SetIndices(IdxDescription.Buffer, 0, IdxDescription.Stride);
        mEncoder.SetPipeline(Pipeline);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::WriteFonts(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material)
    {
        if (!Pipeline.HasCompleted() || !Material.HasCompleted())   // TODO: Prevent checking this per frame.
        {
            return;
        }

        auto [VtxPointer, VtxDescription] = mGraphics->Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription] = mGraphics->Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            Ptr<Command> CommandPtr = mCommandTracker[Element];
            VtxPointer[0].Position = CommandPtr->Edges[0];
            VtxPointer[0].Texture.Set(CommandPtr->Uv.GetLeft(), CommandPtr->Uv.GetBottom());
            VtxPointer[0].Color    = CommandPtr->Tint;
            VtxPointer[0].Instance = CommandPtr->Instance;
            VtxPointer[1].Position = CommandPtr->Edges[1];
            VtxPointer[1].Texture.Set(CommandPtr->Uv.GetRight(), CommandPtr->Uv.GetBottom());
            VtxPointer[1].Color    = CommandPtr->Tint;
            VtxPointer[1].Instance = CommandPtr->Instance;
            VtxPointer[2].Position = CommandPtr->Edges[2];
            VtxPointer[2].Texture.Set(CommandPtr->Uv.GetRight(), CommandPtr->Uv.GetTop());
            VtxPointer[2].Color    = CommandPtr->Tint;
            VtxPointer[2].Instance = CommandPtr->Instance;
            VtxPointer[3].Position = CommandPtr->Edges[3];
            VtxPointer[3].Texture.Set(CommandPtr->Uv.GetLeft(),  CommandPtr->Uv.GetTop());
            VtxPointer[3].Color    = CommandPtr->Tint;
            VtxPointer[3].Instance = CommandPtr->Instance;

            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniform(Enum::Cast(Scope::Global), mParameters[Enum::Cast(Scope::Global)]);
        mEncoder.SetUniform(Enum::Cast(Scope::Effect), mParameters[Enum::Cast(Scope::Effect)]);

        if (Span<const UInt8> Parameters = Material.GetParameters(); !Parameters.empty())
        {
            const auto Binding = mGraphics->Allocate(Usage::Uniform, Parameters);
            mEncoder.SetUniform(Enum::Cast(Scope::Style), Binding);
        }

        if (mFontStream.Buffer > 0)
        {
            mEncoder.SetUniform(Enum::Cast(Scope::Object), mFontStream);
        }

        mEncoder.SetVertices(0, VtxDescription.Buffer, 0, VtxDescription.Stride);
        mEncoder.SetIndices(IdxDescription.Buffer, 0, IdxDescription.Stride);
        mEncoder.SetPipeline(Pipeline);
        mEncoder.Bind(Pipeline, Material);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::WriteSprites(UInt32 Offset, UInt32 Count, Ref<Pipeline> Pipeline, Ref<Material> Material)
    {
        if (!Pipeline.HasCompleted() || !Material.HasCompleted())   // TODO: Prevent checking this per frame.
        {
            return;
        }

        auto [VtxPointer, VtxDescription] = mGraphics->Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription] = mGraphics->Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            Ptr<Command> CommandPtr = mCommandTracker[Element];
            VtxPointer[0].Position = CommandPtr->Edges[0];
            VtxPointer[0].Texture.Set(CommandPtr->Uv.GetLeft(), CommandPtr->Uv.GetBottom());
            VtxPointer[0].Color    = CommandPtr->Tint;
            VtxPointer[0].Instance = CommandPtr->Instance;
            VtxPointer[1].Position = CommandPtr->Edges[1];
            VtxPointer[1].Texture.Set(CommandPtr->Uv.GetRight(), CommandPtr->Uv.GetBottom());
            VtxPointer[1].Color    = CommandPtr->Tint;
            VtxPointer[1].Instance = CommandPtr->Instance;
            VtxPointer[2].Position = CommandPtr->Edges[2];
            VtxPointer[2].Texture.Set(CommandPtr->Uv.GetRight(), CommandPtr->Uv.GetTop());
            VtxPointer[2].Color    = CommandPtr->Tint;
            VtxPointer[2].Instance = CommandPtr->Instance;
            VtxPointer[3].Position = CommandPtr->Edges[3];
            VtxPointer[3].Texture.Set(CommandPtr->Uv.GetLeft(),  CommandPtr->Uv.GetTop());
            VtxPointer[3].Color    = CommandPtr->Tint;
            VtxPointer[3].Instance = CommandPtr->Instance;

            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniform(Enum::Cast(Scope::Global), mParameters[Enum::Cast(Scope::Global)]);
        mEncoder.SetUniform(Enum::Cast(Scope::Effect), mParameters[Enum::Cast(Scope::Effect)]);

        if (Span<const UInt8> Parameters = Material.GetParameters(); !Parameters.empty())
        {
            const auto Binding = mGraphics->Allocate(Usage::Uniform, Parameters);
            mEncoder.SetUniform(Enum::Cast(Scope::Style), Binding);
        }

        //mEncoder.SetUniform(Enum::Cast(Scope::Object), mParameters[Enum::Cast(Scope::Object)]);

        mEncoder.SetVertices(0, VtxDescription.Buffer, 0, VtxDescription.Stride);
        mEncoder.SetIndices(IdxDescription.Buffer, 0, IdxDescription.Stride);
        mEncoder.SetPipeline(Pipeline);
        mEncoder.Bind(Pipeline, Material);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::PushFontData()
    {
        if (mFontStyles.empty())
        {
            mFontStream = Stream();
            return;
        }

        mFontStream = mGraphics->Allocate<FontStyleSDF>(Usage::Uniform, mFontStyles);
        mFontStylesToUniform.clear();
        mFontStyles.clear();
        mFontStylesSelected = 0;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Render2D::CreateDefaultResources(ConstTracker<Content::Service> Content)
    {
        // Initialize the default pipelines
        mPipelines[Enum::Cast(Type::Primitive)] = Content->Load<Pipeline>("Engine://Pipeline/Primitive.effect");
        mPipelines[Enum::Cast(Type::Font)]      = Content->Load<Pipeline>("Engine://Pipeline/MSDF.effect");
        mPipelines[Enum::Cast(Type::Sprite)]    = Content->Load<Pipeline>("Engine://Pipeline/Sprite.effect");
    }
}