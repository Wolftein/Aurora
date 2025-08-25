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
#include "Vector3.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    /// \brief Represents a 4D vector with floating point components (x, y, z, w).
    class AURORA_ALIGN(16) Vector4 final
    {
        friend class Matrix4x4;

    public:

        /// \brief Initializes the vector to (0.0f, 0.0f, 0.0f, 0.0f).
        AURORA_INLINE Vector4()
            : mRegister { _mm_setzero_ps() }
        {
        }

        /// \brief Constructor initializing all components to the same value.
        /// 
        /// \param Scalar The scalar value used to initialize all components.
        AURORA_INLINE Vector4(Real32 Scalar)
            : mRegister { _mm_set_ps1(Scalar) }
        {
        }

        /// \brief Constructor initializing the vector with specified x, y, z and w values.
        /// 
        /// \param X The x-component of the vector.
        /// \param Y The y-component of the vector.
        /// \param Z The z-component of the vector.
        /// \param W The w-component of the vector (default to 0.0).
        AURORA_INLINE Vector4(Real32 X, Real32 Y, Real32 Z, Real32 W = 0.0f)
            : mRegister { _mm_set_ps(W, Z, Y, X) }
        {
        }

        /// \brief Copy constructor.
        /// 
        /// \param Other The vector to copy.
        AURORA_INLINE Vector4(Ref<const Vector4> Other) = default;

        /// \brief Checks if the vector is the zero vector.
        /// 
        /// \return `true` if all components are approximately zero, `false` otherwise.
        AURORA_INLINE Bool IsAlmostZero() const
        {
            return GetLengthSquared() <= (kEpsilon<Real32> * kEpsilon<Real32>);
        }

        /// \brief Checks if all components are approximately equal to the given values.
        /// 
        /// \param X Value to compare with the X component.
        /// \param Y Value to compare with the Y component.
        /// \param Z Value to compare with the Z component.
        /// \param W Value to compare with the W component.
        /// \return `true` if all components are within epsilon, `false` otherwise.
        AURORA_INLINE Bool IsAlmostEqual(Real32 X, Real32 Y, Real32 Z, Real32 W) const
        {
            const __m128 Diff = _mm_sub_ps(mRegister, _mm_set_ps(W, Z, Y, X));
            const __m128 Mask = _mm_cmplt_ps(_mm_andnot_ps(_mm_set1_ps(-0.0f), Diff), _mm_set1_ps(kEpsilon<Real32>));
            return _mm_movemask_ps(Mask) == 0xF;
        }

        /// \brief Checks if the vector is normalized.
        /// 
        /// \return `true` if the vector is normalized, `false` otherwise.
        AURORA_INLINE Bool IsNormalized() const
        {
            const __m128 Diff = _mm_sub_ps(_mm_dp_ps(mRegister, mRegister, 0xFF), _mm_set_ps1(1.0f));
            const __m128 Mask = _mm_cmplt_ps(_mm_andnot_ps(_mm_set1_ps(-0.0f), Diff), _mm_set1_ps(kEpsilon<Real32>));
            return _mm_movemask_ps(Mask) == 0xF;
        }

        /// \brief Checks if the vector is finite.
        /// 
        /// \return `true` if the vector is finite, `false` otherwise.
        AURORA_INLINE Bool IsFinite() const
        {
            const __m128 Infinite = _mm_set1_ps(std::numeric_limits<Real32>::infinity());
            const __m128 Mask     = _mm_cmpge_ps(_mm_andnot_ps(_mm_set1_ps(-0.0f), mRegister), Infinite);
            return _mm_movemask_ps(Mask) == 0;
        }

        /// \brief Sets the components of the vector to new values.
        /// 
        /// \param X New x value.
        /// \param Y New y value.
        /// \param Z New z value.
        /// \param W New w value (default to 0.0).
        /// \return A reference to this vector after the update.
        AURORA_INLINE Ref<Vector4> Set(Real32 X, Real32 Y, Real32 Z, Real32 W = 0.0f)
        {
            mRegister = _mm_set_ps(W, Z, Y, X);
            return (* this);
        }

        /// \brief Gets the x-component of the vector.
        /// 
        /// \return The x-component.
        AURORA_INLINE Real32 GetX() const
        {
            return _mm_cvtss_f32(mRegister);
        }

        /// \brief Gets the y-component of the vector.
        /// 
        /// \return The y-component.
        AURORA_INLINE Real32 GetY() const
        {
            return _mm_cvtss_f32(_mm_shuffle_ps(mRegister, mRegister, _MM_SHUFFLE(1, 1, 1, 1)));
        }

        /// \brief Gets the z-component of the vector.
        /// 
        /// \return The z-component.
        AURORA_INLINE Real32 GetZ() const
        {
            return _mm_cvtss_f32(_mm_shuffle_ps(mRegister, mRegister, _MM_SHUFFLE(2, 2, 2, 2)));
        }

        /// \brief Gets the w-component of the vector.
        /// 
        /// \return The w-component.
        AURORA_INLINE Real32 GetW() const
        {
            return _mm_cvtss_f32(_mm_shuffle_ps(mRegister, mRegister, _MM_SHUFFLE(3, 3, 3, 3)));
        }

        /// \brief Gets the XY components of the vector.
        ///
        /// \return A new Vector2 containing the X and Y components.
        AURORA_INLINE Vector2 GetXY() const
        {
            return Vector2(GetX(), GetY());
        }

        /// \brief Gets the XYZ components of the vector.
        ///
        /// \return A new Vector3 containing the X, Y, and Z components.
        AURORA_INLINE Vector3 GetXYZ() const
        {
            return Vector3(GetX(), GetY(), GetZ());
        }

        /// \brief Calculates the length (magnitude) of the vector.
        /// 
        /// \return The length of the vector.
        AURORA_INLINE Real32 GetLength() const
        {
            return Sqrt(GetLengthSquared());
        }

        /// \brief Calculates the squared length of the vector.
        /// 
        /// \return The squared length of the vector (no square root).
        AURORA_INLINE Real32 GetLengthSquared() const
        {
            return _mm_cvtss_f32(_mm_dp_ps(mRegister, mRegister, 0xFF));
        }

        /// \brief Calculates the distance between this vector and another vector.
        /// 
        /// \param Vector The vector to calculate the distance to.
        /// \return The distance between the two vectors.
        AURORA_INLINE Real32 GetDistance(ConstRef<Vector4> Vector) const
        {
            return (* this - Vector).GetLength();
        }

        /// \brief Calculates the squared distance between this vector and another vector.
        /// 
        /// \param Vector The vector to calculate the squared distance to.
        /// \return The squared distance between the two vectors.
        AURORA_INLINE Real32 GetDistanceSquared(ConstRef<Vector4> Vector) const
        {
            return (* this - Vector).GetLengthSquared();
        }

        /// \brief Returns the angle in radians between this vector and another.
        /// 
        /// \param Other The other vector to compare with.
        /// \return Angle in radians between the two vectors (range [0, π]).
        AURORA_INLINE Real32 GetAngle(ConstRef<Vector4> Other) const
        {
            if (const Real32 Length = GetLength() * Other.GetLength(); Length > kEpsilon<Real32>)
            {
                return InvCos(Dot3(* this, Other) / Length);
            }
            return 0.0f;
        }

        /// \brief Stores the vector components into a float array.
        ///
        /// \tparam Align  When `true`, `Output` must be 16-byte aligned. When `false`, any alignment is allowed.
        /// \param  Output Pointer to four 32-bit floats. Must be 16-byte aligned if `Align` is `true`.
        template<Bool Align = true>
        AURORA_INLINE void Store(Ptr<Real32> Output) const
        {
            if constexpr (Align)
            {
                _mm_store_ps(Output, mRegister);
            }
            else
            {
                _mm_storeu_ps(Output, mRegister);
            }
        }

        /// \brief Adds another vector to this vector.
        /// 
        /// \param Vector The vector to add.
        /// \return A new vector that is the sum of this vector and the input vector.
        AURORA_INLINE Vector4 operator+(ConstRef<Vector4> Vector) const
        {
            return Vector4(_mm_add_ps(mRegister, Vector.mRegister));
        }

        /// \brief Adds a scalar to all components of this vector.
        ///
        /// \param Scalar The scalar to add.
        /// \return A new vector with the scalar added to all components.
        AURORA_INLINE Vector4 operator+(Real32 Scalar) const
        {
            return Vector4(_mm_add_ps(mRegister, _mm_set_ps1(Scalar)));
        }

        /// \brief Negates the vector.
        /// 
        /// \return A new vector that is the negation of this vector.
        AURORA_INLINE Vector4 operator-() const
        {
            return Vector4(_mm_sub_ps(_mm_setzero_ps(), mRegister));
        }

        /// \brief Subtracts another vector from this vector.
        /// 
        /// \param Vector The vector to subtract.
        /// \return A new vector that is the difference of the two vectors.
        AURORA_INLINE Vector4 operator-(ConstRef<Vector4> Vector) const
        {
            return Vector4(_mm_sub_ps(mRegister, Vector.mRegister));
        }

        /// \brief Subtracts a scalar from all components of this vector.
        /// 
        /// \param Scalar The scalar to subtract.
        /// \return A new vector with the scalar subtracted from all components.
        AURORA_INLINE Vector4 operator-(Real32 Scalar) const
        {
            return Vector4(_mm_sub_ps(mRegister, _mm_set_ps1(Scalar)));
        }

        /// \brief Multiplies all components of this vector by another vector.
        /// 
        /// \param Vector The vector to multiply by.
        /// \return A new vector that is the product of the two vectors.
        AURORA_INLINE Vector4 operator*(ConstRef<Vector4> Vector) const
        {
            return Vector4(_mm_mul_ps(mRegister, Vector.mRegister));
        }

        /// \brief Multiplies all components of this vector by a scalar.
        /// 
        /// \param Scalar The scalar to multiply by.
        /// \return A new vector with the scalar multiplied by all components.
        AURORA_INLINE Vector4 operator*(Real32 Scalar) const
        {
            return Vector4(_mm_mul_ps(mRegister, _mm_set_ps1(Scalar)));
        }

        /// \brief Divides all components of this vector by another vector.
        /// 
        /// \param Vector The vector to divide by.
        /// \return A new vector that is the quotient of the two vectors.
        AURORA_INLINE Vector4 operator/(ConstRef<Vector4> Vector) const
        {
            return Vector4(_mm_div_ps(mRegister, Vector.mRegister));
        }

        /// \brief Divides all components of this vector by a scalar.
        /// 
        /// \param Scalar The scalar to divide by.
        /// \return A new vector with the scalar divided by all components.
        AURORA_INLINE Vector4 operator/(Real32 Scalar) const
        {
            return Vector4(_mm_div_ps(mRegister, _mm_set_ps1(Scalar)));
        }

        /// \brief Pre-increments the vector components.
        /// 
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator++()
        {
            mRegister = _mm_add_ps(mRegister, _mm_set_ps1(1.0f));
            return (* this);
        }

        /// \brief Pre-decrements the vector components.
        /// 
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator--()
        {
            mRegister = _mm_sub_ps(mRegister, _mm_set_ps1(1.0f));
            return (* this);
        }

        /// \brief Adds another vector to the current vector.
        /// 
        /// \param Vector The vector to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator+=(ConstRef<Vector4> Vector)
        {
            mRegister = _mm_add_ps(mRegister, Vector.mRegister);
            return (* this);
        }

        /// \brief Adds a scalar value to the vector.
        /// 
        /// \param Scalar The scalar value to add.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator+=(Real32 Scalar)
        {
            mRegister = _mm_add_ps(mRegister, _mm_set_ps1(Scalar));
            return (* this);
        }

        /// \brief Subtracts another vector from the current vector.
        /// 
        /// \param Vector The vector to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator-=(ConstRef<Vector4> Vector)
        {
            mRegister = _mm_sub_ps(mRegister, Vector.mRegister);
            return (* this);
        }

        /// \brief Subtracts a scalar value from the vector.
        /// 
        /// \param Scalar The scalar value to subtract.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator-=(Real32 Scalar)
        {
            mRegister = _mm_sub_ps(mRegister, _mm_set_ps1(Scalar));
            return (* this);
        }

        /// \brief Multiplies the vector components by another vector.
        /// 
        /// \param Vector The vector to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator*=(ConstRef<Vector4> Vector)
        {
            mRegister = _mm_mul_ps(mRegister, Vector.mRegister);
            return (* this);
        }

        /// \brief Multiplies the vector components by a scalar value.
        /// 
        /// \param Scalar The scalar value to multiply.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator*=(Real32 Scalar)
        {
            mRegister = _mm_mul_ps(mRegister, _mm_set_ps1(Scalar));
            return (* this);
        }

        /// \brief Divides the vector components by another vector.
        /// 
        /// \param Vector The vector to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator/=(ConstRef<Vector4> Vector)
        {
            mRegister = _mm_div_ps(mRegister, Vector.mRegister);
            return (* this);
        }

        /// \brief Divides the vector components by a scalar value.
        /// 
        /// \param Scalar The scalar value to divide by.
        /// \return A reference to the updated vector.
        AURORA_INLINE Ref<Vector4> operator/=(Real32 Scalar)
        {
            mRegister = _mm_div_ps(mRegister, _mm_set_ps1(Scalar));
            return (* this);
        }

        /// \brief Checks if this vector is equal to another vector.
        /// 
        /// \param Vector The vector to compare to.
        /// \return `true` if all vectors are approximately equal, `false` otherwise.
        AURORA_INLINE Bool operator==(ConstRef<Vector4> Vector) const
        {
            const __m128 Diff     = _mm_sub_ps(mRegister, Vector.mRegister);
            const __m128 Register = _mm_cmplt_ps(_mm_andnot_ps(_mm_set1_ps(-0.0f), Diff), _mm_set1_ps(kEpsilon<Real32>));
            return _mm_movemask_ps(Register) == 0xF;
        }

        /// \brief Checks if this vector is not equal to another vector.
        /// 
        /// \param Other The vector to compare to.
        /// \return `true` if the vectors are not equal, `false` otherwise.
        AURORA_INLINE Bool operator!=(ConstRef<Vector4> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Compares this vector with another for less-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than the other vector’s components, otherwise `false`.
        AURORA_INLINE Bool operator<(ConstRef<Vector4> Vector) const
        {
            return _mm_movemask_ps(_mm_cmplt_ps(mRegister, Vector.mRegister)) == 0xF;
        }

        /// \brief Compares this vector with another for greater-than relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than the other vector’s components, otherwise `false`.
        AURORA_INLINE Bool operator>(ConstRef<Vector4> Vector) const
        {
            return _mm_movemask_ps(_mm_cmpgt_ps(mRegister, Vector.mRegister)) == 0xF;
        }

        /// \brief Compares this vector with another for less-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are less than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE Bool operator<=(ConstRef<Vector4> Vector) const
        {
            return _mm_movemask_ps(_mm_cmple_ps(mRegister, Vector.mRegister)) == 0xF;
        }

        /// \brief Compares this vector with another for greater-than or equal relationship.
        ///
        /// \param Vector The vector to compare against.
        /// \return `true` if all components of this vector are greater than or equal to the other vector’s components, otherwise `false`.
        AURORA_INLINE Bool operator>=(ConstRef<Vector4> Vector) const
        {
            return _mm_movemask_ps(_mm_cmpge_ps(mRegister, Vector.mRegister)) == 0xF;
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        /// 
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            AURORA_ALIGN(16) Real32 Array[4];

            if constexpr (Serializer::IsReader)
            {
                Archive.SerializeReal32(Array[0]);
                Archive.SerializeReal32(Array[1]);
                Archive.SerializeReal32(Array[2]);
                Archive.SerializeReal32(Array[3]);

                mRegister = _mm_load_ps(Array);
            }
            else
            {
                _mm_store_ps(Array, mRegister);

                Archive.SerializeReal32(Array[0]);
                Archive.SerializeReal32(Array[1]);
                Archive.SerializeReal32(Array[2]);
                Archive.SerializeReal32(Array[3]);
            }
        }

    public:

        /// \brief Returns the zero vector (0.0f, 0.0f, 0.0f, 0.0f).
        /// 
        /// \return The zero vector.
        AURORA_INLINE static Vector4 Zero()
        {
            return Vector4();
        }

        /// \brief Returns the one vector (1.0f, 1.0f, 1.0f, 1.0f).
        /// 
        /// \return The one vector.
        AURORA_INLINE static Vector4 One()
        {
            return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        /// \brief Returns the unit vector along the X-axis (1.0f, 0.0f, 0.0f, 0.0f).
        /// 
        /// \return The unit vector along the X-axis.
        AURORA_INLINE static Vector4 UnitX()
        {
            return Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the Y-axis (0.0f, 1.0f, 0.0f, 0.0f).
        /// 
        /// \return The unit vector along the Y-axis.
        AURORA_INLINE static Vector4 UnitY()
        {
            return Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the Z-axis (0.0f, 0.0f, 1.0f, 0.0f).
        /// 
        /// \return The unit vector along the Z-axis.
        AURORA_INLINE static Vector4 UnitZ()
        {
            return Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        }

        /// \brief Returns the unit vector along the W-axis (0.0f, 0.0f, 0.0f, 1.0f).
        /// 
        /// \return The unit vector along the W-axis.
        AURORA_INLINE static Vector4 UnitW()
        {
            return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        }

        /// \brief Calculates the horizontal sum of the components in the vector.
        /// 
        /// \param Vector The input 4D vector to sum.
        /// \return The sum of all components in the vector.
        AURORA_INLINE static Real32 Sum(ConstRef<Vector4> Vector)
        {
            const __m128 Lane = _mm_hadd_ps(Vector.mRegister, Vector.mRegister);
            return _mm_cvtss_f32(_mm_hadd_ps(Lane, Lane));
        }

        /// \brief Calculates the horizontal difference of the components in the vector.
        /// 
        /// \param Vector The input 4D vector to compute the difference.
        /// \return The difference of all components in the vector.
        AURORA_INLINE static Real32 Subtract(ConstRef<Vector4> Vector)
        {
            const __m128 Lane = _mm_hsub_ps(Vector.mRegister, Vector.mRegister);
            return _mm_cvtss_f32(_mm_hsub_ps(Lane, Lane));
        }

        /// \brief Rearranges the components of a vector based on specified lane indices.
        /// 
        /// \tparam Z The index (0–3) of the component to place in the Z position.
        /// \tparam Y The index (0–3) of the component to place in the Y position.
        /// \tparam X The index (0–3) of the component to place in the X position.
        /// \tparam W The index (0–3) of the component to place in the W position.
        /// \param Vector The input vector to be swizzled.
        /// \return A new vector with reordered components based on the specified indices.
        template<UInt8 Z, UInt8 Y, UInt8 X, UInt8 W>
        AURORA_INLINE static Vector4 Swizzle(ConstRef<Vector4> Vector)
        {
            return Vector4(_mm_shuffle_ps(Vector.mRegister, Vector.mRegister, _MM_SHUFFLE(Z, Y, X, W)));
        }

        /// \brief Rearranges the components between two vectors based on specified lane indices.
        ///
        /// \tparam Z The index (0–3) of the component to place in the Z position.
        /// \tparam Y The index (0–3) of the component to place in the Y position.
        /// \tparam X The index (0–3) of the component to place in the X position.
        /// \tparam W The index (0–3) of the component to place in the W position.
        /// \param Vec1 The first input vector.
        /// \param Vec2 The second input vector.
        /// \return A new vector with reordered components based on the specified indices.
        template<UInt8 Z, UInt8 Y, UInt8 X, UInt8 W>
        AURORA_INLINE static Vector4 Swizzle(ConstRef<Vector4> First, ConstRef<Vector4> Second)
        {
            return Vector4(_mm_shuffle_ps(First.mRegister, Second.mRegister, _MM_SHUFFLE(Z, Y, X, W)));
        }

        /// \brief Rearranges the components of a vector by duplicating the even-indexed elements (x and z).
        ///
        /// \param Vector The input vector (x, y, z, w).
        /// \return A new vector with the pattern (x, x, z, z).
        AURORA_INLINE static Vector4 Swizzle0022(ConstRef<Vector4> Vector)
        {
            return Vector4(_mm_moveldup_ps(Vector.mRegister));
        }

        /// \brief Rearranges the components of a vector by duplicating the odd-indexed elements (y and w).
        ///
        /// \param Vector The input vector (x, y, z, w).
        /// \return A new vector with the pattern (y, y, w, w).
        AURORA_INLINE static Vector4 Swizzle1133(ConstRef<Vector4> Vector)
        {
            return Vector4(_mm_movehdup_ps(Vector.mRegister));
        }

        /// \brief Combines the low halves (x, y) of two vectors into a new vector.
        ///
        /// \param Vec1 The first input vector (a0, a1, a2, a3).
        /// \param Vec2 The second input vector (b0, b1, b2, b3).
        /// \return A new vector with the pattern (a0, a1, b0, b1).
        AURORA_INLINE static Vector4 Shuffle0101(ConstRef<Vector4> Vec1, ConstRef<Vector4> Vec2)
        {
            return Vector4(_mm_movelh_ps(Vec1.mRegister, Vec2.mRegister));
        }

        /// \brief Combines the high halves (z, w) of two vectors into a new vector.
        ///
        /// \param Vec1 The first input vector (a0, a1, a2, a3).
        /// \param Vec2 The second input vector (b0, b1, b2, b3).
        /// \return A new vector (a2, a3, b2, b3).
        AURORA_INLINE static Vector4 Shuffle2323(ConstRef<Vector4> Vec1, ConstRef<Vector4> Vec2)
        {
            return Vector4(_mm_movehl_ps(Vec2.mRegister, Vec1.mRegister));
        }

        /// \brief Replicates a single component of a vector across all four lanes.
        /// 
        /// \tparam Lane The index (0–3) of the component to replicate.
        /// \param Vector The input vector.
        /// \return A new vector with all components set to the specified lane of the input.
        template<UInt8 Lane>
        AURORA_INLINE static Vector4 Splat(ConstRef<Vector4> Vector)
        {
            return Swizzle<Lane, Lane, Lane, Lane>(Vector);
        }

        /// \brief Retrieves the X component from the vector and replicates it across all components.
        /// 
        /// \param Vector The input vector from which to extract the X component.
        /// \return A new vector where all components are equal to the X component of the input vector.
        AURORA_INLINE static Vector4 SplatX(ConstRef<Vector4> Vector)
        {
            return Splat<0>(Vector);
        }

        /// \brief Retrieves the Y component from the vector and replicates it across all components.
        /// 
        /// \param Vector The input vector from which to extract the Y component.
        /// \return A new vector where all components are equal to the Y component of the input vector.
        AURORA_INLINE static Vector4 SplatY(ConstRef<Vector4> Vector)
        {
            return Splat<1>(Vector);
        }

        /// \brief Retrieves the Z component from the vector and replicates it across all components.
        /// 
        /// \param Vector The input vector from which to extract the Z component.
        /// \return A new vector where all components are equal to the Z component of the input vector.
        AURORA_INLINE static Vector4 SplatZ(ConstRef<Vector4> Vector)
        {
            return Splat<2>(Vector);
        }

        /// \brief Retrieves the W component from the vector and replicates it across all components.
        /// 
        /// \param Vector The input vector from which to extract the W component.
        /// \return A new vector where all components are equal to the W component of the input vector.
        AURORA_INLINE static Vector4 SplatW(ConstRef<Vector4> Vector)
        {
            return Splat<3>(Vector);
        }

        /// \brief Normalizes the given vector.
        /// 
        /// \param Vector The vector to normalize.
        /// \return A normalized vector.
        AURORA_INLINE static Vector4 Normalize(ConstRef<Vector4> Vector)
        {
            const __m128 Length = _mm_sqrt_ps(_mm_dp_ps(Vector.mRegister, Vector.mRegister, 0xFF));
            return Vector4(_mm_div_ps(Vector.mRegister, Length));
        }

        /// \brief Projects the source vector onto the target vector using XYZ components only (W ignored).
        /// 
        /// \param Source The vector to be projected.
        /// \param Target The vector onto which the source is projected.
        /// \return The projection of source onto target.
        AURORA_INLINE static Vector4 Project(ConstRef<Vector4> Source, ConstRef<Vector4> Target)
        {
            return Target * (Dot3(Source, Target) / Dot3(Target, Target));
        }

        /// \brief Reflects the incident vector over the given normal.
        /// 
        /// \param Incident The incoming vector to reflect.
        /// \param Normal   The surface normal to reflect across (should be normalized).
        /// \return The reflected vector.
        AURORA_INLINE static Vector4 Reflect(ConstRef<Vector4> Incident, ConstRef<Vector4> Normal)
        {
            return Incident - Normal * (2.0f * Dot3(Incident, Normal));
        }

        /// \brief Performs an alternating subtract/add between two vectors.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The result of the add/sub operation.
        AURORA_INLINE static Vector4 Mix(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return Vector4(_mm_addsub_ps(P0.mRegister, P1.mRegister));
        }

        /// \brief Blends two vectors using a compile-time mask.
        /// 
        /// \tparam Mask A 4-bit mask where each bit indicates selection (0 = from A, 1 = from B).
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The blended vector.
        template <UInt8 Mask>
        AURORA_INLINE static Vector4 Blend(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return Vector4(_mm_blend_ps(P0.mRegister, P1.mRegister, Mask));
        }

        /// \brief Blends two vectors using a runtime mask.
        /// 
        /// \param P0   The first vector.
        /// \param P1   The second vector.
        /// \param Mask The mask vector.
        /// \return The blended vector.
        AURORA_INLINE static Vector4 Blend(ConstRef<Vector4> P0, ConstRef<Vector4> P1, ConstRef<Vector4> Mask)
        {
            return Vector4(_mm_blendv_ps(P0.mRegister, P1.mRegister, Mask.mRegister));
        }

        /// \brief Computes the dot product of two vectors.
        /// 
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The dot product of the two vectors.
        template<UInt8 Mask = 0xFF>
        AURORA_INLINE static Real32 Dot(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return _mm_cvtss_f32(_mm_dp_ps(P0.mRegister, P1.mRegister, Mask));
        }

        /// \brief 3-lane dot product (XYZ) of two 4D vectors; W is ignored.
        /// 
        /// \param P0 First operand (XYZ used).
        /// \param P1 Second operand (XYZ used).
        /// \return P0.x * P1.x + P0.y * P1.y + P0.z * P1.z
        AURORA_INLINE static Real32 Dot3(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return Dot<0x71>(P0 ,P1);
        }

        /// \brief Computes the 3D cross product of two vectors (XYZ lanes); W is set to 0.0f.
        ///
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return The cross product of the two vectors, with W = 0.
        AURORA_INLINE static Vector4 Cross(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            const Vector4 A = Swizzle<3, 0, 2, 1>(P0); // (y, z, x, w)
            const Vector4 B = Swizzle<3, 1, 0, 2>(P1); // (z, x, y, w)
            const Vector4 C = Swizzle<3, 0, 2, 1>(P1); // (y, z, x, w)
            const Vector4 D = Swizzle<3, 1, 0, 2>(P0); // (z, x, y, w)
            return Blend<0b1000>(A * B - C * D, Zero());
        }

        /// \brief Returns the component-wise minimum of two vectors.
        /// 
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise minimum values.
        AURORA_INLINE static Vector4 Min(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return Vector4(_mm_min_ps(P0.mRegister, P1.mRegister));
        }

        /// \brief Returns the component-wise maximum of two vectors.
        /// 
        /// \param P0 The first vector.
        /// \param P1 The second vector.
        /// \return A vector with the component-wise maximum values.
        AURORA_INLINE static Vector4 Max(ConstRef<Vector4> P0, ConstRef<Vector4> P1)
        {
            return Vector4(_mm_max_ps(P0.mRegister, P1.mRegister));
        }

        /// \brief Floors each component of the given vector.
        /// 
        /// \param Vector The vector to floor.
        /// \return A vector with floored components.
        AURORA_INLINE static Vector4 Floor(ConstRef<Vector4> Vector)
        {
            return Vector4(_mm_floor_ps(Vector.mRegister));
        }

        /// \brief Ceils each component of the given vector.
        /// 
        /// \param Vector The vector to ceil.
        /// \return A vector with ceiled components.
        AURORA_INLINE static Vector4 Ceil(ConstRef<Vector4> Vector)
        {
            return Vector4(_mm_ceil_ps(Vector.mRegister));
        }

        /// \brief Linearly interpolates between two vectors.
        /// 
        /// \param Start      The starting vector.
        /// \param End        The ending vector.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A vector interpolated between the start and end vectors.
        AURORA_INLINE static Vector4 Lerp(ConstRef<Vector4> Start, ConstRef<Vector4> End, Real32 Percentage)
        {
            return Start + (End - Start) * Percentage;
        }

    private:

        /// \brief Constructs a vector from a SSE register (intended for low-level SIMD operations).
        /// 
        /// \param Register The register containing four 32-bit floating point values.
        AURORA_INLINE explicit Vector4(__m128 Register)
            : mRegister { Register }
        {
        }

    public:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        __m128 mRegister;
    };
}