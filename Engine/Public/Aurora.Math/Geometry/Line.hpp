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
    /// \brief Represents a 2D line segment defined by two points.
    class Line final
    {
    public:

        /// \brief Initializes the line to points (0.0f, 0.0f) and (0.0f, 0.0f).
        AURORA_INLINE constexpr Line()
            : mStart { Vector2::Zero() },
              mEnd   { Vector2::Zero() }
        {
        }

        /// \brief Constructor initializing the line with specified start and end points.
        ///
        /// \param Start The start point of the line.
        /// \param End   The end point of the line.
        AURORA_INLINE constexpr Line(ConstRef<Vector2> Start, ConstRef<Vector2> End)
            : mStart { Start },
              mEnd   { End }
        {
        }

        /// \brief Checks if the line is the zero line.
        ///
        /// \return `true` if both points are approximately zero, `false` otherwise.
        AURORA_INLINE constexpr Bool IsAlmostZero() const
        {
            return mStart.IsAlmostZero() && mEnd.IsAlmostZero();
        }

        /// \brief Checks if the line is valid.
        ///
        /// \return `true` if the line is valid, `false` otherwise.
        AURORA_INLINE constexpr Bool IsValid() const
        {
            return mStart != mEnd;
        }

        /// \brief Checks if the line has zero length.
        ///
        /// \return `true` if the line has zero length, `false` otherwise.
        AURORA_INLINE constexpr Bool IsEmpty() const
        {
            return mStart == mEnd;
        }

        /// \brief Sets the start and end points of the line.
        ///
        /// \param Start The new start point.
        /// \param End   The new end point.
        AURORA_INLINE constexpr void Set(ConstRef<Vector2> Start, ConstRef<Vector2> End)
        {
            mStart = Start;
            mEnd   = End;
        }

        /// \brief Sets the start point of the line.
        ///
        /// \param Start The new start point.
        AURORA_INLINE constexpr void SetStart(ConstRef<Vector2> Start)
        {
            mStart = Start;
        }

        /// \brief Gets the start point of the line.
        ///
        /// \return The start point of the line.
        AURORA_INLINE constexpr ConstRef<Vector2> GetStart() const
        {
            return mStart;
        }

        /// \brief Sets the end point of the line.
        ///
        /// \param End The new end point.
        AURORA_INLINE constexpr void SetEnd(ConstRef<Vector2> End)
        {
            mEnd = End;
        }

        /// \brief Gets the end point of the line.
        ///
        /// \return The end point of the line.
        AURORA_INLINE constexpr ConstRef<Vector2> GetEnd() const
        {
            return mEnd;
        }

        /// \brief Calculates the length of the line.
        ///
        /// \return The length of the line.
        AURORA_INLINE constexpr Real32 GetLength() const
        {
            return mStart.GetDistance(mEnd);
        }

        /// \brief Calculates the squared length of the line.
        ///
        /// \return The squared length of the line.
        AURORA_INLINE constexpr Real32 GetLengthSquared() const
        {
            return mStart.GetDistanceSquared(mEnd);
        }

        /// \brief Calculates the direction vector of the line.
        ///
        /// \return The normalized direction vector of the line.
        AURORA_INLINE constexpr Vector2 GetDirection() const
        {
            return Vector2::Normalize(mEnd - mStart);
        }

        /// \brief Calculates the midpoint of the line.
        ///
        /// \return The midpoint of the line.
        AURORA_INLINE constexpr Vector2 GetMidpoint() const
        {
            return (mStart + mEnd) * 0.5f;
        }

        /// \brief Calculates the normal vector of the line.
        ///
        /// \return The normalized normal vector of the line.
        AURORA_INLINE constexpr Vector2 GetNormal() const
        {
            const Vector2 Direction = GetDirection();
            return Vector2(-Direction.GetY(), Direction.GetX());
        }

        /// \brief Returns a point on the line segment by linear interpolation.
        ///
        /// \param Factor Interpolation parameter (0 = start, 1 = end).
        /// \return The interpolated point.
        AURORA_INLINE constexpr Vector2 GetPoint(Real32 Factor) const
        {
            return Vector2::Lerp(mStart, mEnd, Factor);
        }

        /// \brief Checks if the line segment contains a point (point lies on the segment).
        ///
        /// \param Point The point to check for containment.
        /// \return `true` if the point lies on the line segment, `false` otherwise.
        AURORA_INLINE constexpr Bool Contains(ConstRef<Vector2> Point) const
        {
            const Vector2 Direction = mEnd - mStart;
            const Vector2 Offset    = Point - mStart;

            if (const Real32 Cross = Vector2::Cross(Direction, Offset); Base::Abs(Cross) > kEpsilon<Real32>)
            {
                return false;
            }

            const Real32 Projection = Vector2::Dot(Direction, Offset);
            return !(Projection < 0.0f || Projection > Vector2::Dot(Direction, Direction));
        }

        /// \brief Checks if this line segment intersects with another line segment.
        ///
        /// \param Other The other line segment to check for intersection.
        /// \return `true` if the line segments intersect, `false` otherwise.
        AURORA_INLINE constexpr Bool Intersects(ConstRef<Line> Other) const
        {
            const Real32 O1 = Vector2::Cross(mStart, mEnd, Other.mStart);
            const Real32 O2 = Vector2::Cross(mStart, mEnd, Other.mEnd);
            const Real32 O3 = Vector2::Cross(Other.mStart, Other.mEnd, mStart);
            const Real32 O4 = Vector2::Cross(Other.mStart, Other.mEnd, mEnd);

            if ((O1 * O2 < 0) && (O3 * O4 < 0))
            {
                return true;
            }

            return (Base::IsAlmostZero(O1) && Contains(Other.mStart)
                ||  Base::IsAlmostZero(O2) && Contains(Other.mEnd)
                ||  Base::IsAlmostZero(O3) && Other.Contains(mStart)
                ||  Base::IsAlmostZero(O4) && Other.Contains(mEnd));
        }

        /// \brief Checks if this line is equal to another line.
        ///
        /// \param Other The line to compare to.
        /// \return `true` if both points are approximately equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Line> Other) const
        {
            return (mStart == Other.mStart && mEnd == Other.mEnd) ||
                   (mStart == Other.mEnd   && mEnd == Other.mStart);
        }

        /// \brief Checks if this line is not equal to another line.
        ///
        /// \param Other The line to compare to.
        /// \return `true` if the lines are not equal, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Line> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Adds a vector to both points of the line.
        ///
        /// \param Vector The vector to add.
        /// \return A new line with translated points.
        AURORA_INLINE constexpr Line operator+(ConstRef<Vector2> Vector) const
        {
            return Line(mStart + Vector, mEnd + Vector);
        }

        /// \brief Subtracts a vector from both points of the line.
        ///
        /// \param Vector The vector to subtract.
        /// \return A new line with translated points.
        AURORA_INLINE constexpr Line operator-(ConstRef<Vector2> Vector) const
        {
            return Line(mStart - Vector, mEnd - Vector);
        }

        /// \brief Adds a vector to both points of the line.
        ///
        /// \param Vector The vector to add.
        /// \return A reference to the updated line.
        AURORA_INLINE constexpr Ref<Line> operator+=(ConstRef<Vector2> Vector)
        {
            mStart += Vector;
            mEnd   += Vector;
            return (* this);
        }

        /// \brief Subtracts a vector from both points of the line.
        ///
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated line.
        AURORA_INLINE constexpr Ref<Line> operator-=(ConstRef<Vector2> Vector)
        {
            mStart -= Vector;
            mEnd   -= Vector;
            return (* this);
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeObject(mStart);
            Archive.SerializeObject(mEnd);
        }

    public:

        /// \brief Returns a unit line segment in a specified direction from the origin.
        ///
        /// \param Direction The direction vector for the unit line (will be normalized).
        /// \return A unit line segment in the specified direction starting from origin.
        AURORA_INLINE constexpr static Line Unit(ConstRef<Vector2> Direction)
        {
            return Line(Vector2::Zero(), Vector2::Normalize(Direction));
        }

        /// \brief Returns a unit line segment along the X-axis from (0, 0) to (1, 0).
        ///
        /// \return A unit line segment along the X-axis.
        AURORA_INLINE constexpr static Line UnitX()
        {
            return Line(Vector2::Zero(), Vector2::UnitX());
        }

        /// \brief Returns a unit line segment along the Y-axis from (0, 0) to (0, 1).
        ///
        /// \return A unit line segment along the Y-axis.
        AURORA_INLINE constexpr static Line UnitY()
        {
            return Line(Vector2::Zero(), Vector2::UnitY());
        }

        /// \brief Linearly interpolates between two lines.
        ///
        /// \param Start      The starting line.
        /// \param End        The ending line.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A line interpolated between the start and end lines.
        AURORA_INLINE constexpr static Line Lerp(ConstRef<Line> Start, ConstRef<Line> End, Real32 Percentage)
        {
            return Line(Vector2::Lerp(Start.mStart, End.mStart, Percentage),
                        Vector2::Lerp(Start.mEnd,   End.mEnd,   Percentage));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2 mStart;
        Vector2 mEnd;
    };
}