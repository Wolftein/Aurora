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

#include "Matrix4x4.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents position, rotation, and scale of an object in space.
    class Transform final
    {
    public:
        /// \brief Identity transform.
        AURORA_INLINE Transform()
            : mScale { Vector3::One() }
        {
        }

        /// \brief Copy constructor.
        AURORA_INLINE Transform(ConstRef<Transform> Other) = default;

        /// \brief Transform from translation, scale, and rotation.
        ///
        /// \param Translation Position vector.
        /// \param Scale Scale vector.
        /// \param Rotation Rotation quaternion.
        AURORA_INLINE Transform(ConstRef<Vector3> Translation, ConstRef<Vector3> Scale, ConstRef<Quaternion> Rotation)
        {
            SetTranslation(Translation);
            SetScale(Scale);
            SetRotation(Rotation);
        }

        /// \brief Transform from 2D translation, scale, and rotation.
        ///
        /// \param Translation Position vector (2D).
        /// \param Scale Scale vector (2D).
        /// \param Rotation Rotation quaternion.
        AURORA_INLINE Transform(ConstRef<Vector2> Translation, ConstRef<Vector2> Scale, ConstRef<Quaternion> Rotation)
        {
            SetTranslation(Translation);
            SetScale(Scale);
            SetRotation(Rotation);
        }

        /// \brief Transform from translation and rotation, default scale.
        ///
        /// \param Translation Position vector.
        /// \param Rotation Rotation quaternion.
        AURORA_INLINE Transform(ConstRef<Vector3> Translation, ConstRef<Quaternion> Rotation)
            : Transform()
        {
            SetTranslation(Translation);
            SetRotation(Rotation);
        }

        /// \brief Transform from 2D translation and rotation, default scale.
        ///
        /// \param Translation Position vector (2D).
        /// \param Rotation Rotation quaternion.
        AURORA_INLINE Transform(ConstRef<Vector2> Translation, ConstRef<Quaternion> Rotation)
            : Transform()
        {
            SetTranslation(Translation);
            SetRotation(Rotation);
        }

        /// \brief Transform from translation only, default scale and rotation.
        ///
        /// \param Translation Position vector.
        AURORA_INLINE explicit Transform(ConstRef<Vector3> Translation)
            : Transform()
        {
            SetTranslation(Translation);
        }

        /// \brief Transform from 2D translation only, default scale and rotation.
        ///
        /// \param Translation Position vector (2D).
        AURORA_INLINE explicit Transform(ConstRef<Vector2> Translation)
            : Transform()
        {
            SetTranslation(Translation);
        }

        /// \brief Computes the final transformation matrix.
        /// 
        /// \return A 4x4 transformation matrix that combines translation, scale, and rotation.
        AURORA_INLINE Matrix4x4 Compute() const
        {
            return Matrix4x4::FromTransform(mTranslation, mScale, mRotation);
        }

        /// \brief Sets the translation vector for the transform.
        /// 
        /// \param Translation The translation vector to set (2D).
        /// \return A reference to this transform, modified by the new translation.
        AURORA_INLINE Ref<Transform> SetTranslation(ConstRef<Vector2> Translation)
        {
            mTranslation = Vector3(Translation.GetX(), Translation.GetY(), 0.0f);
            return (* this);
        }

        /// \brief Sets the translation vector for the transform in 3D space.
        /// 
        /// \param Translation The translation vector to set (3D).
        /// \return A reference to this transform, modified by the new translation.
        AURORA_INLINE Ref<Transform> SetTranslation(ConstRef<Vector3> Translation)
        {
            mTranslation = Translation;
            return (* this);
        }

        /// \brief Gets the current translation of the transform.
        /// 
        /// \return A reference to the translation vector (4D).
        AURORA_INLINE ConstRef<Vector3> GetTranslation() const
        {
            return mTranslation;
        }

        /// \brief Sets the scale vector for the transform in 2D space (Z scale = 1.0f).
        /// 
        /// \param Scale The scale vector to set (2D).
        /// \return A reference to this transform, modified by the new scale.
        AURORA_INLINE Ref<Transform> SetScale(ConstRef<Vector2> Scale)
        {
            return SetScale(Vector3(Scale.GetX(), Scale.GetY(), 1.0f));
        }

        /// \brief Sets the scale vector for the transform.
        /// 
        /// \param Scale The scale vector to set (3D).
        /// \return A reference to this transform, modified by the new scale.
        AURORA_INLINE Ref<Transform> SetScale(ConstRef<Vector3> Scale)
        {
            mScale = Scale;
            return (* this);
        }

        /// \brief Gets the current scale of the transform.
        /// 
        /// \return A reference to the scale vector.
        AURORA_INLINE ConstRef<Vector3> GetScale() const
        {
            return mScale;
        }

        /// \brief Sets the rotation for the transform using a quaternion.
        /// 
        /// \param Rotation The quaternion representing the rotation.
        /// \return A reference to this transform, modified by the new rotation.
        AURORA_INLINE Ref<Transform> SetRotation(ConstRef<Quaternion> Rotation)
        {
            mRotation = Rotation;
            return (* this);
        }

        /// \brief Gets the current rotation of the transform.
        /// 
        /// \return A reference to the quaternion representing the rotation.
        AURORA_INLINE ConstRef<Quaternion> GetRotation() const
        {
            return mRotation;
        }

        /// \brief Translates the transform by the given 2D vector.
        /// 
        /// \param Translation The translation vector to apply (2D).
        /// \return A reference to this transform, modified by the translation.
        AURORA_INLINE Ref<Transform> Translate(ConstRef<Vector2> Translation)
        {
            return Translate(Vector3(Translation.GetX(), Translation.GetY(), 0));
        }

        /// \brief Translates the transform by the given 3D vector.
        /// 
        /// \param Translation The translation vector to apply (3D).
        /// \return A reference to this transform, modified by the translation.
        AURORA_INLINE Ref<Transform> Translate(ConstRef<Vector3> Translation)
        {
            mTranslation += Translation;
            return (* this);
        }

        /// \brief Scales the transform by a scalar value.
        /// 
        /// \param Scalar The scalar value to apply as a scale factor.
        /// \return A reference to this transform, modified by the scale.
        AURORA_INLINE Ref<Transform> Scale(Real32 Scalar)
        {
            mScale *= Scalar;
            return (* this);
        }

        /// \brief Scales the transform by a 2D vector (Z scale = 1.0f).
        /// 
        /// \param Vector The 2D vector to scale the transform by.
        /// \return A reference to this transform, modified by the scale.
        AURORA_INLINE Ref<Transform> Scale(ConstRef<Vector2> Vector)
        {
            return Scale(Vector3(Vector.GetX(), Vector.GetY(), 1.0f));
        }

        /// \brief Scales the transform by a 3D vector.
        /// 
        /// \param Vector The 3D vector to scale the transform by.
        /// \return A reference to this transform, modified by the scale.
        AURORA_INLINE Ref<Transform> Scale(ConstRef<Vector3> Vector)
        {
            mScale *= Vector;
            return (* this);
        }

        /// \brief Rotates the transform by the specified quaternion.
        /// 
        /// \param Rotation The quaternion representing the rotation to apply.
        /// \return A reference to this transform, modified by the rotation.
        AURORA_INLINE Ref<Transform> Rotate(ConstRef<Quaternion> Rotation)
        {
            mRotation = mRotation * Rotation;
            return (* this);
        }

        /// \brief Rotates the transform by pitch and yaw (using Euler angles).
        /// 
        /// \param Angles The 2D vector containing pitch and yaw (X = pitch, Y = yaw).
        /// \return A reference to this transform, modified by the rotation.
        AURORA_INLINE Ref<Transform> Rotate(ConstRef<Vector2> Angles)
        {
            mRotation = mRotation * Quaternion::FromEulerAngles(Vector3(Angles.GetX(), Angles.GetY(), 0.0f));
            return (* this);
        }

        /// \brief Rotates the transform by using Euler angles.
        /// 
        /// \param Angles The 3D vector containing Euler angles for rotation.
        /// \return A reference to this transform, modified by the rotation.
        AURORA_INLINE Ref<Transform> Rotate(ConstRef<Vector3> Angles)
        {
            mRotation = mRotation * Quaternion::FromEulerAngles(Angles);
            return (*this);
        }

        /// \brief Rotates the transform around a specified axis by a given angle.
        /// 
        /// \param Angle The rotation angle in radians.
        /// \param Axis  The axis around which to rotate (representing the direction of the axis).
        /// 
        /// \return A reference to this transform, modified by the rotation.
        AURORA_INLINE Ref<Transform> Rotate(Real32 Angle, ConstRef<Vector3> Axis)
        {
            mRotation = mRotation * Quaternion::FromAngles(Angle, Axis);
            return (*this);
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        /// 
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeObject(mTranslation);
            Archive.SerializeObject(mScale);
            Archive.SerializeObject(mRotation);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3    mTranslation;
        Vector3    mScale;
        Quaternion mRotation;
    };
}