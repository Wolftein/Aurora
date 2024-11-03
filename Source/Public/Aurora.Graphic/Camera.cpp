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

#include "Camera.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Camera::Camera()
        : mDirty { false }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Camera::Compute()
    {
        const Bool Dirty = mDirty;

        if (Dirty)
        {
            mDirty   = false;

            // Calculate the view matrix from the transformation and calculate world matrix
            mWorld   = mProjection * (mView = mTransformation.AsReverseMatrix());

            // Calculate the inverse of the world matrix
            mInverse = mWorld.Inverse();
        }
        return Dirty;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3f Camera::GetWorldCoordinates(Ref<const Vector3f> Position, Ref<const Rectf> Viewport) const
    {
        const Real32 Width  = Viewport.GetWidth();
        const Real32 Height = Viewport.GetHeight();

        // Undo the Z normalization first
        const Real32 Z = Position.GetZ() * 2.0f - 1.0f;

        // Calculate X and Y in normalized device coordinates
        const Real32 X = (Position.GetX() - Viewport.GetX()) / Width  * 2.0f - 1.0f;
        const Real32 Y = (Height - (Position.GetY() - Viewport.GetY())) / Height * 2.0f - 1.0f;

        // Transform the normalized device coordinates back to world space
        const Vector4f WorldPosition = mInverse * Vector4f(X, Y, Z, 1.0f);
        return Vector3f(WorldPosition.GetX(), WorldPosition.GetY(), WorldPosition.GetZ());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2f Camera::GetWorldCoordinates(Ref<const Vector2f> Position, Ref<const Rectf> Viewport) const
    {
        const Vector3f Coordinates = GetWorldCoordinates(Vector3f(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2f(Coordinates.GetX(), Coordinates.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3f Camera::GetScreenCoordinates(Ref<const Vector3f> Position, Ref<const Rectf> Viewport) const
    {
        const Vector3f Point = mWorld * Position;

        const Real32 Width  = Viewport.GetWidth();
        const Real32 Height = Viewport.GetHeight();

        const Real32 X = Width  * (Point.GetX() + 1.0f) * 0.5f + Viewport.GetX();
        const Real32 Y = Height - Height * (Point.GetY() + 1.0f) * 0.5f + Viewport.GetY();
        const Real32 Z = (Point.GetZ() + 1.0f) * 0.5f;

        return Vector3f(X, Y, Z);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2f Camera::GetScreenCoordinates(Ref<const Vector2f> Position, Ref<const Rectf> Viewport) const
    {
        const Vector3f Coordinates = GetScreenCoordinates(Vector3f(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2f(Coordinates.GetX(), Coordinates.GetY());
    }
}