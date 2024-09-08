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
    inline void SetLevel(Level Level)
    {
        switch (Level)
        {
        case Level::None:
            spdlog::set_level(spdlog::level::off);
            break;
        case Level::Debug:
            spdlog::set_level(spdlog::level::debug);
            break;
        case Level::Information:
            spdlog::set_level(spdlog::level::info);
            break;
        case Level::Warning:
            spdlog::set_level(spdlog::level::warn);
            break;
        case Level::Error:
            spdlog::set_level(spdlog::level::err);
            break;
        case Level::Critical:
            spdlog::set_level(spdlog::level::critical);
            break;
        }
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Trace(spdlog::format_string_t<Args...> Format, Args &&... Arguments)
    {
        spdlog::log(spdlog::level::trace, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Debug(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::debug, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Info(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::info, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Warn(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::warn, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Error(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::err, Format, std::forward<Args>(Arguments)...);
    }

    // -=(Undocumented)=-
    template <typename... Args>
    inline void Critical(spdlog::format_string_t<Args...> Format, Args &&...Arguments)
    {
        spdlog::log(spdlog::level::critical, Format, std::forward<Args>(Arguments)...);
    }
}