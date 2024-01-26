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
            : mInnerRadius      { 0.0f },
              mInnerRadiusAngle { 0.0f },
              mAttenuation      { 1.0f }
        {
        }

        // -=(Undocumented)=-
        void SetPosition(Vector3f Position)
        {
            mPosition = Position;
        }

        // -=(Undocumented)=-
        void SetVelocity(Vector3f Velocity)
        {
            mVelocity = Velocity;
        }

        // -=(Undocumented)=-
        void SetOrientation(Vector4f Orientation)
        {
            mOrientation = Orientation;
        }

        // -=(Undocumented)=-
        void SetAttenuation(Real32 Attenuation)
        {
            mAttenuation = Attenuation;
        }

        // -=(Undocumented)=-
        void SetInnerRadius(Real32 Radius)
        {
            mInnerRadius = Radius;
        }

        // -=(Undocumented)=-
        void SetInnerRadiusAngle(Real32 Angle)
        {
            mInnerRadiusAngle = Angle;
        }

        // -=(Undocumented)=-
        Vector3f GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        Vector3f GetVelocity() const
        {
            return mVelocity;
        }

        // -=(Undocumented)=-
        Vector4f GetOrientation() const
        {
            return mOrientation;
        }

        // -=(Undocumented)=-
        Real32 GetAttenuation() const
        {
            return mAttenuation;
        }

        // -=(Undocumented)=-
        Real32 GetInnerRadius() const
        {
            return mInnerRadius;
        }

        // -=(Undocumented)=-
        Real32 GetInnerRadiusAngle() const
        {
            return mInnerRadiusAngle;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector3f mPosition;
        Vector3f mVelocity;
        Vector4f mOrientation;
        Real32   mInnerRadius;
        Real32   mInnerRadiusAngle;
        Real32   mAttenuation;
    };
}