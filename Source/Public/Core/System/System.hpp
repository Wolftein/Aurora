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

#include "Core/Types.hpp"
#include "Core/Trait.hpp"

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
            : mMode { Mode::Client }
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
            return (mMode != Mode::Server);
        }

        // -=(Undocumented)=-
        Bool IsServerMode() const
        {
            return (mMode != Mode::Client);
        }

        // -=(Undocumented)=-
        template<typename T>
        void Execute(T Functor)
        {
            for (ConstSPtr<Subsystem> SubsystemPtr : mSubsystems)
            {
                Functor(* SubsystemPtr);
            }
        }

        // -=(Undocumented)=-
        template<typename T, typename ... Args>
        ConstSPtr<T> AddSubsystem(Args && ... Arguments)
        {
            return CastPtr<T>(
                mSubsystems.emplace_back(NewPtr<T>(* this, Arguments...)));
        }

        // -=(Undocumented)=-
        template<typename T>
        void RemoveSubsystem()
        {
            std::erase_if(mSubsystems, [](ConstSPtr<Subsystem> SubsystemPtr)
            {
                return (typeid(T) == typeid(* SubsystemPtr));
            });
        }

        // -=(Undocumented)=-
        template<typename T>
        ConstSPtr<T> GetSubsystem()
        {
            for (ConstSPtr<Subsystem> SubsystemPtr : mSubsystems)
            {
                if (typeid(T) == typeid(* SubsystemPtr))
                {
                    return CastPtr<T>(SubsystemPtr);
                }
            }
            return nullptr;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Mode                    mMode;
        Vector<SPtr<Subsystem>> mSubsystems;
    };
}