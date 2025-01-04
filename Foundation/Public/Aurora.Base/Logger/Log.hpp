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

#include "Level.hpp"
#include <spdlog/spdlog.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log
{
    // -=(Undocumented)=-
    void Initialize(CStr Filename);

    // -=(Undocumented)=-
    void Shutdown();

    // -=(Undocumented)=-
    void SetLevel(Level Level);
    
    // -=(Undocumented)=-
    template <typename... Args>
    void Trace(spdlog::format_string_t<Args...> Format, Any<Args> ... Arguments)
    {
        spdlog::log(spdlog::level::trace, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    void Debug(spdlog::format_string_t<Args...> Format, Any<Args> ...Arguments)
    {
        spdlog::log(spdlog::level::debug, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    void Info(spdlog::format_string_t<Args...> Format, Any<Args> ...Arguments)
    {
        spdlog::log(spdlog::level::info, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    void Warn(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::warn, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    void Error(spdlog::format_string_t<Args...> Format, Any<Args> ...Arguments)
    {
        spdlog::log(spdlog::level::err, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    void Critical(spdlog::format_string_t<Args...> Format, Any<Args> ...Arguments)
    {
        spdlog::log(spdlog::level::critical, Format, std::forward<Args>(Arguments)...);
    }
}