// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service()
        : mLogger  { nullptr },
          mAdapter { std::make_shared<CustomHandlerAdapter>() }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Initialise(CStr Filename)
    {
        quill::Config Configuration;
        Configuration.default_logger_name = "Aurora";

#ifdef    EASTL_DEBUG
        Configuration.default_handlers.emplace_back(quill::stdout_handler());
#endif // EASTL_DEBUG
        Configuration.default_handlers.emplace_back(mAdapter);

        if (!Filename.empty())
        {
            quill::FileHandlerConfig Settings;
            Settings.set_open_mode('w');

            Configuration.default_handlers.emplace_back(quill::file_handler(Filename.data(), Settings));
        }

        // Configure the pattern of all handlers
        for (const auto Handler : Configuration.default_handlers)
        {
            Handler->set_pattern(
                "%(ascii_time) [%(thread)] - %(level_name) - %(message)", "%D %H:%M:%S.%Qms", quill::Timezone::GmtTime);
        }

        // Create the backend logging thread
        quill::configure(Configuration);
        quill::start();

        // Cache the default logger
        mLogger = quill::get_logger("Aurora");

#ifdef    EASTL_DEBUG
        mLogger->set_log_level(quill::LogLevel::Debug);
#else
        mLogger->set_log_level(quill::LogLevel::Info);
#endif // EASTL_DEBUG
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Shutdown()
    {
        quill::detail::LogManagerSingleton::instance().log_manager().stop_backend_worker();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SetRouter(const SPtr<Sink> Router)
    {
        mAdapter->SetRouter(Router);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SetVerbosity(Verbosity Value)
    {
        switch (Value)
        {
        case Verbosity::None:
            mLogger->set_log_level(quill::LogLevel::None);
            break;
        case Verbosity::Debug:
            mLogger->set_log_level(quill::LogLevel::Debug);
            break;
        case Verbosity::Information:
            mLogger->set_log_level(quill::LogLevel::Info);
            break;
        case Verbosity::Warning:
            mLogger->set_log_level(quill::LogLevel::Warning);
            break;
        case Verbosity::Error:
            mLogger->set_log_level(quill::LogLevel::Error);
            break;
        case Verbosity::Critical:
            mLogger->set_log_level(quill::LogLevel::Critical);
            break;
        }
    }
}
