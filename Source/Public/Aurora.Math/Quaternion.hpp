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

#include "Vector3.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Quaternion final
    {
    public:

        // -=(Undocumented)=-
        constexpr Quaternion(Base X = 0, Base Y = 0, Base Z = 0, Base W = 1)
            : mComplexes { X, Y, Z },
              mReal      { W }
        {
        }

        // -=(Undocumented)=-
        constexpr Quaternion(Ref<const Vector3<Base>> Complexes, Base Real)
            : mComplexes { Complexes },
              mReal      { Real }
        {
        }

        // -=(Undocumented)=-
        constexpr Quaternion(Ref<const Quaternion> Other)
            : mComplexes { Other.mComplexes },
              mReal      { Other.mReal }
        {
        }

        // -=(Undocumented)=-
        Base GetX() const
        {
            return mComplexes.GetX();
        }

        // -=(Undocumented)=-
        Base GetY() const
        {
            return mComplexes.GetY();
        }

        // -=(Undocumented)=-
        Base GetZ() const
        {
            return mComplexes.GetZ();
        }

        // -=(Undocumented)=-
        Base GetW() const
        {
            return mReal;
        }

        // -=(Undocumented)=-
        Real32 GetLength() const
        {
            return Sqrt(GetLengthSquared());
        }

        // -=(Undocumented)=-
        Real32 GetLengthSquared() const
        {
            return Dot(* this);
        }

        // -=(Undocumented)=-
        Vector3<Base> GetForward() const
        {
            return Vector3<Base>(
                2.0 * (mComplexes.GetX() * mComplexes.GetZ() + mReal * mComplexes.GetY()),
                2.0 * (mComplexes.GetY() * mComplexes.GetZ() - mReal * mComplexes.GetX()),
                1.0 - 2.0 * (mComplexes.GetX() * mComplexes.GetX() + mComplexes.GetY() * mComplexes.GetY()));
        }

        // -=(Undocumented)=-
        Vector3<Base> GetUp() const
        {
            return Vector3<Base>(
                2.0 * (mComplexes.GetX() * mComplexes.GetY() - mReal * mComplexes.GetZ()),
                1.0 - 2.0 * (mComplexes.GetX() * mComplexes.GetX() + mComplexes.GetZ() * mComplexes.GetZ()),
                2.0 * (mComplexes.GetY() * mComplexes.GetZ() + mReal * mComplexes.GetX()));
        }

        // -=(Undocumented)=-
        Real32 Dot(Ref<const Quaternion<Base>> Other) const
        {
            return mReal * Other.mReal + mComplexes.Dot(Other.mComplexes);
        }

        // -=(Undocumented)=-
        Quaternion<Base> operator+(Ref<const Quaternion<Base>> Other) const
        {
            return Quaternion<Base>(mComplexes + Other.mComplexes, mReal + Other.mReal);
        }

        // -=(Undocumented)=-
        Quaternion<Base> operator-() const
        {
            return Quaternion<Base>(-mComplexes, -mReal);
        }

        // -=(Undocumented)=-
        Quaternion<Base> operator-(Ref<const Quaternion<Base>> Other) const
        {
            return Quaternion<Base>(mComplexes - Other.mComplexes, mReal - Other.mReal);
        }

        // -=(Undocumented)=-
        Quaternion<Base> operator*(Ref<const Quaternion<Base>> Other) const
        {
            const Vector3<Base> Complexes = Other.mComplexes * mReal + mComplexes * Other.mReal
                    + Vector3<Base>::Cross(mComplexes, Other.mComplexes);
            return Quaternion<Base>(Complexes, mReal * Other.mReal - mComplexes.Dot(Other.mComplexes));
        }

        // -=(Undocumented)=-
        Vector3<Base> operator*(Ref<const Vector3<Base>> Vector) const
        {
            const Vector3<Base> Cross = Vector3<Base>::Cross(mComplexes, Vector);

            const Base Real = mReal + mReal;
            return Real * Cross + (Real * mReal - 1) * Vector + 2 * mComplexes.Dot(Vector) * mComplexes;
        }

        // -=(Undocumented)=-
        Ref<Quaternion<Base>> operator+=(Ref<const Quaternion<Base>> Other)
        {
            mComplexes += Other.mComplexes;
            mReal      += Other.mReal;

            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Quaternion<Base>> operator-=(Ref<const Quaternion<Base>> Other)
        {
            mComplexes -= Other.mComplexes;
            mReal      -= Other.mReal;

            return (* this);
        }

    public:

        // -=(Undocumented)=-
        static Quaternion<Base> Normalize(Ref<const Quaternion<Base>> Value)
        {
            const Base Length = Value.GetLength();

            if (Length > 0)
            {
                const Base InvLength = 1.0f / Length;
                return Quaternion<Base>(Value.mComplexes * InvLength, Value.mReal * InvLength);
            }
            return Value;
        }

        // -=(Undocumented)=-
        static Quaternion<Base> Invert(Ref<const Quaternion<Base>> Value)
        {
            const Base Length = Value.GetLengthSquared();

            if (Length > 0)
            {
                const Base InvLength = 1.0f / Length;
                return Quaternion<Base>(Value.mComplexes * -InvLength, Value.mReal * InvLength);
            }
            return Value;
        }

        // -=(Undocumented)=-
        static Quaternion<Base> Conjugate(Ref<const Quaternion<Base>> Value)
        {
            return Quaternion<Base>(- Value.mComplexes, Value.mReal);
        }

        // -=(Undocumented)=-
        static Quaternion<Base> FromDirection(Ref<const Vector3<Base>> Direction, Ref<const Vector3<Base>> Up)
        {
            const Vector3<Base> vForward = Vector3<Base>::Normalize(Direction);
            const Vector3<Base> vRight   = Vector3<Base>::Normalize(Vector3<Base>::Cross(Up, vForward));
            const Vector3<Base> vUp      = Vector3<Base>::Cross(vForward, vRight);

            const Base M00 = vRight.GetX();
            const Base M01 = vRight.GetY();
            const Base M02 = vRight.GetZ();
            const Base M10 = vUp.GetX();
            const Base M11 = vUp.GetY();
            const Base M12 = vUp.GetZ();
            const Base M20 = vForward.GetX();
            const Base M21 = vForward.GetY();
            const Base M22 = vForward.GetZ();

            if (const Base Trace = M00 + M11 + M22; Trace > 0.0)
            {
                const Base InvSqrt = 0.5 / Sqrt(Trace + 1.0);
                return Quaternion<Base>(
                        (M12 - M21) * InvSqrt, (M20 - M02) * InvSqrt, (M01 - M10) * InvSqrt, 0.25 / InvSqrt);
            }
            else if (M00 > M11 && M00 > M22)
            {
                const Base InvSqrt = 2.0 * Sqrt(1.0 + M00 - M11 - M22);
                return Quaternion<Base>(
                        0.25 * InvSqrt, (M10 + M01) / InvSqrt, (M20 + M02) / InvSqrt, (M12 - M21) / InvSqrt);
            }
            else if (M11 > M22)
            {
                const Base InvSqrt = 2.0 * Sqrt(1.0 + M11 - M00 - M22);
                return Quaternion<Base>(
                        (M10 + M01) / InvSqrt, 0.25 * InvSqrt, (M21 + M12) / InvSqrt, (M20 - M02) / InvSqrt);
            }
            else
            {
                const Base InvSqrt = 2.0 * Sqrt(1.0 + M22 - M00 - M11);
                return Quaternion<Base>(
                        (M20 + M02) / InvSqrt, (M21 + M12) / InvSqrt, 0.25 * InvSqrt, (M01 - M10) / InvSqrt);
            }
        }

        // -=(Undocumented)=-
        static constexpr Quaternion<Base> FromAngles(Base Angle, Ref<const Vector3<Base>> Axis)
        {
            const Base HalfAngle = 0.5 * Angle;

            return Quaternion<Base>(Vector3<Base>::Normalize(Axis) * Sine(HalfAngle), Cosine(HalfAngle));
        }

        // -=(Undocumented)=-
        static constexpr Quaternion<Base> FromEulerAngles(Ref<const Vector3<Base>> Angles)
        {
            const Vector3<Base> HalfAngles = Angles * static_cast<Base>(0.5);

            const Base SinX = Sine(HalfAngles.GetX());
            const Base CosX = Cosine(HalfAngles.GetX());
            const Base SinY = Sine(HalfAngles.GetY());
            const Base CosY = Cosine(HalfAngles.GetY());
            const Base SinZ = Sine(HalfAngles.GetZ());
            const Base CosZ = Cosine(HalfAngles.GetZ());

            return Quaternion<Base>(CosX * CosY * CosZ + SinX * SinY * SinZ,
                                    SinX * CosY * CosZ - CosX * SinY * SinZ,
                                    CosX * SinY * CosZ + SinX * CosY * SinZ,
                                    CosX * CosY * SinZ - SinX * SinY * CosZ);
        }

        // -=(Undocumented)=-
        static Quaternion<Base> Lerp(Ref<const Quaternion<Base>> Start, Ref<const Quaternion<Base>> End, Real32 Percentage)
        {
            const Real32 Dot = Start.Dot(End);
            return (Start + ((Dot < 0.0f ? -End : End) - Start) * Percentage);
        }

        // -=(Undocumented)=-
        static Quaternion<Base> Slerp(Ref<const Quaternion<Base>> Start, Ref<const Quaternion<Base>> End, Real32 Percentage)
        {
            const Base Dot   = Clamp(Start.Dot(End), -1.0, +1.0);
            const Base Theta = InvCosine(Dot) * Percentage;

            const Quaternion<Base> Relative = Normalize(End - Start * Dot);

            return (Start * Cosine(Theta)) + (Relative * Sine(Theta));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3<Base> mComplexes;
        Base          mReal;
    };

    // -=(Undocumented)=-
    using Quaternionf = Quaternion<Real32>;

    // -=(Undocumented)=-
    using Quaternioni = Quaternion<UInt32>;
}