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

#include "Core/Types.hpp"
#include <math.h> // TODO: Implement our own using SSE

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Type>
    Type Cosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return cos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return cosf(Angle);
        }
        else
        {
            return cosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    Type Sine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return sin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return sinf(Angle);
        }
        else
        {
            return sinl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    Type Tangent(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return tan(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return tanf(Angle);
        }
        else
        {
            return tanl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    Type InverseCosine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return acos(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return acosf(Angle);
        }
        else
        {
            return acosl(Angle);
        }
    }

    // -=(Undocumented)=-
    template<typename Type>
    Type InverseSine(Type Angle)
    {
        if constexpr (std::is_base_of_v<Real64, Type>)
        {
            return asin(Angle);
        }
        else if constexpr (std::is_base_of_v<Real32, Type>)
        {
            return asinf(Angle);
        }
        else
        {
            return asinl(Angle);
        }
    }
}