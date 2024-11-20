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

#include "Trigonometry.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Vector2 final
    {
    public:

        // -=(Undocumented)=-
        constexpr Vector2()
            : mX { 0 },
              mY { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Vector2(Base X, Base Y)
            : mX { X },
              mY { Y }
        {
        }

        // -=(Undocumented)=-
        template<typename Any>
        constexpr Vector2(ConstRef<Vector2<Any>> Other)
            : mX { static_cast<Base>(Other.GetX()) },
              mY { static_cast<Base>(Other.GetY()) }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return mX == 0 && mY == 0;
        }

        // -=(Undocumented)=-
        void Set(Base X, Base Y)
        {
            mX = X;
            mY = Y;
        }

        // -=(Undocumented)=-
        void SetX(Base Value)
        {
            mX = Value;
        }

        // -=(Undocumented)=-
        Base GetX() const
        {
            return mX;
        }

        // -=(Undocumented)=-
        void SetY(Base Value)
        {
            mY = Value;
        }

        // -=(Undocumented)=-
        Base GetY() const
        {
            return mY;
        }

        // -=(Undocumented)=-
        Real32 GetAngle(ConstRef<Vector2<Base>> Other) const
        {
            const Real32 Divisor = GetLength() * Other.GetLength();

            if (Divisor != 0)
            {
                const Real32 Cosine = Dot(Other) / Divisor;

                return (Cosine <= 1.0f ? InvCosine(Cosine) : 0.0f);
            }
            return 0.0f;
        }

        // -=(Undocumented)=-
        Real32 GetDistance(ConstRef<Vector2<Base>> Other) const
        {
            const Vector2<Base> Result = (* this) - Other;

            return Result.GetLength();
        }

        // -=(Undocumented)=-
        Real32 GetLength() const
        {
            return Sqrt(GetLengthSquared());
        }

        // -=(Undocumented)=-
        Real32 GetLengthSquared() const
        {
            return (mX * mX + mY * mY);
        }

        // -=(Undocumented)=-
        Real32 Dot(ConstRef<Vector2<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator+(ConstRef<Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX + Vector.mX, mY + Vector.mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator+(Base Scalar) const
        {
            return Vector2<Base>(mX + Scalar, mY + Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator-() const
        {
            return Vector2<Base>(-mX, -mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator-(ConstRef<Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX - Vector.mX, mY - Vector.mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator-(Base Scalar) const
        {
            return Vector2<Base>(mX - Scalar, mY - Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator*(ConstRef<Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX * Vector.mX, mY * Vector.mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator*(Base Scalar) const
        {
            return Vector2<Base>(mX * Scalar, mY * Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator/(ConstRef<Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX / Vector.mX, mY / Vector.mY);
        }

        // -=(Undocumented)=-
        constexpr Vector2<Base> operator/(Base Scalar) const
        {
            return Vector2<Base>(mX / Scalar, mY / Scalar);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator++()
        {
            ++mX;
            ++mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator--()
        {
            --mX;
            --mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator+=(ConstRef<Vector2<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator-=(ConstRef<Vector2<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator*=(ConstRef<Vector2<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator/=(ConstRef<Vector2<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector2<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Bool operator==(ConstRef<Vector2<Base>> Vector) const
        {
            return mX == Vector.mX && mY == Vector.mY;
        }

        // -=(Undocumented)=-
        constexpr Bool operator<(ConstRef<Vector2<Base>> Vector) const
        {
            return mX < Vector.mX || (mX == Vector.mX && mY < Vector.mY);
        }

        // -=(Undocumented)=-
        constexpr Bool operator>(ConstRef<Vector2<Base>> Vector) const
        {
            return mX > Vector.mX || (mX == Vector.mX && mY > Vector.mY);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeNumber(mX);
            Archive.SerializeNumber(mY);
        }

    public:

        // -=(Undocumented)=-
        static Vector2<Base> Normalize(ConstRef<Vector2<Base>> Vector)
        {
            const Real32 Length = Vector.GetLength();

            if (Length > 0)
            {
                return Vector * (1.0f / Length);
            }
            return Vector;
        }

        // -=(Undocumented)=-
        static Base Cross(ConstRef<Vector2<Base>> P0, ConstRef<Vector2<Base>> P1)
        {
            return P0.GetX() * P1.GetY() - P0.GetY() * P1.GetX();
        }

        // -=(Undocumented)=-
        static Vector2<Base> Min(ConstRef<Vector2<Base>> P0, ConstRef<Vector2<Base>> P1)
        {
            const Base X = P0.GetX() < P1.GetX() ? P0.GetX() : P1.GetX();
            const Base Y = P0.GetY() < P1.GetY() ? P0.GetY() : P1.GetY();
            return Vector2<Base>(X, Y);
        }

        // -=(Undocumented)=-
        static Vector2<Base> Max(ConstRef<Vector2<Base>> P0, ConstRef<Vector2<Base>> P1)
        {
            const Base X = P0.GetX() < P1.GetX() ? P1.GetX() : P0.GetX();
            const Base Y = P0.GetY() < P1.GetY() ? P1.GetY() : P0.GetY();
            return Vector2<Base>(X, Y);
        }

        // -=(Undocumented)=-
        static Vector2<Base> Lerp(ConstRef<Vector2<Base>> Start, ConstRef<Vector2<Base>> End, Real32 Percentage)
        {
            return (Start + (End - Start) * Percentage);
        }

        // -=(Undocumented)=-
        static Vector2<Base> Slerp(ConstRef<Vector2<Base>> Start, ConstRef<Vector2<Base>> End, Real32 Percentage)
        {
            const Base Dot   = Clamp(Start.Dot(End), -1.0, +1.0);
            const Base Theta = InvCosine(Dot) * Percentage;

            const Vector2<Base> Relative = Normalize(End - Start * Dot);

            return (Start * Cosine(Theta)) + (Relative * Sine(Theta));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Base mX;
        Base mY;
    };

    // -=(Undocumented)=-
    using Vector2f = Vector2<Real32>;

    // -=(Undocumented)=-
    using Vector2i = Vector2<SInt32>;
}