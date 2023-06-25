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
    class Vector4 final
    {
    public:

        // -=(Undocumented)=-
        Vector4(Base X = 0, Base Y = 0, Base Z = 0, Base W = 0)
            : mX { X },
              mY { Y },
              mZ { Z },
              mW { W }
        {
        }

        // -=(Undocumented)=-
        Vector4(Ref<const Vector4> Other)
            : mX { Other.GetX() },
              mY { Other.GetY() },
              mZ { Other.GetZ() },
              mW { Other.GetW() }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return mX == 0 && mY == 0 && mZ == 0 && mW == 0;
        }

        // -=(Undocumented)=-
        void Set(Base X, Base Y, Base Z, Base W)
        {
            mX = X;
            mY = Y;
            mZ = Z;
            mW = W;
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
        void SetW(Base Value)
        {
            mW = Value;
        }

        // -=(Undocumented)=-
        Base GetW() const
        {
            return mW;
        }

        // -=(Undocumented)=-
        Real32 GetAngle(Ref<const Vector4<Base>> Other) const
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
        Real32 GetDistance(Ref<const Vector4<Base>> Other) const
        {
            const Vector4<Base> Result = (* this) - Other;

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
            return (mX * mX + mY * mY + mZ * mZ + mW * mW);
        }

        // -=(Undocumented)=-
        Real32 Dot(Ref<const Vector4<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY) + (mZ * Other.mZ) + (mW * Other.mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator+(Ref<const Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX + Vector.mX, mY + Vector.mY, mZ + Vector.mZ, mW + Vector.mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator+(Base Scalar) const
        {
            return Vector4<Base>(mX + Scalar, mY + Scalar, mZ + Scalar, mW + Scalar);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator-() const
        {
            return Vector4<Base>(-mX, -mY, -mZ, -mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator-(Ref<const Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX - Vector.mX, mY - Vector.mY, mZ - Vector.mZ, mW - Vector.mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator-(Base Scalar) const
        {
            return Vector4<Base>(mX - Scalar, mY - Scalar, mZ - Scalar, mW - Scalar);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator*(Ref<const Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX * Vector.mX, mY * Vector.mY, mZ * Vector.mZ, mW * Vector.mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator*(Base Scalar) const
        {
            return Vector4<Base>(mX * Scalar, mY * Scalar, mZ * Scalar, mW * Scalar);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator/(Ref<const Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX / Vector.mX, mY / Vector.mY, mZ / Vector.mZ, mW / Vector.mW);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator/(Base Scalar) const
        {
            return Vector4<Base>(mX / Scalar, mY / Scalar, mZ / Scalar, mW / Scalar);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            ++mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            --mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator+=(Ref<const Vector4<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            mZ += Vector.mZ;
            mW += Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            mZ += Scalar;
            mW += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator-=(Ref<const Vector4<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            mZ -= Vector.mZ;
            mW -= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            mZ -= Scalar;
            mW -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator*=(Ref<const Vector4<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            mZ *= Vector.mZ;
            mW *= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            mZ *= Scalar;
            mW *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator/=(Ref<const Vector4<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            mZ /= Vector.mZ;
            mW /= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Vector4<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            mZ /= Scalar;
            mW /= Scalar;
            return (* this);
        }

    public:

        // -=(Undocumented)=-
        static Vector4<Base> Normalize(Ref<const Vector4<Base>> Vector)
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
        static Vector4<Base> Min(Ref<const Vector4<Base>> Lhs, Ref<const Vector4<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Lhs.GetX() : Rhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Lhs.GetY() : Rhs.GetY());
            const Base Z = (Lhs.GetZ() < Rhs.GetZ() ? Lhs.GetZ() : Rhs.GetZ());
            const Base W = (Lhs.GetW() < Rhs.GetW() ? Lhs.GetW() : Rhs.GetW());
            return Vector4<Base>(X, Y, Z, W);
        }

        // -=(Undocumented)=-
        static Vector4<Base> Max(Ref<const Vector4<Base>> Lhs, Ref<const Vector4<Base>> Rhs)
        {
            const Base X = (Lhs.GetX() < Rhs.GetX() ? Rhs.GetX() : Lhs.GetX());
            const Base Y = (Lhs.GetY() < Rhs.GetY() ? Rhs.GetY() : Lhs.GetY());
            const Base Z = (Lhs.GetZ() < Rhs.GetZ() ? Rhs.GetZ() : Lhs.GetZ());
            const Base W = (Lhs.GetW() < Rhs.GetW() ? Rhs.GetW() : Lhs.GetW());
            return Vector4<Base>(X, Y, Z, W);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Base mX;
        Base mY;
        Base mZ;
        Base mW;
    };

    // -=(Undocumented)=-
    using Vector4f = Vector4<Real32>;

    // -=(Undocumented)=-
    using Vector4i = Vector4<UInt32>;
}