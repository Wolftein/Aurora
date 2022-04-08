// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Common.hpp"
#include "Core/Math/Transform.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Camera
    {
    public:

        // -=(Undocumented)=-
        void Compute();

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

        // TODO Vector3f GetRight() const;
        // TODO Vector3f GetLeft() const;
        // TODO Vector3f GetUp() const;
        // TODO Vector3f GetForward() const;

        // -=(Undocumented)=-
        void SetPerspective(Real32 Eyes, Real32 Aspect, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4f::CreatePerspective(Eyes, Aspect, ZNear, ZFar);
        }

        // -=(Undocumented)=-
        void SetOrthographic(Real32 Left, Real32 Right, Real32 Bottom, Real32 Top, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4f::CreateOrthographic(Left, Right, Bottom, Top, ZNear, ZFar);
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
        }

        // -=(Undocumented)=-
        void SetPosition(Ref<const Vector2f> Position)
        {
            mTransformation.SetPosition(Position);
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetPosition(Vector3f(X, Y, Y));
        }

        // -=(Undocumented)=-
        void SetPosition(Real32 X, Real32 Y)
        {
            mTransformation.SetPosition(Vector2f(X, Y));
        }

        // -=(Undocumented)=-
        void SetScale(Ref<const Vector3f> Scale)
        {
            mTransformation.SetScale(Scale);
        }

        // -=(Undocumented)=-
        void SetScale(Ref<const Vector2f> Scale)
        {
            mTransformation.SetScale(Scale);
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.SetScale(Vector3f(X, Y, Y));
        }

        // -=(Undocumented)=-
        void SetScale(Real32 X, Real32 Y)
        {
            mTransformation.SetScale(Vector2f(X, Y));
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, Ref<const Vector3f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Axis));
        }

        // -=(Undocumented)=-
        void SetRotation(Real32 Angle, Ref<const Vector2f> Axis)
        {
            mTransformation.SetRotation(Quaternionf::FromAngles(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0)));
        }

        // -=(Undocumented)=-
        void SetRotation(Ref<const Vector3f> Angles)
        {
            mTransformation.SetRotation(Quaternionf::FromEulerAngles(Angles));
        }

        // -=(Undocumented)=-
        void Translate(Ref<const Vector3f> Translation)
        {
            mTransformation.Translate(Translation);
        }

        // -=(Undocumented)=-
        void Translate(Ref<const Vector2f> Translation)
        {
            mTransformation.Translate(Translation);
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Translate(Vector3f(X, Y, Z));
        }

        // -=(Undocumented)=-
        void Translate(Real32 X, Real32 Y)
        {
            mTransformation.Translate(Vector2f(X, Y));
        }

        // -=(Undocumented)=-
        void Scale(Ref<const Vector3f> Scale)
        {
            mTransformation.Scale(Scale);
        }

        // -=(Undocumented)=-
        void Scale(Ref<const Vector2f> Scale)
        {
            mTransformation.Scale(Scale);
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransformation.Scale(Vector3f(X, Y, Z));
        }

        // -=(Undocumented)=-
        void Scale(Real32 X, Real32 Y)
        {
            mTransformation.Scale(Vector2f(X, Y));
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, Ref<const Vector3f> Axis)
        {
            mTransformation.Rotate(Angle, Axis);
        }

        // -=(Undocumented)=-
        void Rotate(Real32 Angle, Ref<const Vector2f> Axis)
        {
            mTransformation.Rotate(Angle, Vector3f(Axis.GetX(), Axis.GetY(), 0));
        }

        // -=(Undocumented)=-
        void Rotate(Ref<const Vector3f> Angles)
        {
            mTransformation.Rotate(Angles);
        }

        // -=(Undocumented)=-
        void Rotate(Ref<const Vector2f> Angles)
        {
            mTransformation.Rotate(Angles);
        }

        // -=(Undocumented)=-
        Vector3f GetWorldCoordinates(Ref<const Vector3f> Position, Ref<const Rect> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetWorldCoordinates(Ref<const Vector2f> Position, Ref<const Rect> Viewport) const;

        // -=(Undocumented)=-
        Vector3f GetScreenCoordinates(Ref<const Vector3f> Position, Ref<const Rect> Viewport) const;

        // -=(Undocumented)=-
        Vector2f GetScreenCoordinates(Ref<const Vector2f> Position, Ref<const Rect> Viewport) const;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Matrix4f   mProjection;
        Matrix4f   mView;
        Matrix4f   mWorld;

        Transformf mTransformation;
    };
}