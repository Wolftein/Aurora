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
            if (HasBit(mDirty, kDirtyBitTransformation))
            {
                mView = Matrix4x4::Inverse<true>(mTransform.Compute());
            }

            // Calculate the view-projection matrix by multiplying the projection matrix
            // with the (possibly updated) view matrix.
            mViewProjection        = mProjection * mView;

            // Compute the inverse of the view-projection matrix
            mViewProjectionInverse = Matrix4x4::Inverse(mViewProjection);

            // Clear the dirty flag to indicate that the transformation has been processed.
            mDirty = 0;
        }
        return Dirty;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3 Camera::GetWorldCoordinates(ConstRef<Vector3> Position, ConstRef<Viewport> Viewport) const
    {
        // Undo the Z normalization first
        const Real32 Z = Position.GetZ() * 2.0f - 1.0f;

        // Calculate X and Y in normalized device coordinates
        const Real32 X = (Position.GetX() - Viewport.X) / Viewport.Width  * 2.0f - 1.0f;
        const Real32 Y = (Viewport.Height - (Position.GetY() - Viewport.Y)) / Viewport.Height * 2.0f - 1.0f;

        // Transform the normalized device coordinates back to world space
        const Vector4 WorldPosition = Matrix4x4::Project(mViewProjectionInverse, Vector4(X, Y, Z, 1.0f));
        return Vector3(WorldPosition.GetX(), WorldPosition.GetY(), WorldPosition.GetZ());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2 Camera::GetWorldCoordinates(ConstRef<Vector2> Position, ConstRef<Viewport> Viewport) const
    {
        const Vector3 Coordinates = GetWorldCoordinates(Vector3(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2(Coordinates.GetX(), Coordinates.GetY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector3 Camera::GetScreenCoordinates(ConstRef<Vector3> Position, ConstRef<Viewport> Viewport) const
    {
        const Vector3 Point = Matrix4x4::Project(mViewProjection, Position);

        const Real32 X = Viewport.Width  * (Point.GetX() + 1.0f) * 0.5f + Viewport.X;
        const Real32 Y = Viewport.Height - Viewport.Height * (Point.GetY() + 1.0f) * 0.5f + Viewport.Y;
        const Real32 Z = (Point.GetZ() + 1.0f) * 0.5f;

        return Vector3(X, Y, Z);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Vector2 Camera::GetScreenCoordinates(ConstRef<Vector2> Position, ConstRef<Viewport> Viewport) const
    {
        const Vector3 Coordinates = GetScreenCoordinates(Vector3(Position.GetX(), Position.GetY(), 0), Viewport);

        return Vector2(Coordinates.GetX(), Coordinates.GetY());
    }
}