// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Vector2.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
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
        constexpr Rect(Ref<const Vector2<Base>> Minimum, Ref<const Vector2<Base>> Maximum)
            : mX1 { Minimum.GetX() },
              mY1 { Minimum.GetY() },
              mX2 { Maximum.GetX() },
              mY2 { Maximum.GetY() }
        {
        }

        // -=(Undocumented)=-
        constexpr Rect(Ref<const Rect<Base>> Other)
            : mX1 { Other.mX1 },
              mY1 { Other.mY1 },
              mX2 { Other.mX2 },
              mY2 { Other.mY2 }
        {
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
        Bool Contains(Ref<const Rect<Base>> Other) const
        {
            return (mX1 <= Other.mX1 && Other.mX2 >= mX2 && mY1 <= Other.mY1 && Other.mY1 >= mY2);
        }

        // -=(Undocumented)=-
        Bool Contains(Base X, Base Y) const
        {
            return X >= mX1 && Y >= mY1 && X <= mX2 && Y <= mY2;
        }

        // -=(Undocumented)=-
        Bool Intersects(Ref<const Rect<Base>> Other) const
        {
            return (mX1 <= Other.mX2 && Other.mX1 <= mX2 && mY1 <= Other.mY2 && Other.mY1 <= mY2);
        }

        // -=(Undocumented)=-
        Bool operator==(Ref<const Rect<Base>> Other) const
        {
            return (mX1 == Other.mX1 && mY1 == Other.mY1 && mX2 == Other.mX2 && mY2 == Other.mY2);
        }

        // -=(Undocumented)=-
        Bool operator!=(Ref<const Rect<Base>> Other) const
        {
            return (mX1 != Other.mX1 || mY1 != Other.mY1 || mX2 != Other.mX2 || mY2 != Other.mY2);
        }

        // -=(Undocumented)=-
        Rect<Base> operator+(Ref<const Rect<Base>> Other) const
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
        Rect<Base> operator-(Ref<const Rect<Base>> Other) const
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
        Ref<Rect<Base>> operator+=(Ref<const Rect<Base>> Other)
        {
            mX1 += Other.X1;
            mY1 += Other.Y1;
            mX2 += Other.X2;
            mY2 += Other.Y2;
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
        Ref<Rect<Base>> operator-=(Ref<const Rect<Base>> Other)
        {
            mX1 -= Other.X1;
            mY1 -= Other.Y1;
            mX2 -= Other.X2;
            mY2 -= Other.Y2;
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

    public:

        // -=(Undocumented)=-
        static Rect<Base> Min(Ref<const Rect<Base>> Lhs, Ref<const Rect<Base>> Rhs)
        {
            const Base X1 = (Lhs.mX1 < Rhs.mX1 ? Lhs.mX1 : Rhs.mX1);
            const Base Y1 = (Lhs.mY1 < Rhs.mY1 ? Lhs.mY1 : Rhs.mY1);
            const Base X2 = (Lhs.mX2 < Rhs.mX2 ? Lhs.mX2 : Rhs.mX2);
            const Base Y2 = (Lhs.mY2 < Rhs.mY2 ? Lhs.mY2 : Rhs.mY2);
            return Rect<Base>(X1, Y1, X2, Y2);
        }

        // -=(Undocumented)=-
        static Rect<Base> Max(Ref<const Rect<Base>> Lhs, Ref<const Rect<Base>> Rhs)
        {
            const Base X1 = (Lhs.mX1 < Rhs.mX1 ? Rhs.mX1 : Lhs.mX1);
            const Base Y1 = (Lhs.mY1 < Rhs.mY1 ? Rhs.mY1 : Lhs.mY1);
            const Base X2 = (Lhs.mX2 < Rhs.mX2 ? Rhs.mX2 : Lhs.mX2);
            const Base Y2 = (Lhs.mY2 < Rhs.mY2 ? Rhs.mY2 : Lhs.mY2);
            return Rect<Base>(X1, Y1, X2, Y2);
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