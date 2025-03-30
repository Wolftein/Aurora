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

#include "Aurora.Base/Type.hpp"
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
        k_MinRangeArchetypes = 1'024,

        // -=(Undocumented)=-
        k_MaxRangeArchetypes = 65'534,

        // -=(Undocumented)=-
        k_MinRangeEntities   = 65'535,

        // -=(Undocumented)=-
        k_MaxRangeEntities   = 4'294'967'295,

        // -=(Undocumented)=-
        k_MaxComponents      = k_MaxRangeComponents - k_MinRangeComponents,

        // -=(Undocumented)=-
        k_MaxArchetypes      = k_MaxRangeArchetypes - k_MinRangeArchetypes,

        // -=(Undocumented)=-
        k_MaxEntities        = k_MaxRangeEntities - k_MinRangeEntities,
    };

    // -=(Undocumented)=-
    enum Trait : UInt32
    {
        // -=(Undocumented)=-
        k_Default      = 0b00000000,

        // -=(Undocumented)=-
        k_Sparse       = 0b00000001,

        // -=(Undocumented)=-
        k_Serializable = 0b00000010,

        // -=(Undocumented)=-
        k_Inheritable  = 0b00000100,

        // -=(Undocumented)=-
        k_Toggleable   = 0b00001000,

        // -=(Undocumented)=-
        k_Final        = 0b00010000,

        // -=(Undocumented)=-
        k_Archetype    = 0b00100000,
    };
}