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
    /// \brief Represents a 2D vector with floating point components (x, y).
    class Vector2 final
    {
    public:

        /// \brief Default constructor.
        ///
        /// Initializes the vector to (0.0f, 0.0f).
        AURORA_INLINE constexpr Vector2()
            : mX { 0 },
              mY { 0 }
        {
        }

        /// \brief Constructor initializing all components to the same value.
        ///
        /// \param Scalar The scalar value used to initialize all components.
        AURORA_INLINE constexpr explicit Vector2(Real32 Scalar)
            : mX { Scalar },
              mY { Scalar }
        {
        }

        /// \brief Constructor initializing the vector with specified x and y values.
        ///
        /// \param X The x-component of the vector.
        /// \param Y The y-component of the vector.
        AURORA_INLINE constexpr Vector2(Real32 X, Real32 Y)
            : mX { X },
              mY { Y }
        {
        }

        /// \brief Copy constructor.
        ///
        /// \param Other The vector to copy.
        AURORA_INLINE constexpr Vector2(ConstRef<Vector2> Other)
            : mX { Other.GetX() },
              mY { Other.GetY() }
        {
        }

        /// \brief Checks if the vector is the zero vector.
        ///
        /// \return `true` if all components are approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostZero() const
        {
            return Base::IsAlmostZero(mX) && Base::IsAlmostZero(mY);
        }

        /// \brief Checks if all components are approximately equal to the given values.
        ///
        /// \param X Value to compare with the X component.
        /// \param Y Value to compare with the Y component.
        /// \return `true` if all components are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostEqual(Real32 X, Real32 Y) const
        {
            return Base::IsAlmostEqual(mX, X) && Base::IsAlmostEqual(mY, Y);
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
            return (Abs(GetX() - GetY()) <= Tolerance);
        }

        /// \brief Sets the components of the vector to new values.
        ///
        /// \param X New x value.
        /// \param Y New y value.
        /// \return A reference to this vector after the update.
        AURORA_INLINE constexpr Ref<Vector2> Set(Real32 X, Real32 Y)
        {
            mX = X;
            mY = Y;
            return (*this);
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
            return (mX * mX + mY * mY);
        }

        /// \brief Calculates the distance between this vector and another vector.
        ///
        /// \param Vector The vector to calculate the distance to.
        /// \return The distance between the two vectors.
        AURORA_INLINE constexpr Real32 GetDistance(ConstRef<Vector2> Vector) const
        {
            return (*this - Vector).GetLength();
        }

        /// \brief Calculates the squared distance between this vector and another vector.
        ///
        /// \param Vector The vector to calculate the squared distance to.
        /// \return The squared distance between the two vectors.
        AURORA_INLINE constexpr Real32 GetDistanceSquared(ConstRef<Vector2> Vector) const
        {
            return (*this - Vector).GetLengthSquared();
        }

        /// \brief Calculates the angle of the vector relative to the x-axis.
        ///
        /// \return The angle in radians (range [-π, π]).
        AURORA_INLINE constexpr Real32 GetAngle() const
        {
            return InvTan(mY, mX);
        }

        /// \brief Returns the angle in radians between this vector and another.
        ///
        /// \param Other The other vector to compare with.
        /// \return Angle in radians between the two vectors (range [0, π]).
        AURORA_INLINE constexpr Real32 GetAngle(ConstRef<Vector2> Other) const
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
        AURORA_INLINE constexpr Vector2 operator+(ConstRef<Vector2> Vector) const
        {
            return Vector2(mX + Vector.mX, mY + Vector.mY);
        }

        /// \brief Adds a scalar to all components of this vector.
        ///
        /// \param Scalar The scalar to add.
        /// \return A new vector with the scalar added to all components.
        AURORA_INLINE constexpr Vector2 operator+(Real32 Scalar) const
        {
            return Vector2(mX + Scalar, mY + Scalar);
        }

        /// \brief Negates the vector.
        ///
        /// \return A new vector that is the negation of this vector.
        AURORA_INLINE constexpr Vector2 operator-() const
        {
            return Vector2(-mX, -mY);
        }

        /// \brief Subtracts another vector from this vector.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new vector that is the difference of the two vectors.
        AURORA_INLINE constexpr Vector2 operator-(ConstRef<Vector2> Vector) const
        {
            return Vector2(mX - Vector.mX, mY - Vector.mY);
        }

        /// \brief Subtracts a scalar from all components of this vector.
        ///
        /// \param Scalar The scalar to subtract.
        /// \return A new vector with the scalar subtracted from all components.
        AURORA_INLINE constexpr Vector2 operator-(Real32 Scalar) const
        {
            return Vector2(mX - Scalar, mY - Scalar);
        }

        /// \brief Multiplies all components of this vector by another vector.
        ///
        /// \param Vector The vector to multiply by.
        /// \return A new vector that is the product of the two vectors.
        AURORA_INLINE constexpr Vector2 operator*(ConstRef<Vector2> Vector) const
        {
            return Vector2(mX * Vector.mX, mY * Vector.mY);
        }

        /// \brief Multiplies all components of this vector by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A new vector with the scalar multiplied by all components.
        AURORA_INLINE constexpr Vector2 operator*(Real32 Scalar) const
        {
            return Vector2(mX * Scalar, mY * Scalar);
        }

        /// \brief Divides all components of this vector by another vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A new vector that is the quotient of the two vectors.
        AURORA_INLINE constexpr Vector2 operator/(ConstRef<Vector2> Vector) const
        {
            return Vector2(mX / Vector.mX, mY / Vector.mY);
        }

        /// \brief Divides all components of this vector by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A new vector with the scalar divided into all components.
        AURORA_INLINE constexpr Vector2 operator/(Real32 Scalar) const
        {
            return Vector2(mX / Scalar, mY / Scalar);
        }

        /// \brief Pre-increments all components of the vector.
        ///
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator++()
        {
            ++mX;
            ++mY;
            return (*this);
        }

        /// \brief Pre-decrements all components of the vector.
        ///
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator--()
        {
            --mX;
            --mY;
            return (*this);
        }

        /// \brief Adds another vector to the current vector.
        ///
        /// \param Vector The vector to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator+=(ConstRef<Vector2> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            return (*this);
        }

        /// \brief Adds a scalar value to all components of the vector.
        ///
        /// \param Scalar The scalar value to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator+=(Real32 Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            return (*this);
        }

        /// \brief Subtracts another vector from the current vector.
        ///
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator-=(ConstRef<Vector2> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            return (*this);
        }

        /// \brief Subtracts a scalar value from all components of the vector.
        ///
        /// \param Scalar The scalar value to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator-=(Real32 Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            return (*this);
        }

        /// \brief Multiplies all components of the vector by another vector.
        ///
        /// \param Vector The vector to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator*=(ConstRef<Vector2> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            return (*this);
        }

        /// \brief Multiplies all components of the vector by a scalar value.
        ///
        /// \param Scalar The scalar value to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator*=(Real32 Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            return (*this);
        }

        /// \brief Divides all components of the vector by another vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator/=(ConstRef<Vector2> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            return (*this);
        }

        /// \brief Divides all components of the vector by a scalar value.
        ///
        /// \param Scalar The scalar value to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE constexpr Ref<Vector2> operator/=(Real32 Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            return (*this);
        }

        /// \brief Checks if this vector is equal to another vector.
        ///
        /// \param Vector The vector to compare to.
        /// \return `true` if all components are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Vector2> Vector) const
        {
            return IsAlmostEqual(mX, Vector.mX) && IsAlmostEqual(mY, Vector.mY);
        }

        /// \brief Checks if this vector is not equal to another vector.
        ///
        /// \param Other The vector to compare to.
        /// \return `true` if the vectors are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Vector2> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Compares this vector with another for less-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator<(ConstRef<Vector2> Vector) const
        {
            return mX < Vector.mX && mY < Vector.mY;
        }

        /// \brief Compares this vector with another for less-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator<=(ConstRef<Vector2> Vector) const
        {
            return mX <= Vector.mX && mY <= Vector.mY;
        }

        /// \brief Compares this vector with another for greater-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator>(ConstRef<Vector2> Vector) const
        {
            return mX > Vector.mX && mY > Vector.mY;
        }

        /// \brief Compares this vector with another for greater-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE constexpr Bool operator>=(ConstRef<Vector2> Vector) const
        {
            return mX >= Vector.mX && mY >= Vector.mY;
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeReal32(mX);
            Archive.SerializeReal32(mY);
        }

    public:

        /// \brief Returns the zero vector (0.0f, 0.0f).
        ///
        /// \return The zero vector.
        AURORA_INLINE constexpr static Vector2 Zero()
        {
            return Vector2();
        }

        /// \brief Returns the one vector (1.0f, 1.0f).
        ///
        /// \return The one vector.
        AURORA_INLINE constexpr static Vector2 One()
        {
            return Vector2(1.0f, 1.0f);
        }

        /// \brief Returns the unit vector along the X-axis (1.0f, 0.0f).
        ///
        /// \return The unit vector along the X-axis.
        AURORA_INLINE constexpr static Vector2 UnitX()
        {
            return Vector2(1.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the Y-axis (0.0f, 1.0f).
        ///
        /// \return The unit vector along the Y-axis.
        AURORA_INLINE constexpr static Vector2 UnitY()
        {
            return Vector2(0.0f, 1.0f);
        }

        /// \brief Returns a vector perpendicular to the given vector.
        ///
        /// \param Vector The vector to compute the perpendicular of.
        /// \return A vector perpendicular to the input vector.
        AURORA_INLINE constexpr static Vector2 Perpendicular(ConstRef<Vector2> Vector)
        {
            return Vector2(-Vector.GetY(), Vector.GetX());
        }

        /// \brief Projects the source vector onto the target vector.
        ///
        /// \param Source The vector to be projected.
        /// \param Target The vector onto which the source is projected.
        /// \return The projection of source onto target.
        AURORA_INLINE constexpr static Vector2 Project(ConstRef<Vector2> Source, ConstRef<Vector2> Target)
        {
            return Target * (Dot(Source, Target) / Dot(Target, Target));
        }

        /// \brief Normalizes the given vector.
        ///
        /// \param Vector The vector to normalize.
        /// \return A normalized vector, or the original if its length is too small.
        AURORA_INLINE constexpr static Vector2 Normalize(ConstRef<Vector2> Vector)
        {
            if (const Real32 Length = Vector.GetLength(); Length > kEpsilon<Real32>)
            {
                return Vector * (1.0f / Length);
            }
            return Vector;
        }

        /// \brief Reflects the incident vector over the given normal.
        ///
        /// \param Incident The incoming vector to reflect.
        /// \param Normal   The surface normal to reflect across (should be normalized).
        /// \return The reflected vector.
        AURORA_INLINE constexpr static Vector2 Reflect(ConstRef<Vector2> Incident, ConstRef<Vector2> Normal)
        {
            return Incident - Normal * (2.0f * Dot(Incident, Normal));
        }

        /// \brief Rotates a vector by a given angle in radians.
        ///
        /// \param Vector The vector to rotate.
        /// \param Angles The angle in radians to rotate by (counter-clockwise).
        /// \return A new rotated vector.
        AURORA_INLINE constexpr static Vector2 Rotate(ConstRef<Vector2> Vector, Real32 Angles)
        {
            const Real32 C = Cos(Angles);
            const Real32 S = Sin(Angles);
            return Vector2(Vector.GetX() * C - Vector.GetY() * S, Vector.GetX() * S + Vector.GetY() * C);
        }

        /// \brief Computes the dot product of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The dot product of the two vectors.
        AURORA_INLINE constexpr static Real32 Dot(ConstRef<Vector2> P0, ConstRef<Vector2> P1)
        {
            return (P0.GetX() * P1.GetX()) + (P0.GetY() * P1.GetY());
        }

        /// \brief Computes the cross product of two vectors (scalar in 2D).
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The scalar cross product of the two vectors.
        AURORA_INLINE constexpr static Real32 Cross(ConstRef<Vector2> P0, ConstRef<Vector2> P1)
        {
            return P0.GetX() * P1.GetY() - P0.GetY() * P1.GetX();
        }

        /// \brief Computes the 2D equivalent of a scalar triple product for three points.
        ///
        /// This calculates the signed area of the parallelogram formed by (P1-P0) and (P2-P0).
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \param P2 The third vector.
        /// \return The scalar triple product value.
        AURORA_INLINE constexpr static Real32 Cross(ConstRef<Vector2> P0, ConstRef<Vector2> P1, ConstRef<Vector2> P2)
        {
            return (P0.GetX() - P2.GetX()) * (P1.GetY() - P2.GetY()) - (P0.GetY() - P2.GetY()) * (P1.GetX() - P2.GetX());
        }

        /// \brief Returns the component-wise minimum of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise minimum values.
        AURORA_INLINE constexpr static Vector2 Min(ConstRef<Vector2> P0, ConstRef<Vector2> P1)
        {
            return Vector2(Base::Min(P0.mX, P1.mX), Base::Min(P0.mY, P1.mY));
        }

        /// \brief Returns the component-wise maximum of two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise maximum values.
        AURORA_INLINE constexpr static Vector2 Max(ConstRef<Vector2> P0, ConstRef<Vector2> P1)
        {
            return Vector2(Base::Max(P0.mX, P1.mX), Base::Max(P0.mY, P1.mY));
        }

        /// \brief Floors each component of the given vector.
        ///
        /// \param Vector The vector to floor.
        /// \return A vector with floored components.
        AURORA_INLINE constexpr static Vector2 Floor(ConstRef<Vector2> Vector)
        {
            return Vector2(Base::Floor(Vector.mX), Base::Floor(Vector.mY));
        }

        /// \brief Ceils each component of the given vector.
        ///
        /// \param Vector The vector to ceil.
        /// \return A vector with ceiled components.
        AURORA_INLINE constexpr static Vector2 Ceil(ConstRef<Vector2> Vector)
        {
            return Vector2(Base::Ceil(Vector.mX), Base::Ceil(Vector.mY));
        }

        /// \brief Linearly interpolates between two vectors.
        ///
        /// \param Start      The starting vector.
        /// \param End        The ending vector.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A vector interpolated between the start and end vectors.
        AURORA_INLINE constexpr static Vector2 Lerp(ConstRef<Vector2> Start, ConstRef<Vector2> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real32 mX;
        Real32 mY;
    };
}