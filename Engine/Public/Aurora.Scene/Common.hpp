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

#include "Aurora.Base/Base.hpp"
#include <flecs.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    /// \brief Constants used across the scene module.
    enum
    {
        // -=(Undocumented)=-
        kDelimiter          = -1,

        /// \brief Minimum ID for component identifiers.
        kMinRangeComponents = 0,

        /// \brief Maximum ID for component identifiers.
        kMaxRangeComponents = 1'023,

        /// \brief Maximum number of supported component types.
        kMaxCountComponents = kMaxRangeComponents - kMinRangeComponents,

        /// \brief Minimum ID for archetype identifiers.
        kMinRangeArchetypes = 1'024,

        /// \brief Maximum ID for archetype identifiers.
        kMaxRangeArchetypes = 65'534,

        /// \brief Maximum number of supported archetypes.
        kMaxCountArchetypes = kMaxRangeArchetypes - kMinRangeArchetypes,

        /// \brief Minimum ID for entity identifiers.
        kMinRangeEntities   = 65'535,

        /// \brief Maximum ID for entity identifiers.
        kMaxRangeEntities   = 4'294'967'295,

        /// \brief Maximum number of supported entities.
        kMaxCountEntities   = kMaxRangeEntities - kMinRangeEntities,
    };

    /// \brief Enumerates behavioral traits that define component capabilities.
    enum class Trait : UInt32
    {
        Serializable,  ///< Component can be serialized/deserialized.
        Inheritable,   ///< Component can be inherited by child entities.
        Toggleable,    ///< Component can be enabled/disabled at runtime.
        Sparse,        ///< Component uses sparse storage for memory efficiency.
        Associative,   ///< Component can be used as a tag in associative pairs/relationships.
        Singleton,     ///< Component is a singleton.
        Final,         ///< Component cannot be inherited (opposite of Inheritable).
    };
}