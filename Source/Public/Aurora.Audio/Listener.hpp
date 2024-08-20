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
    class Listener final
    {
    public:

        // -=(Undocumented)=-
        Listener()
            : mDirty { true }
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
        void SetPosition(Ref<const Vector3f> Position)
        {
            SetDirty(true);
            mPosition = Position;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        void SetVelocity(Ref<const Vector3f> Velocity)
        {
            SetDirty(true);
            mVelocity = Velocity;
        }

        // -=(Undocumented)=-
        Ref<const Vector3f> GetVelocity() const
        {
            return mVelocity;
        }

        // -=(Undocumented)=-
        void SetOrientation(Ref<const Quaternionf> Orientation)
        {
            SetDirty(true);
            mOrientation = Orientation;
        }

        // -=(Undocumented)=-
        Ref<const Quaternionf> GetOrientation() const
        {
            return mOrientation;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool        mDirty;
        Vector3f    mPosition;
        Vector3f    mVelocity;
        Quaternionf mOrientation;
    };
}