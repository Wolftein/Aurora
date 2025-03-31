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
            : mScale { 1.0, 1.0, 1.0 }
        {
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Transform> Other) = default;

        // -=(Undocumented)=-
        Transform(ConstRef<Vector3<Base>> Position, ConstRef<Vector3<Base>> Scale, ConstRef<Quaternion<Base>> Rotation)
        {
            SetPosition(Position);
            SetScale(Scale);
            SetRotation(Rotation);
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Vector2<Base>> Position, ConstRef<Vector2<Base>> Scale, ConstRef<Quaternion<Base>> Rotation)
        {
            SetPosition(Position);
            SetScale(Scale);
            SetRotation(Rotation);
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Vector3<Base>> Position, ConstRef<Quaternion<Base>> Rotation)
            : Transform()
        {
            SetPosition(Position);
            SetRotation(Rotation);
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Vector2<Base>> Position, ConstRef<Quaternion<Base>> Rotation)
            : Transform()
        {
            SetPosition(Position);
            SetRotation(Rotation);
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Vector3<Base>> Position)
            : Transform()
        {
            SetPosition(Position);
        }

        // -=(Undocumented)=-
        Transform(ConstRef<Vector2<Base>> Position)
            : Transform()
        {
            SetPosition(Position);
        }

        // -=(Undocumented)=-
        Matrix4<Base> Compute() const
        {
            const Matrix4<Base> Origin      = Matrix4<Base>::FromTranslation(-mOrigin);
            const Matrix4<Base> Scale       = Matrix4<Base>::FromScale(mScale);
            const Matrix4<Base> Rotation    = Matrix4<Base>::FromRotation(mRotation);
            const Matrix4<Base> Translation = Matrix4<Base>::FromTranslation(mOrigin + mPosition);

            return Translation * Rotation * Scale * Origin;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetOrigin(ConstRef<Vector2<Base>> Origin)
        {
            return SetOrigin(Vector3<Base>(Origin.GetX(), Origin.GetY(), 0));
        }

        // -=(Undocumented)=-
        Ref<Transform> SetOrigin(ConstRef<Vector3<Base>> Origin)
        {
            mOrigin = Origin;
            return (* this);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3<Base>> GetOrigin() const
        {
            return mOrigin;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetPosition(ConstRef<Vector2<Base>> Position)
        {
            return SetPosition(Vector3<Base>(Position.GetX(), Position.GetY(), 0));
        }

        // -=(Undocumented)=-
        Ref<Transform> SetPosition(ConstRef<Vector3<Base>> Position)
        {
            mPosition = Position;
            return (* this);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3<Base>> GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetScale(ConstRef<Vector2<Base>> Scale)
        {
            return SetScale(Vector3<Base>(Scale.GetX(), Scale.GetY(), 1));
        }

        // -=(Undocumented)=-
        Ref<Transform> SetScale(ConstRef<Vector3<Base>> Scale)
        {
            mScale = Scale;
            return (* this);
        }

        // -=(Undocumented)=-
        ConstRef<Vector3<Base>> GetScale() const
        {
            return mScale;
        }

        // -=(Undocumented)=-
        Ref<Transform> SetRotation(ConstRef<Quaternion<Base>> Rotation)
        {
            mRotation = Rotation;
            return (* this);
        }

        // -=(Undocumented)=-
        ConstRef<Quaternion<Base>> GetRotation() const
        {
            return mRotation;
        }

        // -=(Undocumented)=-
        Ref<Transform> Translate(ConstRef<Vector2<Base>> Translation)
        {
            return Translate(Vector3<Base>(Translation.GetX(), Translation.GetY(), 0));
        }

        // -=(Undocumented)=-
        Ref<Transform> Translate(ConstRef<Vector3<Base>> Translation)
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
        Ref<Transform> Scale(ConstRef<Vector2<Base>> Vector)
        {
            return Scale(Vector3<Base>(Vector.GetX(), Vector.GetY(), 1));
        }

        // -=(Undocumented)=-
        Ref<Transform> Scale(ConstRef<Vector3<Base>> Vector)
        {
            mScale *= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(ConstRef<Quaternion<Base>> Rotation)
        {
            mRotation = mRotation * Rotation;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(ConstRef<Vector2<Base>> Angles)
        {
            const Quaternionf XAxis(Quaternion<Base>::FromAngles(Angles.GetX(), Vector3<Base>(1, 0, 0)));
            const Quaternionf YAxis(Quaternion<Base>::FromAngles(Angles.GetY(), Vector3<Base>(0, 1, 0)));

            mRotation = mRotation * XAxis * YAxis;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(ConstRef<Vector3<Base>> Angles)
        {
            const Quaternionf XAxis(Quaternion<Base>::FromAngles(Angles.GetX(), Vector3<Base>(1, 0, 0)));
            const Quaternionf YAxis(Quaternion<Base>::FromAngles(Angles.GetY(), Vector3<Base>(0, 1, 0)));
            const Quaternionf ZAxis(Quaternion<Base>::FromAngles(Angles.GetZ(), Vector3<Base>(0, 0, 1)));

            mRotation = mRotation * XAxis * YAxis * ZAxis;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Transform> Rotate(Base Angle, ConstRef<Vector3<Base>> Axis)
        {
            return Rotate(Quaternion<Base>::FromAngles(Angle, Axis));
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeObject(mOrigin);
            Archive.SerializeObject(mPosition);
            Archive.SerializeObject(mScale);
            Archive.SerializeObject(mRotation);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3<Base>    mOrigin;
        Vector3<Base>    mPosition;
        Vector3<Base>    mScale;
        Quaternion<Base> mRotation;
    };

    // -=(Undocumented)=-
    using Transformf = Transform<Real32>;

    // -=(Undocumented)=-
    using Transformd = Transform<Real64>;
}