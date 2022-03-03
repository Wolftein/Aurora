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
    class Vector3 final
    {
    public:

        // -=(Undocumented)=-
        Vector3(Base X = 0, Base Y = 0, Base Z = 0)
            : mX { X },
              mY { Y },
              mZ { Z }
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
        template<typename Type>
        void Set(Type X, Type Y, Type Z)
        {
            mX = X;
            mY = Y;
            mZ = Z;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator+(Ref<const Vector3<Type>> Value) const
        {
            return Vector3<Base>(mX + Value.mX, mY + Value.mY, mZ + Value.mZ);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator+(Type Value) const
        {
            return Vector3<Base>(mX + Value, mY + Value, mZ + Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator-(Ref<const Vector3<Type>> Value) const
        {
            return Vector3<Base>(mX - Value.mX, mY - Value.mY, mZ - Value.mZ);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator-(Type Value) const
        {
            return Vector3<Base>(mX + Value, mY + Value, mZ - Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator*(Type Value) const
        {
            return Vector3<Base>(mX * Value, mY * Value, mZ * Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator/(Type Value) const
        {
            return Vector3<Base>(mX / Value, mY / Value, mZ / Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Vector3<Base> operator%(Type Value) const
        {
            return Vector3<Base>(mX % Value, mY % Value, mZ % Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator++()
        {
            ++mX;
            ++mY;
            ++mZ;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator--()
        {
            --mX;
            --mY;
            --mZ;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator+=(Ref<const Vector3<Type>> Value)
        {
            mX += Value.mX;
            mY += Value.mY;
            mZ += Value.mZ;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator+=(Type Value)
        {
            mX += Value;
            mY += Value;
            mZ += Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator-=(Ref<const Vector3<Type>> Value)
        {
            mX -= Value.mX;
            mY -= Value.mY;
            mZ -= Value.mZ;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator-=(Type Value)
        {
            mX -= Value;
            mY -= Value;
            mZ -= Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator*=(Type Value)
        {
            mX *= Value;
            mY *= Value;
            mZ *= Value;
            return * this;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Vector3<Base>> operator/=(Type Value)
        {
            mX /= Value;
            mY /= Value;
            mZ /= Value;
            return * this;
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
    using Vector3i = Vector3<UInt32>;
}