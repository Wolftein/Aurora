// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Math/Rect.hpp"
#include "Aurora.Math/Transform.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Camera final
    {
    public:

        // -=(Undocumented)=-
        Camera();

        // -=(Undocumented)=-
        Bool Compute();

        // -=(Undocumented)=-
        ConstRef<Matrix4f> GetProjection() const
        {
            return mProjection;
        }

        // -=(Undocumented)=-
        ConstRef<Matrix4f> GetScene() const
        {
            return mScene;
        }

        // -=(Undocumented)=-
        ConstRef<Matrix4f> GetWorld() const
        {
            return mWorld;
        }

        // -=(Undocumented)=-
        Vector3f GetRight() const
        {
            return mScene.GetRight();
        }

        // -=(Undocumented)=-
        Vector3f GetLeft() const
        {
            return mScene.GetLeft();
        }

        // -=(Undocumented)=-
        Vector3f GetUp() const
        {
            return mScene.GetUp();
        }

        // -=(Undocumented)=-
        Vector3f GetDown() const
        {
            return mScene.GetDown();
        }

        // -=(Undocumented)=-
        Vector3f GetForward() const
        {
            return mScene.GetForward();
        }

        // -=(Undocumented)=-
        Vector3f GetBackward() const
        {
            return mScene.GetBackward();
        }

        // -=(Undocumented)=-
        void SetPerspective(Real32 Eyes, Real32 Aspect, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4f::CreatePerspective(Eyes, Aspect, ZNear, ZFar);
            mDirty = SetBit(mDirty, k_DirtyBitProjection);
        }

        // -=(Undocumented)=-
        void SetOrthographic(Real32 Left, Real32 Right, Real32 Bottom, Real32 Top, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4f::CreateOrthographic(Left, Right, Bottom, Top, ZNear, ZFar);
            mDirty = SetBit(mDirty, k_DirtyBitProjection);
        }

        // -=(Undocumented)=-
        void SetOrthographic(Real32 Width, Real32 Height, Real32 ZNear, Real32 ZFar)
        {
            SetOrthographic(0, Width, Height, 0, ZNear, ZFar);
        }

        // -=(Undocumented)=-
        void SetLook(ConstRef<Vector3f> Eye, ConstRef<Vector3f> Focus, ConstRef<Vector3f> Up)
        {
            mScene = Matrix4f::CreateLook(Eye, Focus, Up);
            mDirty = SetBit(0u, k_DirtyBitProjection);
        }

        // -=(Undocumented)=-
        void SetPosition(ConstRef<Vector3f> Position)
        {
            mTransformation.SetPosition(Position);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(ConstRef<Vector2f> Position)
        {
            mTransformation.SetPosition(Position);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetPosition(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y)
        {
            mTransformation.SetPosition(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetPosition() const
        {
            return mTransformation.GetPosition();
        }

        // -=(Undocumented)=-
        void SetScale(ConstRef<Vector3f> Scale)
        {
            mTransformation.SetScale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(ConstRef<Vector2f> Scale)
        {
            mTransformation.SetScale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetScale(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y)
        {
            mTransformation.SetScale(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetScale() const
        {
            return mTransformation.GetScale();
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, ConstRef<Vector3f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Axis));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, ConstRef<Vector2f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0)));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Angles)
        {
            mTransformation.SetRotation(Quaternionf::FromEulerAngles(Angles));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Pitch, Real32 Yaw, Real32 Roll)
        {
            mTransformation.SetRotation(Quaternionf::FromEulerAngles(Pitch, Yaw, Roll));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Direction, ConstRef<Vector3f> Up)
        {
            mTransformation.SetRotation(Quaternionf::FromDirection(Direction, Up));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Eye, ConstRef<Vector3f> Focus, ConstRef<Vector3f> Up)
        {
            mTransformation.SetRotation(Quaternionf::FromDirection(Focus - Eye, Up));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Quaternionf> GetRotation() const
        {
            return mTransformation.GetRotation();
        }

        // -=(Undocumented)=-
        void Translate(ConstRef<Vector3f> Translation)
        {
            mTransformation.Translate(Translation);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(ConstRef<Vector2f> Translation)
        {
            mTransformation.Translate(Translation);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Translate(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y)
        {
            mTransformation.Translate(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(ConstRef<Vector3f> Scale)
        {
            mTransformation.Scale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(ConstRef<Vector2f> Scale)
        {
            mTransformation.Scale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Scale(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y)
        {
            mTransformation.Scale(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, ConstRef<Vector3f> Axis)
        {
            mTransformation.Rotate(Angle, Axis);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, ConstRef<Vector2f> Axis)
        {
            mTransformation.Rotate(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(ConstRef<Vector3f> Angles)
        {
            mTransformation.Rotate(Angles);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(ConstRef<Vector2f> Angles)
        {
            mTransformation.Rotate(Angles);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        Vector3f GetWorldCoordinates(ConstRef<Vector3f> Position, ConstRef<Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetWorldCoordinates(ConstRef<Vector2f> Position, ConstRef<Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector3f GetScreenCoordinates(ConstRef<Vector3f> Position, ConstRef<Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetScreenCoordinates(ConstRef<Vector2f> Position, ConstRef<Rectf> Viewport) const;

    private:

        static constexpr UInt32 k_DirtyBitTransformation = 1 << 0;
        static constexpr UInt32 k_DirtyBitProjection     = 1 << 1;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32     mDirty;
        Matrix4f   mProjection;
        Matrix4f   mScene;
        Matrix4f   mWorld;
        Matrix4f   mInverse;
        Transformf mTransformation;
    };
}