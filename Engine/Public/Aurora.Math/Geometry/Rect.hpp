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

#include "Aurora.Math/Matrix4x4.hpp"
#include "Aurora.Math/Pivot.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents a 2D rectangle defined by left, top, right, and bottom coordinates.
    class Rect final
    {
    public:

        /// \brief Initializes the rectangle to (0.0f, 0.0f, 0.0f, 0.0f).
        AURORA_INLINE constexpr Rect()
            : mX1 { 0 },
              mY1 { 0 },
              mX2 { 0 },
              mY2 { 0 }
        {
        }

        /// \brief Constructor initializing the rectangle with specified left, top, right, and bottom values.
        ///
        /// \param Left   The left coordinate of the rectangle.
        /// \param Top    The top coordinate of the rectangle.
        /// \param Right  The right coordinate of the rectangle.
        /// \param Bottom The bottom coordinate of the rectangle.
        AURORA_INLINE constexpr Rect(Real32 Left, Real32 Top, Real32 Right, Real32 Bottom)
            : mX1 { Left },
              mY1 { Top },
              mX2 { Right },
              mY2 { Bottom }
        {
        }

        /// \brief Constructor initializing the rectangle from minimum and maximum points.
        ///
        /// \param Minimum The minimum point (top-left) of the rectangle.
        /// \param Maximum The maximum point (bottom-right) of the rectangle.
        AURORA_INLINE constexpr Rect(ConstRef<Vector2> Minimum, ConstRef<Vector2> Maximum)
            : mX1 { Minimum.GetX() },
              mY1 { Minimum.GetY() },
              mX2 { Maximum.GetX() },
              mY2 { Maximum.GetY() }
        {
        }

        /// \brief Checks if the rectangle is the zero rectangle.
        ///
        /// \return `true` if all coordinates are approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostZero() const
        {
            return Base::IsAlmostZero(mX1) && Base::IsAlmostZero(mY1) && Base::IsAlmostZero(mX2) && Base::IsAlmostZero(mY2);
        }

        /// \brief Checks if the rectangle is valid (left <= right and top <= bottom).
        ///
        /// \return `true` if the rectangle is valid, `false` otherwise.
        AURORA_INLINE constexpr Bool IsValid() const
        {
            return mX1 <= mX2 && mY1 <= mY2;
        }

        /// \brief Checks if the rectangle has zero area.
        ///
        /// \return `true` if the rectangle has zero area, `false` otherwise.
        AURORA_INLINE constexpr Bool IsEmpty() const
        {
            return Base::IsAlmostEqual(mX1, mX2) || Base::IsAlmostEqual(mY1, mY2);
        }

        /// \brief Sets the coordinates of the rectangle to new values.
        ///
        /// \param Left   New left coordinate.
        /// \param Top    New top coordinate.
        /// \param Right  New right coordinate.
        /// \param Bottom New bottom coordinate.
        AURORA_INLINE constexpr void Set(Real32 Left, Real32 Top, Real32 Right, Real32 Bottom)
        {
            mX1 = Left;
            mY1 = Top;
            mX2 = Right;
            mY2 = Bottom;
        }

        /// \brief Sets the left coordinate of the rectangle.
        ///
        /// \param Value The new left coordinate.
        AURORA_INLINE constexpr void SetLeft(Real32 Value)
        {
            mX1 = Value;
        }

        /// \brief Gets the left coordinate of the rectangle.
        ///
        /// \return The left coordinate.
        AURORA_INLINE constexpr Real32 GetLeft() const
        {
            return mX1;
        }

        /// \brief Sets the top coordinate of the rectangle.
        ///
        /// \param Value The new top coordinate.
        AURORA_INLINE constexpr void SetTop(Real32 Value)
        {
            mY1 = Value;
        }

        /// \brief Gets the top coordinate of the rectangle.
        ///
        /// \return The top coordinate.
        AURORA_INLINE constexpr Real32 GetTop() const
        {
            return mY1;
        }

        /// \brief Sets the right coordinate of the rectangle.
        ///
        /// \param Value The new right coordinate.
        AURORA_INLINE constexpr void SetRight(Real32 Value)
        {
            mX2 = Value;
        }

        /// \brief Gets the right coordinate of the rectangle.
        ///
        /// \return The right coordinate.
        AURORA_INLINE constexpr Real32 GetRight() const
        {
            return mX2;
        }

        /// \brief Sets the bottom coordinate of the rectangle.
        ///
        /// \param Value The new bottom coordinate.
        AURORA_INLINE constexpr void SetBottom(Real32 Value)
        {
            mY2 = Value;
        }

        /// \brief Gets the bottom coordinate of the rectangle.
        ///
        /// \return The bottom coordinate.
        AURORA_INLINE constexpr Real32 GetBottom() const
        {
            return mY2;
        }

        /// \brief Gets the X coordinate of the rectangle (equivalent to left).
        ///
        /// \return The X coordinate.
        AURORA_INLINE constexpr Real32 GetX() const
        {
            return GetLeft();
        }

        /// \brief Gets the Y coordinate of the rectangle (equivalent to top).
        ///
        /// \return The Y coordinate.
        AURORA_INLINE constexpr Real32 GetY() const
        {
            return GetTop();
        }

        /// \brief Calculates the width of the rectangle.
        ///
        /// \return The width of the rectangle.
        AURORA_INLINE constexpr Real32 GetWidth() const
        {
            return GetRight() - GetLeft();
        }

        /// \brief Calculates the height of the rectangle.
        ///
        /// \return The height of the rectangle.
        AURORA_INLINE constexpr Real32 GetHeight() const
        {
            return GetBottom() - GetTop();
        }

        /// \brief Gets the position of the rectangle (top-left corner).
        ///
        /// \return The position of the rectangle as a Vector2.
        AURORA_INLINE constexpr Vector2 GetPosition() const
        {
            return Vector2(GetX(), GetY());
        }

        /// \brief Gets the size of the rectangle.
        ///
        /// \return The size of the rectangle as a Vector2.
        AURORA_INLINE constexpr Vector2 GetSize() const
        {
            return Vector2(GetWidth(), GetHeight());
        }

        /// \brief Calculates the center point of the rectangle.
        ///
        /// \return The center point of the rectangle.
        AURORA_INLINE constexpr Vector2 GetCenter() const
        {
            return Vector2(mX1 + GetWidth() * 0.5, mY1 + GetHeight() * 0.5);
        }

        /// \brief Gets the area of the rectangle.
        ///
        /// \return The area of the rectangle.
        AURORA_INLINE constexpr Real32 GetArea() const
        {
            return GetWidth() * GetHeight();
        }

        /// \brief Gets the perimeter of the rectangle.
        ///
        /// \return The perimeter of the rectangle.
        AURORA_INLINE  constexpr Real32 GetPerimeter() const
        {
            return 2.0f * (GetWidth() + GetHeight());
        }

        /// \brief Expands the rectangle by the given amount in all directions.
        ///
        /// \param Amount The amount to expand by.
        /// \return The expanded rectangle.
        AURORA_INLINE constexpr Rect Expand(Real32 Amount) const
        {
            return Rect(mX1 - Amount, mY1 - Amount, mX2 + Amount, mY2 + Amount);
        }

        /// \brief Contracts the rectangle by the given amount in all directions.
        ///
        /// \param Amount The amount to contract by.
        /// \return The contracted rectangle.
        AURORA_INLINE constexpr Rect Contract(Real32 Amount) const
        {
            return Rect(mX1 + Amount, mY1 + Amount, mX2 - Amount, mY2 - Amount);
        }

        /// \brief Checks if this rectangle completely contains another rectangle.
        ///
        /// \param Other The other rectangle to check.
        /// \return `true` if this rectangle contains the other, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<Rect> Other) const
        {
            return mX1 <= Other.mX1 && mX2 >= Other.mX2 && mY1 <= Other.mY1 && mY2 >= Other.mY2;
        }

        /// \brief Checks if this rectangle contains a point.
        ///
        /// \param X The X coordinate of the point.
        /// \param Y The Y coordinate of the point.
        /// \return `true` if the point is inside the rectangle, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(Real32 X, Real32 Y) const
        {
            return X >= mX1 && X < mX2 && Y >= mY1 && Y < mY2;
        }

        /// \brief Checks if this rectangle contains a point.
        ///
        /// \param Position The position to check.
        /// \return `true` if the point is inside the rectangle, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<Vector2> Position) const
        {
            return Contains(Position.GetX(), Position.GetY());
        }

        /// \brief Checks if this rectangle intersects with another rectangle.
        ///
        /// \param Other The other rectangle to check.
        /// \return `true` if the rectangles intersect, `false` otherwise.
        AURORA_INLINE constexpr Bool Intersects(ConstRef<Rect> Other) const
        {
            return !(mX2 <= Other.mX1 || mX1 >= Other.mX2 ||
                     mY2 <= Other.mY1 || mY1 >= Other.mY2);
        }

        /// \brief Checks if this rectangle is equal to another rectangle.
        ///
        /// \param Other The rectangle to compare to.
        /// \return `true` if all coordinates are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Rect> Other) const
        {
            return IsAlmostEqual(mX1, Other.mX1)
                   && IsAlmostEqual(mX2, Other.mX2)
                   && IsAlmostEqual(mY1, Other.mY1)
                   && IsAlmostEqual(mY2, Other.mY2);
        }

        /// \brief Checks if this rectangle is not equal to another rectangle.
        ///
        /// \param Other The rectangle to compare to.
        /// \return `true` if the rectangles are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Rect> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Adds another rectangle to this rectangle.
        ///
        /// \param Other The rectangle to add.
        /// \return A new rectangle that is the sum of the two rectangles.
        AURORA_INLINE constexpr Rect operator+(ConstRef<Rect> Other) const
        {
            return Rect(mX1 + Other.mX1, mY1 + Other.mY1, mX2 + Other.mX2, mY2 + Other.mY2);
        }

        /// \brief Adds a scalar to all coordinates of this rectangle.
        ///
        /// \param Scalar The scalar to add.
        /// \return A new rectangle with the scalar added to all coordinates.
        AURORA_INLINE constexpr Rect operator+(Real32 Scalar) const
        {
            return Rect(mX1 + Scalar, mY1 + Scalar, mX2 + Scalar, mY2 + Scalar);
        }

        /// \brief Adds a vector to all coordinates of this rectangle.
        ///
        /// \param Vector The vector to add.
        /// \return A new rectangle with the vector added to all coordinates.
        AURORA_INLINE constexpr Rect operator+(ConstRef<Vector2> Vector) const
        {
            return Rect(mX1 + Vector.GetX(), mY1 + Vector.GetY(), mX2 + Vector.GetX(), mY2 + Vector.GetY());
        }

        /// \brief Subtracts another rectangle from this rectangle.
        ///
        /// \param Other The rectangle to subtract.
        /// \return A new rectangle that is the difference of the two rectangles.
        AURORA_INLINE constexpr Rect operator-(ConstRef<Rect> Other) const
        {
            return Rect(mX1 - Other.mX1, mY1 - Other.mY1, mX2 - Other.mX2, mY2 - Other.mY2);
        }

        /// \brief Subtracts a scalar from all coordinates of this rectangle.
        ///
        /// \param Scalar The scalar to subtract.
        /// \return A new rectangle with the scalar subtracted from all coordinates.
        AURORA_INLINE constexpr Rect operator-(Real32 Scalar) const
        {
            return Rect(mX1 - Scalar, mY1 - Scalar, mX2 - Scalar, mY2 - Scalar);
        }

        /// \brief Subtracts a vector from all coordinates of this rectangle.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new rectangle with the vector subtracted from all coordinates.
        AURORA_INLINE constexpr Rect operator-(ConstRef<Vector2> Vector) const
        {
            return Rect(mX1 - Vector.GetX(), mY1 - Vector.GetY(), mX2 - Vector.GetX(), mY2 - Vector.GetY());
        }

        /// \brief Multiplies all coordinates of this rectangle by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A new rectangle with the scalar multiplied by all coordinates.
        AURORA_INLINE constexpr Rect operator*(Real32 Scalar) const
        {
            return Rect(mX1 * Scalar, mY1 * Scalar, mX2 * Scalar, mY2 * Scalar);
        }

        /// \brief Multiplies all coordinates of this rectangle by a vector.
        ///
        /// \param Vector The vector to multiply by.
        /// \return A new rectangle with the vector multiplied by all coordinates.
        AURORA_INLINE constexpr Rect operator*(ConstRef<Vector2> Vector) const
        {
            return Rect(mX1 * Vector.GetX(), mY1 * Vector.GetY(), mX2 * Vector.GetX(), mY2 * Vector.GetY());
        }

        /// \brief Divides all coordinates of this rectangle by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A new rectangle with the scalar divided by all coordinates.
        AURORA_INLINE constexpr Rect operator/(Real32 Scalar) const
        {
            return Rect(mX1 / Scalar, mY1 / Scalar, mX2 / Scalar, mY2 / Scalar);
        }

        /// \brief Divides all coordinates of this rectangle by a vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A new rectangle with the vector divided by all coordinates.
        AURORA_INLINE constexpr Rect operator/(ConstRef<Vector2> Vector) const
        {
            return Rect(mX1 / Vector.GetX(), mY1 / Vector.GetY(), mX2 / Vector.GetX(), mY2 / Vector.GetY());
        }

        /// \brief Adds another rectangle to the current rectangle.
        ///
        /// \param Other The rectangle to add.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator+=(ConstRef<Rect> Other)
        {
            mX1 += Other.mX1;
            mY1 += Other.mY1;
            mX2 += Other.mX2;
            mY2 += Other.mY2;
            return (* this);
        }

        /// \brief Adds a scalar value to the rectangle.
        ///
        /// \param Scalar The scalar value to add.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator+=(Real32 Scalar)
        {
            mX1 += Scalar;
            mY1 += Scalar;
            mX2 += Scalar;
            mY2 += Scalar;
            return (* this);
        }

        /// \brief Adds a vector to the rectangle.
        ///
        /// \param Vector The vector to add.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator+=(Vector2 Vector)
        {
            mX1 += Vector.GetX();
            mY1 += Vector.GetY();
            mX2 += Vector.GetX();
            mY2 += Vector.GetY();

            return (* this);
        }

        /// \brief Subtracts another rectangle from the current rectangle.
        ///
        /// \param Other The rectangle to subtract.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator-=(ConstRef<Rect> Other)
        {
            mX1 -= Other.mX1;
            mY1 -= Other.mY1;
            mX2 -= Other.mX2;
            mY2 -= Other.mY2;
            return (* this);
        }

        /// \brief Subtracts a scalar value from the rectangle.
        ///
        /// \param Scalar The scalar value to subtract.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator-=(Real32 Scalar)
        {
            mX1 -= Scalar;
            mY1 -= Scalar;
            mX2 -= Scalar;
            mY2 -= Scalar;
            return (* this);
        }

        /// \brief Subtracts a vector from the rectangle.
        ///
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator-=(ConstRef<Vector2> Vector)
        {
            mX1 -= Vector.GetX();
            mY1 -= Vector.GetY();
            mX2 -= Vector.GetX();
            mY2 -= Vector.GetY();

            return (* this);
        }

        /// \brief Multiplies the rectangle coordinates by a scalar value.
        ///
        /// \param Scalar The scalar value to multiply.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator*=(Real32 Scalar)
        {
            mX1 *= Scalar;
            mY1 *= Scalar;
            mX2 *= Scalar;
            mY2 *= Scalar;

            return (* this);
        }

        /// \brief Multiplies the rectangle coordinates by a vector.
        ///
        /// \param Vector The vector to multiply.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator*=(ConstRef<Vector2> Vector)
        {
            mX1 *= Vector.GetX();
            mY1 *= Vector.GetY();
            mX2 *= Vector.GetX();
            mY2 *= Vector.GetY();

            return (* this);
        }

        /// \brief Divides the rectangle coordinates by a scalar value.
        ///
        /// \param Scalar The scalar value to divide by.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator/=(Real32 Scalar)
        {
            mX1 /= Scalar;
            mY1 /= Scalar;
            mX2 /= Scalar;
            mY2 /= Scalar;

            return (* this);
        }

        /// \brief Divides the rectangle coordinates by a vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<Rect> operator/=(ConstRef<Vector2> Vector)
        {
            mX1 /= Vector.GetX();
            mY1 /= Vector.GetY();
            mX2 /= Vector.GetX();
            mY2 /= Vector.GetY();

            return (* this);
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeReal32(mX1);
            Archive.SerializeReal32(mY1);
            Archive.SerializeReal32(mX2);
            Archive.SerializeReal32(mY2);
        }

    public:

        /// \brief Returns the unit rect with size 1.
        ///
        /// \return A unit rect.
        AURORA_INLINE constexpr static Rect Unit()
        {
            return Rect(0.0f, 0.0f, 1.0f, 1.0f);
        }

        /// \brief Canonicalizes a rectangle by ensuring Left <= Right and Top <= Bottom.
        ///
        /// \param Rectangle The rectangle to canonicalize.
        /// \return A canonicalized rectangle.
        AURORA_INLINE constexpr static Rect Canonicalize(ConstRef<Rect> Rectangle)
        {
            const Real32 Left   = Min(Rectangle.mX1, Rectangle.mX2);
            const Real32 Right  = Max(Rectangle.mX1, Rectangle.mX2);
            const Real32 Top    = Min(Rectangle.mY1, Rectangle.mY2);
            const Real32 Bottom = Max(Rectangle.mY1, Rectangle.mY2);

            return Rect(Left, Top, Right, Bottom);
        }

        /// \brief Returns the intersection of two rectangles.
        ///
        /// \param First  The first rectangle.
        /// \param Second The second rectangle.
        /// \return A rectangle representing the overlapping region. If they don't overlap, the rect may be invalid.
        AURORA_INLINE constexpr static Rect Intersection(ConstRef<Rect> First, ConstRef<Rect> Second)
        {
            return Rect(
                Base::Max(First.mX1, Second.mX1),
                Base::Max(First.mY1, Second.mY1),
                Base::Min(First.mX2, Second.mX2),
                Base::Min(First.mY2, Second.mY2));
        }

        /// \brief Returns the union (bounding box) of two rectangles.
        ///
        /// \param First  The first rectangle.
        /// \param Second The second rectangle.
        /// \return A rectangle that fully contains both inputs.
        AURORA_INLINE constexpr static Rect Union(ConstRef<Rect> First, ConstRef<Rect> Second)
        {
            return Rect(
                Base::Min(First.mX1, Second.mX1),
                Base::Min(First.mY1, Second.mY1),
                Base::Max(First.mX2, Second.mX2),
                Base::Max(First.mY2, Second.mY2));
        }

        /// \brief Anchors a rectangle relative to a pivot point.
        ///
        /// \param Rectangle The source rectangle.
        /// \param Pivot     The pivot alignment mode.
        /// \return A rectangle anchored according to the pivot.
        AURORA_INLINE constexpr static Rect Anchor(ConstRef<Rect> Rectangle, Pivot Pivot)
        {
            constexpr Rect kMultiplier[] = {
                Rect( 0.0,  0.0, 1.0, 1.0),  // LeftTop
                Rect( 0.0, -0.5, 1.0, 0.5),  // LeftMiddle
                Rect( 0.0, -1.0, 1.0, 0.0),  // LeftBottom
                Rect(-0.5,  0.0, 0.5, 1.0),  // CenterTop
                Rect(-0.5, -0.5, 0.5, 0.5),  // CenterMiddle
                Rect(-0.5, -1.0, 0.5, 0.0),  // CenterBottom
                Rect(-1.0,  0.0, 0.0, 1.0),  // RightTop
                Rect(-1.0, -0.5, 0.0, 0.5),  // RightMiddle
                Rect(-1.0, -1.0, 0.0, 0.0),  // RightBottom
            };
            return kMultiplier[Enum::Cast(Pivot)] * Rectangle.GetSize() + Rectangle.GetPosition();
        }

        /// \brief Linearly interpolates between two rectangles.
        ///
        /// \param Start      The starting rectangle.
        /// \param End        The ending rectangle.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A rectangle interpolated between the start and end rectangles.
        AURORA_INLINE constexpr static Rect Lerp(ConstRef<Rect> Start, ConstRef<Rect> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        /// \brief Transforms an axis-aligned rectangle with a 4x4 matrix.
        ///
        /// \param Rectangle The source rectangle in local space.
        /// \param Transform The 4x4 transformation matrix to apply.
        /// \return An axis-aligned rectangle bounding the transformed corners.
        AURORA_INLINE static Rect Transform(ConstRef<Rect> Rectangle, ConstRef<Matrix4x4> Transform) // TODO: Optimize
        {
            const Vector2 V0 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetLeft(),  Rectangle.GetBottom()));
            const Vector2 V1 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetRight(), Rectangle.GetBottom()));
            const Vector2 V2 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetRight(), Rectangle.GetTop()));
            const Vector2 V3 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetLeft(),  Rectangle.GetTop()));

            const Real32 MinX = Base::Min(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Real32 MinY = Base::Min(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());
            const Real32 MaxX = Base::Max(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Real32 MaxY = Base::Max(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());

            return Rect(MinX, MinY, MaxX, MaxY);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real32 mX1;
        Real32 mY1;
        Real32 mX2;
        Real32 mY2;
    };
}