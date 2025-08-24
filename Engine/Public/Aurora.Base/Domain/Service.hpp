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

#include "System.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Provides a base interface for engine or application-level services.
    /// 
    /// A `Service` is a self-contained, reusable unit of functionality that is registered
    /// into a central `System` context. It supports per-frame updates, lifecycle management,
    /// and communication with other services through the shared context.
    class Service : public Trackable
    {
    public:

        /// \brief Alias for the system context that manages all services.
        using Host = System<Service>;

    public:

        /// \brief Constructs a service with a reference to its managing system and a unique identifier.
        /// 
        /// \param Host The parent system managing this service.
        /// \param Id   The unique identifier of this service (typically a type hash).
        AURORA_INLINE Service(Ref<Host> Host, UInt64 Guid)
            : mHost { Host },
              mGuid { Guid }
        {
        }

        /// \brief Gets the unique identifier of this service.
        /// 
        /// \return The hash-based identifier of the service.
        AURORA_INLINE UInt64 GetGuid() const
        {
            return mGuid;
        }

        /// \brief Called once per frame or simulation tick by the system.
        /// 
        /// \note Override this to implement per-frame logic.
        /// 
        /// \param Time The current time step or frame delta.
        virtual void OnTick(ConstRef<Time> Time)
        {
        }

        /// \brief Called when the service is removed or the system is shutting down.
        /// 
        /// \note Override this to release resources or unregister events.
        virtual void OnTeardown()
        {

        }

    protected:

        /// \brief Gets the system context managing this service.
        /// 
        /// \return A reference to the system context.
        AURORA_INLINE Ref<Host> GetHost()
        {
            return mHost;
        }

        /// \brief Checks whether the system is running in client mode.
        /// 
        /// \return `true` if the system is in client mode, `false` otherwise.
        AURORA_INLINE Bool IsClientMode() const
        {
            return mHost.IsClientMode();
        }

        /// \brief Checks whether the system is running in server mode.
        /// 
        /// \return `true` if the system is in server mode, `false` otherwise.
        AURORA_INLINE Bool IsServerMode() const
        {
            return mHost.IsServerMode();
        }

        /// \brief Adds a new service to the same system context.
        /// 
        /// \tparam Type The concrete service type to create.
        /// \param Parameters The forwarded constructor arguments.
        /// \return A tracker to the newly created service.
        template<typename Type, typename ... Arguments>
        AURORA_INLINE Tracker<Type> AddService(AnyRef<Arguments>... Parameters)
        {
            return mHost.AddService<Type>(Parameters...);
        }

        /// \brief Retrieves an existing service from the same system context.
        /// 
        /// \tparam Type The service type to search for.
        /// \return A tracker to the service, or `nullptr` if not found.
        template<typename Type>
        AURORA_INLINE Tracker<Type> GetService()
        {
            return mHost.GetService<Type>();
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<Host> mHost;
        UInt64    mGuid;
    };

    /// \brief A helper base class that assigns a hash-based ID to a typed subsystem.
    template<typename Type>
    class AbstractService : public Service
    {
    public:

        /// \brief Constructs a typed service with its system context.
        /// 
        /// \param Host The parent system managing this service.
        AURORA_INLINE explicit AbstractService(Ref<Host> Host)
            : Service { Host, Hash<Type>() }
        {
        }
    };
}