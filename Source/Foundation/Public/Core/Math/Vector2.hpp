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
    class Vector2 final
    {
    public:

        // -=(Undocumented)=-
        Vector2(Base X = 0, Base Y = 0)
            : mX { X },
              mY { Y }
        {
        }

        // -=(Undocumented)=-
        Bool IsZero() const
        {
            return mX == 0 && mY == 0;
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
        template<typename Type>
        void Set(Type X, Type Y)
        {
            mX = X;
            mY = Y;
        }

        // -=(Undocumented)=-
        Vector2<Base> operator+(Ref<const Vector2<Base>> Value) const
        {
            return Vector2<Base>(mX + Value.mX, mY + Value.mY);
        }

        // -=(Undocumented)=-
        Vector2<Base> operator-(Ref<const Vector2<Base>> Value) const
        {
            return Vector2<Base>(mX - Value.mX, mY - Value.mY);
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator++()
        {
            ++mX;
            ++mY;
            return * this;
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator--()
        {
            --mX;
            --mY;
            return * this;
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator+=(Ref<const Vector2<Base>> Value)
        {
            mX += Value.mX;
            mY += Value.mY;
            return * this;
        }

        // -=(Undocumented)=-
        Ref<Vector2<Base>> operator-=(Ref<const Vector2<Base>> Value)
        {
            mX -= Value.mX;
            mY -= Value.mY;
            return * this;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Base mX;
        Base mY;
    };

    // -=(Undocumented)=-
    using Vector2f = Vector2<Real32>;

    // -=(Undocumented)=-
    using Vector2i = Vector2<SInt32>;
}