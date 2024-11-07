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
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Matrix4 final
    {
    public:

        // -=(Undocumented)=-
        using Column = Vector4<Base>;
        
    public:

        // -=(Undocumented)=-
        constexpr Matrix4()
            : Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
        {
        }

        // -=(Undocumented)=-
        constexpr Matrix4(Ref<const Column> C1, Ref<const Column> C2, Ref<const Column> C3, Ref<const Column> C4)
            : mColumns { C1, C2, C3, C4 }
        {
        }

        // -=(Undocumented)=-
        constexpr Matrix4(
            Base S00, Base S10, Base S20, Base S30,
            Base S01, Base S11, Base S21, Base S31,
            Base S02, Base S12, Base S22, Base S32,
            Base S03, Base S13, Base S23, Base S33)
        {
            mColumns[0].Set(S00, S10, S20, S30);
            mColumns[1].Set(S01, S11, S21, S31);
            mColumns[2].Set(S02, S12, S22, S32);
            mColumns[3].Set(S03, S13, S23, S33);
        }

        // -=(Undocumented)=-
        Ref<const Column> GetColumn(UInt Column) const
        {
            return mColumns[Column];
        }

        // -=(Undocumented)=-
        Base GetComponent(UInt Component) const
        {
            return reinterpret_cast<Ptr<const Base>>(mColumns)[Component];
        }

        // -=(Undocumented)=-
        Base GetTrace() const
        {
            return GetComponent(0) + GetComponent(5) + GetComponent(10) + GetComponent(15);
        }

        // -=(Undocumented)=-
        Base GetDeterminant() const
        {
            const Base Sub11 =  GetComponent(4) * GetComponent(8) - GetComponent(5) * GetComponent(7);
            const Base Sub12 = -GetComponent(1) * GetComponent(8) + GetComponent(2) * GetComponent(7);
            const Base Sub13 =  GetComponent(1) * GetComponent(5) - GetComponent(2) * GetComponent(4);

            return GetComponent(0) * Sub11 + GetComponent(3) * Sub12 + GetComponent(6) * Sub13;
        }

        // -=(Undocumented)=-
        Vector3<Base> GetRight() const
        {
            return Vector3<Base>(GetComponent(0), GetComponent(1), GetComponent(2));
        }

        // -=(Undocumented)=-
        Vector3<Base> GetLeft() const
        {
            return -GetRight();
        }

        // -=(Undocumented)=-
        Vector3<Base> GetUp() const
        {
            return Vector3<Base>(GetComponent(4), GetComponent(5), GetComponent(6));
        }

        // -=(Undocumented)=-
        Vector3<Base> GetDown() const
        {
            return -GetUp();
        }

        // -=(Undocumented)=-
        Vector3<Base> GetForward() const
        {
            return Vector3<Base>(GetComponent(8), GetComponent(9), GetComponent(10));
        }

        // -=(Undocumented)=-
        Vector3<Base> GetBackward() const
        {
            return -GetForward();
        }

        // -=(Undocumented)=-
        Matrix4<Base> Inverse() const
        {
            const Base C0 =
                GetComponent(5)  * GetComponent(10) * GetComponent(15) -
                GetComponent(5)  * GetComponent(11) * GetComponent(14) -
                GetComponent(9)  * GetComponent(6)  * GetComponent(15) +
                GetComponent(9)  * GetComponent(7)  * GetComponent(14) +
                GetComponent(13) * GetComponent(6)  * GetComponent(11) -
                GetComponent(13) * GetComponent(7)  * GetComponent(10);

            const Base C4 =
                -GetComponent(4)  * GetComponent(10) * GetComponent(15) +
                 GetComponent(4)  * GetComponent(11) * GetComponent(14) +
                 GetComponent(8)  * GetComponent(6)  * GetComponent(15) -
                 GetComponent(8)  * GetComponent(7)  * GetComponent(14) -
                 GetComponent(12) * GetComponent(6)  * GetComponent(11) +
                 GetComponent(12) * GetComponent(7)  * GetComponent(10);

            const Base C8 =
                GetComponent(4)  * GetComponent(9)  * GetComponent(15) -
                GetComponent(4)  * GetComponent(11) * GetComponent(13) -
                GetComponent(8)  * GetComponent(5)  * GetComponent(15) +
                GetComponent(8)  * GetComponent(7)  * GetComponent(13) +
                GetComponent(12) * GetComponent(5)  * GetComponent(11) -
                GetComponent(12) * GetComponent(7)  * GetComponent(9);

            const Base C12 =
                -GetComponent(4)  * GetComponent(9)  * GetComponent(14) +
                 GetComponent(4)  * GetComponent(10) * GetComponent(13) +
                 GetComponent(8)  * GetComponent(5)  * GetComponent(14) -
                 GetComponent(8)  * GetComponent(6)  * GetComponent(13) -
                 GetComponent(12) * GetComponent(5)  * GetComponent(10) +
                 GetComponent(12) * GetComponent(6)  * GetComponent(9);

            const Base C1 =
                -GetComponent(1)  * GetComponent(10) * GetComponent(15) +
                 GetComponent(1)  * GetComponent(11) * GetComponent(14) +
                 GetComponent(9)  * GetComponent(2)  * GetComponent(15) -
                 GetComponent(9)  * GetComponent(3)  * GetComponent(14) -
                 GetComponent(13) * GetComponent(2)  * GetComponent(11) +
                 GetComponent(13) * GetComponent(3)  * GetComponent(10);

            const Base C5 =
                GetComponent(0)  * GetComponent(10) * GetComponent(15) -
                GetComponent(0)  * GetComponent(11) * GetComponent(14) -
                GetComponent(8)  * GetComponent(2)  * GetComponent(15) +
                GetComponent(8)  * GetComponent(3)  * GetComponent(14) +
                GetComponent(12) * GetComponent(2)  * GetComponent(11) -
                GetComponent(12) * GetComponent(3)  * GetComponent(10);

            const Base C9 =
                -GetComponent(0)  * GetComponent(9)  * GetComponent(15) +
                 GetComponent(0)  * GetComponent(11) * GetComponent(13) +
                 GetComponent(8)  * GetComponent(1)  * GetComponent(15) -
                 GetComponent(8)  * GetComponent(3)  * GetComponent(13) -
                 GetComponent(12) * GetComponent(1)  * GetComponent(11) +
                 GetComponent(12) * GetComponent(3)  * GetComponent(9);

            const Base C13 =
                GetComponent(0)  * GetComponent(9)  * GetComponent(14) -
                GetComponent(0)  * GetComponent(10) * GetComponent(13) -
                GetComponent(8)  * GetComponent(1)  * GetComponent(14) +
                GetComponent(8)  * GetComponent(2)  * GetComponent(13) +
                GetComponent(12) * GetComponent(1)  * GetComponent(10) -
                GetComponent(12) * GetComponent(2)  * GetComponent(9);

            const Base C2 =
                GetComponent(1)  * GetComponent(6) * GetComponent(15) -
                GetComponent(1)  * GetComponent(7) * GetComponent(14) -
                GetComponent(5)  * GetComponent(2) * GetComponent(15) +
                GetComponent(5)  * GetComponent(3) * GetComponent(14) +
                GetComponent(13) * GetComponent(2) * GetComponent(7)  -
                GetComponent(13) * GetComponent(3) * GetComponent(6);

            const Base C6 =
                -GetComponent(0)  * GetComponent(6) * GetComponent(15) +
                 GetComponent(0)  * GetComponent(7) * GetComponent(14) +
                 GetComponent(4)  * GetComponent(2) * GetComponent(15) -
                 GetComponent(4)  * GetComponent(3) * GetComponent(14) -
                 GetComponent(12) * GetComponent(2) * GetComponent(7)  +
                 GetComponent(12) * GetComponent(3) * GetComponent(6);

            const Base C10 =
                GetComponent(0)  * GetComponent(5) * GetComponent(15) -
                GetComponent(0)  * GetComponent(7) * GetComponent(13) -
                GetComponent(4)  * GetComponent(1) * GetComponent(15) +
                GetComponent(4)  * GetComponent(3) * GetComponent(13) +
                GetComponent(12) * GetComponent(1) * GetComponent(7)  -
                GetComponent(12) * GetComponent(3) * GetComponent(5);

            const Base C14 =
                -GetComponent(0)  * GetComponent(5) * GetComponent(14) +
                 GetComponent(0)  * GetComponent(6) * GetComponent(13) +
                 GetComponent(4)  * GetComponent(1) * GetComponent(14) -
                 GetComponent(4)  * GetComponent(2) * GetComponent(13) -
                 GetComponent(12) * GetComponent(1) * GetComponent(6) +
                 GetComponent(12) * GetComponent(2) * GetComponent(5);

            const Base C3 =
                -GetComponent(1) * GetComponent(6) * GetComponent(11) +
                 GetComponent(1) * GetComponent(7) * GetComponent(10) +
                 GetComponent(5) * GetComponent(2) * GetComponent(11) -
                 GetComponent(5) * GetComponent(3) * GetComponent(10) -
                 GetComponent(9) * GetComponent(2) * GetComponent(7) +
                 GetComponent(9) * GetComponent(3) * GetComponent(6);

            const Base C7 =
                GetComponent(0) * GetComponent(6) * GetComponent(11) -
                GetComponent(0) * GetComponent(7) * GetComponent(10) -
                GetComponent(4) * GetComponent(2) * GetComponent(11) +
                GetComponent(4) * GetComponent(3) * GetComponent(10) +
                GetComponent(8) * GetComponent(2) * GetComponent(7) -
                GetComponent(8) * GetComponent(3) * GetComponent(6);

            const Base C11 =
                -GetComponent(0) * GetComponent(5) * GetComponent(11) +
                 GetComponent(0) * GetComponent(7) * GetComponent(9) +
                 GetComponent(4) * GetComponent(1) * GetComponent(11) -
                 GetComponent(4) * GetComponent(3) * GetComponent(9) -
                 GetComponent(8) * GetComponent(1) * GetComponent(7) +
                 GetComponent(8) * GetComponent(3) * GetComponent(5);

            const Base C15 =
                GetComponent(0) * GetComponent(5) * GetComponent(10) -
                GetComponent(0) * GetComponent(6) * GetComponent(9) -
                GetComponent(4) * GetComponent(1) * GetComponent(10) +
                GetComponent(4) * GetComponent(2) * GetComponent(9) +
                GetComponent(8) * GetComponent(1) * GetComponent(6) -
                GetComponent(8) * GetComponent(2) * GetComponent(5);

            const Base InvDeterminant =
                1.0f / (GetComponent(0) * C0 + GetComponent(1) * C4 + GetComponent(2) * C8 + GetComponent(3) * C12);
            if (InvDeterminant != 0)
            {
                const Matrix4<Base> Result(
                    C0,  C1,  C2,  C3,
                    C4,  C5,  C6,  C7,
                    C8,  C9,  C10, C11,
                    C12, C13, C14, C15);
                return Result * InvDeterminant;
            }
            return Matrix4<Base>();
        }

        // -=(Undocumented)=-
        Ref<Base> operator[](UInt Component)
        {
            return reinterpret_cast<Ptr<Base>>(mColumns)[Component];
        }

        // -=(Undocumented)=-
        Ref<const Base> operator[](UInt Component) const
        {
            return reinterpret_cast<Ptr<const Base>>(mColumns)[Component];
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator+(Ref<const Matrix4<Base>> Matrix) const
        {
            return Matrix4<Base>(mColumns[0] + Matrix.mColumns[0],
                                 mColumns[1] + Matrix.mColumns[1],
                                 mColumns[2] + Matrix.mColumns[2],
                                 mColumns[3] + Matrix.mColumns[3]);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator+(Base Scalar) const
        {
            return Matrix4<Base>(mColumns[0] + Scalar, mColumns[1] + Scalar, mColumns[2] + Scalar, mColumns[3] + Scalar);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator-() const
        {
            return Matrix4<Base>(-mColumns[0], -mColumns[1], -mColumns[2], -mColumns[3]);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator-(Ref<const Matrix4<Base>> Matrix) const
        {
            return Matrix4<Base>(mColumns[0] - Matrix.mColumns[0],
                                 mColumns[1] - Matrix.mColumns[1],
                                 mColumns[2] - Matrix.mColumns[2],
                                 mColumns[3] - Matrix.mColumns[3]);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator-(Base Scalar) const
        {
            return Matrix4<Base>(mColumns[0] - Scalar, mColumns[1] - Scalar, mColumns[2] - Scalar, mColumns[3] - Scalar);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator*(Ref<const Matrix4<Base>> Matrix) const
        {
            Matrix4<Base> Result;

            const Vector4<Base> R0(GetComponent(0), GetComponent(4), GetComponent(8), GetComponent(12));
            const Vector4<Base> R1(GetComponent(1), GetComponent(5), GetComponent(9), GetComponent(13));
            const Vector4<Base> R2(GetComponent(2), GetComponent(6), GetComponent(10), GetComponent(14));
            const Vector4<Base> R3(GetComponent(3), GetComponent(7), GetComponent(11), GetComponent(15));

            for (UInt32 Column = 0; Column < 4; ++Column)
            {
                Ref<const Matrix4<Base>::Column> Vector = Matrix.GetColumn(Column);
                Result[Column * 4 + 0] = Vector.Dot(R0);
                Result[Column * 4 + 1] = Vector.Dot(R1);
                Result[Column * 4 + 2] = Vector.Dot(R2);
                Result[Column * 4 + 3] = Vector.Dot(R3);
            }

            return Result;
        }

        // -=(Undocumented)=-
        Vector2<Base> operator*(Ref<const Vector2<Base>> Vector) const
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base W = 1.0f / (X * GetComponent(3) + Y * GetComponent(7) + GetComponent(15));

            const Base VectorX = (GetComponent(0) * X + GetComponent(4) * Y + GetComponent(12)) * W;
            const Base VectorY = (GetComponent(1) * X + GetComponent(5) * Y + GetComponent(13)) * W;

            return Vector2<Base>(VectorX, VectorY);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator*(Ref<const Vector3<Base>> Vector) const
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base Z = Vector.GetZ();
            const Base W = 1.0f / (X * GetComponent(3) + Y * GetComponent(7) + Z * GetComponent(11) + GetComponent(15));

            const Base VectorX = (GetComponent(0) * X + GetComponent(4) * Y + GetComponent(8)  * Z + GetComponent(12)) * W;
            const Base VectorY = (GetComponent(1) * X + GetComponent(5) * Y + GetComponent(9)  * Z + GetComponent(13)) * W;
            const Base VectorZ = (GetComponent(2) * X + GetComponent(6) * Y + GetComponent(10) * Z + GetComponent(14)) * W;

            return Vector3<Base>(VectorX, VectorY, VectorZ);
        }

        // -=(Undocumented)=-
        Vector4<Base> operator*(Ref<const Vector4<Base>> Vector) const
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base Z = Vector.GetZ();
            const Base W = Vector.GetW();

            const Base VectorX = GetComponent(0) * X + GetComponent(4) * Y + GetComponent(8)  * Z + GetComponent(12) * W;
            const Base VectorY = GetComponent(1) * X + GetComponent(5) * Y + GetComponent(9)  * Z + GetComponent(13) * W;
            const Base VectorZ = GetComponent(2) * X + GetComponent(6) * Y + GetComponent(10) * Z + GetComponent(14) * W;
            const Base VectorW = GetComponent(3) * X + GetComponent(7) * Y + GetComponent(11) * Z + GetComponent(15) * W;

            return Vector4<Base>(VectorX, VectorY, VectorZ, VectorW);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator*(Base Scalar) const
        {
            return Matrix4<Base>(mColumns[0] * Scalar, mColumns[1] * Scalar, mColumns[2] * Scalar, mColumns[3] * Scalar);
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator/(Base Scalar) const
        {
            return Matrix4<Base>(mColumns[0] / Scalar, mColumns[1] / Scalar, mColumns[2] / Scalar, mColumns[3] / Scalar);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator+=(Ref<const Matrix4<Base>> Matrix)
        {
            mColumns[0] += Matrix.mColumns[0];
            mColumns[1] += Matrix.mColumns[1];
            mColumns[2] += Matrix.mColumns[2];
            mColumns[3] += Matrix.mColumns[3];
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator+=(Base Scalar)
        {
            mColumns[0] += Scalar;
            mColumns[1] += Scalar;
            mColumns[2] += Scalar;
            mColumns[3] += Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator-=(Ref<const Matrix4<Base>> Matrix)
        {
            mColumns[0] -= Matrix.mColumns[0];
            mColumns[1] -= Matrix.mColumns[1];
            mColumns[2] -= Matrix.mColumns[2];
            mColumns[3] -= Matrix.mColumns[3];
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator-=(Base Scalar)
        {
            mColumns[0] -= Scalar;
            mColumns[1] -= Scalar;
            mColumns[2] -= Scalar;
            mColumns[3] -= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator*=(Ref<const Matrix4<Base>> Matrix)
        {
            const Matrix4<Base> Result = (* this) * Matrix;
            mColumns[0] = Result.mColumns[0];
            mColumns[1] = Result.mColumns[1];
            mColumns[2] = Result.mColumns[2];
            mColumns[3] = Result.mColumns[3];
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator*=(Base Scalar)
        {
            mColumns[0] *= Scalar;
            mColumns[1] *= Scalar;
            mColumns[2] *= Scalar;
            mColumns[3] *= Scalar;
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Matrix4<Base>> operator/=(Base Scalar)
        {
            mColumns[0] /= Scalar;
            mColumns[1] /= Scalar;
            mColumns[2] /= Scalar;
            mColumns[3] /= Scalar;
            return (* this);
        }

    public:

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreatePerspective(Base Eyes, Base Aspect, Base ZNear, Base ZFar)
        {
            const Base Height = Cosine(0.5f * Eyes) / Sine(0.5f * Eyes);
            const Base Width  = Height / Aspect;
            const Base Range  = ZFar / (ZFar - ZNear);

            return Matrix4<Base>(Width, 0,       0,              0,
                                 0,     Height,  0,              0,
                                 0,     0,       Range,          1,
                                 0,     0,       -Range * ZNear, 0);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreateOrthographic(Base Left, Base Right, Base Bottom, Base Top, Base ZNear, Base ZFar)
        {
            return Matrix4<Base>(2.0 / (Right - Left),
                                 0,
                                 0,
                                 0,
                                 0,
                                 2.0 / (Top - Bottom),
                                 0,
                                 0,
                                 0,
                                 0,
                                 1.0 / (ZFar - ZNear),
                                 0,
                                 -(Left + Right) / (Right - Left),
                                 -(Top + Bottom) / (Top - Bottom),
                                 -ZNear / (ZFar - ZNear),
                                 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreateLook(Ref<const Vector3<Base>> Eye, Ref<const Vector3<Base>> Center, Ref<const Vector3<Base>> Up)
        {
            const Vector3<Base> vForward = Vector3<Base>::Normalize(Center - Eye);
            const Vector3<Base> vRight   = Vector3<Base>::Normalize(Vector3<Base>::Cross(Up, vForward));
            const Vector3<Base> vUp      = Vector3<Base>::Cross(vForward, vRight);

            const Vector4<Base> C0(vRight.GetX(), vUp.GetX(), vForward.GetX(), 0);
            const Vector4<Base> C1(vRight.GetY(), vUp.GetY(), vForward.GetY(), 0);
            const Vector4<Base> C2(vRight.GetZ(), vUp.GetZ(), vForward.GetZ(), 0);
            const Vector4<Base> C3(-vRight.Dot(Eye), -vUp.Dot(Eye), -vForward.Dot(Eye), 1);

            return Matrix4<Base>(C0, C1, C2, C3);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromTranslation(Ref<const Vector2<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();

            return Matrix4<Base>(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 X, Y, 0, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromTranslation(Ref<const Vector3<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base Z = Vector.GetZ();

            return Matrix4<Base>(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 X, Y, Z, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromScale(Ref<const Vector2<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();

            return Matrix4<Base>(X, 0, 0, 0,
                                 0, Y, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromScale(Ref<const Vector3<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base Z = Vector.GetZ();

            return Matrix4<Base>(X, 0, 0, 0,
                                 0, Y, 0, 0,
                                 0, 0, Z, 0,
                                 0, 0, 0, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromRotation(Ref<const Quaternion<Base>> Rotation)
        {
            const Vector3<Base> Vector(Rotation.GetX(), Rotation.GetY(), Rotation.GetZ());

            const Base XX = Vector.GetX() * Vector.GetX();
            const Base YY = Vector.GetY() * Vector.GetY();
            const Base ZZ = Vector.GetZ() * Vector.GetZ();
            const Base SX = Vector.GetX() * Rotation.GetW();
            const Base SY = Vector.GetY() * Rotation.GetW();
            const Base SZ = Vector.GetZ() * Rotation.GetW();
            const Base XZ = Vector.GetX() * Vector.GetZ();
            const Base YZ = Vector.GetY() * Vector.GetZ();
            const Base XY = Vector.GetX() * Vector.GetY();

            const Vector4<Base> C0(1 - 2 * (YY + ZZ), 2 * (XY + SZ), 2 * (XZ - SY), 0);
            const Vector4<Base> C1(2 * (XY - SZ), 1 - 2 * (XX + ZZ), 2 * (SX + YZ), 0);
            const Vector4<Base> C2(2 * (SY + XZ), 2 * (YZ - SX), 1 - 2 * (XX + YY), 0);
            const Vector4<Base> C3(0, 0, 0, 1);

            return Matrix4<Base>(C0, C1, C2, C3);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Column mColumns[4];
    };

    // -=(Undocumented)=-
    using Matrix4f = Matrix4<Real32>;
    
    // -=(Undocumented)=-
    using Matrix4d = Matrix4<Real64>;
}