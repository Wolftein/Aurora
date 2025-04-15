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
    // -=(Undocumented)=-
    template<typename Base>
    class Manifold final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxCollisionPoints = 4;

    public:

        // -=(Undocumented)=-
        Manifold()
            : mNormal      { 0 },
              mPenetration { 0 }
        {
        }

        // -=(Undocumented)=-
        Bool IsCollided() const
        {
            return !mPoints.IsEmpty();
        }

        // -=(Undocumented)=-
        void SetNormal(ConstRef<Vector2<Base>> Normal)
        {
            SetNormal(Vector3<Base>(Normal.GetX(), Normal.GetY(), 0));
        }

        // -=(Undocumented)=-
        void SetNormal(ConstRef<Vector3<Base>> Normal)
        {
            mNormal = Normal;
        }

        // -=(Undocumented)=-
        ConstRef<Vector3<Base>> GetNormal() const
        {
            return mNormal;
        }

        // -=(Undocumented)=-
        void SetPenetration(Base Penetration)
        {
            mPenetration = Penetration;
        }

        // -=(Undocumented)=-
        Base GetPenetration() const
        {
            return mPenetration;
        }

        // -=(Undocumented)=-
        Bool AddPoint(ConstRef<Vector2<Base>> Point)
        {
            return AddPoint(Vector3<Base>(Point.GetX(), Point.GetY(), 0));
        }

        // -=(Undocumented)=-
        Bool AddPoint(ConstRef<Vector3<Base>> Point)
        {
            if (const UInt32 Element = mPoints.Allocate(); Element > 0)
            {
                mPoints[Element] = Point;
            }
            else
            {
                return false;
            }
            return true;
        }

        // -=(Undocumented)=-
        CPtr<const Vector3<Base>> GetPoints() const
        {
            return mPoints.GetContent();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3<Base>                             mNormal;
        Base                                      mPenetration;
        Pool<Vector3<Base>, k_MaxCollisionPoints> mPoints;
    };

    // -=(Undocumented)=-
    using Manifoldf = Manifold<Real32>;
}