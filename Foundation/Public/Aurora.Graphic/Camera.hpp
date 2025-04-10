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
        ConstRef<Matrix4f> GetView() const
        {
            return mView;
        }

        // -=(Undocumented)=-
        ConstRef<Matrix4f> GetViewProjection() const
        {
            return mViewProjection;
        }

        // -=(Undocumented)=-
        ConstRef<Matrix4f> GetViewProjectionInverse() const
        {
            return mViewProjectionInverse;
        }

        // -=(Undocumented)=-
        Vector3f GetRight() const
        {
            return mView.GetRight();
        }

        // -=(Undocumented)=-
        Vector3f GetLeft() const
        {
            return mView.GetLeft();
        }

        // -=(Undocumented)=-
        Vector3f GetUp() const
        {
            return mView.GetUp();
        }

        // -=(Undocumented)=-
        Vector3f GetDown() const
        {
            return mView.GetDown();
        }

        // -=(Undocumented)=-
        Vector3f GetForward() const
        {
            return mView.GetForward();
        }

        // -=(Undocumented)=-
        Vector3f GetBackward() const
        {
            return mView.GetBackward();
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
            mView = Matrix4f::CreateLook(Eye, Focus, Up);
            mDirty = SetBit(0u, k_DirtyBitProjection);
        }

        // -=(Undocumented)=-
        void SetPosition(ConstRef<Vector3f> Position)
        {
            mTransform.SetPosition(Position);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(ConstRef<Vector2f> Position)
        {
            mTransform.SetPosition(Position);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.SetPosition(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y)
        {
            mTransform.SetPosition(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetPosition() const
        {
            return mTransform.GetPosition();
        }

        // -=(Undocumented)=-
        void SetScale(ConstRef<Vector3f> Scale)
        {
            mTransform.SetScale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(ConstRef<Vector2f> Scale)
        {
            mTransform.SetScale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.SetScale(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y)
        {
            mTransform.SetScale(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetScale() const
        {
            return mTransform.GetScale();
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, ConstRef<Vector3f> Axis)
        {
            mTransform.SetRotation(Quaternionf::FromAngles(Angle, Axis));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, ConstRef<Vector2f> Axis)
        {
            mTransform.SetRotation(Quaternionf::FromAngles(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0)));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Angles)
        {
            mTransform.SetRotation(Quaternionf::FromEulerAngles(Angles));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Pitch, Real32 Yaw, Real32 Roll)
        {
            mTransform.SetRotation(Quaternionf::FromEulerAngles(Pitch, Yaw, Roll));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Direction, ConstRef<Vector3f> Up)
        {
            mTransform.SetRotation(Quaternionf::FromDirection(Direction, Up));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void SetRotation(ConstRef<Vector3f> Eye, ConstRef<Vector3f> Focus, ConstRef<Vector3f> Up)
        {
            mTransform.SetRotation(Quaternionf::FromDirection(Focus - Eye, Up));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        ConstRef<Quaternionf> GetRotation() const
        {
            return mTransform.GetRotation();
        }

        // -=(Undocumented)=-
        void Translate(ConstRef<Vector3f> Translation)
        {
            mTransform.Translate(Translation);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(ConstRef<Vector2f> Translation)
        {
            mTransform.Translate(Translation);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.Translate(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y)
        {
            mTransform.Translate(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(ConstRef<Vector3f> Scale)
        {
            mTransform.Scale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(ConstRef<Vector2f> Scale)
        {
            mTransform.Scale(Scale);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.Scale(Vector3f(X, Y, Z));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y)
        {
            mTransform.Scale(Vector2f(X, Y));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, ConstRef<Vector3f> Axis)
        {
            mTransform.Rotate(Angle, Axis);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, ConstRef<Vector2f> Axis)
        {
            mTransform.Rotate(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0));
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(ConstRef<Vector3f> Angles)
        {
            mTransform.Rotate(Angles);
            mDirty = SetBit(mDirty, k_DirtyBitTransformation);
        }

        // -=(Undocumented)=-
        void Rotate(ConstRef<Vector2f> Angles)
        {
            mTransform.Rotate(Angles);
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
        Matrix4f   mView;
        Matrix4f   mViewProjection;
        Matrix4f   mViewProjectionInverse;
        Transformf mTransform;
    };
}