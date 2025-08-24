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

#include "Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A helper class that injects service dependencies into a derived class.
    /// 
    /// The `Provider` class template is used to statically fetch and store a tuple of
    /// service dependencies from the system host during construction. It allows derived
    /// classes to access their required services without manual lookup.
    /// 
    /// \tparam Dependencies The list of service types to inject.
    template<typename... Dependencies>
    class Provider
    {
    public:

        /// \brief Constructs the provider and fetches all specified service dependencies.
        /// 
        /// \param Host The parent system host containing all registered services.
        AURORA_INLINE explicit Provider(Ref<Service::Host> Host)
            : mDependencies { Host.GetService<Dependencies>()... }
        {
        }

        /// \brief Retrieves a mutable reference to a specific injected service.
        /// 
        /// \tparam Type The service type to retrieve.
        /// \return A mutable reference to the requested service.
        template<typename Type>
        AURORA_INLINE Ref<Type> GetService()
        {
            return * Fetch<Tracker<Type>>(mDependencies);
        }

        /// \brief Retrieves a const reference to a specific injected service.
        /// 
        /// \tparam Type The service type to retrieve.
        /// \return A const reference to the requested service.
        template<typename Type>
        AURORA_INLINE ConstRef<Type> GetService() const
        {
            return * Fetch<Tracker<Type>>(mDependencies);
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Tuple<Tracker<Dependencies>...> mDependencies;
    };
}