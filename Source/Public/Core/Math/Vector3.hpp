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
        template<typename Any>
        constexpr Vector3(Ref<const Vector3<Any>> Other)
            : mX { static_cast<Base>(Other.GetX()) },
              mY { static_cast<Base>(Other.GetY()) },
              mZ { static_cast<Base>(Other.GetZ()) }
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
        template<typename Reader>
        static Vector3<Base> Read(Ref<Reader> Input)
        {
            const Base X = Input.template Read<Base>();
            const Base Y = Input.template Read<Base>();
            const Base Z = Input.template Read<Base>();
            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        template<typename Writer>
        static void Write(Ref<Writer> Output, Ref<const Vector3<Base>> Vector)
        {
            Output.Write(Vector.GetX());
            Output.Write(Vector.GetY());
            Output.Write(Vector.GetZ());
        }

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
        static Vector3<Base> Cross(Ref<const Vector3<Base>> P0, Ref<const Vector3<Base>> P1)
        {
            const Base X = P0.GetY() * P1.GetZ() - P0.GetZ() * P1.GetY();
            const Base Y = P0.GetZ() * P1.GetX() - P0.GetX() * P1.GetZ();
            const Base Z = P0.GetX() * P1.GetY() - P0.GetY() * P1.GetX();

            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Min(Ref<const Vector3<Base>> P0, Ref<const Vector3<Base>> P1)
        {
            const Base X = (P0.GetX() < P1.GetX() ? P0.GetX() : P1.GetX());
            const Base Y = (P0.GetY() < P1.GetY() ? P0.GetY() : P1.GetY());
            const Base Z = (P0.GetZ() < P1.GetZ() ? P0.GetZ() : P1.GetZ());
            return Vector3<Base>(X, Y, Z);
        }

        // -=(Undocumented)=-
        static Vector3<Base> Max(Ref<const Vector3<Base>> P0, Ref<const Vector3<Base>> P1)
        {
            const Base X = (P0.GetX() < P1.GetX() ? P1.GetX() : P0.GetX());
            const Base Y = (P0.GetY() < P1.GetY() ? P1.GetY() : P0.GetY());
            const Base Z = (P0.GetZ() < P1.GetZ() ? P1.GetZ() : P0.GetZ());
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