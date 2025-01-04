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
    class Vector4 final
    {
    public:

        // -=(Undocumented)=-
        constexpr Vector4()
            : mX { 0 },
              mY { 0 },
              mZ { 0 },
              mW { 0 }
        {
        }

        // -=(Undocumented)=-
        constexpr Vector4(Base X, Base Y, Base Z, Base W)
            : mX { X },
              mY { Y },
              mZ { Z },
              mW { W }
        {
        }

        // -=(Undocumented)=-
        template<typename Any>
        constexpr Vector4(ConstRef<Vector4<Any>> Other)
            : mX { static_cast<Base>(Other.GetX()) },
              mY { static_cast<Base>(Other.GetY()) },
              mZ { static_cast<Base>(Other.GetZ()) },
              mW { static_cast<Base>(Other.GetW()) }
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
        Real32 GetAngle(ConstRef<Vector4<Base>> Other) const
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
        Real32 GetDistance(ConstRef<Vector4<Base>> Other) const
        {
            const Vector4<Base> Result = (* this) - Other;

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
            return (mX * mX + mY * mY + mZ * mZ + mW * mW);
        }

        // -=(Undocumented)=-
        Real32 Dot(ConstRef<Vector4<Base>> Other) const
        {
            return (mX * Other.mX) + (mY * Other.mY) + (mZ * Other.mZ) + (mW * Other.mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator+(ConstRef<Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX + Vector.mX, mY + Vector.mY, mZ + Vector.mZ, mW + Vector.mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator+(Base Scalar) const
        {
            return Vector4<Base>(mX + Scalar, mY + Scalar, mZ + Scalar, mW + Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator-() const
        {
            return Vector4<Base>(-mX, -mY, -mZ, -mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator-(ConstRef<Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX - Vector.mX, mY - Vector.mY, mZ - Vector.mZ, mW - Vector.mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator-(Base Scalar) const
        {
            return Vector4<Base>(mX - Scalar, mY - Scalar, mZ - Scalar, mW - Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator*(ConstRef<Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX * Vector.mX, mY * Vector.mY, mZ * Vector.mZ, mW * Vector.mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator*(Base Scalar) const
        {
            return Vector4<Base>(mX * Scalar, mY * Scalar, mZ * Scalar, mW * Scalar);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator/(ConstRef<Vector4<Base>> Vector) const
        {
            return Vector4<Base>(mX / Vector.mX, mY / Vector.mY, mZ / Vector.mZ, mW / Vector.mW);
        }

        // -=(Undocumented)=-
        constexpr Vector4<Base> operator/(Base Scalar) const
        {
            return Vector4<Base>(mX / Scalar, mY / Scalar, mZ / Scalar, mW / Scalar);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            ++mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            --mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator+=(ConstRef<Vector4<Base>> Vector)
        {
            mX += Vector.mX;
            mY += Vector.mY;
            mZ += Vector.mZ;
            mW += Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator+=(Base Scalar)
        {
            mX += Scalar;
            mY += Scalar;
            mZ += Scalar;
            mW += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator-=(ConstRef<Vector4<Base>> Vector)
        {
            mX -= Vector.mX;
            mY -= Vector.mY;
            mZ -= Vector.mZ;
            mW -= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator-=(Base Scalar)
        {
            mX -= Scalar;
            mY -= Scalar;
            mZ -= Scalar;
            mW -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator*=(ConstRef<Vector4<Base>> Vector)
        {
            mX *= Vector.mX;
            mY *= Vector.mY;
            mZ *= Vector.mZ;
            mW *= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator*=(Base Scalar)
        {
            mX *= Scalar;
            mY *= Scalar;
            mZ *= Scalar;
            mW *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator/=(ConstRef<Vector4<Base>> Vector)
        {
            mX /= Vector.mX;
            mY /= Vector.mY;
            mZ /= Vector.mZ;
            mW /= Vector.mW;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Ref<Vector4<Base>> operator/=(Base Scalar)
        {
            mX /= Scalar;
            mY /= Scalar;
            mZ /= Scalar;
            mW /= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        constexpr Bool operator==(ConstRef<Vector4<Base>> Vector) const
        {
            return mX == Vector.mX && mY == Vector.mY && mZ == Vector.mZ && mW == Vector.mW;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeNumber(mX);
            Archive.SerializeNumber(mY);
            Archive.SerializeNumber(mZ);
            Archive.SerializeNumber(mW);
        }

    public:

        // -=(Undocumented)=-
        static Vector4<Base> Normalize(ConstRef<Vector4<Base>> Vector)
        {
            const Real32 Length = Vector.GetLength();

            if (Length > 0)
            {
                return Vector * (1.0f / Length);
            }
            return Vector;
        }

        // -=(Undocumented)=-
        static Vector4<Base> Min(ConstRef<Vector4<Base>> P0, ConstRef<Vector4<Base>> P1)
        {
            const Base X = P0.GetX() < P1.GetX() ? P0.GetX() : P1.GetX();
            const Base Y = P0.GetY() < P1.GetY() ? P0.GetY() : P1.GetY();
            const Base Z = P0.GetZ() < P1.GetZ() ? P0.GetZ() : P1.GetZ();
            const Base W = P0.GetW() < P1.GetW() ? P0.GetW() : P1.GetW();
            return Vector4<Base>(X, Y, Z, W);
        }

        // -=(Undocumented)=-
        static Vector4<Base> Max(ConstRef<Vector4<Base>> P0, ConstRef<Vector4<Base>> P1)
        {
            const Base X = P0.GetX() < P1.GetX() ? P1.GetX() : P0.GetX();
            const Base Y = P0.GetY() < P1.GetY() ? P1.GetY() : P0.GetY();
            const Base Z = P0.GetZ() < P1.GetZ() ? P1.GetZ() : P0.GetZ();
            const Base W = P0.GetW() < P1.GetW() ? P1.GetW() : P0.GetW();
            return Vector4<Base>(X, Y, Z, W);
        }

        // -=(Undocumented)=-
        static Vector4<Base> Lerp(ConstRef<Vector4<Base>> Start, ConstRef<Vector4<Base>> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

        // -=(Undocumented)=-
        static Vector4<Base> Slerp(ConstRef<Vector4<Base>> Start, ConstRef<Vector4<Base>> End, Real32 Percentage)
        {
            const Base Dot   = Clamp(Start.Dot(End), -1.0, +1.0);
            const Base Theta = InvCosine(Dot) * Percentage;

            const Vector4<Base> Relative = Normalize(End - Start * Dot);

            return Start * Cosine(Theta) + Relative * Sine(Theta);
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