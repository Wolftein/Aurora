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

#include "Core/Log/Log.hpp"
#include "Core/Log/Sink.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Log
{
    // -=(Undocumented)=-
    class Service final
    {
    public:

        // -=(Undocumented)=-
        static Ref<Service> GetSingleton()
        {
            static Service sImplementation;
            return sImplementation;
        }

    public:

        // -=(Undocumented)=-
        Service();

        // -=(Undocumented)=-
        void Initialize(CStr Filename);

        // -=(Undocumented)=-
        void Shutdown();

        // -=(Undocumented)=-
        void SetVerbosity(Verbosity Value);

        // -=(Undocumented)=-
        SPtr<spdlog::logger> GetLogger()
        {
            return mLogger;
        }

    private:

        // -=(Undocumented)=-
        template<typename Mutex>
        class CustomHandlerAdapter : public spdlog::sinks::base_sink<Mutex>
        {
        public:

            // -=(Undocumented)=-
            CustomHandlerAdapter()
                : mRouter { nullptr }
            {
            }

            // -=(Undocumented)=-
            ~CustomHandlerAdapter() override = default;

            // -=(Undocumented)=-
            void SetRouter(ConstSPtr<Sink> Router)
            {
                mRouter = Router;
            }

        protected:

            // \see base_sink::sink_it_
            void sink_it_(Ref<const spdlog::details::log_msg> Message) override
            {
                if (mRouter)
                {
                    mRouter->Write(CStr(Message.payload.data(), Message.payload.size()));
                }
            }

            // \see base_sink::flush_
            void flush_() override
            {
                if (mRouter)
                {
                    mRouter->Flush();
                }
            }

        private:

            // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

            SPtr<Sink> mRouter;
        };

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<spdlog::logger>                                    mLogger;
        SPtr<CustomHandlerAdapter<spdlog::details::null_mutex>> mAdapter;
    };

    // -=(Undocumented)=-
    #define LOG_DEBUG(Format, ...)                    \
        if (Log::Service::GetSingleton().GetLogger()) \
            Log::Service::GetSingleton().GetLogger()->debug(Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_INFO(Format, ...)                     \
        if (Log::Service::GetSingleton().GetLogger()) \
            Log::Service::GetSingleton().GetLogger()->info(Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_WARNING(Format, ...)                  \
        if (Log::Service::GetSingleton().GetLogger()) \
            Log::Service::GetSingleton().GetLogger()->warn(Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_ERROR(Format, ...)                    \
        if (Log::Service::GetSingleton().GetLogger()) \
            Log::Service::GetSingleton().GetLogger()->error(Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_CRITICAL(Format, ...)                 \
        if (Log::Service::GetSingleton().GetLogger()) \
            Log::Service::GetSingleton().GetLogger()->critical(Format, ##__VA_ARGS__)
}