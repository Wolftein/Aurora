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

#include "Common.hpp"
#include "Aurora.Math/Transform.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Represents a 3D camera with view and projection transforms.
    class Camera final
    {
    public:

        /// \brief Constructs a camera with identity view and projection.
        Camera();

        /// \brief Recalculates the camera's matrices if marked dirty.
        ///
        /// \return `true` if any matrix was updated, `false` otherwise.
        Bool Compute();

        /// \brief Returns the current projection of the camera.
        ///
        /// \return The projection matrix.
        AURORA_INLINE ConstRef<Matrix4x4> GetProjection() const
        {
            return mProjection;
        }

        /// \brief Returns the current view of the camera.
        ///
        /// \return The view matrix.
        AURORA_INLINE ConstRef<Matrix4x4> GetView() const
        {
            return mView;
        }

        /// \brief Returns the current view-projection of the camera.
        ///
        /// \return The view-projection matrix.
        AURORA_INLINE ConstRef<Matrix4x4> GetViewProjection() const
        {
            return mViewProjection;
        }

        /// \brief Returns the inverse of the view-projection matrix.
        ///
        /// \return The inverse view-projection matrix.
        AURORA_INLINE ConstRef<Matrix4x4> GetViewProjectionInverse() const
        {
            return mViewProjectionInverse;
        }

        /// \brief Gets the camera's right vector.
        ///
        /// \return The right direction vector in world space.
        AURORA_INLINE Vector3 GetRight() const
        {
            return mView.GetRight();
        }

        /// \brief Gets the camera's left vector.
        ///
        /// \return The left direction vector in world space.
        AURORA_INLINE Vector3 GetLeft() const
        {
            return mView.GetLeft();
        }

        /// \brief Gets the camera's up vector.
        ///
        /// \return The up direction vector in world space.
        AURORA_INLINE Vector3 GetUp() const
        {
            return mView.GetUp();
        }

        /// \brief Gets the camera's down vector.
        ///
        /// \return The down direction vector in world space.
        AURORA_INLINE Vector3 GetDown() const
        {
            return mView.GetDown();
        }

        /// \brief Gets the camera's forward vector.
        ///
        /// \return The forward direction vector in world space.
        AURORA_INLINE Vector3 GetForward() const
        {
            return mView.GetForward();
        }

        /// \brief Gets the camera's backward vector.
        ///
        /// \return The backward direction vector in world space.
        AURORA_INLINE Vector3 GetBackward() const
        {
            return mView.GetBackward();
        }

        /// \brief Sets the perspective projection matrix.
        ///
        /// \param Eyes   Field of view in radians.
        /// \param Aspect Aspect ratio (width / height).
        /// \param ZNear  Distance to the near clipping plane.
        /// \param ZFar   Distance to the far clipping plane.
        AURORA_INLINE void SetPerspective(Real32 Eyes, Real32 Aspect, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4x4::CreatePerspective(Eyes, Aspect, ZNear, ZFar);
            mDirty = SetBit(mDirty, kDirtyBitProjection);
        }

        /// \brief Sets the orthographic projection matrix using bounds.
        ///
        /// \param Left   Left clipping plane.
        /// \param Right  Right clipping plane.
        /// \param Bottom Bottom clipping plane.
        /// \param Top    Top clipping plane.
        /// \param ZNear  Near clipping plane.
        /// \param ZFar   Far clipping plane.
        AURORA_INLINE void SetOrthographic(Real32 Left, Real32 Right, Real32 Bottom, Real32 Top, Real32 ZNear, Real32 ZFar)
        {
            mProjection = Matrix4x4::CreateOrthographic(Left, Right, Bottom, Top, ZNear, ZFar);
            mDirty = SetBit(mDirty, kDirtyBitProjection);
        }

        /// \brief Sets an orthographic projection with given dimensions.
        ///
        /// \param Width  Viewport width.
        /// \param Height Viewport height.
        /// \param ZNear  Distance to near clipping plane.
        /// \param ZFar   Distance to far clipping plane.
        AURORA_INLINE void SetOrthographic(Real32 Width, Real32 Height, Real32 ZNear, Real32 ZFar)
        {
            SetOrthographic(0, Width, Height, 0, ZNear, ZFar);
        }

        /// \brief Sets the view matrix using eye position, target focus, and up direction.
        ///
        /// \param Eye   Camera position.
        /// \param Focus Target position the camera looks at.
        /// \param Up    Upward direction for orientation.
        AURORA_INLINE void SetLook(ConstRef<Vector3> Eye, ConstRef<Vector3> Focus, ConstRef<Vector3> Up)
        {
            mView = Matrix4x4::CreateLook(Eye, Focus, Up);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera translation.
        ///
        /// \param Translation Camera's 3D coordinate.
        AURORA_INLINE void SetTranslation(ConstRef<Vector3> Translation)
        {
            mTransform.SetTranslation(Translation);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera translation.
        ///
        /// \param Translation Camera's 2D coordinate.
        AURORA_INLINE void SetTranslation(ConstRef<Vector2> Translation)
        {
            mTransform.SetTranslation(Translation);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera translation.
        ///
        /// \param X Camera's X coordinate.
        /// \param Y Camera's Y coordinate.
        /// \param Z Camera's Z coordinate.
        AURORA_INLINE void SetTranslation(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.SetTranslation(Vector3(X, Y, Z));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera translation.
        ///
        /// \param X Camera's X coordinate.
        /// \param Y Camera's Y coordinate.
        AURORA_INLINE void SetTranslation(Real32 X, Real32 Y)
        {
            mTransform.SetTranslation(Vector2(X, Y));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Returns the current translation.
        ///
        /// \return The camera's 3D position.
        AURORA_INLINE ConstRef<Vector3> GetTranslation() const
        {
            return mTransform.GetTranslation();
        }

        /// \brief Sets the camera scale.
        ///
        /// \param Scale Camera's 3D scale factor.
        AURORA_INLINE void SetScale(ConstRef<Vector3> Scale)
        {
            mTransform.SetScale(Scale);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera scale.
        ///
        /// \param Scale Camera's 2D scale factor.
        AURORA_INLINE void SetScale(ConstRef<Vector2> Scale)
        {
            mTransform.SetScale(Scale);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera scale.
        ///
        /// \param X Camera's X scale factor.
        /// \param Y Camera's Y scale factor.
        /// \param Z Camera's Z scale factor.
        AURORA_INLINE void SetScale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.SetScale(Vector3(X, Y, Z));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera scale.
        ///
        /// \param X Camera's X scale factor.
        /// \param Y Camera's Y scale factor.
        AURORA_INLINE void SetScale(Real32 X, Real32 Y)
        {
            mTransform.SetScale(Vector2(X, Y));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Returns the current scale factor.
        ///
        /// \return The camera's 3D scale.
        AURORA_INLINE ConstRef<Vector3> GetScale() const
        {
            return mTransform.GetScale();
        }

        /// \brief Sets the camera rotation using an angle and axis.
        ///
        /// \param Angle Rotation angle in radians.
        /// \param Axis  Rotation axis.
        AURORA_INLINE void SetRotation(Real32 Angle, ConstRef<Vector3> Axis)
        {
            mTransform.SetRotation(Quaternion::FromAngles(Angle, Axis));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera rotation using an angle and axis.
        ///
        /// \param Angle Rotation angle in radians.
        /// \param Axis  2D axis (X, Y).
        AURORA_INLINE void SetRotation(Real32 Angle, ConstRef<Vector2> Axis)
        {
            mTransform.SetRotation(Quaternion::FromAngles(Angle, Vector3(Axis.GetX(), Axis.GetY(), 0)));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera rotation using Euler angles.
        ///
        /// \param Angles Euler angles in radians (pitch, yaw, roll).
        AURORA_INLINE void SetRotation(ConstRef<Vector3> Angles)
        {
            mTransform.SetRotation(Quaternion::FromEulerAngles(Angles));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera rotation using Euler angles.
        ///
        /// \param Pitch The pitch angle in radians.
        /// \param Yaw   The yaw angle in radians.
        /// \param Roll  The roll angle in radians.
        void SetRotation(Real32 Pitch, Real32 Yaw, Real32 Roll)
        {
            mTransform.SetRotation(Quaternion::FromEulerAngles(Vector3(Pitch, Yaw, Roll)));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera rotation from a direction vector and up vector.
        ///
        /// \param Direction Forward direction.
        /// \param Up        Upward vector.
        AURORA_INLINE void SetRotation(ConstRef<Vector3> Direction, ConstRef<Vector3> Up)
        {
            mTransform.SetRotation(Quaternion::FromDirection(Direction, Up));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Sets the camera rotation from look-at vectors.
        ///
        /// \param Eye   Camera position.
        /// \param Focus Look-at target.
        /// \param Up    Upward vector.
        AURORA_INLINE void SetRotation(ConstRef<Vector3> Eye, ConstRef<Vector3> Focus, ConstRef<Vector3> Up)
        {
            mTransform.SetRotation(Quaternion::FromDirection(Focus - Eye, Up));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Returns the current rotation.
        ///
        /// \return The camera's orientation.
        AURORA_INLINE ConstRef<Quaternion> GetRotation() const
        {
            return mTransform.GetRotation();
        }

        /// \brief Applies a relative translation to the camera.
        ///
        /// \param Translation 3D vector to translate.
        AURORA_INLINE void Translate(ConstRef<Vector3> Translation)
        {
            mTransform.Translate(Translation);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative translation to the camera.
        ///
        /// \param Translation 2D vector to translate.
        AURORA_INLINE void Translate(ConstRef<Vector2> Translation)
        {
            mTransform.Translate(Translation);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative translation to the camera.
        ///
        /// \param X X offset.
        /// \param Y Y offset.
        /// \param Z Z offset.
        AURORA_INLINE void Translate(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.Translate(Vector3(X, Y, Z));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative translation to the camera.
        ///
        /// \param X X offset.
        /// \param Y Y offset.
        AURORA_INLINE void Translate(Real32 X, Real32 Y)
        {
            mTransform.Translate(Vector2(X, Y));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative scaling to the camera.
        ///
        /// \param Scale 3D scale factor.
        AURORA_INLINE void Scale(ConstRef<Vector3> Scale)
        {
            mTransform.Scale(Scale);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative scaling to the camera.
        ///
        /// \param Scale 2D scale factor.
        AURORA_INLINE void Scale(ConstRef<Vector2> Scale)
        {
            mTransform.Scale(Scale);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative scaling to the camera.
        ///
        /// \param X Scale on X-axis.
        /// \param Y Scale on Y-axis.
        /// \param Z Scale on Z-axis.
        AURORA_INLINE void Scale(Real32 X, Real32 Y, Real32 Z)
        {
            mTransform.Scale(Vector3(X, Y, Z));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative scaling to the camera.
        ///
        /// \param X Scale on X-axis.
        /// \param Y Scale on Y-axis.
        AURORA_INLINE void Scale(Real32 X, Real32 Y)
        {
            mTransform.Scale(Vector2(X, Y));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative rotation to the camera.
        ///
        /// \param Angle Rotation angle.
        /// \param Axis  3D axis of rotation.
        AURORA_INLINE void Rotate(Real32 Angle, ConstRef<Vector3> Axis)
        {
            mTransform.Rotate(Angle, Axis);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies a relative rotation to the camera.
        ///
        /// \param Angle Rotation angle.
        /// \param Axis  2D axis.
        AURORA_INLINE void Rotate(Real32 Angle, ConstRef<Vector2> Axis)
        {
            mTransform.Rotate(Angle, Vector3(Axis.GetX(), Axis.GetY(), 0));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies relative Euler angle rotation.
        ///
        /// \param Angles Pitch, Yaw, Roll angles.
        AURORA_INLINE void Rotate(ConstRef<Vector3> Angles)
        {
            mTransform.Rotate(Angles);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies relative Euler angle rotation.
        ///
        /// \param Pitch The pitch angle in radians.
        /// \param Yaw   The yaw angle in radians.
        /// \param Roll  The roll angle in radians.
        AURORA_INLINE void Rotate(Real32 Pitch, Real32 Yaw, Real32 Roll)
        {
            mTransform.Rotate(Vector3(Pitch, Yaw, Roll));
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Applies relative Euler angle rotation.
        ///
        /// \param Angles 2D rotation angles (X, Y).
        AURORA_INLINE void Rotate(ConstRef<Vector2> Angles)
        {
            mTransform.Rotate(Angles);
            mDirty = SetBit(mDirty, kDirtyBitTransformation);
        }

        /// \brief Converts screen coordinates to world space.
        ///
        /// \param Translation 3D Screen position.
        /// \param Viewport    Viewport rectangle.
        /// \return Reconstructed world-space 3D position.
        Vector3 GetWorldCoordinates(ConstRef<Vector3> Translation, ConstRef<Viewport> Viewport) const;

        /// \brief Converts screen coordinates to world space.
        ///
        /// \param Translation 2D screen position.
        /// \param Viewport    Viewport rectangle.
        /// \return Reconstructed world-space 2D position.
        Vector2 GetWorldCoordinates(ConstRef<Vector2> Translation, ConstRef<Viewport> Viewport) const;

        /// \brief Converts world coordinates to screen space.
        ///
        /// \param Translation 3D World position.
        /// \param Viewport    Viewport rectangle.
        /// \return Projected screen-space 3D position.
        Vector3 GetScreenCoordinates(ConstRef<Vector3> Translation, ConstRef<Viewport> Viewport) const;

        /// \brief Converts world coordinates to screen space.
        ///
        /// \param Translation 2D world position.
        /// \param Viewport    Viewport rectangle.
        /// \return Projected screen-space 2D position.
        Vector2 GetScreenCoordinates(ConstRef<Vector2> Translation, ConstRef<Viewport> Viewport) const;

    private:

        static constexpr UInt32 kDirtyBitTransformation = 1 << 0;
        static constexpr UInt32 kDirtyBitProjection     = 1 << 1;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32    mDirty;
        Matrix4x4 mProjection;
        Matrix4x4 mView;
        Matrix4x4 mViewProjection;
        Matrix4x4 mViewProjectionInverse;
        Transform mTransform;
    };
}