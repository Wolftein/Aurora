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

#include "Aurora.Base/Base.hpp"
#include "Aurora.Math/Math.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=(Undocumented)=-
    enum
    {
        // -=(Undocumented)=-
        k_Default     = 0,

        // -=(Undocumented)=-
        k_MaxBuffers  = 3,

        // -=(Undocumented)=-
        k_MaxMixes    = 128,

        // -=(Undocumented)=-
        k_MaxSubmixes = 16,
    };

    // -=(Undocumented)=-
    enum class Backend
    {
        None,
        FAudio,
    };

    // -=(Undocumented)=-
    enum class Kind
    {
        ADPCM,
        PCM,
        IEEE,
    };

    // -=(Undocumented)=-
    using Object = UInt;

    // -=(Undocumented)=-
    struct Adapter
    {
        // -=(Undocumented)=-
        SStr   Name;

        // -=(Undocumented)=-
        Bool   Default;
    };

    // -=(Undocumented)=-
    struct Capabilities
    {
        // -=(Undocumented)=-
        Backend Backend  = Backend::None;

        // -=(Undocumented)=-
        SStr    Device;

        // -=(Undocumented)=-
        Vector<Adapter> Adapters;
    };
}