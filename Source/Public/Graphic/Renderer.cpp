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
#include "Content/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Renderer::Renderer(Ref<Core::Subsystem::Context> Context)
        : mEncoder { Context.GetSubsystem<Graphic::Service>(), k_MaxVertices, k_MaxIndices, k_MaxUniform }
    {
        CreateDefaultResources(Context);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Begin(Ref<const Matrix4f> Projection, Real32 Time)
    {
        struct
        {
            const Matrix4f uProjection;
            const Real32   uTime;
        } Scene { Projection, Time };

        mEncoder.SetScene<decltype(Scene)>(Scene);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawLine(Ref<const Array<Vector2f, 2>> Points, Real32 Depth, Ref<const Array<Color, 2>> Tints, Real32 Thickness)
    {
        Array<Vector2f, 4> Position;
        Array<Color,    4> Color;

        const Real32 HalfThickness = Thickness * 0.5f;

        if (Points[0].GetX() == Points[1].GetX())
        {
            // Vertical line (angle is 0 degrees)
            Position[0] = { Points[0].GetX() - HalfThickness, Points[0].GetY() };
            Position[1] = { Points[0].GetX() + HalfThickness, Points[0].GetY() };
            Position[2] = { Points[1].GetX() - HalfThickness, Points[1].GetY() };
            Position[3] = { Points[1].GetX() + HalfThickness, Points[1].GetY() };
            Color[0]    = Tints[0];
            Color[1]    = Tints[0];
            Color[2]    = Tints[1];
            Color[3]    = Tints[1];
        }
        else if (Points[1].GetY() == Points[1].GetY())
        {
            // Horizontal line (angle is 90 degrees)
            Position[0] = { Points[0].GetX(), Points[0].GetY() - HalfThickness };
            Position[1] = { Points[1].GetX(), Points[1].GetY() - HalfThickness };
            Position[2] = { Points[0].GetX(), Points[0].GetY() + HalfThickness };
            Position[3] = { Points[1].GetX(), Points[1].GetY() + HalfThickness };
            Color[0]    = Tints[0];
            Color[1]    = Tints[1];
            Color[2]    = Tints[0];
            Color[3]    = Tints[1];
        }
        else
        {
            // Calculate the direction vector of the line
            const Real32 DirX = Points[1].GetX() - Points[0].GetX();
            const Real32 DirY = Points[1].GetY() - Points[0].GetY();

            // Calculate the length of the line
            const Real32 Length = std::sqrtf(DirX * DirX + DirY * DirY);

            // Calculate unit vectors along the line
            const Real32 UnitX = DirX / Length;
            const Real32 UnitY = DirY / Length;

            // Calculate the perpendicular vector (normal) to the line and apply thickness
            const Real32 NormalX = -UnitY * HalfThickness;
            const Real32 NormalY =  UnitX * HalfThickness;

            Position[0] = { Points[0].GetX() - NormalX, Points[0].GetY() - NormalY };
            Position[1] = { Points[1].GetX() - NormalX, Points[1].GetY() - NormalY };
            Position[2] = { Points[0].GetX() + NormalX, Points[0].GetY() + NormalY };
            Position[3] = { Points[1].GetX() + NormalX, Points[1].GetY() + NormalY };
            Color[0]    = Tints[0];
            Color[1]    = Tints[1];
            Color[2]    = Tints[0];
            Color[3]    = Tints[1];
        }

        DrawRectangle(Position, Depth, Color);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawRectangle(Ref<const Array<Vector2f, 4>> Points, Real32 Depth, Ref<const Array<Color, 4>> Tint)
    {
        Push(Points, Tint, Array<Vector2f, 4> { }, Order::Opaque, Depth, mPipelines[0], nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawRectangle(Ref<const Rectf> Rectangle, Real32 Depth, Real32 Angle, Ref<const Array<Color, 4>> Tint)
    {
        const Real DestinationX1 = Rectangle.GetLeft();
        const Real DestinationX2 = Rectangle.GetRight();
        const Real DestinationX3 = Rectangle.GetLeft();
        const Real DestinationX4 = Rectangle.GetRight();

        const Real DestinationY1 = Rectangle.GetTop();
        const Real DestinationY2 = Rectangle.GetTop();
        const Real DestinationY3 = Rectangle.GetBottom();
        const Real DestinationY4 = Rectangle.GetBottom();

        Array<Vector2f, 4> Position;

        if (Angle != 0.0f)
        {
            const Real Radians = (Angle * 3.141592654f) / 180.0f;

            const Vector3f    Translation(Rectangle.GetX(), Rectangle.GetY(), Depth);
            const Vector3f    Size(Rectangle.GetWidth(), Rectangle.GetHeight(), 1.0f);
            const Quaternionf Rotation = Quaternionf::FromAngles(Radians, Vector3f(0.0f, 0.0f, 1.0f));

            const Matrix4f Transformation = Matrix4f::CreateTransform(Translation, Rotation, Size);

            Position[0] = Transformation * Vector2f(-0.5f, -0.5f);
            Position[1] = Transformation * Vector2f( 0.5f, -0.5f);
            Position[2] = Transformation * Vector2f(-0.5f,  0.5f);
            Position[3] = Transformation * Vector2f( 0.5f,  0.5f);
        }
        else
        {
            Position[0] = Vector2f(DestinationX1, DestinationY1);
            Position[1] = Vector2f(DestinationX2, DestinationY2);
            Position[2] = Vector2f(DestinationX3, DestinationY3);
            Position[3] = Vector2f(DestinationX4, DestinationY4);
        }

        DrawRectangle(Position, Depth, Tint);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawTexture(Ref<const Rectf> Rectangle, Ref<const Rectf> Source, Real32 Depth, Real32 Angle,
        Order Order, Ref<const Array<Color, 4>> Tint, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
    {
        if (!Pipeline->HasLoaded() || !Material->HasLoaded())
        {
            return;
        }

        const Real DestinationX1 = Rectangle.GetLeft();
        const Real DestinationX2 = Rectangle.GetRight();
        const Real DestinationX3 = Rectangle.GetLeft();
        const Real DestinationX4 = Rectangle.GetRight();

        const Real DestinationY1 = Rectangle.GetTop();
        const Real DestinationY2 = Rectangle.GetTop();
        const Real DestinationY3 = Rectangle.GetBottom();
        const Real DestinationY4 = Rectangle.GetBottom();

        Array<Vector2f, 4> Position;

        if (Angle != 0.0f)
        {
            const Real Radians = (Angle * 3.141592654f) / 180.0f;

            const Vector3f    Translation(Rectangle.GetX(), Rectangle.GetY(), Depth);
            const Vector3f    Size(Rectangle.GetWidth(), Rectangle.GetHeight(), 1.0f);
            const Quaternionf Rotation = Quaternionf::FromAngles(Radians, Vector3f(0.0f, 0.0f, 1.0f));

            const Matrix4f Transformation = Matrix4f::CreateTransform(Translation, Rotation, Size);

            Position[0] = Transformation * Vector2f(-0.5f, -0.5f);
            Position[1] = Transformation * Vector2f( 0.5f, -0.5f);
            Position[2] = Transformation * Vector2f(-0.5f,  0.5f);
            Position[3] = Transformation * Vector2f( 0.5f,  0.5f);
        }
        else
        {
            Position[0] = Vector2f(DestinationX1, DestinationY1);
            Position[1] = Vector2f(DestinationX2, DestinationY2);
            Position[2] = Vector2f(DestinationX3, DestinationY3);
            Position[3] = Vector2f(DestinationX4, DestinationY4);
        }

        Array<Vector2f, 4> Texture {
            Vector2f { Source.GetLeft(),  Source.GetTop() },
            Vector2f { Source.GetRight(), Source.GetTop() },
            Vector2f { Source.GetLeft(),  Source.GetBottom() },
            Vector2f { Source.GetRight(), Source.GetBottom() }
        };

        Push(Position, Tint, Texture, Order, Depth, Pipeline, Material);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::DrawFont(ConstSPtr<Font> Font, CStr16 Text, Vector2f Position, Real32 Depth, Real32 Scale, Color Tint, Alignment Alignment)
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

                DrawTexture(PositionRect, Glyph->ImageBounds, Depth, 0.0f, Order::Normal, Tint, mPipelines[1], Font->GetMaterial());

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

    void Renderer::CreateDefaultResources(Ref<Core::Subsystem::Context> Context)
    {
        ConstSPtr<Content::Service> ContentService = Context.GetSubsystem<Content::Service>();
        mPipelines[0] = ContentService->Load<Graphic::Pipeline>("Engine://Pipeline/Geometry.effect");
        mPipelines[1] = ContentService->Load<Graphic::Pipeline>("Engine://Pipeline/Font.effect");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::Flush()
    {
        // Sort all drawables back to front and by material/pipeline
        std::sort(mDrawablesPtr.begin(), mDrawablesPtr.end(), [](Ptr<const Drawable> Left, Ptr<const Drawable> Right)
        {
            return Left->ID > Right->ID;
        });

        // Apply technique parameter(s)
        mEncoder.SetTechnique(mTechnique);

        // Render all drawables in batches
        for (UInt Element = 0, Start = 0, Max = mDrawablesPtr.size() - 1; Element <= Max; ++Element)
        {
            const Ptr<const Drawable> This  = mDrawablesPtr[Element];
            const Ptr<const Drawable> Other = (Element == Max ? This : mDrawablesPtr[Element + 1]);

            // Break batch if material or pipeline differ or we are handling the last element
            if (This->Material != Other->Material || This->Pipeline != Other->Pipeline || This == Other)
            {
                const UInt Count = Element - Start + 1;

                // Ensure we have enough space for the batch otherwise flush previous batches
                mEncoder.Ensure<PosColorTextureLayout>(
                    Count * 4,
                    Count * 6,
                    This->Material ? This->Material->GetParameters().size() : 0);

                for (UInt Drawable = Start, Index = 0; Drawable <= Element; ++Drawable, Index += 4)
                {
                    Ptr<PosColorTextureLayout> Vertices = mEncoder.AllocateTransientVertices<PosColorTextureLayout>(4);
                    WriteGeometry(mDrawablesPtr[Drawable], Vertices);

                    const Ptr<UInt16> Indices = mEncoder.AllocateTransientIndices<UInt16>(6);
                    Indices[0] = Index;
                    Indices[1] = Index + 1;
                    Indices[2] = Index + 2;
                    Indices[3] = Index + 1;
                    Indices[4] = Index + 3;
                    Indices[5] = Index + 2;
                }

                mEncoder.SetPipeline(This->Pipeline);

                if (This->Material)
                {
                    mEncoder.SetMaterial(This->Material);
                }

                mEncoder.Draw<PosColorTextureLayout>();

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

    void Renderer::Push(
        Ref<const Array<Vector2f, 4>> Position,
        Ref<const Array<Color, 4>> Tint,
        Ref<const Array<Vector2f, 4>> Texture,
        Order Order, Real32 Depth, ConstSPtr<Pipeline> Pipeline, ConstSPtr<Material> Material)
    {
        Ref<Drawable> Drawable  = mDrawables.emplace_back();
        Drawable.ID             = GenerateUniqueID(Order, Pipeline->GetID(), Material ? Material->GetID() : 0, Depth);
        Drawable.Depth          = Depth;
        Drawable.Position       = Position;
        Drawable.Color          = Tint;
        Drawable.Texture        = Texture;
        Drawable.Pipeline       = Pipeline;
        Drawable.Material       = Material;
        mDrawablesPtr.push_back(& Drawable);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Renderer::WriteGeometry(Ptr<const Drawable> Drawable, Ptr<PosColorTextureLayout> Buffer)
    {
        Buffer[0].Position.Set(Drawable->Position[0].GetX(), Drawable->Position[0].GetY(), Drawable->Depth);
        Buffer[1].Position.Set(Drawable->Position[1].GetX(), Drawable->Position[1].GetY(), Drawable->Depth);
        Buffer[2].Position.Set(Drawable->Position[2].GetX(), Drawable->Position[2].GetY(), Drawable->Depth);
        Buffer[3].Position.Set(Drawable->Position[3].GetX(), Drawable->Position[3].GetY(), Drawable->Depth);
        Buffer[0].Color = Drawable->Color[0].AsPacked();
        Buffer[1].Color = Drawable->Color[1].AsPacked();
        Buffer[2].Color = Drawable->Color[2].AsPacked();
        Buffer[3].Color = Drawable->Color[3].AsPacked();
        Buffer[0].Texture = Drawable->Texture[0];
        Buffer[1].Texture = Drawable->Texture[1];
        Buffer[2].Texture = Drawable->Texture[2];
        Buffer[3].Texture = Drawable->Texture[3];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt64 Renderer::GenerateUniqueID(Order Order, UInt Pipeline, UInt Material, Real32 Depth) const
    {
        union
        {
            UInt64 Value;

            struct
            {
                UInt64 iDepth        : 32;
                UInt64 iMaterial     : 16;
                UInt64 iTechnique    : 8;
                UInt64 iTranslucency : 8;
            } Opaque;

            struct
            {
                UInt64 iMaterial     : 16;
                UInt64 iTechnique    : 8;
                UInt64 iDepth        : 32;
                UInt64 iTranslucency : 8;
            } Transparent;
        } ID;

        switch (Order)
        {
        case Order::Subtractive:
        case Order::Additive:
        case Order::Normal:
            ID.Transparent.iMaterial     = Material;
            ID.Transparent.iTechnique    = Pipeline;
            ID.Transparent.iDepth        = * reinterpret_cast<Ptr<const UInt32>>(& Depth);
            ID.Transparent.iTranslucency = CastEnum(Order);
            break;
        case Order::Opaque:
            const Real32 DepthInv = (1.0f + Depth);

            ID.Opaque.iMaterial          = Material;
            ID.Opaque.iTechnique         = Pipeline;
            ID.Opaque.iDepth             = * reinterpret_cast<Ptr<const UInt32>>(& DepthInv);
            ID.Opaque.iTranslucency      = CastEnum(Order);
            break;
        }
        return ID.Value;
    }
}