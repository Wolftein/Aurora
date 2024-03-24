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

#include "Core/Types.hpp"
#include "Trigonometry.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Base>
    class Vector3 final
    {
    public:

        // -=(Undocumented)=-
        constexpr Vector3()
            : mX { 0 },
              mY { 0 },
              mZ { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Vector3(Base X, Base Y, Base Z)
            : mX { X },
              mY { Y },
              mZ { Z }
        {
        }

        // -=(Undocumented)=-
        constexpr Vector3(Ref<const Vector3> Other)
            : mX { Other.GetX() },
              mY { Other.GetY() },
              mZ { Other.GetZ() }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return mX == 0 && mY == 0 && mZ == 0;
        }

        // -=(Undocumented)=-
        void Set(Base X, Base Y, Base Z)
        {
            mX = X;
            mY = Y;
            mZ = Z;
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
        void SetZ(Base Value)
        {
            mZ = Value;
        }

        // -=(Undocumented)=-
        Base GetZ() const
        {
            return mZ;
        }

        // -=(Undocumented)=-
        Real32 GetAngle(Ref<const Vector3<Base>> Other) const
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
        Real32 GetDistance(Ref<const Vector3<Base>> Other) const
        {
            const Vector3<Base> Result = (* this) - Other;

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
            return (mX * mX + mY * mY + mZ * mZ);
        }

        // -=(Undocumented)=-
        Real32 Dot(Ref<const Vector3<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY) + (mZ * Other.mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator+(Ref<const Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX + Vector.mX, mY + Vector.mY, mZ + Vector.mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator+(Base Scalar) const
        {
            return Vector3<Base>(mX + Scalar, mY + Scalar, mZ + Scalar);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator-() const
        {
            return Vector3<Base>(-mX, -mY, -mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator-(Ref<const Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX - Vector.mX, mY - Vector.mY, mZ - Vector.mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator-(Base Scalar) const
        {
            return Vector3<Base>(mX - Scalar, mY - Scalar, mZ - Scalar);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator*(Ref<const Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX * Vector.mX, mY * Vector.mY, mZ * Vector.mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator*(Base Scalar) const
        {
            return Vector3<Base>(mX * Scalar, mY * Scalar, mZ * Scalar);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator/(Ref<const Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX / Vector.mX, mY / Vector.mY, mZ / Vector.mZ);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator/(Base Scalar) const
        {
            return Vector3<Base>(mX / Scalar, mY / Scalar, mZ / Scalar);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator+=(Ref<const Vector3<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            mZ += Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            mZ += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator-=(Ref<const Vector3<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            mZ -= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            mZ -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator*=(Ref<const Vector3<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            mZ *= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            mZ *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator/=(Ref<const Vector3<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            mZ /= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector3<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            mZ /= Scalar;
            return (* this);
        }

    public:

        // -=(Undocumented)=-
        static Vector3<Base> Normalize(Ref<const Vector3<Base>> Vector)
        {
            const Real32 Length = Vector.GetLength();

            if (Length > 0)
            {
                return Vector * (1.0f / Length);
            }
            return Vector;
        }

        // -=(Undocumented)=-
        static Vector3<Base> Cross(Ref<const Vector3<Base>> Lhs, Ref<const Vector3<Base>> Rhs)
        {
            const Base X = Lhs.GetY() * Rhs.GetZ() - Lhs.GetZ() * Rhs.GetY();
            const Base Y = Lhs.GetZ() * Rhs.GetX() - Lhs.GetX() * Rhs.GetZ();
            const Base Z = Lhs.GetX() * Rhs.GetY() - Lhs.GetY() * Rhs.GetX();

            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Min(Ref<const Vector3<Base>> Lhs, Ref<const Vector3<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Lhs.GetX() : Rhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Lhs.GetY() : Rhs.GetY());
            const Base Z = (Lhs.GetZ() < Rhs.GetZ() ? Lhs.GetZ() : Rhs.GetZ());
            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Max(Ref<const Vector3<Base>> Lhs, Ref<const Vector3<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Rhs.GetX() : Lhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Rhs.GetY() : Lhs.GetY());
            const Base Z = (Lhs.GetZ() < Rhs.GetZ() ? Rhs.GetZ() : Lhs.GetZ());
            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Lerp(Ref<const Vector3<Base>> Start, Ref<const Vector3<Base>> End, Real32 Percentage)
        {
            return (Start + (End - Start) * Percentage);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Slerp(Ref<const Vector3<Base>> Start, Ref<const Vector3<Base>> End, Real32 Percentage)
        {
            const Real32 Dot   = clamp(Start.Dot(End), -1.0f, +1.0f);
            const Real32 Theta = InverseCosine(Dot) * Percentage;

            const Vector3<Base> Relative = Normalize(End - Start * Dot);

            return (Start * Cosine(Theta)) + (Relative * Sine(Theta));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Base mX;
        Base mY;
        Base mZ;
    };

    // -=(Undocumented)=-
    using Vector3f = Vector3<Real32>;

    // -=(Undocumented)=-
    using Vector3i = Vector3<SInt32>;
}