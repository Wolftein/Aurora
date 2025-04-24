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

#include "Vector2.hpp"
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
        constexpr Matrix4(ConstRef<Column> C1, ConstRef<Column> C2, ConstRef<Column> C3, ConstRef<Column> C4)
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
        ConstRef<Column> GetColumn(UInt Column) const
        {
            return mColumns[Column];
        }

        // -=(Undocumented)=-
        Base GetComponent(UInt Component) const
        {
            return reinterpret_cast<ConstPtr<Base>>(mColumns)[Component];
        }

        // -=(Undocumented)=-
        Base GetTrace() const
        {
            return GetComponent(0) + GetComponent(5) + GetComponent(10) + GetComponent(15);
        }

        // -=(Undocumented)=-
        Base GetDeterminant() const
        {
            const Base Sub0  = GetComponent(5) * GetComponent(10) - GetComponent(6) * GetComponent(9);
            const Base Sub1  = GetComponent(1) * GetComponent(10) - GetComponent(2) * GetComponent(9);
            const Base Sub2  = GetComponent(1) * GetComponent(6) - GetComponent(2) * GetComponent(5);
            const Base Sub3  = GetComponent(1) * GetComponent(6) * GetComponent(11) - GetComponent(2);

            const Base Sub01 = GetComponent(0) * Sub0 - GetComponent(4) * Sub1;
            const Base Sub23 = GetComponent(8) * Sub2 - GetComponent(12) * Sub3 * GetComponent(5) * GetComponent(11);
            return Sub01 + Sub23;
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
        Vector3<Base> GetScale() const
        {
            const Vector3<Base> XAxis(GetComponent(0), GetComponent(1), GetComponent(2));
            const Vector3<Base> YAxis(GetComponent(4), GetComponent(5), GetComponent(6));
            const Vector3<Base> ZAxis(GetComponent(8), GetComponent(9), GetComponent(10));

            return Vector3<Base>(XAxis.GetLength(), YAxis.GetLength(), ZAxis.GetLength());
        }

        // -=(Undocumented)=-
        Vector3<Base> GetTranslation() const
        {
            return Vector3<Base>(GetComponent(12), GetComponent(13), GetComponent(14));
        }

        // -=(Undocumented)=-
        Quaternion<Base> GetRotation() const
        {
            const Base M00 = GetComponent(0);
            const Base M01 = GetComponent(1);
            const Base M02 = GetComponent(2);
            const Base M10 = GetComponent(4);
            const Base M11 = GetComponent(5);
            const Base M12 = GetComponent(6);
            const Base M20 = GetComponent(8);
            const Base M21 = GetComponent(9);
            const Base M22 = GetComponent(10);

            const Base Trace = M00 + M11 + M22;

            if (Trace > 0.0)
            {
                const Base S = Sqrt(Trace + 1.0) * 2.0;
                return Quaternion<Base>(
                    (M21 - M12) / S,
                    (M02 - M20) / S,
                    (M10 - M01) / S,
                    0.25 * S);
            }
            else if (M00 > M11 && M00 > M22)
            {
                const Base S = Sqrt(1.0 + M00 - M11 - M22) * 2.0;
                return Quaternion<Base>(
                    0.25 * S,
                    (M01 + M10) / S,
                    (M02 + M20) / S,
                    (M21 - M12) / S);
            }
            else if (M11 > M22)
            {
                const Base S = Sqrt(1.0 + M11 - M00 - M22) * 2.0;
                return Quaternion<Base>(
                    (M01 + M10) / S,
                    0.25 * S,
                    (M12 + M21) / S,
                    (M02 - M20) / S);
            }
            else
            {
                const Base S = Sqrt(1.0 + M22 - M00 - M11) * 2.0;
                return Quaternion<Base>(
                    (M02 + M20) / S,
                    (M12 + M21) / S,
                    0.25 * S,
                    (M10 - M01) / S);
            }
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
        ConstRef<Base> operator[](UInt Component) const
        {
            return reinterpret_cast<ConstPtr<Base>>(mColumns)[Component];
        }

        // -=(Undocumented)=-
        Matrix4<Base> operator+(ConstRef<Matrix4<Base>> Matrix) const
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
        Matrix4<Base> operator-(ConstRef<Matrix4<Base>> Matrix) const
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
        Matrix4<Base> operator*(ConstRef<Matrix4<Base>> Matrix) const
        {
            Matrix4<Base> Result;

            const Vector4<Base> R0(GetComponent(0), GetComponent(4), GetComponent(8), GetComponent(12));
            const Vector4<Base> R1(GetComponent(1), GetComponent(5), GetComponent(9), GetComponent(13));
            const Vector4<Base> R2(GetComponent(2), GetComponent(6), GetComponent(10), GetComponent(14));
            const Vector4<Base> R3(GetComponent(3), GetComponent(7), GetComponent(11), GetComponent(15));

            for (UInt32 Column = 0; Column < 4; ++Column)
            {
                ConstRef<Matrix4<Base>::Column> Vector = Matrix.GetColumn(Column);
                Result[Column * 4 + 0] = Vector.Dot(R0);
                Result[Column * 4 + 1] = Vector.Dot(R1);
                Result[Column * 4 + 2] = Vector.Dot(R2);
                Result[Column * 4 + 3] = Vector.Dot(R3);
            }

            return Result;
        }

        // -=(Undocumented)=-
        Vector2<Base> operator*(ConstRef<Vector2<Base>> Vector) const
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();
            const Base W = 1.0f / (X * GetComponent(3) + Y * GetComponent(7) + GetComponent(15));

            const Base VectorX = (GetComponent(0) * X + GetComponent(4) * Y + GetComponent(12)) * W;
            const Base VectorY = (GetComponent(1) * X + GetComponent(5) * Y + GetComponent(13)) * W;

            return Vector2<Base>(VectorX, VectorY);
        }

        // -=(Undocumented)=-
        Vector3<Base> operator*(ConstRef<Vector3<Base>> Vector) const
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
        Vector4<Base> operator*(ConstRef<Vector4<Base>> Vector) const
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
        Ref<Matrix4<Base>> operator+=(ConstRef<Matrix4<Base>> Matrix)
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
        Ref<Matrix4<Base>> operator-=(ConstRef<Matrix4<Base>> Matrix)
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
        Ref<Matrix4<Base>> operator*=(ConstRef<Matrix4<Base>> Matrix)
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

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeObject(mColumns[0]);
            Archive.SerializeObject(mColumns[1]);
            Archive.SerializeObject(mColumns[2]);
            Archive.SerializeObject(mColumns[3]);
        }

    public:

        // -=(Undocumented)=-
        static constexpr  Bool IsIdentity(ConstRef<Matrix4<Base>> Matrix)
        {
            constexpr Vector4<Base> Sub1(1.0, 0.0, 0.0, 0.0);
            constexpr Vector4<Base> Sub2(0.0, 1.0, 0.0, 0.0);
            constexpr Vector4<Base> Sub3(0.0, 0.0, 1.0, 0.0);
            constexpr Vector4<Base> Sub4(0.0, 0.0, 0.0, 1.0);
            return Matrix.GetColumn(0) == Sub1
                && Matrix.GetColumn(1) == Sub2
                && Matrix.GetColumn(2) == Sub3
                && Matrix.GetColumn(3) == Sub4;
        }

        // -=(Undocumented)=-
        static constexpr  Bool IsOrthogonal(ConstRef<Matrix4<Base>> Matrix)
        {
            return IsIdentity(Matrix * Transpose(Matrix));
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> Transpose(ConstRef<Matrix4<Base>> Matrix)
        {
            return Matrix4<Base>(Matrix.GetComponent(0),  Matrix.GetComponent(4),
                                 Matrix.GetComponent(8),  Matrix.GetComponent(12),
                                 Matrix.GetComponent(1),  Matrix.GetComponent(5),
                                 Matrix.GetComponent(9),  Matrix.GetComponent(13),
                                 Matrix.GetComponent(2),  Matrix.GetComponent(6),
                                 Matrix.GetComponent(10), Matrix.GetComponent(14),
                                 Matrix.GetComponent(3),  Matrix.GetComponent(7),
                                 Matrix.GetComponent(11), Matrix.GetComponent(15));
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreatePerspective(Base Eyes, Base Aspect, Base ZNear, Base ZFar)
        {
            const Base Fov    = 0.5f * Eyes;
            const Base Height = Cosine(Fov) / Sine(Fov);
            const Base Width  = Height / Aspect;
            const Base Range  = ZFar / (ZFar - ZNear);

            return Matrix4<Base>(Width, 0.0,    0.0,            0.0,
                                   0.0, Height, 0.0,            0.0,
                                   0.0, 0.0,    Range,          1.0,
                                   0.0, 0.0,    -Range * ZNear, 0.0);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreateOrthographic(Base Left, Base Right, Base Bottom, Base Top, Base ZNear, Base ZFar)
        {
            const Base ReciprocalWidth  = 1.0 / (Right - Left);
            const Base ReciprocalHeight = 1.0 / (Top - Bottom);
            const Base ReciprocalRange  = 1.0 / (ZFar - ZNear);

            return Matrix4<Base>(ReciprocalWidth + ReciprocalWidth,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 ReciprocalHeight + ReciprocalHeight,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 ReciprocalRange,
                                 0.0,
                                 -(Left + Right) * ReciprocalWidth,
                                 -(Top + Bottom) * ReciprocalHeight,
                                 -ReciprocalRange * ZNear,
                                 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> CreateLook(ConstRef<Vector3<Base>> Eye, ConstRef<Vector3<Base>> Focus, ConstRef<Vector3<Base>> Up)
        {
            const Vector3<Base> vForward = Vector3<Base>::Normalize(Focus - Eye);
            const Vector3<Base> vRight   = Vector3<Base>::Normalize(Vector3<Base>::Cross(Up, vForward));
            const Vector3<Base> vUp      = Vector3<Base>::Cross(vForward, vRight);
            const Vector3<Base> vEye     = -Eye;

            return Matrix4<Base>(
                     vRight.GetX(),    vUp.GetX(),    vForward.GetX(),    0.0,
                     vRight.GetY(),    vUp.GetY(),    vForward.GetY(),    0.0,
                     vRight.GetZ(),    vUp.GetZ(),    vForward.GetZ(),    0.0,
                     vRight.Dot(vEye), vUp.Dot(vEye), vForward.Dot(vEye), 1.0);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromTranslation(ConstRef<Vector2<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();

            return Matrix4<Base>(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 X, Y, 0, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromTranslation(ConstRef<Vector3<Base>> Vector)
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
        static constexpr Matrix4<Base> FromScale(ConstRef<Vector2<Base>> Vector)
        {
            const Base X = Vector.GetX();
            const Base Y = Vector.GetY();

            return Matrix4<Base>(X, 0, 0, 0,
                                 0, Y, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);
        }

        // -=(Undocumented)=-
        static constexpr Matrix4<Base> FromScale(ConstRef<Vector3<Base>> Vector)
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
        static constexpr Matrix4<Base> FromRotation(ConstRef<Quaternion<Base>> Rotation)
        {
            const Vector4<Base> Vector(Rotation.GetX(), Rotation.GetY(), Rotation.GetZ(), Rotation.GetW());

            const Base XX = Vector.GetX() * Vector.GetX();
            const Base YY = Vector.GetY() * Vector.GetY();
            const Base ZZ = Vector.GetZ() * Vector.GetZ();
            const Base XZ = Vector.GetX() * Vector.GetZ();
            const Base YZ = Vector.GetY() * Vector.GetZ();
            const Base XY = Vector.GetX() * Vector.GetY();
            const Base WX = Vector.GetW() * Vector.GetX();
            const Base WY = Vector.GetW() * Vector.GetY();
            const Base WZ = Vector.GetW() * Vector.GetZ();

            return Matrix4<Base>(
                    1.0 - 2.0 * (YY + ZZ), 2.0 * (XY + WZ), 2.0 * (XZ - WY), 0.0,
                    2.0 * (XY - WZ), 1.0 - 2.0 * (XX + ZZ), 2.0 * (WX + YZ), 0.0,
                    2.0 * (WY + XZ), 2.0 * (YZ - WX), 1.0 - 2.0 * (XX + YY), 0.0,
                    0.0,             0.0,             0.0,                   1.0);
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