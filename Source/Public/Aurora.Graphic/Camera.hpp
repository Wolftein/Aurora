// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
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
        Ref<const Matrix4f> GetProjection() const
        {
            return mProjection;
        }

        // -=(Undocumented)=-
        Ref<const Matrix4f> GetView() const
        {
            return mView;
        }

        // -=(Undocumented)=-
        Ref<const Matrix4f> GetWorld() const
        {
            return mWorld;
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
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetOrthographic(Real32 Left, Real32 Right, Real32 Bottom, Real32 Top, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4f::CreateOrthographic(Left, Right, Bottom, Top, ZNear, ZFar);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetOrthographic(Real32 Width, Real32 Height, Real32 ZNear, Real32 ZFar)
        {
            SetOrthographic(0, Width, Height, 0, ZNear, ZFar);
        }

        // -=(Undocumented)=-
        void SetPosition(Ref<const Vector3f> Position)
        {
            mTransformation.SetPosition(Position);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetPosition(Ref<const Vector2f> Position)
        {
            mTransformation.SetPosition(Position);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetPosition(Vector3f(X, Y, Z));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y)
        {
            mTransformation.SetPosition(Vector2f(X, Y));
            mDirty = true;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetPosition() const
        {
            return mTransformation.GetPosition();
        }

        // -=(Undocumented)=-
        void SetScale(Ref<const Vector3f> Scale)
        {
            mTransformation.SetScale(Scale);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetScale(Ref<const Vector2f> Scale)
        {
            mTransformation.SetScale(Scale);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetScale(Vector3f(X, Y, Z));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y)
        {
            mTransformation.SetScale(Vector2f(X, Y));
            mDirty = true;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetScale() const
        {
            return mTransformation.GetScale();
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, Ref<const Vector3f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Axis));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, Ref<const Vector2f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0)));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetRotation(Ref<const Vector3f> Angles)
        {
            mTransformation.SetRotation(Quaternionf::FromEulerAngles(Angles));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Pitch, Real32 Yaw, Real32 Roll)
        {
            mTransformation.SetRotation(Quaternionf::FromEulerAngles(Pitch, Yaw, Roll));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetRotation(Ref<const Vector3f> Direction, Ref<const Vector3f> Up)
        {
            mTransformation.SetRotation(Quaternionf::FromDirection(Direction, Up));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void SetRotation(Ref<const Vector3f> Eye, Ref<const Vector3f> Focus, Ref<const Vector3f> Up)
        {
            mTransformation.SetRotation(Quaternionf::FromDirection(Focus - Eye, Up));
            mDirty = true;
        }

        // -=(Undocumented)=-
        Ref<const Quaternionf> GetRotation() const
        {
            return mTransformation.GetRotation();
        }

        // -=(Undocumented)=-
        void Translate(Ref<const Vector3f> Translation)
        {
            mTransformation.Translate(Translation);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Translate(Ref<const Vector2f> Translation)
        {
            mTransformation.Translate(Translation);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Translate(Vector3f(X, Y, Z));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y)
        {
            mTransformation.Translate(Vector2f(X, Y));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Scale(Ref<const Vector3f> Scale)
        {
            mTransformation.Scale(Scale);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Scale(Ref<const Vector2f> Scale)
        {
            mTransformation.Scale(Scale);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Scale(Vector3f(X, Y, Z));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y)
        {
            mTransformation.Scale(Vector2f(X, Y));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, Ref<const Vector3f> Axis)
        {
            mTransformation.Rotate(Angle, Axis);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, Ref<const Vector2f> Axis)
        {
            mTransformation.Rotate(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0));
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Rotate(Ref<const Vector3f> Angles)
        {
            mTransformation.Rotate(Angles);
            mDirty = true;
        }

        // -=(Undocumented)=-
        void Rotate(Ref<const Vector2f> Angles)
        {
            mTransformation.Rotate(Angles);
            mDirty = true;
        }

        // -=(Undocumented)=-
        Vector3f GetWorldCoordinates(Ref<const Vector3f> Position, Ref<const Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetWorldCoordinates(Ref<const Vector2f> Position, Ref<const Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector3f GetScreenCoordinates(Ref<const Vector3f> Position, Ref<const Rectf> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetScreenCoordinates(Ref<const Vector2f> Position, Ref<const Rectf> Viewport) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool       mDirty;
        Matrix4f   mProjection;
        Matrix4f   mView;
        Matrix4f   mWorld;
        Matrix4f   mInverse;
        Transformf mTransformation;
    };
}