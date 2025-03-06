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

inline namespace Core
{
    // -=(Undocumented)=-
    class Subsystem
    {
    public:

        // -=(Undocumented)=-
        using Context = System<Subsystem>;

    public:

        // -=(Undocumented)=-
        explicit Subsystem(Ref<Context> Context, UInt ID)
            : mContext { Context },
              mID      { ID }
        {
        }

        // -=(Undocumented)=-
        virtual ~Subsystem() = default;

        // -=(Undocumented)=-
        UInt GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        virtual void OnDispose()
        {

        }

        // -=(Undocumented)=-
        virtual void OnTick(Real64 Time)
        {
        }

    protected:

        // -=(Undocumented)=-
        Ref<Context> GetContext()
        {
            return mContext;
        }

        // -=(Undocumented)=-
        Bool IsClientMode() const
        {
            return mContext.IsClientMode();
        }

        // -=(Undocumented)=-
        Bool IsServerMode() const
        {
            return mContext.IsServerMode();
        }

        // -=(Undocumented)=-
        template<typename T, typename ... Args>
        SPtr<T> AddSubsystem(Any<Args> ... Arguments)
        {
            return mContext.AddSubsystem<T>(Arguments...);
        }

        // -=(Undocumented)=-
        template<typename T>
        SPtr<T> GetSubsystem()
        {
            return mContext.GetSubsystem<T>();
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<Context> mContext;
        UInt         mID;
    };

    // -=(Undocumented)=-
    template<typename Type>
    class AbstractSubsystem : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt k_ID = Hash<Type>();

    public:

        // -=(Undocumented)=-
        explicit AbstractSubsystem(Ref<Subsystem::Context> Context)
            : Subsystem { Context, k_ID }
        {
        }
    };
}