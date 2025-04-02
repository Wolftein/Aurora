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
            return mX1 == 0 && mY1 == 0 && mX2 == 0 && mY2 == 0;
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
            return Vector2<Base>(mX1 + GetWidth() * 0.5f, mY1 + GetHeight() * 0.5f);
        }

        // -=(Undocumented)=-
        Bool Contains(ConstRef<Rect<Base>> Other) const
        {
            return mX1 <= Other.mX1 && mX2 >= Other.mX2 && mY1 <= Other.mY1 && mY2 >= Other.mY2;
        }

        // -=(Undocumented)=-
        Bool Contains(Base X, Base Y) const
        {
            return X >= mX1 && X < mX2 &&  Y >= mY1 && Y < mY2;
        }

        // -=(Undocumented)=-
        Bool Intersects(ConstRef<Rect<Base>> Other) const
        {
            return mX1 < Other.mX2 && mX2 > Other.mX1 && mY1 < Other.mY2 && mY2 > Other.mY1;
        }

        // -=(Undocumented)=-
        Bool operator==(ConstRef<Rect<Base>> Other) const
        {
            return mX1 == Other.mX1 && mY1 == Other.mY1 && mX2 == Other.mX2 && mY2 == Other.mY2;
        }

        // -=(Undocumented)=-
        Bool operator!=(ConstRef<Rect<Base>> Other) const
        {
            return mX1 != Other.mX1 || mY1 != Other.mY1 || mX2 != Other.mX2 || mY2 != Other.mY2;
        }

        // -=(Undocumented)=-
        Rect<Base> operator+(ConstRef<Rect<Base>> Other) const
        {
            return Rect<Base>(mX1 + Other.mX1, mY1 + Other.mY1, mX2 + Other.mX2, mY2 + Other.mY2);
        }

        // -=(Undocumented)=-
        Rect<Base> operator+(Base Scalar) const
        {
            return Rect<Base>(mX1 + Scalar, mY1 + Scalar, mX2 + Scalar, mY2 + Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Rect<Base> operator+(Vector2<Type> Vector) const
        {
            return Rect<Base>(mX1 + Vector.GetX(), mY1 + Vector.GetY(), mX2 + Vector.GetX(), mY2 + Vector.GetY());
        }

        // -=(Undocumented)=-
        Rect<Base> operator-(ConstRef<Rect<Base>> Other) const
        {
            return Rect<Base>(mX1 - Other.mX1, mY1 - Other.mY1, mX2 - Other.mX2, mY2 - Other.mY2);
        }

        // -=(Undocumented)=-
        Rect<Base> operator-(Base Scalar) const
        {
            return Rect<Base>(mX1 - Scalar, mY1 - Scalar, mX2 - Scalar, mY2 - Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Rect<Base> operator-(Vector2<Type> Vector) const
        {
            return Rect<Base>(mX1 - Vector.GetX(), mY1 - Vector.GetY(), mX2 - Vector.GetX(), mY2 - Vector.GetY());
        }

        // -=(Undocumented)=-
        Rect<Base> operator*(Base Scalar) const
        {
            return Rect<Base>(mX1 * Scalar, mY1 * Scalar, mX2 * Scalar, mY2 * Scalar);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Rect<Base> operator*(Vector2<Type> Vector) const
        {
            return Rect<Base>(mX1 * Vector.GetX(), mY1 * Vector.GetY(), mX2 * Vector.GetX(), mY2 * Vector.GetY());
        }

        // -=(Undocumented)=-
        Rect<Base> operator/(Base Scalar) const
        {
            return Rect<Base>(mX1 / Scalar, mY1 / Scalar, mX2 / Scalar, mY2 / Scalar);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator+=(ConstRef<Rect<Base>> Other)
        {
            mX1 += Other.mX1;
            mY1 += Other.mY1;
            mX2 += Other.mX2;
            mY2 += Other.mY2;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator+=(Base Scalar)
        {
            mX1 += Scalar;
            mY1 += Scalar;
            mX2 += Scalar;
            mY2 += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Rect<Base>> operator+=(Vector2<Type> Vector)
        {
            mX1 += Vector.GetX();
            mY1 += Vector.GetY();
            mX2 += Vector.GetX();
            mY2 += Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator-=(ConstRef<Rect<Base>> Other)
        {
            mX1 -= Other.mX1;
            mY1 -= Other.mY1;
            mX2 -= Other.mX2;
            mY2 -= Other.mY2;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator-=(Base Scalar)
        {
            mX1 -= Scalar;
            mY1 -= Scalar;
            mX2 -= Scalar;
            mY2 -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Rect<Base>> operator-=(Vector2<Type> Vector)
        {
            mX1 -= Vector.GetX();
            mY1 -= Vector.GetY();
            mX2 -= Vector.GetX();
            mY2 -= Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator*=(Base Scalar)
        {
            mX1 *= Scalar;
            mY1 *= Scalar;
            mX2 *= Scalar;
            mY2 *= Scalar;
            
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Rect<Base>> operator*=(Vector2<Type> Vector)
        {
            mX1 *= Vector.GetX();
            mY1 *= Vector.GetY();
            mX2 *= Vector.GetX();
            mY2 *= Vector.GetY();

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Rect<Base>> operator/=(Base Scalar)
        {
            mX1 /= Scalar;
            mY1 /= Scalar;
            mX2 /= Scalar;
            mY2 /= Scalar;
            
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
        static Rect<Base> Overlap(ConstRef<Rect<Base>> Lhs, ConstRef<Rect<Base>> Rhs)
        {
            const Base X1 = Core::Max(Lhs.mX1, Rhs.mX1);
            const Base Y1 = Core::Max(Lhs.mY1, Rhs.mY1);
            const Base X2 = Core::Min(Lhs.mX2, Rhs.mX2);
            const Base Y2 = Core::Min(Lhs.mY2, Rhs.mY2);
            return Rect<Base>(X1, Y1, X2, Y2);
        }

        // -=(Undocumented)=-
        static Rect<Base> Min(ConstRef<Rect<Base>> Lhs, ConstRef<Rect<Base>> Rhs)
        {
            const Base X1 = Lhs.mX1 < Rhs.mX1 ? Lhs.mX1 : Rhs.mX1;
            const Base Y1 = Lhs.mY1 < Rhs.mY1 ? Lhs.mY1 : Rhs.mY1;
            const Base X2 = Lhs.mX2 < Rhs.mX2 ? Lhs.mX2 : Rhs.mX2;
            const Base Y2 = Lhs.mY2 < Rhs.mY2 ? Lhs.mY2 : Rhs.mY2;
            return Rect<Base>(X1, Y1, X2, Y2);
        }

        // -=(Undocumented)=-
        static Rect<Base> Max(ConstRef<Rect<Base>> Lhs, ConstRef<Rect<Base>> Rhs)
        {
            const Base X1 = Lhs.mX1 < Rhs.mX1 ? Rhs.mX1 : Lhs.mX1;
            const Base Y1 = Lhs.mY1 < Rhs.mY1 ? Rhs.mY1 : Lhs.mY1;
            const Base X2 = Lhs.mX2 < Rhs.mX2 ? Rhs.mX2 : Lhs.mX2;
            const Base Y2 = Lhs.mY2 < Rhs.mY2 ? Rhs.mY2 : Lhs.mY2;
            return Rect<Base>(X1, Y1, X2, Y2);
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