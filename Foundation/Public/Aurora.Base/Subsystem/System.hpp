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

#include "Tickable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Subsystem>
    class System
    {
    public:

        // -=(Undocumented)=-
        enum class Mode
        {
            // -=(Undocumented)=-
            Client,

            // -=(Undocumented)=-
            Server,

            // -=(Undocumented)=-
            Both,
        };

    public:

        // -=(Undocumented)=-
        System()
            : mMode { Mode::Both }
        {
        }

        // -=(Undocumented)=-
        void SetMode(Mode Mode)
        {
            mMode = Mode;
        }

        // -=(Undocumented)=-
        Bool IsClientMode() const
        {
            return mMode != Mode::Server;
        }

        // -=(Undocumented)=-
        Bool IsServerMode() const
        {
            return mMode != Mode::Client;
        }

        // -=(Undocumented)=-
        void Tick(ConstRef<Time> Time)
        {
            for (const Ptr<Tickable> Tickable : mTickables)
            {
                Tickable->OnTick(Time);
            }
        }

        // -=(Undocumented)=-
        template<typename T, typename ... Args>
        SPtr<T> AddSubsystem(Any<Args> ... Arguments)
        {
            const SPtr<T> SubsystemPtr = NewPtr<T>(* this, Arguments...);
            mRegistry.push_back(SubsystemPtr);

            if constexpr (std::is_base_of_v<Tickable, T>)
            {
                mTickables.push_back(SubsystemPtr.get());
            }
            return SubsystemPtr;
        }

        // -=(Undocumented)=-
        template<typename T>
        SPtr<T> GetSubsystem()
        {
            for (ConstSPtr<Subsystem> SubsystemPtr : mRegistry)
            {
                if (Hash<T>() == SubsystemPtr->GetID())
                {
                    return CastPtr<T>(SubsystemPtr);
                }
            }
            return nullptr;
        }

        // -=(Undocumented)=-
        template<typename T>
        void RemoveSubsystem()
        {
            constexpr auto FindByID = [](ConstSPtr<Subsystem> SubsystemPtr)
            {
                return SubsystemPtr->GetID() == Hash<T>();
            };

            auto Iterator = std::find_if(mRegistry.begin(), mRegistry.end(), FindByID);
            if (Iterator != mRegistry.end())
            {
                if constexpr (std::is_base_of_v<Tickable, T>)
                {
                    const Ptr<Tickable> TickablePtr = reinterpret_cast<Ptr<Tickable>>(Iterator->get());
                    mTickables.erase(
                            std::remove(mTickables.begin(), mTickables.end(), TickablePtr), mTickables.end());
                }
                mRegistry.erase(Iterator, mRegistry.end());
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Mode                    mMode;
        Vector<Ptr<Tickable>>   mTickables;
        Vector<SPtr<Subsystem>> mRegistry;
    };
}