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

#include "Aurora.Base/Enum.hpp"
#include "Aurora.Math/Matrix4x4.hpp"
#include "Aurora.Math/Pivot.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents a generic 2D rectangle defined by (X1, Y1, X2, Y2) coordinates.
    template<typename Type>
    class AnyRect final
    {
    public:

        /// \brief Constructor initializing all components zero.
        AURORA_INLINE constexpr AnyRect()
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
        AURORA_INLINE constexpr AnyRect(Type Left, Type Top, Type Right, Type Bottom)
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
        AURORA_INLINE constexpr AnyRect(ConstRef<AnyVector2<Type>> Minimum, ConstRef<AnyVector2<Type>> Maximum)
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
            return Base::IsAlmostZero(mX1)
                && Base::IsAlmostZero(mY1)
                && Base::IsAlmostZero(mX2)
                && Base::IsAlmostZero(mY2);
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
        AURORA_INLINE constexpr void Set(Type Left, Type Top, Type Right, Type Bottom)
        {
            mX1 = Left;
            mY1 = Top;
            mX2 = Right;
            mY2 = Bottom;
        }

        /// \brief Sets the left coordinate of the rectangle.
        ///
        /// \param Value The new left coordinate.
        AURORA_INLINE constexpr void SetLeft(Type Value)
        {
            mX1 = Value;
        }

        /// \brief Gets the left coordinate of the rectangle.
        ///
        /// \return The left coordinate.
        AURORA_INLINE constexpr Type GetLeft() const
        {
            return mX1;
        }

        /// \brief Sets the top coordinate of the rectangle.
        ///
        /// \param Value The new top coordinate.
        AURORA_INLINE constexpr void SetTop(Type Value)
        {
            mY1 = Value;
        }

        /// \brief Gets the top coordinate of the rectangle.
        ///
        /// \return The top coordinate.
        AURORA_INLINE constexpr Type GetTop() const
        {
            return mY1;
        }

        /// \brief Sets the right coordinate of the rectangle.
        ///
        /// \param Value The new right coordinate.
        AURORA_INLINE constexpr void SetRight(Type Value)
        {
            mX2 = Value;
        }

        /// \brief Gets the right coordinate of the rectangle.
        ///
        /// \return The right coordinate.
        AURORA_INLINE constexpr Type GetRight() const
        {
            return mX2;
        }

        /// \brief Sets the bottom coordinate of the rectangle.
        ///
        /// \param Value The new bottom coordinate.
        AURORA_INLINE constexpr void SetBottom(Type Value)
        {
            mY2 = Value;
        }

        /// \brief Gets the bottom coordinate of the rectangle.
        ///
        /// \return The bottom coordinate.
        AURORA_INLINE constexpr Type GetBottom() const
        {
            return mY2;
        }

        /// \brief Gets the X coordinate of the rectangle (equivalent to left).
        ///
        /// \return The X coordinate.
        AURORA_INLINE constexpr Type GetX() const
        {
            return GetLeft();
        }

        /// \brief Gets the Y coordinate of the rectangle (equivalent to top).
        ///
        /// \return The Y coordinate.
        AURORA_INLINE constexpr Type GetY() const
        {
            return GetTop();
        }

        /// \brief Calculates the width of the rectangle.
        ///
        /// \return The width of the rectangle.
        AURORA_INLINE constexpr Type GetWidth() const
        {
            return GetRight() - GetLeft();
        }

        /// \brief Calculates the height of the rectangle.
        ///
        /// \return The height of the rectangle.
        AURORA_INLINE constexpr Type GetHeight() const
        {
            return GetBottom() - GetTop();
        }

        /// \brief Gets the position of the rectangle (top-left corner).
        ///
        /// \return The position of the rectangle2.
        AURORA_INLINE constexpr AnyVector2<Type> GetPosition() const
        {
            return Vector2(GetX(), GetY());
        }

        /// \brief Gets the size of the rectangle.
        ///
        /// \return The size of the rectangle as a Vector2.
        AURORA_INLINE constexpr AnyVector2<Type> GetSize() const
        {
            return Vector2(GetWidth(), GetHeight());
        }

        /// \brief Calculates the center point of the rectangle.
        ///
        /// \return The center point of the rectangle.
        AURORA_INLINE constexpr AnyVector2<Type> GetCenter() const
        {
            if constexpr(IsReal<Type>)
            {
                return AnyVector2<Type>(mX1 + GetWidth() * Type(0.5), mY1 + GetHeight() * Type(0.5));
            }
            else
            {
                return AnyVector2<Type>(mX1 + GetWidth() / Type(2), mY1 + GetHeight() / Type(2));
            }
        }

        /// \brief Gets the area of the rectangle.
        ///
        /// \return The area of the rectangle.
        AURORA_INLINE constexpr Type GetArea() const
        {
            return GetWidth() * GetHeight();
        }

        /// \brief Gets the perimeter of the rectangle.
        ///
        /// \return The perimeter of the rectangle.
        AURORA_INLINE  constexpr Type GetPerimeter() const
        {
            return Type(2) * (GetWidth() + GetHeight());
        }

        /// \brief Expands the rectangle by the given amount in all directions.
        ///
        /// \param Amount The amount to expand by.
        /// \return The expanded rectangle.
        AURORA_INLINE constexpr AnyRect Expand(Type Amount) const
        {
            return AnyRect(mX1 - Amount, mY1 - Amount, mX2 + Amount, mY2 + Amount);
        }

        /// \brief Contracts the rectangle by the given amount in all directions.
        ///
        /// \param Amount The amount to contract by.
        /// \return The contracted rectangle.
        AURORA_INLINE constexpr AnyRect Contract(Type Amount) const
        {
            return AnyRect(mX1 + Amount, mY1 + Amount, mX2 - Amount, mY2 - Amount);
        }

        /// \brief Checks if this rectangle completely contains another rectangle.
        ///
        /// \param Other The other rectangle to check.
        /// \return `true` if this rectangle contains the other, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<AnyRect> Other) const
        {
            return mX1 <= Other.mX1 && mX2 >= Other.mX2 && mY1 <= Other.mY1 && mY2 >= Other.mY2;
        }

        /// \brief Checks if this rectangle contains a point.
        ///
        /// \param X The X coordinate of the point.
        /// \param Y The Y coordinate of the point.
        /// \return `true` if the point is inside the rectangle, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(Type X, Type Y) const
        {
            return X >= mX1 && X < mX2 && Y >= mY1 && Y < mY2;
        }

        /// \brief Checks if this rectangle contains a point.
        ///
        /// \param Position The position to check.
        /// \return `true` if the point is inside the rectangle, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<AnyVector2<Type>> Position) const
        {
            return Contains(Position.GetX(), Position.GetY());
        }

        /// \brief Checks if this rectangle intersects with another rectangle.
        ///
        /// \param Other The other rectangle to check.
        /// \return `true` if the rectangles intersect, `false` otherwise.
        AURORA_INLINE constexpr Bool Intersects(ConstRef<AnyRect> Other) const
        {
            return !(mX2 <= Other.mX1 || mX1 >= Other.mX2 ||
                     mY2 <= Other.mY1 || mY1 >= Other.mY2);
        }

        /// \brief Checks if this rectangle is equal to another rectangle.
        ///
        /// \param Other The rectangle to compare to.
        /// \return `true` if all coordinates are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<AnyRect> Other) const
        {
            return IsAlmostEqual(* this, Other);
        }

        /// \brief Checks if this rectangle is not equal to another rectangle.
        ///
        /// \param Other The rectangle to compare to.
        /// \return `true` if the rectangles are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<AnyRect> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Adds another rectangle to this rectangle.
        ///
        /// \param Other The rectangle to add.
        /// \return A new rectangle that is the sum of the two rectangles.
        AURORA_INLINE constexpr AnyRect operator+(ConstRef<AnyRect> Other) const
        {
            return AnyRect(mX1 + Other.mX1, mY1 + Other.mY1, mX2 + Other.mX2, mY2 + Other.mY2);
        }

        /// \brief Adds a scalar to all coordinates of this rectangle.
        ///
        /// \param Scalar The scalar to add.
        /// \return A new rectangle with the scalar added to all coordinates.
        AURORA_INLINE constexpr AnyRect operator+(Type Scalar) const
        {
            return AnyRect(mX1 + Scalar, mY1 + Scalar, mX2 + Scalar, mY2 + Scalar);
        }

        /// \brief Adds a vector to all coordinates of this rectangle.
        ///
        /// \param Vector The vector to add.
        /// \return A new rectangle with the vector added to all coordinates.
        AURORA_INLINE constexpr AnyRect operator+(ConstRef<AnyVector2<Type>> Vector) const
        {
            return AnyRect(mX1 + Vector.GetX(), mY1 + Vector.GetY(), mX2 + Vector.GetX(), mY2 + Vector.GetY());
        }

        /// \brief Subtracts another rectangle from this rectangle.
        ///
        /// \param Other The rectangle to subtract.
        /// \return A new rectangle that is the difference of the two rectangles.
        AURORA_INLINE constexpr AnyRect operator-(ConstRef<AnyRect> Other) const
        {
            return AnyRect(mX1 - Other.mX1, mY1 - Other.mY1, mX2 - Other.mX2, mY2 - Other.mY2);
        }

        /// \brief Subtracts a scalar from all coordinates of this rectangle.
        ///
        /// \param Scalar The scalar to subtract.
        /// \return A new rectangle with the scalar subtracted from all coordinates.
        AURORA_INLINE constexpr AnyRect operator-(Type Scalar) const
        {
            return AnyRect(mX1 - Scalar, mY1 - Scalar, mX2 - Scalar, mY2 - Scalar);
        }

        /// \brief Subtracts a vector from all coordinates of this rectangle.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new rectangle with the vector subtracted from all coordinates.
        AURORA_INLINE constexpr AnyRect operator-(ConstRef<AnyVector2<Type>> Vector) const
        {
            return AnyRect(mX1 - Vector.GetX(), mY1 - Vector.GetY(), mX2 - Vector.GetX(), mY2 - Vector.GetY());
        }

        /// \brief Multiplies all coordinates of this rectangle by a scalar.
        ///
        /// \param Scalar The scalar to multiply by.
        /// \return A new rectangle with the scalar multiplied by all coordinates.
        AURORA_INLINE constexpr AnyRect operator*(Type Scalar) const
        {
            return AnyRect(mX1 * Scalar, mY1 * Scalar, mX2 * Scalar, mY2 * Scalar);
        }

        /// \brief Multiplies all coordinates of this rectangle by a vector.
        ///
        /// \param Vector The vector to multiply by.
        /// \return A new rectangle with the vector multiplied by all coordinates.
        AURORA_INLINE constexpr AnyRect operator*(ConstRef<AnyVector2<Type>> Vector) const
        {
            return AnyRect(mX1 * Vector.GetX(), mY1 * Vector.GetY(), mX2 * Vector.GetX(), mY2 * Vector.GetY());
        }

        /// \brief Divides all coordinates of this rectangle by a scalar.
        ///
        /// \param Scalar The scalar to divide by.
        /// \return A new rectangle with the scalar divided by all coordinates.
        AURORA_INLINE constexpr AnyRect operator/(Type Scalar) const
        {
            return AnyRect(mX1 / Scalar, mY1 / Scalar, mX2 / Scalar, mY2 / Scalar);
        }

        /// \brief Divides all coordinates of this rectangle by a vector.
        ///
        /// \param Vector The vector to divide by.
        /// \return A new rectangle with the vector divided by all coordinates.
        AURORA_INLINE constexpr AnyRect operator/(ConstRef<AnyVector2<Type>> Vector) const
        {
            return AnyRect(mX1 / Vector.GetX(), mY1 / Vector.GetY(), mX2 / Vector.GetX(), mY2 / Vector.GetY());
        }

        /// \brief Adds another rectangle to the current rectangle.
        ///
        /// \param Other The rectangle to add.
        /// \return A reference to the updated rectangle.
        AURORA_INLINE constexpr Ref<AnyRect> operator+=(ConstRef<AnyRect> Other)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator+=(Type Scalar)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator+=(AnyVector2<Type> Vector)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator-=(ConstRef<AnyRect> Other)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator-=(Type Scalar)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator-=(ConstRef<AnyVector2<Type>> Vector)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator*=(Type Scalar)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator*=(ConstRef<AnyVector2<Type>> Vector)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator/=(Type Scalar)
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
        AURORA_INLINE constexpr Ref<AnyRect> operator/=(ConstRef<AnyVector2<Type>> Vector)
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
            Archive.SerializeObject(mX1);
            Archive.SerializeObject(mY1);
            Archive.SerializeObject(mX2);
            Archive.SerializeObject(mY2);
        }

    public:

        /// \brief Returns the unit rect with size 1.
        ///
        /// \return An unit rect.
        AURORA_INLINE constexpr static AnyRect Unit()
        {
            return AnyRect(Type(0), Type(0), Type(1), Type(1));
        }

        /// \brief Canonicalizes a rectangle by ensuring Left <= Right and Top <= Bottom.
        ///
        /// \param Rectangle The rectangle to canonicalize.
        /// \return A canonicalized rectangle.
        AURORA_INLINE constexpr static AnyRect Canonicalize(ConstRef<AnyRect> Rectangle)
        {
            const Type Left   = Base::Min(Rectangle.mX1, Rectangle.mX2);
            const Type Right  = Base::Max(Rectangle.mX1, Rectangle.mX2);
            const Type Top    = Base::Min(Rectangle.mY1, Rectangle.mY2);
            const Type Bottom = Base::Max(Rectangle.mY1, Rectangle.mY2);

            return AnyRect(Left, Top, Right, Bottom);
        }

        /// \brief Returns the component-wise minimum of two rectangles.
        ///
        /// \param First  The first rectangle.
        /// \param Second The second rectangle.
        /// \return A vector with the component-wise minimum values.
        AURORA_INLINE constexpr static AnyRect Min(ConstRef<AnyRect> First, ConstRef<AnyRect> Second)
        {
            return AnyRect(
                Base::Min(First.mX1, Second.mX1),
                Base::Min(First.mY1, Second.mY1),
                Base::Min(First.mX2, Second.mX2),
                Base::Min(First.mY2, Second.mY2));
        }

        /// \brief Returns the component-wise maximum of two rectangles.
        ///
        /// \param First  The first rectangle.
        /// \param Second The second rectangle.
        /// \return A rectangle with the component-wise maximum values.
        AURORA_INLINE constexpr static AnyRect Max(ConstRef<AnyRect> First, ConstRef<AnyRect> Second)
        {
            return AnyRect(
                Base::Max(First.mX1, Second.mX1),
                Base::Max(First.mY1, Second.mY1),
                Base::Max(First.mX2, Second.mX2),
                Base::Max(First.mY2, Second.mY2));
        }

        /// \brief Returns the intersection of two rectangles.
        ///
        /// \param First  The first rectangle.
        /// \param Second The second rectangle.
        /// \return A rectangle representing the overlapping region. If they don't overlap, the rect may be invalid.
        AURORA_INLINE constexpr static AnyRect Intersection(ConstRef<AnyRect> First, ConstRef<AnyRect> Second)
        {
            return AnyRect(
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
        AURORA_INLINE constexpr static AnyRect Union(ConstRef<AnyRect> First, ConstRef<AnyRect> Second)
        {
            return AnyRect(
                Base::Min(First.mX1, Second.mX1),
                Base::Min(First.mY1, Second.mY1),
                Base::Max(First.mX2, Second.mX2),
                Base::Max(First.mY2, Second.mY2));
        }

        /// \brief Returns a rectangle of type \p Target that tightly encloses this rectangle.
        ///
        /// \param Rect The source rectangle to enclose.
        /// \return A rectangle of type \p Target that fully contains the input.
        template<typename Target>
        AURORA_INLINE constexpr static Target Enclose(ConstRef<AnyRect> Rectangle)
        {
            return Target(Base::Floor(Rectangle.mX1), Base::Floor(Rectangle.mY1),
                           Base::Ceil(Rectangle.mX2),  Base::Ceil(Rectangle.mY2));
        }

        /// \brief Anchors a rectangle relative to a pivot point.
        ///
        /// \param Rectangle The source rectangle.
        /// \param Pivot     The pivot alignment mode.
        /// \return A rectangle anchored according to the pivot.
        AURORA_INLINE constexpr static AnyRect Anchor(ConstRef<AnyRect> Rectangle, Pivot Pivot)
            requires (IsReal<Type>)
        {
            constexpr AnyRect kMultiplier[] = {
                AnyRect( Type(0.0),  Type(0.0), Type(1.0), Type(1.0)),  // LeftTop
                AnyRect( Type(0.0), -Type(0.5), Type(1.0), Type(0.5)),  // LeftMiddle
                AnyRect( Type(0.0), -Type(1.0), Type(1.0), Type(0.0)),  // LeftBottom
                AnyRect(-Type(0.5),  Type(0.0), Type(0.5), Type(1.0)),  // CenterTop
                AnyRect(-Type(0.5), -Type(0.5), Type(0.5), Type(0.5)),  // CenterMiddle
                AnyRect(-Type(0.5), -Type(1.0), Type(0.5), Type(0.0)),  // CenterBottom
                AnyRect(-Type(1.0),  Type(0.0), Type(0.0), Type(1.0)),  // RightTop
                AnyRect(-Type(1.0), -Type(0.5), Type(0.0), Type(0.5)),  // RightMiddle
                AnyRect(-Type(1.0), -Type(1.0), Type(0.0), Type(0.0)),  // RightBottom
            };
            return kMultiplier[Enum::Cast(Pivot)] * Rectangle.GetSize() + Rectangle.GetPosition();
        }

        /// \brief Linearly interpolates between two rectangles.
        ///
        /// \param Start      The starting rectangle.
        /// \param End        The ending rectangle.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A rectangle interpolated between the start and end rectangles.
        AURORA_INLINE constexpr static AnyRect Lerp(ConstRef<AnyRect> Start, ConstRef<AnyRect> End, Type Percentage)
            requires (IsReal<Type>)
        {
            return Start + (End - Start) * Percentage;
        }

        /// \brief Transforms an axis-aligned rectangle with a 4x4 matrix.
        ///
        /// \param Rectangle The source rectangle in local space.
        /// \param Transform The 4x4 transformation matrix to apply.
        /// \return An axis-aligned rectangle bounding the transformed corners.
        AURORA_INLINE static AnyRect Transform(ConstRef<AnyRect> Rectangle, ConstRef<Matrix4x4> Transform) // TODO: Optimize
            requires (IsReal<Type>)
        {
            const Vector2 V0 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetLeft(),  Rectangle.GetBottom()));
            const Vector2 V1 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetRight(), Rectangle.GetBottom()));
            const Vector2 V2 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetRight(), Rectangle.GetTop()));
            const Vector2 V3 = Matrix4x4::Project(Transform, Vector2(Rectangle.GetLeft(),  Rectangle.GetTop()));

            const Type MinX = Base::Min(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Type MinY = Base::Min(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());
            const Type MaxX = Base::Max(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Type MaxY = Base::Max(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());

            return AnyRect(MinX, MinY, MaxX, MaxY);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Type mX1;
        Type mY1;
        Type mX2;
        Type mY2;
    };

    /// \brief Represents a 2D rectangle defined by (X1, Y1, X2, Y2) floating-point coordinates.
    using Rect    = AnyRect<Real32>;

    /// \brief Represents a 2D rectangle defined by (X1, Y1, X2, Y2) signed integer coordinates.
    using IntRect = AnyRect<SInt32>;
}