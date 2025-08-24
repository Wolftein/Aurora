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

#include "Primitive.hpp"
#include <quill/Logger.h>
#include <quill/LogMacros.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   DATA   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log::_
{
    /// \brief The default global logger instance used by logging macros.
    /// 
    /// This pointer is initialized by `Log::Initialize()` and released by `Log::Shutdown()`.
    /// All log macros route their output through this logger.
    extern Ptr<quill::Logger> sLogger;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log
{
    /// \brief Initializes the logging system with the specified output file.
    /// 
    /// \param Filename The output file path for log messages.
    void Initialize(ConstText Filename);

    /// \brief Shuts down the logging system.
    void Shutdown();

/// \brief Logs a debug-level message.
#define LOG_DEBUG(Format, ...)    QUILL_LOG_DEBUG(Log::_::sLogger, Format, ##__VA_ARGS__)

/// \brief Logs an informational message.
#define LOG_INFO(Format, ...)     QUILL_LOG_INFO(Log::_::sLogger, Format, ##__VA_ARGS__)

/// \brief Logs a warning-level message.
#define LOG_WARNING(Format, ...)  QUILL_LOG_WARNING(Log::_::sLogger, Format, ##__VA_ARGS__)

/// \brief Logs an error-level message.
#define LOG_ERROR(Format, ...)    QUILL_LOG_ERROR(Log::_::sLogger, Format, ##__VA_ARGS__)

/// \brief Logs a critical-level message.
#define LOG_CRITICAL(Format, ...) QUILL_LOG_CRITICAL(Log::_::sLogger, Format, ##__VA_ARGS__)
}