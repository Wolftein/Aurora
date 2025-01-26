// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
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
        const Bool Dirty = (mDirty > 0);

        if (Dirty)
        {
            // If the transformation matrix has been modified (dirty flag is set),
            // compute the inverse scene matrix from the updated transformation.
            if (HasBit(mDirty, k_DirtyBitTransformation))
            {
                mScene = mTransformation.Compute().Inverse();
            }

            // Calculate the world matrix by multiplying the projection matrix
            // with the (possibly updated) scene matrix.
            mWorld   = mProjection * mScene;

            // Compute the inverse of the world matrix
            mInverse = mWorld.Inverse();

            // Clear the dirty flag to indicate that the transformation has been processed.
            mDirty   = 0;
        }
        return Dirty;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3f Camera::GetWorldCoordinates(ConstRef<Vector3f> Position, ConstRef<Rectf> Viewport) const
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

    Vector2f Camera::GetWorldCoordinates(ConstRef<Vector2f> Position, ConstRef<Rectf> Viewport) const
    {
        const Vector3f Coordinates = GetWorldCoordinates(Vector3f(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2f(Coordinates.GetX(), Coordinates.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3f Camera::GetScreenCoordinates(ConstRef<Vector3f> Position, ConstRef<Rectf> Viewport) const
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

    Vector2f Camera::GetScreenCoordinates(ConstRef<Vector2f> Position, ConstRef<Rectf> Viewport) const
    {
        const Vector3f Coordinates = GetScreenCoordinates(Vector3f(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2f(Coordinates.GetX(), Coordinates.GetY());
    }
}