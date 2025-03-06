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
#include "Aurora.Content/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
    {
        CreateDefaultResources(Context);

        mService = Context.GetSubsystem<Graphic::Service>();
        mAllocator.Initialize(mService);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::~Renderer()
    {
        mAllocator.Dispose();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(ConstRef<Matrix4f> Transformation, ConstRef<Rectf> Origin, ConstRef<Rectf> Source, Color Tint, ConstRef<Pivot> Pivot, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
    {
        PushDrawable(Transformation, Rectf::Transform(Origin, Pivot), Source, Tint, Order, Pipeline, Material);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(ConstRef<Matrix4f> Transformation, CStr16 Text, UInt16 Size, Color Tint, ConstRef<Pivot> Pivot, Order Order, ConstSPtr<Font> Font)
    {
        const Rectf Boundaries = Font->Calculate(Text, Size, Pivot);
        Real32 CurrentX = Boundaries.GetX();
        Real32 CurrentY = Boundaries.GetY();

        for (Char16 Previous = 0, Symbol = 0; Symbol < Text.size(); ++Symbol)
        {
            const Char16 Codepoint = Text[Symbol];

            switch (Codepoint)
            {
            case '\r':
                CurrentX = Boundaries.GetX();
                break;
            case '\n':
                CurrentY += Font->GetMetrics().UnderlineHeight * Size;
                break;
            default:
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Codepoint);

                if (Glyph->PlaneBounds.GetWidth() > 0 && Glyph->PlaneBounds.GetHeight() > 0)
                {
                    const Rectf Destination = (Glyph->PlaneBounds * Size) + Vector2f(CurrentX, CurrentY);
                    PushDrawable(Transformation, Destination, Glyph->ImageBounds, Tint, Order, mPipelines[0], Font->GetMaterial());
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
        if (mDrawablesPtr.empty())
        {
            return;
        }

        // Sort all drawables back to front and by material/pipeline
        Sort(mDrawablesPtr, [](ConstPtr<Drawable> Left, ConstPtr<Drawable> Right)
        {
            return Left->ID > Right->ID;
        });

        // Render all drawables in batches
        for (UInt32 Element = 0, Start = 0, Max = mDrawablesPtr.size() - 1; Element <= Max; ++Element)
        {
            const ConstPtr<Drawable> Current = mDrawablesPtr[Element];
            const ConstPtr<Drawable> Other   = (Element == Max ? Current : mDrawablesPtr[Element + 1]);

            // Break batch if material and pipeline differ or we are handling the last element
            if (Current->Material != Other->Material || Current->Pipeline != Other->Pipeline || Current == Other)
            {
                const UInt32 Count = Element - Start + 1;

                // Ensure we have enough space for the batch otherwise flush previous batches
                PushBatch(Start, Count, * Current);

                // Continue with the next batch
                Start = Element + 1;
            }
        }

        // Flush all data to the GPU
        mAllocator.Commit(Copy);

        // Flush all batches to the GPU
        mService->Submit(mEncoder, true);
        mEncoder.Clear();

        // Reset all stack(s) back to original states
        mDrawablesPtr.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::CreateDefaultResources(Ref<Core::Subsystem::Context> Context)
    {
        ConstSPtr<Content::Service> Content = Context.GetSubsystem<Content::Service>();
        mPipelines[0] = Content->Load<Graphic::Pipeline>("Engine://Pipeline/MSDF.effect");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::PushBatch(UInt32 Offset, UInt32 Count, ConstRef<Drawable> Drawable)
    {
        const Bool FitsVertices = mAllocator.Reserve<Layout>(Usage::Vertex, Count * 4);
        const Bool FitsIndices  = mAllocator.Reserve<UInt16>(Usage::Index,  Count * 6);
        const Bool FitsUniforms = mAllocator.Reserve<UInt8>(Usage::Uniform, Drawable.Material->GetParameters().size_bytes());

        if (!FitsVertices || !FitsIndices || !FitsUniforms)
        {
            // Flush all data to the GPU
            mAllocator.Commit(true);

            // Flush all batches to the GPU
            mService->Submit(mEncoder, true);
            mEncoder.Clear();

            if (!FitsUniforms)
            {
                mAllocator.Reallocate(mBlocks[CastEnum(Block::Scene)]);
                mAllocator.Reallocate(mBlocks[CastEnum(Block::Technique)]);
            }
        }

        auto [VtxPointer, VtxDescription] = mAllocator.Allocate<Layout>(Usage::Vertex, 4 * Count);
        auto [IdxPointer, IdxDescription]   = mAllocator.Allocate<UInt16>(Usage::Index, 6 * Count);

        for (UInt32 Index = 0, Element = Offset; Element < Offset + Count; ++Element, Index += 4, VtxPointer += 4, IdxPointer += 6)
        {
            PushGeometry(mDrawablesPtr[Element], VtxPointer);
            IdxPointer[0] = Index;
            IdxPointer[1] = Index + 1;
            IdxPointer[2] = Index + 2;
            IdxPointer[3] = Index;
            IdxPointer[4] = Index + 2;
            IdxPointer[5] = Index + 3;
        }

        mEncoder.SetUniforms(CastEnum(Block::Scene), mBlocks[CastEnum(Block::Scene)]);
        mEncoder.SetUniforms(CastEnum(Block::Technique), mBlocks[CastEnum(Block::Technique)]);

        if (CPtr<const UInt8> Parameters = Drawable.Material->GetParameters(); !Parameters.empty())
        {
            const auto Binding = mAllocator.Allocate(Usage::Uniform, Parameters);
            mEncoder.SetUniforms(CastEnum(Block::Material), Binding);
        }

        // @TODO Instance Uniform

        mEncoder.SetVertices(0, Binding(VtxDescription.Buffer, VtxDescription.Stride, 0));
        mEncoder.SetIndices(Binding(IdxDescription.Buffer, IdxDescription.Stride, 0));
        mEncoder.SetPipeline(* Drawable.Pipeline);
        mEncoder.Bind(* Drawable.Pipeline, * Drawable.Material);
        mEncoder.Draw(6 * Count, VtxDescription.Offset / sizeof(Layout), IdxDescription.Offset / sizeof(UInt16));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::PushDrawable(ConstRef<Matrix4f> Transformation, ConstRef<Rectf> Destination, ConstRef<Rectf> Source,
        Color Tint, Order Order, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
    {
        if (mDrawablesPtr.size() == k_MaxDrawables)
        {
            Flush(true);
        }

        Ref<Drawable> Drawable  = mDrawables[mDrawablesPtr.size()];
        Drawable.Coordinates[0] = Transformation * Vector3f(Destination.GetLeft(),  Destination.GetBottom(), 0.0f);
        Drawable.Coordinates[1] = Transformation * Vector3f(Destination.GetRight(), Destination.GetBottom(), 0.0f);
        Drawable.Coordinates[2] = Transformation * Vector3f(Destination.GetRight(), Destination.GetTop(),    0.0f);
        Drawable.Coordinates[3] = Transformation * Vector3f(Destination.GetLeft(),  Destination.GetTop(),    0.0f);
        Drawable.Source         = Source;
        Drawable.Tint           = Tint;
        Drawable.Pipeline       = Pipeline.get();
        Drawable.Material       = Material.get();
        Drawable.ID             = GenerateUniqueId(Order, Pipeline->GetID(), Material->GetID(), Drawable.Coordinates[0].GetZ());
        mDrawablesPtr.push_back(AddressOf(Drawable));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::PushGeometry(ConstPtr<Drawable> Drawable, Ptr<Layout> Buffer)
    {
        Buffer[0].Position = Drawable->Coordinates[0];
        Buffer[0].Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetBottom());
        Buffer[0].Color = Drawable->Tint.GetValue();

        Buffer[1].Position = Drawable->Coordinates[1];
        Buffer[1].Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetBottom());
        Buffer[1].Color = Drawable->Tint.GetValue();

        Buffer[2].Position = Drawable->Coordinates[2];
        Buffer[2].Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetTop());
        Buffer[2].Color = Drawable->Tint.GetValue();

        Buffer[3].Position = Drawable->Coordinates[3];
        Buffer[3].Texture.Set(Drawable->Source.GetLeft(),  Drawable->Source.GetTop());
        Buffer[3].Color = Drawable->Tint.GetValue();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt64 Renderer::GenerateUniqueId(Order Order, Object Pipeline, Object Material, Real32 Depth) const
    {
        if (Order == Order::Opaque)
        {
            const UInt32 DepthBits = std::bit_cast<UInt32>(1.0f + Depth);

            return  (static_cast<UInt64>(DepthBits)                << 32)
                |   (static_cast<UInt64>(Material        & 0xFFFF) << 16)
                |   (static_cast<UInt64>(Pipeline        & 0xFF)   << 8)
                |   (static_cast<UInt64>(CastEnum(Order) & 0xFF));
        }
        else
        {
            const UInt32 DepthBits = std::bit_cast<UInt32>(Depth);

            return  (static_cast<UInt64>(Material        & 0xFFFF) << 48)
                |   (static_cast<UInt64>(Pipeline        & 0xFF)   << 40)
                |   (static_cast<UInt64>(DepthBits)                << 8)
                |   (static_cast<UInt64>(CastEnum(Order) & 0xFF));
        }
    }
}