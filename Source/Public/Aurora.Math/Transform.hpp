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

#include "Matrix4.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Transform final
    {
    public:

        // -=(Undocumented)=-
        Transform()
            : mScale { 1, 1, 1 }
        {
        }

        // -=(Undocumented)=-
        Transform(Ref<const Transform> Other)
            : mPosition { Other.GetPosition() },
              mRotation { Other.GetRotation() },
              mScale    { Other.GetScale() }
        {
        }

        // -=(Undocumented)=-
        Ref<Transform> SetPosition(Ref<const Vector2<Base>> Position)
        {
            return SetPosition(Vector3<Base>(Position.GetX(), Position.GetY(), 0));
        }

        // -=(Undocumented)=-
        Ref<Transform> SetPosition(Ref<const Vector3<Base>> Position)
        {
            mPosition = Position;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<const Vector3<Base>> GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetScale(Ref<const Vector2<Base>> Scale)
        {
            return SetScale(Vector3<Base>(Scale.GetX(), Scale.GetY(), 1));
        }

        // -=(Undocumented)=-
        Ref<Transform> SetScale(Ref<const Vector3<Base>> Scale)
        {
            mScale = Scale;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<const Vector3<Base>> GetScale() const
        {
            return mScale;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetRotation(Ref<const Quaternion<Base>> Rotation)
        {
            mRotation = Rotation;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<const Quaternion<Base>> GetRotation() const
        {
            return mRotation;
        }

        // -=(Undocumented)=-
        Ref<Transform> Translate(Ref<const Vector2<Base>> Translation)
        {
            return Translate(Vector3<Base>(Translation.GetX(), Translation.GetY(), 0));
        }

        // -=(Undocumented)=-
        Ref<Transform> Translate(Ref<const Vector3<Base>> Translation)
        {
            mPosition += Translation;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Scale(Base Scalar)
        {
            mScale *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Scale(Ref<const Vector2<Base>> Vector)
        {
            return Scale(Vector3<Base>(Vector.GetX(), Vector.GetY(), 1));
        }

        // -=(Undocumented)=-
        Ref<Transform> Scale(Ref<const Vector3<Base>> Vector)
        {
            mScale *= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(Ref<const Quaternion<Base>> Rotation)
        {
            mRotation = mRotation * Rotation;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(Ref<const Vector2<Base>> Angles)
        {
            const Quaternionf XAxis(Vector3<Base>(1, 0, 0), Angles.GetX());
            const Quaternionf YAxis(Vector3<Base>(0, 1, 0), Angles.GetY());

            mRotation = YAxis * mRotation * XAxis;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(Ref<const Vector3<Base>> Angles)
        {
            const Quaternionf XAxis(Vector3<Base>(1, 0, 0), Angles.GetX());
            const Quaternionf YAxis(Vector3<Base>(0, 1, 0), Angles.GetY());
            const Quaternionf ZAxis(Vector3<Base>(0, 0, 1), Angles.GetZ());

            mRotation = mRotation * XAxis * YAxis * ZAxis;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(Base Angle, Ref<const Vector3<Base>> Axis)
        {
            return Rotate(Quaternion<Base>::FromAngles(Angle, Axis));
        }

        // -=(Undocumented)=-
        Matrix4<Base> AsMatrix() const
        {
            const Matrix4<Base> Scale       = Matrix4<Base>::FromScale(mScale);
            const Matrix4<Base> Rotation    = Matrix4<Base>::FromRotation(mRotation);
            const Matrix4<Base> Translation = Matrix4<Base>::FromTranslation(mPosition);
            return Translation * Rotation * Scale;
        }

        // -=(Undocumented)=-
        Matrix4<Base> AsReverseMatrix() const
        {
            const Matrix4<Base> Scale       = Matrix4<Base>::FromScale(mScale);
            const Matrix4<Base> Rotation    = Matrix4<Base>::FromRotation(mRotation);
            const Matrix4<Base> Translation = Matrix4<Base>::FromTranslation(-mPosition);
            return Translation * Rotation * Scale;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3<Base>    mPosition;
        Vector3<Base>    mScale;
        Quaternion<Base> mRotation;
    };

    // -=(Undocumented)=-
    using Transformf = Transform<Real32>;

    // -=(Undocumented)=-
    using Transformd = Transform<Real64>;
}