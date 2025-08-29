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

#include "Aurora.Math/Vector2.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents a 2D circle defined by a center point and radius.
    class Circle final
    {
    public:

        /// \brief Initializes the circle to center (0.0f, 0.0f) with radius 0.0f.
        AURORA_INLINE constexpr Circle()
            : mCenter { 0.0f, 0.0f },
              mRadius { 0.0f }
        {
        }

        /// \brief Constructor initializing the circle with specified center and radius.
        ///
        /// \param Center The center point of the circle.
        /// \param Radius The radius of the circle.
        AURORA_INLINE constexpr Circle(ConstRef<Vector2> Center, Real32 Radius)
            : mCenter { Center },
              mRadius { Radius }
        {
        }

        /// \brief Constructor initializing the circle with specified center coordinates and radius.
        ///
        /// \param X      The x-coordinate of the center.
        /// \param Y      The y-coordinate of the center.
        /// \param Radius The radius of the circle.
        AURORA_INLINE constexpr Circle(Real32 X, Real32 Y, Real32 Radius)
            : mCenter { X, Y },
              mRadius { Radius }
        {
        }

        /// \brief Checks if the circle is the zero circle.
        ///
        /// \return `true` if center is approximately zero and radius is approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostZero() const
        {
            return mCenter.IsAlmostZero() && Base::IsAlmostZero(mRadius);
        }

        /// \brief Checks if the circle is valid (radius >= 0).
        ///
        /// \return `true` if the circle is valid, `false` otherwise.
        AURORA_INLINE constexpr Bool IsValid() const
        {
            return mRadius >= 0.0f;
        }

        /// \brief Checks if the circle has zero area.
        ///
        /// \return `true` if the radius is approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsEmpty() const
        {
            return Base::IsAlmostZero(mRadius);
        }

        /// \brief Sets the center and radius of the circle.
        ///
        /// \param Center The new center point.
        /// \param Radius The new radius.
        AURORA_INLINE constexpr void Set(ConstRef<Vector2> Center, Real32 Radius)
        {
            mCenter = Center;
            mRadius = Radius;
        }

        /// \brief Sets the center of the circle.
        ///
        /// \param Center The new center point.
        AURORA_INLINE constexpr void SetCenter(ConstRef<Vector2> Center)
        {
            mCenter = Center;
        }

        /// \brief Gets the center of the circle.
        ///
        /// \return The center point of the circle.
        AURORA_INLINE constexpr ConstRef<Vector2> GetCenter() const
        {
            return mCenter;
        }

        /// \brief Sets the radius of the circle.
        ///
        /// \param Radius The new radius.
        AURORA_INLINE constexpr void SetRadius(Real32 Radius)
        {
            mRadius = Radius;
        }

        /// \brief Gets the radius of the circle.
        ///
        /// \return The radius of the circle.
        AURORA_INLINE constexpr Real32 GetRadius() const
        {
            return mRadius;
        }

        /// \brief Calculates the diameter of the circle.
        ///
        /// \return The diameter of the circle.
        AURORA_INLINE constexpr Real32 GetDiameter() const
        {
            return mRadius * 2.0f;
        }

        /// \brief Calculates the circumference of the circle.
        ///
        /// \return The circumference of the circle.
        AURORA_INLINE constexpr Real32 GetCircumference() const
        {
            return 2.0f * kPI<Real32> * mRadius;
        }

        /// \brief Calculates the area of the circle.
        ///
        /// \return The area of the circle.
        AURORA_INLINE constexpr Real32 GetArea() const
        {
            return kPI<Real32> * mRadius * mRadius;
        }

        /// \brief Calculates the distance from the circle's boundary to a point.
        ///
        /// \param Point The point to calculate distance to.
        /// \return The distance from the circle's boundary to the point (negative if inside the circle).
        AURORA_INLINE constexpr Real32 GetDistance(ConstRef<Vector2> Point) const
        {
            return mCenter.GetDistance(Point) - mRadius;
        }

        /// \brief Calculates the signed distance between this circle's boundary and another circle's boundary.
        ///
        /// \param Other The other circle to calculate distance to.
        /// \return The signed distance between boundaries (negative if circles overlap).
        AURORA_INLINE constexpr Real32 GetDistance(ConstRef<Circle> Other) const
        {
            return mCenter.GetDistance(Other.mCenter) - (mRadius + Other.mRadius);
        }

        /// \brief Expands the circle by the given amount.
        ///
        /// \param Amount The amount to expand by.
        /// \return The expanded circle.
        AURORA_INLINE constexpr Circle Expand(Real32 Amount) const
        {
            return Circle(mCenter, mRadius + Amount);
        }

        /// \brief Contracts the circle by the given amount.
        ///
        /// \param Amount The amount to contract by.
        /// \return The contracted circle.
        AURORA_INLINE constexpr Circle Contract(Real32 Amount) const
        {
            return Circle(mCenter, Base::Max(0.0f, mRadius - Amount));
        }

        /// \brief Checks if this circle completely contains another circle.
        ///
        /// \param Other The other circle to check.
        /// \return `true` if this circle contains the other, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<Circle> Other) const
        {
            const Real32 DistanceSquared = mCenter.GetDistanceSquared(Other.mCenter);
            return DistanceSquared <= ((mRadius - Other.mRadius) * (mRadius - Other.mRadius));
        }

        /// \brief Checks if this circle contains a point.
        ///
        /// \param Point The point to check.
        /// \return `true` if the point is inside the circle, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<Vector2> Point) const
        {
            return mCenter.GetDistanceSquared(Point) <= (mRadius * mRadius);
        }

        /// \brief Checks if this circle intersects with another circle.
        ///
        /// \param Other The other circle to check.
        /// \return `true` if the circles intersect, `false` otherwise.
        AURORA_INLINE constexpr Bool Intersects(ConstRef<Circle> Other) const
        {
            const Real32 DistanceSquared = mCenter.GetDistanceSquared(Other.mCenter);
            return DistanceSquared <= ((mRadius + Other.mRadius) * 2);
        }

        /// \brief Checks if this circle is equal to another circle.
        ///
        /// \param Other The circle to compare to.
        /// \return `true` if center and radius are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Circle> Other) const
        {
            return mCenter == Other.mCenter && Base::IsAlmostEqual(mRadius, Other.mRadius);
        }

        /// \brief Checks if this circle is not equal to another circle.
        ///
        /// \param Other The circle to compare to.
        /// \return `true` if the circles are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Circle> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Adds a vector to the circle's center.
        ///
        /// \param Vector The vector to add.
        /// \return A new circle with translated center.
        AURORA_INLINE constexpr Circle operator+(ConstRef<Vector2> Vector) const
        {
            return Circle(mCenter + Vector, mRadius);
        }

        /// \brief Subtracts a vector from the circle's center.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new circle with translated center.
        AURORA_INLINE constexpr Circle operator-(ConstRef<Vector2> Vector) const
        {
            return Circle(mCenter - Vector, mRadius);
        }

        /// \brief Multiplies the circle's radius by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A new circle with scaled radius.
        AURORA_INLINE constexpr Circle operator*(Real32 Scalar) const
        {
            return Circle(mCenter, mRadius * Scalar);
        }

        /// \brief Divides the circle's radius by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A new circle with scaled radius.
        AURORA_INLINE constexpr Circle operator/(Real32 Scalar) const
        {
            return Circle(mCenter, mRadius / Scalar);
        }

        /// \brief Adds a vector to the circle's center.
        ///
        /// \param Vector The vector to add.
        /// \return A reference to the updated circle.
        AURORA_INLINE constexpr Ref<Circle> operator+=(ConstRef<Vector2> Vector)
        {
            mCenter += Vector;
            return (* this);
        }

        /// \brief Subtracts a vector from the circle's center.
        ///
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated circle.
        AURORA_INLINE constexpr Ref<Circle> operator-=(ConstRef<Vector2> Vector)
        {
            mCenter -= Vector;
            return (* this);
        }

        /// \brief Multiplies the circle's radius by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A reference to the updated circle.
        AURORA_INLINE constexpr Ref<Circle> operator*=(Real32 Scalar)
        {
            mRadius *= Scalar;
            return (* this);
        }

        /// \brief Divides the circle's radius by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A reference to the updated circle.
        AURORA_INLINE constexpr Ref<Circle> operator/=(Real32 Scalar)
        {
            mRadius /= Scalar;
            return (* this);
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeObject(mCenter);
            Archive.SerializeReal32(mRadius);
        }

    public:

        /// \brief Returns the unit circle centered at origin with radius 1.0.
        ///
        /// \return A unit circle.
        AURORA_INLINE constexpr static Circle Unit()
        {
            return Circle(Vector2::Zero(), 1.0f);
        }

        /// \brief Canonicalizes a circle by ensuring its radius is non-negative.
        ///
        /// \param Circle The circle to canonicalize.
        /// \return A circle with non-negative radius.
        AURORA_INLINE constexpr static Circle Canonicalize(ConstRef<Circle> Circle)
        {
            return Math::Circle(Circle.mCenter, Base::Abs(Circle.mRadius));
        }

        /// \brief Returns the bounding circle of two circles.
        ///
        /// \param First  The first circle.
        /// \param Second The second circle.
        /// \return The bounding circle that contains both circles.
        AURORA_INLINE constexpr static Circle Merge(ConstRef<Circle> First, ConstRef<Circle> Second)
        {
            const Vector2 Direction = Second.mCenter - First.mCenter;
            const Real32 Distance   = Direction.GetLength();

            if (Distance + Second.mRadius <= First.mRadius)
            {
                return First;
            }

            if (Distance + First.mRadius <= Second.mRadius)
            {
                return Second;
            }

            const Real32 Radius = (Distance + First.mRadius + Second.mRadius) * 0.5f;
            return Circle(First.mCenter + Direction * ((Radius - First.mRadius) / Distance), Radius);
        }

        /// \brief Linearly interpolates between two circles.
        ///
        /// \param Start      The starting circle.
        /// \param End        The ending circle.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A rectangle interpolated between the start and end circle.
        AURORA_INLINE constexpr static Circle Lerp(ConstRef<Circle> Start, ConstRef<Circle> End, Real32 Percentage)
        {
            const Real32 Radius = Base::Lerp(Start.mRadius, End.mRadius, Percentage);
            return Circle(Vector2::Lerp(Start.mCenter, End.mCenter, Percentage), Radius);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2 mCenter;
        Real32  mRadius;
    };
}