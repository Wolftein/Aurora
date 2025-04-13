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

#include "Aurora.Math/Math.hpp"
#include <flecs.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    enum
    {
        // -=(Undocumented)=-
        k_Terminator         = 0xFFFF'FFFF,

        // -=(Undocumented)=-
        k_MinRangeComponents = 0,

        // -=(Undocumented)=-
        k_MaxRangeComponents = 1'023,

        // -=(Undocumented)=-
        k_MaxCountComponents = k_MaxRangeComponents - k_MinRangeComponents,

        // -=(Undocumented)=-
        k_MinRangeArchetypes = 1'024,

        // -=(Undocumented)=-
        k_MaxRangeArchetypes = 65'534,

        // -=(Undocumented)=-
        k_MaxCountArchetypes = k_MaxRangeArchetypes - k_MinRangeArchetypes,

        // -=(Undocumented)=-
        k_MinRangeDynamics   = 65'535,

        // -=(Undocumented)=-
        k_MaxRangeDynamics   = 4'294'967'295,

        // -=(Undocumented)=-
        k_MaxCountDynamics   = k_MaxRangeDynamics - k_MinRangeDynamics,
    };

    // -=(Undocumented)=-
    enum Trait : UInt32
    {
        // -=(Undocumented)=-
        Sparse       = 0b00000001,

        // -=(Undocumented)=-
        Serializable = 0b00000010,

        // -=(Undocumented)=-
        Inheritable  = 0b00000100,

        // -=(Undocumented)=-
        Toggleable   = 0b00001000,
    };
}