// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Core/Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Base>
    class Vector2 final
    {
    public:

        // -=(Undocumented)=-
        Vector2(Base X = 0, Base Y = 0)
            : mX { X },
              mY { Y }
        {
        }

        // -=(Undocumented)=-
        Vector2(Ref<const Vector2> Other)
            : mX { Other.GetX() },
              mY { Other.GetY() }
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
        Real32 GetAngle(Ref<const Vector2<Base>> Other) const
        {
            const Real32 Divisor = (GetLength() * Other.GetLength());

            if (Divisor != 0)
            {
                const Real32 Cosine = Dot(Other) / Divisor;

                return (Cosine <= 1.0f ? acos(Cosine) : 0.0f);
            }
            return 0.0f;
        }

        // -=(Undocumented)=-
        Real32 GetDistance(Ref<const Vector2<Base>> Other) const
        {
            const Vector2<Base> Result = (* this) - Other;

            return Result.GetLength();
        }

        // -=(Undocumented)=-
        Real32 GetLength() const
        {
            return sqrt(GetLengthSquared());
        }

        // -=(Undocumented)=-
        Real32 GetLengthSquared() const
        {
            return (mX * mX + mY * mY);
        }

        // -=(Undocumented)=-
        Real32 Dot(Ref<const Vector2<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator+(Ref<const Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX + Vector.mX, mY + Vector.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator+(Base Scalar) const
        {
            return Vector2<Base>(mX + Scalar, mY + Scalar);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator-() const
        {
            return Vector2<Base>(-mX, -mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator-(Ref<const Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX - Vector.mX, mY - Vector.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator-(Base Scalar) const
        {
            return Vector2<Base>(mX - Scalar, mY - Scalar);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator*(Ref<const Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX * Vector.mX, mY * Vector.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator*(Base Scalar) const
        {
            return Vector2<Base>(mX * Scalar, mY * Scalar);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator/(Ref<const Vector2<Base>> Vector) const
        {
            return Vector2<Base>(mX / Vector.mX, mY / Vector.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator/(Base Scalar) const
        {
            return Vector2<Base>(mX / Scalar, mY / Scalar);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator++()
        {
            ++mX;
            ++mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator--()
        {
            --mX;
            --mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator+=(Ref<const Vector2<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator-=(Ref<const Vector2<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator*=(Ref<const Vector2<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator/=(Ref<const Vector2<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            return (* this);
        }

    public:

        // -=(Undocumented)=-
        static Vector2<Base> Normalize(Ref<const Vector2<Base>> Vector)
        {
            const Real32 Length = Vector.GetLength();

            if (Length > 0)
            {
                const Real32 InvLength = 1.0f / Length;

                return Vector * Length;
            }
            return Vector;
        }

        // -=(Undocumented)=-
        static Vector2<Base> Min(Ref<const Vector2<Base>> Lhs, Ref<const Vector2<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Lhs.GetX() : Rhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Lhs.GetY() : Rhs.GetY());
            return Vector2<Base>(X, Y);
        }

        // -=(Undocumented)=-
        static Vector2<Base> Max(Ref<const Vector2<Base>> Lhs, Ref<const Vector2<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Rhs.GetX() : Lhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Rhs.GetY() : Lhs.GetY());
            return Vector2<Base>(X, Y);
        }

        // -=(Undocumented)=-
        static Vector2<Base> Lerp(Ref<const Vector2<Base>> Start, Ref<const Vector2<Base>> End, Real32 Percentage)
        {
            return (Start + Percentage * (End - Start));
        }

        // -=(Undocumented)=-
        static Vector2<Base> Slerp(Ref<const Vector2<Base>> Start, Ref<const Vector2<Base>> End, Real32 Percentage)
        {
            const Real32 Dot   = eastl::clamp(Start.Dot(End), -1.0f, +1.0f);
            const Real32 Theta = std::acos(Dot) * Percentage;

            const Vector2<Base> Relative = Normalize(End - Start * Dot);

            return (Start * std::cos(Theta)) + (Relative * std::sin(Theta));
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