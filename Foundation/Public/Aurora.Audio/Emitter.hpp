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

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    class Emitter final
    {
    public:

        // -=(Undocumented)=-
        Emitter()
            : mDirty  { true },
              mRadius { 0.0f }
        {
        }

        // -=(Undocumented)=-
        void SetDirty(Bool Dirty)
        {
            mDirty = Dirty;
        }

        // -=(Undocumented)=-
        Bool GetDirty() const
        {
            return mDirty;
        }

        // -=(Undocumented)=-
        void SetPosition(Vector3f Position)
        {
            SetDirty(true);
            mPosition = Position;
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        void SetVelocity(Vector3f Velocity)
        {
            SetDirty(true);
            mVelocity = Velocity;
        }

        // -=(Undocumented)=-
        ConstRef<Vector3f> GetVelocity() const
        {
            return mVelocity;
        }

        // -=(Undocumented)=-
        void SetOrientation(ConstRef<Quaternionf> Orientation)
        {
            SetDirty(true);
            mOrientation = Orientation;
        }

        // -=(Undocumented)=-
        ConstRef<Quaternionf> GetOrientation() const
        {
            return mOrientation;
        }

        // -=(Undocumented)=-
        void SetRadius(Real32 Radius)
        {
            SetDirty(true);
            mRadius = Radius;
        }

        // -=(Undocumented)=-
        Real32 GetRadius() const
        {
            return mRadius;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool        mDirty;
        Vector3f    mPosition;
        Vector3f    mVelocity;
        Quaternionf mOrientation;
        Real32      mRadius;
    };
}