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

#include "Aurora.Base/Scalar.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents a 3D vector with floating point components (x, y, z).
    class Vector3 final
    {
    public:

        /// \brief Default constructor.
        ///
        /// Initializes the vector to (0.0f, 0.0f, 0.0f).
        AURORA_INLINE constexpr Vector3()
            : mX { 0 },
              mY { 0 },
              mZ { 0 }
        {
        }

        /// \brief Constructor initializing all components to the same value.
        ///
        /// \param Scalar The scalar value used to initialize all components.
        AURORA_INLINE constexpr explicit Vector3(Real32 Scalar)
            : mX { Scalar },
              mY { Scalar },
              mZ { Scalar }
        {
        }

        /// \brief Constructor initializing the vector with specified x, y and z values.
        ///
        /// \param X The x-component of the vector.
        /// \param Y The y-component of the vector.
        /// \param Z The z-component of the vector.
        AURORA_INLINE constexpr Vector3(Real32 X, Real32 Y, Real32 Z)
            : mX { X },
              mY { Y },
              mZ { Z }
        {
        }

        /// \brief Copy constructor.
        ///
        /// \param Other The vector to copy.
        AURORA_INLINE constexpr Vector3(ConstRef<Vector3> Other)
            : mX { Other.GetX() },
              mY { Other.GetY() },
              mZ { Other.GetZ() }
        {
        }

        /// \brief Checks if the vector is the zero vector.
        ///
        /// \return `true` if all components are approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostZero() const
        {
            return Base::IsAlmostZero(mX) && Base::IsAlmostZero(mY) && Base::IsAlmostZero(mZ);
        }

        /// \brief Checks if all components are approximately equal to the given values.
        ///
        /// \param X Value to compare with the X component.
        /// \param Y Value to compare with the Y component.
        /// \param Z Value to compare with the Z component.
        /// \return `true` if all components are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostEqual(Real32 X, Real32 Y, Real32 Z) const
        {
            return Base::IsAlmostEqual(mX, X) && Base::IsAlmostEqual(mY, Y) && Base::IsAlmostEqual(mZ, Z);
        }

        /// \brief Checks if the vector is normalized.
        ///
        /// \return `true` if the vector is normalized, `false` otherwise.
        AURORA_INLINE constexpr Bool IsNormalized() const
        {
            return Base::IsAlmostEqual(GetLengthSquared(), 1.0f);
        }

        /// \brief Checks if all components are approximately equal within a tolerance.
        ///
        /// \param Tolerance Maximum allowed difference between components.
        /// \return `true` if all components are within \p Tolerance of each other, `false` otherwise.
        AURORA_INLINE constexpr Bool IsUniform(Real32 Tolerance = kEpsilon<Real32>) const
        {
            return (Abs(GetX() - GetY()) <= Tolerance) &&
                   (Abs(GetY() - GetZ()) <= Tolerance);
        }

        /// \brief Sets the components of the vector to new values.
        ///
        /// \param X New x value.
        /// \param Y New y value.
        /// \param Z New z value.
        /// \return A reference to this vector after the update.
        AURORA_INLINE constexpr Ref<Vector3> Set(Real32 X, Real32 Y, Real32 Z)
        {
            mX = X;
            mY = Y;
            mZ = Z;
            return (* this);
        }

        /// \brief Gets the x-component of the vector.
        ///
        /// \return The x-component.
        AURORA_INLINE constexpr Real32 GetX() const
        {
            return mX;
        }

        /// \brief Gets the y-component of the vector.
        ///
        /// \return The y-component.
        AURORA_INLINE constexpr Real32 GetY() const
        {
            return mY;
        }

        /// \brief Gets the z-component of the vector.
        ///
        /// \return The z-component.
        AURORA_INLINE constexpr Real32 GetZ() const
        {
            return mZ;
        }

        /// \brief Calculates the length (magnitude) of the vector.
        ///
        /// \return The length of the vector.
        AURORA_INLINE constexpr Real32 GetLength() const
        {
            return Sqrt(GetLengthSquared());
        }

        /// \brief Calculates the squared length of the vector.
        ///
        /// \return The squared length of the vector (no square root).
        AURORA_INLINE constexpr Real32 GetLengthSquared() const
        {
            return (mX * mX + mY * mY + mZ * mZ);
        }

        /// \brief Calculates the distance between this vector and another vector.
        ///
        /// \param Vector The vector to calculate the distance to.
        /// \return The distance between the two vectors.
        AURORA_INLINE constexpr Real32 GetDistance(ConstRef<Vector3> Vector) const
        {
            return (* this - Vector).GetLength();
        }

        /// \brief Calculates the squared distance between this vector and another vector.
        ///
        /// \param Vector The vector to calculate the squared distance to.
        /// \return The squared distance between the two vectors.
        AURORA_INLINE constexpr Real32 GetDistanceSquared(ConstRef<Vector3> Vector) const
        {
            return (* this - Vector).GetLengthSquared();
        }

        /// \brief Returns the angle in radians between this vector and another.
        ///
        /// \param Other The other vector to compare with.
        /// \return Angle in radians between the two vectors (range [0, π]).
        AURORA_INLINE constexpr Real32 GetAngle(ConstRef<Vector3> Other) const
        {
            if (const Real32 Length = GetLength() * Other.GetLength(); Length > kEpsilon<Real32>)
            {
                return InvCos(Dot(* this, Other) / Length);
            }
            return 0.0f;
        }

        /// \brief Adds another vector to this vector.
        ///
        /// \param Vector The vector to add.
        /// \return A new vector that is the sum of this vector and the input vector.
        AURORA_INLINE constexpr Vector3 operator+(ConstRef<Vector3> Vector) const
        {
            return Vector3(mX + Vector.mX, mY + Vector.mY, mZ + Vector.mZ);
        }

        /// \brief Adds a scalar to all components of this vector.
        ///
        /// \param Scalar The scalar to add.
        /// \return A new vector with the scalar added to all components.
        AURORA_INLINE constexpr Vector3 operator+(Real32 Scalar) const
        {
            return Vector3(mX + Scalar, mY + Scalar, mZ + Scalar);
        }

        /// \brief Negates the vector.
        ///
        /// \return A new vector that is the negation of this vector.
        AURORA_INLINE constexpr Vector3 operator-() const
        {
            return Vector3(-mX, -mY, -mZ);
        }

        /// \brief Subtracts another vector from this vector.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new vector that is the difference of the two vectors.
        AURORA_INLINE constexpr Vector3 operator-(ConstRef<Vector3> Vector) const
        {
            return Vector3(mX - Vector.mX, mY - Vector.mY, mZ - Vector.mZ);
        }

        /// \brief Subtracts a scalar from all components of this vector.
        ///
        /// \param Scalar The scalar to subtract.
        /// \return A new vector with the scalar subtracted from all components.
        AURORA_INLINE constexpr Vector3 operator-(Real32 Scalar) const
        {
            return Vector3(mX - Scalar, mY - Scalar, mZ - Scalar);
        }

        /// \brief Multiplies all components of this vector by another vector.
        ///
        /// \param Vector The vector to multiply by.
        /// \return A new vector that is the product of the two vectors.
        AURORA_INLINE constexpr Vector3 operator*(ConstRef<Vector3> Vector) const
        {
            return Vector3(mX * Vector.mX, mY * Vector.mY, mZ * Vector.mZ);
        }

        /// \brief Multiplies all components of this vector by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A new vector with the scalar multiplied by all components.
        AURORA_INLINE constexpr Vector3 operator*(Real32 Scalar) const
        {
            return Vector3(mX * Scalar, mY * Scalar, mZ * Scalar);
        }

        /// \brief Divides all components of this vector by another vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A new vector that is the quotient of the two vectors.
        AURORA_INLINE constexpr Vector3 operator/(ConstRef<Vector3> Vector) const
        {
            return Vector3(mX / Vector.mX, mY / Vector.mY, mZ / Vector.mZ);
        }

        /// \brief Divides all components of this vector by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A new vector with the scalar divided into all components.
        AURORA_INLINE constexpr Vector3 operator/(Real32 Scalar) const
        {
            return Vector3(mX / Scalar, mY / Scalar, mZ / Scalar);
        }

        /// \brief Pre-increments all components of the vector.
        ///
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            return (* this);
        }

        /// \brief Pre-decrements all components of the vector.
        ///
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator--()
        {
            --mX;
            --mY;
            --mZ;
            return (* this);
        }

        /// \brief Adds another vector to the current vector.
        ///
        /// \param Vector The vector to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator+=(ConstRef<Vector3> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            mZ += Vector.mZ;
            return (* this);
        }

        /// \brief Adds a scalar value to all components of the vector.
        ///
        /// \param Scalar The scalar value to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator+=(Real32 Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            mZ += Scalar;
            return (* this);
        }

        /// \brief Subtracts another vector from the current vector.
        ///
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator-=(ConstRef<Vector3> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            mZ -= Vector.mZ;
            return (* this);
        }

        /// \brief Subtracts a scalar value from all components of the vector.
        ///
        /// \param Scalar The scalar value to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator-=(Real32 Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            mZ -= Scalar;
            return (* this);
        }

        /// \brief Multiplies all components of the vector by another vector.
        ///
        /// \param Vector The vector to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator*=(ConstRef<Vector3> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            mZ *= Vector.mZ;
            return (* this);
        }

        /// \brief Multiplies all components of the vector by a scalar value.
        ///
        /// \param Scalar The scalar value to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator*=(Real32 Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            mZ *= Scalar;
            return (* this);
        }

        /// \brief Divides all components of the vector by another vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator/=(ConstRef<Vector3> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            mZ /= Vector.mZ;
            return (* this);
        }

        /// \brief Divides all components of the vector by a scalar value.
        ///
        /// \param Scalar The scalar value to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector3> operator/=(Real32 Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            mZ /= Scalar;
            return (* this);
        }

        /// \brief Checks if this vector is equal to another vector.
        ///
        /// \param Vector The vector to compare to.
        /// \return `true` if all components are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Vector3> Vector) const
        {
            return IsAlmostEqual(Vector.mX, Vector.mY, Vector.mZ);
        }

        /// \brief Checks if this vector is not equal to another vector.
        ///
        /// \param Other The vector to compare to.
        /// \return `true` if the vectors are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Vector3> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Compares this vector with another for less-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator<(ConstRef<Vector3> Vector) const
        {
            return mX < Vector.mX && mY < Vector.mY && mZ < Vector.mZ;
        }

        /// \brief Compares this vector with another for less-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator<=(ConstRef<Vector3> Vector) const
        {
            return mX <= Vector.mX && mY <= Vector.mY && mZ <= Vector.mZ;
        }

        /// \brief Compares this vector with another for greater-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator>(ConstRef<Vector3> Vector) const
        {
            return mX > Vector.mX && mY > Vector.mY && mZ > Vector.mZ;
        }

        /// \brief Compares this vector with another for greater-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator>=(ConstRef<Vector3> Vector) const
        {
            return mX >= Vector.mX && mY >= Vector.mY && mZ >= Vector.mZ;
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeReal32(mX);
            Archive.SerializeReal32(mY);
            Archive.SerializeReal32(mZ);
        }

    public:

        /// \brief Returns the zero vector (0.0f, 0.0f, 0.0f).
        ///
        /// \return The zero vector.
        AURORA_INLINE constexpr static Vector3 Zero()
        {
            return Vector3();
        }

        /// \brief Returns the one vector (1.0f, 1.0f, 1.0f).
        ///
        /// \return The one vector.
        AURORA_INLINE constexpr static Vector3 One()
        {
            return Vector3(1.0f, 1.0f, 1.0f);
        }

        /// \brief Returns the unit vector along the X-axis (1.0f, 0.0f, 0.0f).
        ///
        /// \return The unit vector along the X-axis.
        AURORA_INLINE constexpr static Vector3 UnitX()
        {
            return Vector3(1.0f, 0.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the Y-axis (0.0f, 1.0f, 0.0f).
        ///
        /// \return The unit vector along the Y-axis.
        AURORA_INLINE constexpr static Vector3 UnitY()
        {
            return Vector3(0.0f, 1.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the Z-axis (0.0f, 0.0f, 1.0f).
        ///
        /// \return The unit vector along the Z-axis.
        AURORA_INLINE constexpr static Vector3 UnitZ()
        {
            return Vector3(0.0f, 0.0f, 1.0f);
        }

        /// \brief Normalizes the given vector.
        ///
        /// \param Vector The vector to normalize.
        /// \return A normalized vector, or the original if its length is too small.
        AURORA_INLINE constexpr static Vector3 Normalize(ConstRef<Vector3> Vector)
        {
            if (const Real32 Length = Vector.GetLength(); Length > kEpsilon<Real32>)
            {
                return Vector * (1.0f / Length);
            }
            return Vector;
        }

        /// \brief Projects the source vector onto the target vector.
        ///
        /// \param Source The vector to be projected.
        /// \param Target The vector onto which the source is projected.
        /// \return The projection of source onto target.
        AURORA_INLINE constexpr static Vector3 Project(ConstRef<Vector3> Source, ConstRef<Vector3> Target)
        {
            return Target * (Dot(Source, Target) / Dot(Target, Target));
        }

        /// \brief Reflects the incident vector over the given normal.
        ///
        /// \param Incident The incoming vector to reflect.
        /// \param Normal   The surface normal to reflect across (should be normalized).
        /// \return The reflected vector.
        AURORA_INLINE constexpr static Vector3 Reflect(ConstRef<Vector3> Incident, ConstRef<Vector3> Normal)
        {
            return Incident - Normal * (2.0f * Dot(Incident, Normal));
        }

        /// \brief Computes the dot product of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The dot product of the two vectors.
        AURORA_INLINE constexpr static Real32 Dot(ConstRef<Vector3> P0, ConstRef<Vector3> P1)
        {
            return (P0.GetX() * P1.GetX()) + (P0.GetY() * P1.GetY()) + (P0.GetZ() * P1.GetZ());
        }

        /// \brief Computes the cross product of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The cross product of the two vectors.
        AURORA_INLINE constexpr static Vector3 Cross(ConstRef<Vector3> P0, ConstRef<Vector3> P1)
        {
            const Real32 X = P0.GetY() * P1.GetZ() - P0.GetZ() * P1.GetY();
            const Real32 Y = P0.GetZ() * P1.GetX() - P0.GetX() * P1.GetZ();
            const Real32 Z = P0.GetX() * P1.GetY() - P0.GetY() * P1.GetX();

            return Vector3(X, Y, Z);
        }

        /// \brief Returns the component-wise minimum of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise minimum values.
        AURORA_INLINE constexpr static Vector3 Min(ConstRef<Vector3> P0, ConstRef<Vector3> P1)
        {
            return Vector3(Base::Min(P0.mX, P1.mX), Base::Min(P0.mY, P1.mY), Base::Min(P0.mZ, P1.mZ));
        }

        /// \brief Returns the component-wise maximum of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise maximum values.
        AURORA_INLINE constexpr static Vector3 Max(ConstRef<Vector3> P0, ConstRef<Vector3> P1)
        {
            return Vector3(Base::Max(P0.mX, P1.mX), Base::Max(P0.mY, P1.mY), Base::Max(P0.mZ, P1.mZ));
        }

        /// \brief Floors each component of the given vector.
        ///
        /// \param Vector The vector to floor.
        /// \return A vector with floored components.
        AURORA_INLINE constexpr static Vector3 Floor(ConstRef<Vector3> Vector)
        {
            return Vector3(Base::Floor(Vector.mX), Base::Floor(Vector.mY), Base::Floor(Vector.mZ));
        }

        /// \brief Ceils each component of the given vector.
        ///
        /// \param Vector The vector to ceil.
        /// \return A vector with ceiled components.
        AURORA_INLINE constexpr static Vector3 Ceil(ConstRef<Vector3> Vector)
        {
            return Vector3(Base::Ceil(Vector.mX), Base::Ceil(Vector.mY), Base::Ceil(Vector.mZ));
        }

        /// \brief Linearly interpolates between two vectors.
        ///
        /// \param Start      The starting vector.
        /// \param End        The ending vector.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A vector interpolated between the start and end vectors.
        AURORA_INLINE constexpr static Vector3 Lerp(ConstRef<Vector3> Start, ConstRef<Vector3> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real32 mX;
        Real32 mY;
        Real32 mZ;
    };
}
