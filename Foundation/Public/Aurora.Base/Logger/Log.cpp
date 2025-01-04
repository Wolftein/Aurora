// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Log.hpp"
#include <spdlog/async.h>

#ifdef    SDL_PLATFORM_ANDROID

    #include "spdlog/sinks/android_sink.h"

#else  // SDL_PLATFORM_ANDROID

    #include <spdlog/sinks/basic_file_sink.h>
    #include <spdlog/sinks/stdout_sinks.h>

#endif // SDL_PLATFORM_ANDROID

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Initialize(CStr Filename)
    {
        if (spdlog::thread_pool())
        {
            return;
        }

        Vector<spdlog::sink_ptr> Sinks;

#ifdef    SDL_PLATFORM_ANDROID

        Sinks.emplace_back(NewPtr<spdlog::sinks::android_sink_mt>("Aurora", false));

#else  // SDL_PLATFORM_ANDROID

#ifdef    _DEBUG

        Sinks.emplace_back(NewPtr<spdlog::sinks::stdout_sink_mt>());

#endif // _DEBUG

        if (! Filename.empty())
        {
            Sinks.emplace_back(NewPtr<spdlog::sinks::basic_file_sink_mt>(Filename.data(), true));
        }

#endif // SDL_PLATFORM_ANDROID

        // Create the backend logging thread
        spdlog::init_thread_pool(8192, 1);
        spdlog::flush_every(std::chrono::seconds(3));

        // Create the logger
        SPtr<spdlog::logger> Logger
            = NewPtr<spdlog::async_logger>("Aurora", Sinks.begin(), Sinks.end(), spdlog::thread_pool());
        spdlog::register_logger(Logger);
        spdlog::set_default_logger(Logger);

        // Configure the pattern of all handlers
        Logger->set_pattern("%D %H:%M:%S.%e [%t] - %L - %v");
        Logger->set_level(spdlog::level::debug);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Shutdown()
    {
        spdlog::shutdown();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SetLevel(Level Level)
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
}
