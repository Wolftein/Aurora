// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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
        : mEncoder { Context.GetSubsystem<Graphic::Service>() }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Begin(Ref<const Camera> Camera)
    {
        mEncoder.SetScene<Matrix4f>(Camera.GetWorld());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Draw(
        Rectf Destination,
        Rectf Source,
        Real32 Depth,
        Real32 Angle,
        Color Tint,
        Order Order,
        ConstSPtr<Graphic::Pipeline> Pipeline,
        ConstSPtr<Graphic::Material> Material)
    {
        if (!Pipeline->HasLoaded() || !Material->HasLoaded())
        {
            return;
        }

        Ref<Drawable> Drawable = mDrawables.push_back();
        Drawable.ID          = GenerateUniqueID(Order, Pipeline->GetID(), Material->GetID(), Depth);
        Drawable.Destination = Destination;
        Drawable.Source      = Source;
        Drawable.Depth       = Depth;
        Drawable.Angle       = (Angle * 3.141592654f) / 180.0f;
        Drawable.Color       = Tint.AsPacked();
        Drawable.Pipeline    = Pipeline;
        Drawable.Material    = Material;
        mDrawablesPtr.push_back(& Drawable);

        if (mDrawables.full())
        {
            Flush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawFont(ConstSPtr<Font> Font, ConstSPtr<Graphic::Pipeline> Pipeline, CStr Text, Vector2f Position, Real32 Depth, Real32 Scale, Color Tint, Alignment Alignment)
    {
        if (!Font->HasLoaded())
        {
            return;
        }

        Ref<const Graphic::Font::Metrics> Metrics = Font->GetMetrics();

        // Align horizontally
        const UInt AlignmentType = static_cast<UInt>(Alignment);

        if (AlignmentType & (1 << 0))
        {
            // Empty
        }
        else
        {
            Real32 BoundariesX = 0;

            for (UInt Letter = 0; Letter < Text.size(); ++Letter)
            {
                const UInt Character = Text[Letter];
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                const Real32 Kerning = (Letter < Text.size() - 1) ? Font->GetKerning(Character, Text[Letter + 1]) : 0.0f;
                BoundariesX += Scale * (Glyph->Advance + Kerning);
            }

            if (AlignmentType & (1 << 2))
            {
                Position.SetX(Position.GetX() - BoundariesX);
            }
            else
            {
                Position.SetX(Position.GetX() - BoundariesX * 0.5f);
            }
        }

        // Align vertically.
        if (AlignmentType &  (1 << 3))
        {
            Position.SetY(Position.GetY() + (Metrics.Descender * Scale));
        }
        else if (AlignmentType &  (1 << 4))
        {
            Position.SetY(Position.GetY() - (Metrics.Ascender + Metrics.Descender) / 2.0f * Scale);
        }
        else if (AlignmentType &  (1 << 5))
        {
            Position.SetY(Position.GetY() + (Metrics.Ascender * Scale));
        }

        Real32 CurrentX = Position.GetX();
        Real32 CurrentY = Position.GetY();

        for (UInt Letter = 0; Letter < Text.size(); ++Letter)
        {
            const UInt Character = Text[Letter];

            switch (Character)
            {
            case ' ' :
            {
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                if (Letter < Text.size() - 1)
                {
                    const Real32 Kerning = Font->GetKerning(Character, Text[Letter + 1]);
                    CurrentX += Scale * (Glyph->Advance + Kerning);
                }
            }
                break;
            case '\r':
                break;
            case '\n':
                CurrentX  = Position.GetX();
                CurrentY += Scale * Font->GetMetrics().UnderlineHeight;
                break;
            default:
            {
                const Ptr<const Graphic::Font::Glyph> Glyph = Font->GetGlyph(Character);

                Rectf PositionRect = (Glyph->PlaneBounds * Scale) + Vector2f(CurrentX, CurrentY);

                // TODO: Make it own queue so that we don't need to sort lot of small objects
                Draw(PositionRect, Glyph->ImageBounds, Depth, 0.0f, Tint, Order::Transparent, Pipeline, Font->GetMaterial());

                if (Letter < Text.size() - 1)
                {
                    const Real32 Kerning = Font->GetKerning(Character, Text[Letter + 1]);
                    CurrentX += Scale * (Glyph->Advance + Kerning);
                }
            }
                break;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::End()
    {
        if (!mDrawables.empty())
        {
            Flush();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::WriteGeometry(Ptr<const Drawable> Drawable, Ptr<VertexShaderGeometry> Buffer)
    {
        const Real32 DestinationX1 = Drawable->Destination.GetLeft();
        const Real32 DestinationX2 = Drawable->Destination.GetRight();
        const Real32 DestinationX3 = Drawable->Destination.GetLeft();
        const Real32 DestinationX4 = Drawable->Destination.GetRight();

        const Real32 DestinationY1 = Drawable->Destination.GetTop();
        const Real32 DestinationY2 = Drawable->Destination.GetTop();
        const Real32 DestinationY3 = Drawable->Destination.GetBottom();
        const Real32 DestinationY4 = Drawable->Destination.GetBottom();

        if (Drawable->Angle != 0.0f)
        {
            const Vector3f    Position(Drawable->Destination.GetX(), Drawable->Destination.GetY(), Drawable->Depth);
            const Vector3f    Size(Drawable->Destination.GetWidth(), Drawable->Destination.GetHeight(), 1.0f);
            const Quaternionf Rotation = Quaternionf::FromAngles(Drawable->Angle, Vector3f(0.0f, 0.0f, 1.0f));

            const Matrix4f Transformation = Matrix4f::CreateTransform(Position, Rotation, Size);

            Buffer->V1.Position = Transformation * Vector3f(-0.5f, -0.5f, 0.0f);
            Buffer->V2.Position = Transformation * Vector3f( 0.5f, -0.5f, 0.0f);
            Buffer->V3.Position = Transformation * Vector3f(-0.5f,  0.5f, 0.0f);
            Buffer->V4.Position = Transformation * Vector3f( 0.5f,  0.5f, 0.0f);
        }
        else
        {
            Buffer->V1.Position.Set(DestinationX1, DestinationY1, Drawable->Depth);
            Buffer->V2.Position.Set(DestinationX2, DestinationY2, Drawable->Depth);
            Buffer->V3.Position.Set(DestinationX3, DestinationY3, Drawable->Depth);
            Buffer->V4.Position.Set(DestinationX4, DestinationY4, Drawable->Depth);
        }

        Buffer->V1.Color = Drawable->Color;
        Buffer->V1.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetTop());

        Buffer->V2.Color = Drawable->Color;
        Buffer->V2.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetTop());

        Buffer->V3.Color = Drawable->Color;
        Buffer->V3.Texture.Set(Drawable->Source.GetLeft(), Drawable->Source.GetBottom());

        Buffer->V4.Color = Drawable->Color;
        Buffer->V4.Texture.Set(Drawable->Source.GetRight(), Drawable->Source.GetBottom());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush()
    {
        // Sort all drawables back to front and by material/pipeline
        eastl::sort(mDrawablesPtr.begin(), mDrawablesPtr.end(), [](Ptr<const Drawable> Left, Ptr<const Drawable> Right)
        {
            return Left->ID > Right->ID;
        });

        // Set technique parameter(s)
        if (! mTechnique.empty())
        {
            mEncoder.SetTechnique(mTechnique);
        }

        // Render all drawables in batches
        for (UInt Element = 0, Start = 0, Max = mDrawablesPtr.size() - 1; Element <= Max; ++Element)
        {
            const Ptr<const Drawable> ThisDrawable = mDrawablesPtr[Element];
            const Ptr<const Drawable> NextDrawable = (Element == Max ? ThisDrawable : mDrawablesPtr[Element + 1]);

            // Break batch if material or pipeline differ or we are handling the last element
            if (   ThisDrawable->Material != NextDrawable->Material
                || ThisDrawable->Pipeline != NextDrawable->Pipeline
                || ThisDrawable           == NextDrawable)
            {
                UInt Count = Element - Start + 1;
                UInt Index = 0;

                // Ensure we have enough space for the batch otherwise flush previous batches
                mEncoder.Ensure<VertexShaderLayout, SInt16>(Count * 4, Count * 6, ThisDrawable->Material);

                for (UInt Drawable = Start; Drawable <= Element; ++Drawable, Index += 4)
                {
                    Ptr<VertexShaderLayout> Vertices = mEncoder.AllocateTransientVertices<VertexShaderLayout>(4);
                    WriteGeometry(mDrawablesPtr[Drawable], reinterpret_cast<Ptr<VertexShaderGeometry>>(Vertices));

                    Ptr<UInt16> Indices = mEncoder.AllocateTransientIndices<UInt16>(6);
                    Indices[0] = Index;
                    Indices[1] = Index + 1;
                    Indices[2] = Index + 2;
                    Indices[3] = Index + 1;
                    Indices[4] = Index + 3;
                    Indices[5] = Index + 2;
                }

                mEncoder.SetPipeline(ThisDrawable->Pipeline);
                mEncoder.SetMaterial(ThisDrawable->Material);
                mEncoder.Draw(sizeof(VertexShaderLayout), sizeof(SInt16));

                // Continue with the next batch
                Start = Element + 1;
            }
        }

        // Flush all batches to the GPU
        mEncoder.Flush();

        // Reset all stack(s) back to original states
        mDrawables.clear();
        mDrawablesPtr.clear();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt64 Renderer::GenerateUniqueID(Order Order, UInt Pipeline, UInt Material, Real32 Depth) const
    {
        // TODO: Clean this
        union
        {
            UInt64 Key;

            struct
            {
                UInt64 bDepth     : 32;
                UInt64 bMaterial  : 16;
                UInt64 bTechnique : 8;
                UInt64 bType      : 8;

            } Opaque;

            struct
            {
                UInt64 bMaterial  : 16;
                UInt64 bTechnique : 8;
                UInt64 bDepth     : 32;
                UInt64 bType      : 8;
            } Transparent;

        } ID;

        if (Order == Order::Transparent)
        {
            ID.Transparent.bType     = 0;
            ID.Transparent.bMaterial = Material;
            ID.Transparent.bTechnique = Pipeline;
            ID.Transparent.bDepth = * (UInt32 *) (& Depth);
        }
        else
        {
            const auto zDepthInv = (1.0f + Depth);

            ID.Opaque.bType     = 1;
            ID.Opaque.bMaterial = Material;
            ID.Opaque.bTechnique = Pipeline;
            ID.Opaque.bDepth = * (UInt32 *) (& zDepthInv);
        }

        return ID.Key;
    }
}