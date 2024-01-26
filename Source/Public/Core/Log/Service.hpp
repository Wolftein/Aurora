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

#define QUILL_DISABLE_NON_PREFIXED_MACROS
#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>

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
            static Service _Implementation;
            return _Implementation;
        }

    public:

        // -=(Undocumented)=-
        Service();

        // -=(Undocumented)=-
        void Initialise(CStr Filename);

        // -=(Undocumented)=-
        void Shutdown();

        // -=(Undocumented)=-
        void SetRouter(const SPtr<Sink> Router);

        // -=(Undocumented)=-
        void SetVerbosity(Verbosity Value);

        // -=(Undocumented)=-
        Ptr<quill::Logger> GetLogger()
        {
            return mLogger;
        }

    private:

        // -=(Undocumented)=-
        class CustomHandlerAdapter : public quill::Handler
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
            void SetRouter(const SPtr<Sink> Router)
            {
                mRouter = Router;
            }

        public:

            // \see quill::Handler::write
            void write(Ref<quill::fmt_buffer_t const> Message, Ref<quill::TransitEvent const> Event) override
            {
                if (mRouter)
                {
                    mRouter->Write(CStr(Message.data(), Message.size()));
                }
            }

            // \see quill::Handler::flush
            void flush() noexcept override
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

        Ptr<quill::Logger>                    mLogger;
        std::shared_ptr<CustomHandlerAdapter> mAdapter;
    };

    // -=(Undocumented)=-
    #define LOG_DEBUG(Format, ...)      \
        QUILL_LOG_DEBUG(Log::Service::GetSingleton().GetLogger(), Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_INFO(Format, ...)       \
        QUILL_LOG_INFO(Log::Service::GetSingleton().GetLogger(), Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_WARNING(Format, ...)    \
        QUILL_LOG_WARNING(Log::Service::GetSingleton().GetLogger(), Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_ERROR(Format, ...)      \
        QUILL_LOG_ERROR(Log::Service::GetSingleton().GetLogger(), Format, ##__VA_ARGS__)

    // -=(Undocumented)=-
    #define LOG_CRITICAL(Format, ...)   \
        QUILL_LOG_CRITICAL(Log::Service::GetSingleton().GetLogger(), Format, ##__VA_ARGS__)
}