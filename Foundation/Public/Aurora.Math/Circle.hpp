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
#include "Pivot.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Circle final
    {
    public:

        // -=(Undocumented)=-
        constexpr Circle()
            : mCenter { 0 },
              mRadius { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Circle(ConstRef<Vector2<Base>> Center, Base Radius)
            : mCenter { Center },
              mRadius { Radius }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return AlmostZero(mRadius);
        }

        // -=(Undocumented)=-
        void SetCenter(ConstRef<Vector2<Base>> Center)
        {
            mCenter = Center;
        }

        // -=(Undocumented)=-
        ConstRef<Vector2<Base>> GetCenter() const
        {
            return mCenter;
        }

        // -=(Undocumented)=-
        void SetRadius(Base Radius)
        {
            mRadius = Radius;
        }

        // -=(Undocumented)=-
        Base GetRadius() const
        {
            return mRadius;
        }

        // -=(Undocumented)=-
        Bool Contains(ConstRef<Vector2<Base>> Position) const
        {
            return (Position - mCenter).GetLengthSquared() <= (mRadius * mRadius);
        }

        // -=(Undocumented)=-
        Bool Intersects(ConstRef<Circle<Base>> Other) const
        {
            const Base Radius = mRadius + Other.mRadius;
            return (Other.mCenter - mCenter).GetLengthSquared() <= (Radius * Radius);
        }

        // -=(Undocumented)=-
        Bool operator==(ConstRef<Circle<Base>> Other) const
        {
            return AlmostEqual(mCenter, Other.mCenter) && AlmostEqual(mRadius, Other.mRadius);
        }

        // -=(Undocumented)=-
        Bool operator!=(ConstRef<Circle<Base>> Other) const
        {
            return !(*this == Other);
        }

        // -=(Undocumented)=-
        constexpr Circle operator+(ConstRef<Circle> Other) const
        {
            return Circle(mCenter + Other.mCenter, mRadius + Other.mRadius);
        }

        // -=(Undocumented)=-
        constexpr Circle operator+(Base Scalar) const
        {
            return Circle(mCenter, mRadius + Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Circle operator+(ConstRef<Vector2<Type>> Vector) const
        {
            return Circle(mCenter + Vector, mRadius);
        }

        // -=(Undocumented)=-
        constexpr Circle operator-(ConstRef<Circle> Other) const
        {
            return Circle(mCenter - Other.mCenter, mRadius - Other.mRadius);
        }

        // -=(Undocumented)=-
        constexpr Circle operator-(Base Scalar) const
        {
            return Circle(mCenter, mRadius - Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Circle operator-(ConstRef<Vector2<Type>> Vector) const
        {
            return Circle(mCenter - Vector, mRadius);
        }

        // -=(Undocumented)=-
        constexpr Circle operator*(Base Scalar) const
        {
            return Circle(mCenter * Scalar, mRadius * Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Circle operator*(ConstRef<Vector2<Type>> Vector) const
        {
            return Circle(mCenter * Vector, mRadius);
        }

        // -=(Undocumented)=-
        constexpr Circle operator/(Base Scalar) const
        {
            return Circle(mCenter / Scalar, mRadius / Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Circle operator/(ConstRef<Vector2<Type>> Vector) const
        {
            return Circle(mCenter / Vector, mRadius);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator+=(ConstRef<Circle> Other)
        {
            mCenter += Other.mCenter;
            mRadius += Other.mRadius;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator+=(Base Scalar)
        {
            mRadius += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Circle> operator+=(ConstRef<Vector2<Type>> Vector)
        {
            mCenter += Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator-=(ConstRef<Circle> Other)
        {
            mCenter -= Other.mCenter;
            mRadius -= Other.mRadius;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator-=(Base Scalar)
        {
            mRadius -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Circle> operator-=(ConstRef<Vector2<Type>> Vector)
        {
            mCenter -= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator*=(Base Scalar)
        {
            mCenter *= Scalar;
            mRadius *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Circle> operator*=(ConstRef<Vector2<Type>> Vector)
        {
            mCenter *= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Circle> operator/=(Base Scalar)
        {
            mCenter /= Scalar;
            mRadius /= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Circle> operator/=(ConstRef<Vector2<Type>> Vector)
        {
            mCenter /= Vector;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeObject(mCenter);
            Archive.SerializeNumber(mRadius);
        }

    public:

        // -=(Undocumented)=-
        static Circle<Base> Min(ConstRef<Circle<Base>> Lhs, ConstRef<Circle<Base>> Rhs)
        {
            return Circle<Base>(Vector2<Base>::Min(Lhs.mCenter, Rhs.mCenter), Core::Min(Lhs.mRadius, Rhs.mRadius));
        }

        // -=(Undocumented)=-
        static Circle<Base> Max(ConstRef<Circle<Base>> Lhs, ConstRef<Circle<Base>> Rhs)
        {
            return Circle<Base>(Vector2<Base>::Max(Lhs.mCenter, Rhs.mCenter), Core::Max(Lhs.mRadius, Rhs.mRadius));
        }

        // -=(Undocumented)=-
        static Circle<Base> Lerp(ConstRef<Circle<Base>> Start, ConstRef<Circle<Base>> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        // -=(Undocumented)=-
        static Circle<Base> Transform(ConstRef<Circle<Base>> Circle, Pivot Pivot)
        {
            static constexpr Vector2<Base> k_Multiplier[] = {
                Vector2<Real32>( 0.0,  0.0),  // LeftTop
                Vector2<Real32>( 0.0, -0.5),  // LeftMiddle
                Vector2<Real32>( 0.0, -1.0),  // LeftBottom
                Vector2<Real32>(-0.5,  0.0),  // CenterTop
                Vector2<Real32>(-0.5, -0.5),  // CenterMiddle
                Vector2<Real32>(-0.5, -1.0),  // CenterBottom
                Vector2<Real32>(-1.0,  0.0),  // RightTop
                Vector2<Real32>(-1.0, -0.5),  // RightMiddle
                Vector2<Real32>(-1.0, -1.0),  // RightBottom
            };

            const Vector2<Base> Offset = k_Multiplier[CastEnum(Pivot)] * (Circle.mRadius * 2);
            return Math::Circle<Base>(Circle.GetCenter() + Offset, Circle.mRadius);
        }

        // -=(Undocumented)=-
        static Circle<Base> Transform(ConstRef<Circle<Base>> Circle, ConstRef<Matrix4<Base>> Transform)
        {
            const Vector2<Base> Center = Transform * Circle.GetCenter();
            const Vector2<Base> Edge   = Transform * Vector2f(Circle.GetCenter() + Vector2<Base>(Circle.mRadius, 0));
            const Base          Radius = (Edge - Center).GetLength();
            return Math::Circle<Base>(Center, Radius);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2<Base> mCenter;
        Base          mRadius;
    };

    // -=(Undocumented)=-
    using Circlef = Circle<Real32>;
}