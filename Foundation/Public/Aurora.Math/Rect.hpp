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

#include "Manifold.hpp"
#include "Matrix4.hpp"
#include "Pivot.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Rect final
    {
    public:

        // -=(Undocumented)=-
        constexpr Rect()
            : mX1 { 0 },
              mY1 { 0 },
              mX2 { 0 },
              mY2 { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Rect(Base Left, Base Top, Base Right, Base Bottom)
            : mX1 { Left },
              mY1 { Top },
              mX2 { Right },
              mY2 { Bottom }
        {
        }

        // -=(Undocumented)=-
        constexpr Rect(ConstRef<Vector2<Base>> Minimum, ConstRef<Vector2<Base>> Maximum)
            : mX1 { Minimum.GetX() },
              mY1 { Minimum.GetY() },
              mX2 { Maximum.GetX() },
              mY2 { Maximum.GetY() }
        {
        }

        // -=(Undocumented)=-
        template<typename Any>
        constexpr Rect(ConstRef<Rect<Any>> Other)
            : mX1 { static_cast<Base>(Other.GetLeft()) },
              mY1 { static_cast<Base>(Other.GetTop()) },
              mX2 { static_cast<Base>(Other.GetRight()) },
              mY2 { static_cast<Base>(Other.GetBottom()) }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return AlmostZero(mX1) && AlmostZero(mY1) && AlmostZero(mX2) && AlmostZero(mY2);
        }

        // -=(Undocumented)=-
        void Set(Base Left, Base Top, Base Right, Base Bottom)
        {
            mX1 = Left;
            mY1 = Top;
            mX2 = Right;
            mY2 = Bottom;
        }

        // -=(Undocumented)=-
        void SetLeft(Base Value)
        {
            mX1 = Value;
        }

        // -=(Undocumented)=-
        Base GetLeft() const
        {
            return mX1;
        }

        // -=(Undocumented)=-
        void SetTop(Base Value)
        {
            mY1 = Value;
        }

        // -=(Undocumented)=-
        Base GetTop() const
        {
            return mY1;
        }

        // -=(Undocumented)=-
        void SetRight(Base Value)
        {
            mX2 = Value;
        }

        // -=(Undocumented)=-
        Base GetRight() const
        {
            return mX2;
        }

        // -=(Undocumented)=-
        void SetBottom(Base Value)
        {
            mY2 = Value;
        }

        // -=(Undocumented)=-
        Base GetBottom() const
        {
            return mY2;
        }

        // -=(Undocumented)=-
        Base GetX() const
        {
            return GetLeft();
        }

        // -=(Undocumented)=-
        Base GetY() const
        {
            return GetTop();
        }

        // -=(Undocumented)=-
        Base GetWidth() const
        {
            return GetRight() - GetLeft();
        }

        // -=(Undocumented)=-
        Base GetHeight() const
        {
            return GetBottom() - GetTop();
        }

        // -=(Undocumented)=-
        Vector2<Base> GetPosition() const
        {
            return Vector2<Base>(GetX(), GetY());
        }

        // -=(Undocumented)=-
        Vector2<Base> GetSize() const
        {
            return Vector2<Base>(GetWidth(), GetHeight());
        }

        // -=(Undocumented)=-
        Vector2<Base> GetCenter() const
        {
            return Vector2<Base>(mX1 + GetWidth() * 0.5, mY1 + GetHeight() * 0.5);
        }

        // -=(Undocumented)=-
        Bool Contains(ConstRef<Rect<Base>> Other) const
        {
            return mX1 <= Other.mX1 && mX2 >= Other.mX2 && mY1 <= Other.mY1 && mY2 >= Other.mY2;
        }

        // -=(Undocumented)=-
        Bool Contains(ConstRef<Vector2<Base>> Position) const
        {
            return Position.GetX() >= mX1
                && Position.GetX() <= mX2
                && Position.GetY() >= mY1
                && Position.GetY() <= mY2;
        }

        // -=(Undocumented)=-
        Bool Intersects(ConstRef<Rect<Base>> Other, Ptr<Manifold<Base>> Manifold) const
        {
            const Base X1 = Core::Max(mX1, Other.mX1);
            const Base Y1 = Core::Max(mY1, Other.mY1);
            const Base X2 = Core::Min(mX2, Other.mX2);
            const Base Y2 = Core::Min(mY2, Other.mY2);

            if (X1 >= X2 || Y1 >= Y2)
            {
                return false;
            }

            if (Manifold)
            {
                const Base OverlapX = X2 - X1;
                const Base OverlapY = Y2 - Y1;

                if (OverlapX < OverlapY)
                {
                    Manifold->SetPenetration(OverlapX);
                    Manifold->SetNormal(Vector2<Base>((GetCenter().x < Other.GetCenter().x) ? -1 : 1, 0));
                }
                else
                {
                    Manifold->SetPenetration(OverlapY);
                    Manifold->SetNormal(Vector2<Base>(0, (GetCenter().y < Other.GetCenter().y) ? -1 : 1));
                }

                if       (AlmostZero(OverlapX))
                {
                    Manifold->AddPoint(Vector2<Base>(X1, (Y1 + Y2) * 0.5));
                }
                else if (AlmostZero(OverlapY))
                {
                    Manifold->AddPoint(Vector2<Base>((X1 + X2) * 0.5, Y1));
                }
                else
                {
                    if (X1 > mX1 && X1 > Other.mX1)
                    {
                        Manifold->AddPoint(Vector2<Base>(X1, (Y1 + Y2) * 0.5));
                    }
                    if (X2 < mX2 && X2 < Other.mX2)
                    {
                        Manifold->AddPoint(Vector2<Base>(X2, (Y1 + Y2) * 0.5));
                    }
                    if (Y1 > mY1 && Y1 > Other.mY1)
                    {
                        Manifold->AddPoint(Vector2<Base>((X1 + X2) * 0.5, Y1));
                    }
                    if (Y2 < mY2 && Y2 < Other.mY2)
                    {
                        Manifold->AddPoint(Vector2<Base>((X1 + X2) * 0.5, Y2));
                    }

                    if (! Manifold->IsCollided())
                    {
                        Manifold->AddPoint(Vector2<Base>((X1 + X2) * 0.5, (Y1 + Y2) * 0.5));
                    }
                }
            }
            return true;
        }

        // -=(Undocumented)=-
        constexpr Bool operator==(ConstRef<Rect<Base>> Other) const
        {
            return AlmostEqual(mX1, Other.mX1)
                && AlmostEqual(mX2, Other.mX2)
                && AlmostEqual(mY1, Other.mY1)
                && AlmostEqual(mY2, Other.mY2);
        }

        // -=(Undocumented)=-
        constexpr Bool operator!=(ConstRef<Rect<Base>> Other) const
        {
            return !(*this == Other);
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator+(ConstRef<Rect<Base>> Other) const
        {
            return Rect<Base>(mX1 + Other.mX1, mY1 + Other.mY1, mX2 + Other.mX2, mY2 + Other.mY2);
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator+(Base Scalar) const
        {
            return Rect<Base>(mX1 + Scalar, mY1 + Scalar, mX2 + Scalar, mY2 + Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Rect<Base> operator+(ConstRef<Vector2<Type>> Vector) const
        {
            return Rect<Base>(mX1 + Vector.GetX(), mY1 + Vector.GetY(), mX2 + Vector.GetX(), mY2 + Vector.GetY());
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator-(ConstRef<Rect<Base>> Other) const
        {
            return Rect<Base>(mX1 - Other.mX1, mY1 - Other.mY1, mX2 - Other.mX2, mY2 - Other.mY2);
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator-(Base Scalar) const
        {
            return Rect<Base>(mX1 - Scalar, mY1 - Scalar, mX2 - Scalar, mY2 - Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Rect<Base> operator-(ConstRef<Vector2<Type>> Vector) const
        {
            return Rect<Base>(mX1 - Vector.GetX(), mY1 - Vector.GetY(), mX2 - Vector.GetX(), mY2 - Vector.GetY());
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator*(Base Scalar) const
        {
            return Rect<Base>(mX1 * Scalar, mY1 * Scalar, mX2 * Scalar, mY2 * Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Rect<Base> operator*(ConstRef<Vector2<Type>> Vector) const
        {
            return Rect<Base>(mX1 * Vector.GetX(), mY1 * Vector.GetY(), mX2 * Vector.GetX(), mY2 * Vector.GetY());
        }

        // -=(Undocumented)=-
        constexpr Rect<Base> operator/(Base Scalar) const
        {
            return Rect<Base>(mX1 / Scalar, mY1 / Scalar, mX2 / Scalar, mY2 / Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Rect<Base> operator/(ConstRef<Vector2<Type>> Vector) const
        {
            return Rect<Base>(mX1 / Vector.GetX(), mY1 / Vector.GetY(), mX2 / Vector.GetX(), mY2 / Vector.GetY());
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator+=(ConstRef<Rect<Base>> Other)
        {
            mX1 += Other.mX1;
            mY1 += Other.mY1;
            mX2 += Other.mX2;
            mY2 += Other.mY2;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator+=(Base Scalar)
        {
            mX1 += Scalar;
            mY1 += Scalar;
            mX2 += Scalar;
            mY2 += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Rect<Base>> operator+=(Vector2<Type> Vector)
        {
            mX1 += Vector.GetX();
            mY1 += Vector.GetY();
            mX2 += Vector.GetX();
            mY2 += Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator-=(ConstRef<Rect<Base>> Other)
        {
            mX1 -= Other.mX1;
            mY1 -= Other.mY1;
            mX2 -= Other.mX2;
            mY2 -= Other.mY2;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator-=(Base Scalar)
        {
            mX1 -= Scalar;
            mY1 -= Scalar;
            mX2 -= Scalar;
            mY2 -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Rect<Base>> operator-=(ConstRef<Vector2<Type>> Vector)
        {
            mX1 -= Vector.GetX();
            mY1 -= Vector.GetY();
            mX2 -= Vector.GetX();
            mY2 -= Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator*=(Base Scalar)
        {
            mX1 *= Scalar;
            mY1 *= Scalar;
            mX2 *= Scalar;
            mY2 *= Scalar;
            
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Rect<Base>> operator*=(ConstRef<Vector2<Type>> Vector)
        {
            mX1 *= Vector.GetX();
            mY1 *= Vector.GetY();
            mX2 *= Vector.GetX();
            mY2 *= Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Rect<Base>> operator/=(Base Scalar)
        {
            mX1 /= Scalar;
            mY1 /= Scalar;
            mX2 /= Scalar;
            mY2 /= Scalar;
            
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        constexpr Ref<Rect<Base>> operator/=(ConstRef<Vector2<Type>> Vector)
        {
            mX1 /= Vector.GetX();
            mY1 /= Vector.GetY();
            mX2 /= Vector.GetX();
            mY2 /= Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeNumber(mX1);
            Archive.SerializeNumber(mY1);
            Archive.SerializeNumber(mX2);
            Archive.SerializeNumber(mY2);
        }

    public:

        // -=(Undocumented)=-
        static Rect<Base> Normalize(ConstRef<Rect<Base>> Rectangle)
        {
            Base Left   = Rectangle.mX1;
            Base Top    = Rectangle.mY1;
            Base Right  = Rectangle.mX2;
            Base Bottom = Rectangle.mY2;

            if (Left > Right)
            {
                Swap(Left, Right);
            }
            if (Top > Bottom)
            {
                Swap(Top, Bottom);
            }
            return Rect<Base>(Left, Top, Right, Bottom);
        }

        // -=(Undocumented)=-
        static Rect<Base> Round(ConstRef<Rect<Base>> Rectangle)
        {
            return Rect<Base>(
                Floor(Rectangle.mX1), Floor(Rectangle.mY1),
                 Ceil(Rectangle.mX2),  Ceil(Rectangle.mY2));
        }

        // -=(Undocumented)=-
        static Rect<Base> Overlap(ConstRef<Rect<Base>> First, ConstRef<Rect<Base>> Second)
        {
            const Base X1 = Core::Max(First.mX1, Second.mX1);
            const Base Y1 = Core::Max(First.mY1, Second.mY1);
            const Base X2 = Core::Min(First.mX2, Second.mX2);
            const Base Y2 = Core::Min(First.mY2, Second.mY2);

            if (X1 < X2 && Y1 < Y2)
            {
                return Rect<Base>(X1, Y1, X2, Y2);
            }
            return Rect<Base>();
        }

        // -=(Undocumented)=-
        static Rect<Base> Min(ConstRef<Rect<Base>> First, ConstRef<Rect<Base>> Second)
        {
            return Rect<Base>(
                Core::Min(First.mX1, Second.mX1),
                Core::Min(First.mY1, Second.mY1),
                Core::Min(First.mX2, Second.mX2),
                Core::Min(First.mY2, Second.mY2));
        }

        // -=(Undocumented)=-
        static Rect<Base> Max(ConstRef<Rect<Base>> First, ConstRef<Rect<Base>> Second)
        {
            return Rect<Base>(
                Core::Max(First.mX1, Second.mX1),
                Core::Max(First.mY1, Second.mY1),
                Core::Max(First.mX2, Second.mX2),
                Core::Max(First.mY2, Second.mY2));
        }

        // -=(Undocumented)=-
        static Rect<Base> Lerp(ConstRef<Rect<Base>> Start, ConstRef<Rect<Base>> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        // -=(Undocumented)=-
        static Rect<Base> Transform(ConstRef<Rect<Base>> Rectangle, Pivot Pivot)
        {
            static constexpr Rect<Real32> k_Multiplier[] = {
                Rect<Real32>( 0.0,  0.0, 1.0, 1.0),  // LeftTop
                Rect<Real32>( 0.0, -0.5, 1.0, 0.5),  // LeftMiddle
                Rect<Real32>( 0.0, -1.0, 1.0, 0.0),  // LeftBottom
                Rect<Real32>(-0.5,  0.0, 0.5, 1.0),  // CenterTop
                Rect<Real32>(-0.5, -0.5, 0.5, 0.5),  // CenterMiddle
                Rect<Real32>(-0.5, -1.0, 0.5, 0.0),  // CenterBottom
                Rect<Real32>(-1.0,  0.0, 0.0, 1.0),  // RightTop
                Rect<Real32>(-1.0, -0.5, 0.0, 0.5),  // RightMiddle
                Rect<Real32>(-1.0, -1.0, 0.0, 0.0),  // RightBottom
            };
            return k_Multiplier[CastEnum(Pivot)] *  Rectangle.GetSize() + Rectangle.GetPosition();
        }

        // -=(Undocumented)=-
        static Rect<Base> Transform(ConstRef<Rect<Base>> Rectangle, ConstRef<Matrix4<Base>> Transform)
        {
            const Vector2<Base> V0 = Transform * Vector2f(Rectangle.GetLeft(),  Rectangle.GetBottom());
            const Vector2<Base> V1 = Transform * Vector2f(Rectangle.GetRight(), Rectangle.GetBottom());
            const Vector2<Base> V2 = Transform * Vector2f(Rectangle.GetRight(), Rectangle.GetTop());
            const Vector2<Base> V3 = Transform * Vector2f(Rectangle.GetLeft(),  Rectangle.GetTop());

            const Base MinX = Core::Min(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Base MinY = Core::Min(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());
            const Base MaxX = Core::Max(V0.GetX(), V1.GetX(), V2.GetX(), V3.GetX());
            const Base MaxY = Core::Max(V0.GetY(), V1.GetY(), V2.GetY(), V3.GetY());

            return Rect<Base>(MinX, MinY, MaxX, MaxY);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Base mX1;
        Base mY1;
        Base mX2;
        Base mY2;
    };

    // -=(Undocumented)=-
    using Rectf = Rect<Real32>;

    // -=(Undocumented)=-
    using Recti = Rect<SInt32>;
}