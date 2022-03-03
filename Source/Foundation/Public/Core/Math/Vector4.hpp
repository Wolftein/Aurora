// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
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
        Base GetX() const
        {
            return mX;
        }

        // -=(Undocumented)=-
        Base GetY() const
        {
            return mY;
        }

        // -=(Undocumented)=-
        Base GetZ() const
        {
            return mZ;
        }

        // -=(Undocumented)=-
        Base GetW() const
        {
            return mW;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Set(Type X, Type Y, Type Z, Type W)
        {
            mX = X;
            mY = Y;
            mZ = Z;
            mW = W;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator+(Ref<const Vector4<Type>> Value) const
        {
            return Vector4<Base>(mX + Value.mX, mY + Value.mY, mZ + Value.mZ, mW + Value.mW);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator+(Type Value) const
        {
            return Vector4<Base>(mX + Value, mY + Value, mZ + Value, mW + Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator-(Ref<const Vector4<Type>> Value) const
        {
            return Vector4<Base>(mX - Value.mX, mY - Value.mY, mZ - Value.mZ, mW - Value.mW);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator-(Type Value) const
        {
            return Vector4<Base>(mX + Value, mY + Value, mZ - Value, mW - Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator*(Type Value) const
        {
            return Vector4<Base>(mX * Value, mY * Value, mZ * Value, mW * Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator/(Type Value) const
        {
            return Vector4<Base>(mX / Value, mY / Value, mZ / Value, mW / Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector4<Base> operator%(Type Value) const
        {
            return Vector4<Base>(mX % Value, mY % Value, mZ % Value, mW % Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            ++mW;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            --mW;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator+=(Ref<const Vector4<Type>> Value)
        {
            mX += Value.mX;
            mY += Value.mY;
            mZ += Value.mZ;
            mW += Value.mW;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator+=(Type Value)
        {
            mX += Value;
            mY += Value;
            mZ += Value;
            mW += Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator-=(Ref<const Vector4<Type>> Value)
        {
            mX -= Value.mX;
            mY -= Value.mY;
            mZ -= Value.mZ;
            mW -= Value.mW;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator-=(Type Value)
        {
            mX -= Value;
            mY -= Value;
            mZ -= Value;
            mW -= Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator*=(Type Value)
        {
            mX *= Value;
            mY *= Value;
            mZ *= Value;
            mW *= Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector4<Base>> operator/=(Type Value)
        {
            mX /= Value;
            mY /= Value;
            mZ /= Value;
            mW /= Value;
            return * this;
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