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

#include "Aurora.Base/Collection.hpp"
#include "Aurora.Base/Time.hpp"
#include "Aurora.Base/Memory/Tracker.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A container for managing a collection of services.
    template<typename Unit>
    class System
    {
    public:

        /// \brief Specifies the execution mode of the system.
        enum class Mode : UInt8
        {
            Client, ///< Runs only in client mode.
            Server, ///< Runs only in server mode.
            Both,   ///< Runs in both client and server modes.
        };

    public:

        /// \brief Default destructor for the system.
        virtual ~System() = default;

        /// \brief Sets the execution mode of the system.
        /// 
        /// \param Mode The new execution mode to set.
        AURORA_INLINE void SetMode(Mode Mode)
        {
            mMode = Mode;
        }

        /// \brief Checks if the system should operate in client mode.
        /// 
        /// \return `true` if the system is not running in server-only mode, `false` otherwise.
        AURORA_INLINE Bool IsClientMode() const
        {
            return mMode != Mode::Server;
        }

        /// \brief Checks if the system should operate in server mode.
        /// 
        /// \return `true` if the system is not running in client-only mode, `false` otherwise.
        AURORA_INLINE Bool IsServerMode() const
        {
            return mMode != Mode::Client;
        }

        /// \brief Updates all registered services.
        /// 
        /// \param Time The current simulation time.
        void Tick(ConstRef<Time> Time)
        {
            for (ConstTracker<Unit> Service : mRegistry)
            {
                Service->OnTick(Time);
            }
        }

        /// \brief Adds a new service instance of the specified type.
        /// 
        /// \tparam Type The concrete service type to create.
        /// \param Parameters The forwarded constructor arguments.
        /// \return A tracker managing the newly created service.
        template<typename Type, typename ... Arguments>
        Tracker<Type> AddService(AnyRef<Arguments>... Parameters)
        {
            const Tracker<Type> Instance = Tracker<Type>::Create(* this, Forward<Arguments>(Parameters)...);
            mRegistry.push_back(Instance);
            return Instance;
        }

        /// \brief Retrieves a previously added service of the specified type.
        /// 
        /// \tparam Type The service type to search for.
        /// \return A tracker to the service instance if found, or `nullptr` otherwise.
        template<typename Type>
        Tracker<Type> GetService()
        {
            for (ConstTracker<Unit> Service : mRegistry)
            {
                if (Service->GetGuid() == Hash<Type>())
                {
                    return Tracker<Type>::Cast(Service);
                }
            }
            return nullptr;
        }

        /// \brief Removes the service of the specified type, if present.
        /// 
        /// \tparam Type The service type to remove.
        template<typename Type>
        void RemoveService()
        {
            constexpr auto FindByID = [](ConstTracker<Unit> Service)
            {
                return Service->GetGuid() == Hash<Type>();
            };
            mRegistry.erase(std::remove_if(mRegistry.begin(), mRegistry.end(), FindByID), mRegistry.end());
        }

    protected:

        /// \brief Performs subsystem-level teardown in reverse registration order.
        void Teardown()
        {
            for (SInt32 Index = mRegistry.size() - 1; Index >= 0; --Index)
            {
                mRegistry[Index]->OnTeardown();
            }
            mRegistry.clear();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Mode                  mMode;
        Vector<Tracker<Unit>> mRegistry;
    };
}