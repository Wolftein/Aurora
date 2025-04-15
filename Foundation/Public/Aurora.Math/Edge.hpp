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

#include "Rect.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Edge final
    {
    public:

        // -=(Undocumented)=-
        constexpr Edge()
            : mPointA { 0 },
              mPointB { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Edge(ConstRef<Vector2<Base>> PointA, ConstRef<Vector2<Base>> PointB)
            : mPointA { PointA },
              mPointB { PointB }
        {
        }

        // -=(Undocumented)=-
        void Set(ConstRef<Vector2<Base>> PointA, ConstRef<Vector2<Base>> PointB)
        {
            mPointA = PointA;
            mPointB = PointB;
        }

        // -=(Undocumented)=-
        ConstRef<Vector2<Base>> GetPointA() const
        {
            return mPointA;
        }

        // -=(Undocumented)=-
        ConstRef<Vector2<Base>> GetPointB() const
        {
            return mPointB;
        }

        // -=(Undocumented)=-
        Vector2<Base> GetDirection() const
        {
            return mPointB - mPointA;
        }

        // -=(Undocumented)=-
        Vector2<Base> GetCenter() const
        {
            return (mPointA + mPointB) * Base(0.5);
        }

        // -=(Undocumented)=-
        Base GetLength() const
        {
            return GetDirection().Length();
        }

        // -=(Undocumented)=-
        Base GetLengthSquared() const
        {
            return GetDirection().LengthSquared();
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return mPointA.IsZero() && mPointB.IsZero();
        }

        // -=(Undocumented)=-
        Bool Contains(ConstRef<Vector2<Base>> Point) const
        {
            const Vector2<Base> AB = GetDirection();
            const Vector2<Base> AP = Point - mPointA;

            if (Abs(Vector2<Base>::Cross(AB, AP)) > k_Epsilon<Base>)
            {
                return false;
            }

            const Base Dot = AP.Dot(AB);
            return Dot >= -k_Epsilon<Base> && Dot <= AB.GetLengthSquared() + k_Epsilon<Base>;
        }

        // -=(Undocumented)=-
        Bool Intersects(ConstRef<Edge<Base>> Other) const
        {
            const Base A1 = Vector2<Base>::Cross(mPointA, mPointB, Other.mPointB);
            const Base A2 = Vector2<Base>::Cross(mPointA, mPointB, Other.mPointA);

            if (A1 * A2 < 0.0)
            {
                const Base A3 = Vector2<Base>::Cross(Other.mPointA, Other.mPointB, mPointA);
                const Base A4 = Vector2<Base>::Cross(Other.mPointA, Other.mPointB, mPointB);

                if (A3 * A4 < 0.0)
                {
                    return true;
                }
            }
            return false;
        }

        // -=(Undocumented)=-
        constexpr Bool operator==(ConstRef<Edge<Base>> Other) const
        {
            return AlmostEqual(mPointA, Other.mPointA) && AlmostEqual(mPointB, Other.mPointB);
        }

        // -=(Undocumented)=-
        constexpr Bool operator!=(ConstRef<Edge<Base>> Other) const
        {
            return !(*this == Other);
        }

        // -=(Undocumented)=-
        constexpr Edge operator+(Base Scalar) const
        {
            return Edge(mPointA + Scalar, mPointB + Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Edge operator+(ConstRef<Vector2<Type>> Vector) const
        {
            return Edge(mPointA + Vector, mPointB + Vector);
        }

        // -=(Undocumented)=-
        constexpr Edge operator-(Base Scalar) const
        {
            return Edge(mPointA - Scalar, mPointB - Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Edge operator-(ConstRef<Vector2<Type>> Vector) const
        {
            return Edge(mPointA - Vector, mPointB - Vector);
        }

        // -=(Undocumented)=-
        constexpr Edge operator*(Base Scalar) const
        {
            return Edge(mPointA * Scalar, mPointB * Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Edge operator*(ConstRef<Vector2<Type>> Vector) const
        {
            return Edge(mPointA * Vector, mPointB * Vector);
        }

        // -=(Undocumented)=-
        constexpr Edge operator/(Base Scalar) const
        {
            return Edge(mPointA / Scalar, mPointB / Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Edge operator/(ConstRef<Vector2<Type>> Vector) const
        {
            return Edge(mPointA / Vector, mPointB / Vector);
        }

        // -=(Undocumented)=-
        constexpr Ref<Edge> operator+=(Base Scalar)
        {
            mPointA += Scalar;
            mPointB += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Edge> operator+=(ConstRef<Vector2<Type>> Vector)
        {
            mPointA += Vector;
            mPointB += Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Edge> operator-=(Base Scalar)
        {
            mPointA -= Scalar;
            mPointB -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Edge> operator-=(ConstRef<Vector2<Type>> Vector)
        {
            mPointA -= Vector;
            mPointB -= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Edge> operator*=(Base Scalar)
        {
            mPointA *= Scalar;
            mPointB *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Edge> operator*=(ConstRef<Vector2<Type>> Vector)
        {
            mPointA *= Vector;
            mPointB *= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Edge> operator/=(Base Scalar)
        {
            mPointA /= Scalar;
            mPointB /= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Edge> operator/=(ConstRef<Vector2<Type>> Vector)
        {
            mPointA /= Vector;
            mPointB /= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeObject(mPointA);
            Archive.SerializeObject(mPointB);
        }

    public:

        // -=(Undocumented)=-
        static constexpr Vector2<Base> GetClosestPoint(ConstRef<Edge> Edge, ConstRef<Vector2<Base>> Position)
        {
            const Vector2<Base> AB = Edge.GetDirection();

            if (const Base Length = AB.LengthSquared(); Length < k_Epsilon<Base>)
            {
                return Edge.mPointA;
            }
            else
            {
                return Edge.mPointA + AB * Clamp((Position - Edge.mPointA).Dot(AB) / Length, Base(0), Base(1));
            }
        }

        // -=(Undocumented)=-
        static Edge<Base> Min(ConstRef<Edge<Base>> Lhs, ConstRef<Edge<Base>> Rhs)
        {
            return Edge<Base>(Vector2<Base>::Min(Lhs.mPointA, Rhs.mPointA), Core::Min(Lhs.mPointB, Rhs.mPointB));
        }

        // -=(Undocumented)=-
        static Edge<Base> Max(ConstRef<Edge<Base>> Lhs, ConstRef<Edge<Base>> Rhs)
        {
            return Edge<Base>(Vector2<Base>::Max(Lhs.mPointA, Rhs.mPointA), Core::Max(Lhs.mPointB, Rhs.mPointB));
        }

        // -=(Undocumented)=-
        static Edge<Base> Lerp(ConstRef<Edge<Base>> Start, ConstRef<Edge<Base>> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        // -=(Undocumented)=-
        static Edge<Base> Transform(ConstRef<Edge<Base>> Edge, ConstRef<Matrix4<Base>> Transform)
        {
            return Math::Edge<Base>(Transform * Edge.GetPointA(), Transform * Edge.GetPointB());
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2<Base> mPointA;
        Vector2<Base> mPointB;
    };

    // -=(Undocumented)=-
    using Edgef = Edge<Real32>;
}