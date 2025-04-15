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

#include "Trigonometry.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
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
        constexpr Vector3(Base Scalar)
            : mX { Scalar },
              mY { Scalar },
              mZ { Scalar }
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
        template<typename Any>
        constexpr Vector3(ConstRef<Vector3<Any>> Other)
            : mX { static_cast<Base>(Other.GetX()) },
              mY { static_cast<Base>(Other.GetY()) },
              mZ { static_cast<Base>(Other.GetZ()) }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return AlmostZero(mX) && AlmostZero(mY) && AlmostZero(mZ);
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
        Base GetAngle(ConstRef<Vector3<Base>> Other) const
        {
            const Base Divisor = GetLength() * Other.GetLength();

            if (Divisor != 0)
            {
                return InvCosine(Clamp(Dot(Other) / Divisor, -1.0, +1.0));
            }
            return 0.0;
        }

        // -=(Undocumented)=-
        Base GetDistance(ConstRef<Vector3<Base>> Other) const
        {
            const Vector3<Base> Result = (* this) - Other;

            return Result.GetLength();
        }

        // -=(Undocumented)=-
        Base GetLength() const
        {
            return Sqrt(GetLengthSquared());
        }

        // -=(Undocumented)=-
        Base GetLengthSquared() const
        {
            return (mX * mX + mY * mY + mZ * mZ);
        }

        // -=(Undocumented)=-
        Base Dot(ConstRef<Vector3<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY) + (mZ * Other.mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator+(ConstRef<Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX + Vector.mX, mY + Vector.mY, mZ + Vector.mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator+(Base Scalar) const
        {
            return Vector3<Base>(mX + Scalar, mY + Scalar, mZ + Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator-() const
        {
            return Vector3<Base>(-mX, -mY, -mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator-(ConstRef<Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX - Vector.mX, mY - Vector.mY, mZ - Vector.mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator-(Base Scalar) const
        {
            return Vector3<Base>(mX - Scalar, mY - Scalar, mZ - Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator*(ConstRef<Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX * Vector.mX, mY * Vector.mY, mZ * Vector.mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator*(Base Scalar) const
        {
            return Vector3<Base>(mX * Scalar, mY * Scalar, mZ * Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator/(ConstRef<Vector3<Base>> Vector) const
        {
            return Vector3<Base>(mX / Vector.mX, mY / Vector.mY, mZ / Vector.mZ);
        }

        // -=(Undocumented)=-
        constexpr Vector3<Base> operator/(Base Scalar) const
        {
            return Vector3<Base>(mX / Scalar, mY / Scalar, mZ / Scalar);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator+=(ConstRef<Vector3<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            mZ += Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            mZ += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator-=(ConstRef<Vector3<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            mZ -= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            mZ -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator*=(ConstRef<Vector3<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            mZ *= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            mZ *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator/=(ConstRef<Vector3<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            mZ /= Vector.mZ;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector3<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            mZ /= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Bool operator==(ConstRef<Vector3<Base>> Vector) const
        {
            return AlmostEqual(mX, Vector.mX)
                && AlmostEqual(mY, Vector.mY)
                && AlmostEqual(mZ, Vector.mZ);
        }

        // -=(Undocumented)=-
        constexpr Bool operator!=(ConstRef<Vector3<Base>> Other) const
        {
            return !(*this == Other);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeNumber(mX);
            Archive.SerializeNumber(mY);
            Archive.SerializeNumber(mZ);
        }

    public:

        // -=(Undocumented)=-
        static Vector3<Base> Normalize(ConstRef<Vector3<Base>> Vector)
        {
            const Base Length = Vector.GetLength();

            if (Length > k_Epsilon<Base>)
            {
                return Vector * (1.0 / Length);
            }
            return Vector;
        }

        // -=(Undocumented)=-
        static Vector3<Base> Cross(ConstRef<Vector3<Base>> P0, ConstRef<Vector3<Base>> P1)
        {
            const Base X = P0.GetY() * P1.GetZ() - P0.GetZ() * P1.GetY();
            const Base Y = P0.GetZ() * P1.GetX() - P0.GetX() * P1.GetZ();
            const Base Z = P0.GetX() * P1.GetY() - P0.GetY() * P1.GetX();

            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Min(ConstRef<Vector3<Base>> P0, ConstRef<Vector3<Base>> P1)
        {
            return Vector3<Base>(Core::Min(P0.mX, P1.mX), Core::Min(P0.mY, P1.mY), Core::Min(P0.mZ, P1.mZ));
        }

        // -=(Undocumented)=-
        static Vector3<Base> Max(ConstRef<Vector3<Base>> P0, ConstRef<Vector3<Base>> P1)
        {
            return Vector3<Base>(Core::Max(P0.mX, P1.mX), Core::Max(P0.mY, P1.mY), Core::Max(P0.mZ, P1.mZ));
        }

        // -=(Undocumented)=-
        static Vector3<Base> Lerp(ConstRef<Vector3<Base>> Start, ConstRef<Vector3<Base>> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        // -=(Undocumented)=-
        static Vector3<Base> Slerp(ConstRef<Vector3<Base>> Start, ConstRef<Vector3<Base>> End, Real32 Percentage)
        {
            const Base Dot   = Clamp(Start.Dot(End), -1.0, +1.0);
            const Base Theta = InvCosine(Dot) * Percentage;

            const Vector3<Base> Relative = Normalize(End - Start * Dot);

            return Start * Cosine(Theta) + (Relative * Sine(Theta));
        }

        // -=(Undocumented)=-
        static Vector3<Base> Floor(ConstRef<Vector3<Base>> Vector)
        {
            return Vector3<Base>(Math::Floor(Vector.mX), Math::Floor(Vector.mY), Math::Floor(Vector.mZ));
        }

        // -=(Undocumented)=-
        static Vector3<Base> Ceil(ConstRef<Vector3<Base>> Vector)
        {
            return Vector3<Base>(Math::Ceil(Vector.mX), Math::Ceil(Vector.mY), Math::Ceil(Vector.mZ));
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