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

#include "Component.hpp"
#include <flecs.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class Entity
    {
    public:

        // -=(Undocumented)=-
        using Handle = flecs::entity;

    public:

        // -=(Undocumented)=-
        Entity()
            : mHandle { Handle::null() }
        {
        }

        // -=(Undocumented)=-
        explicit Entity(Handle Handle)
            : mHandle { Handle }
        {
        }

        // -=(Undocumented)=-
        UInt64 GetID() const
        {
            return mHandle.raw_id();
        }

        // -=(Undocumented)=-
        Handle GetHandle() const
        {
            return mHandle;
        }

        // -=(Undocumented)=-
        void Destruct()
        {
            mHandle.destruct();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Attach()
        {
            mHandle.add<Type>();
        }

        // -=(Undocumented)=-
        template<typename First>
        void Attach(Entity Component)
        {
            mHandle.add<First>(Component.GetID());
        }

        // -=(Undocumented)=-
        void Attach(Entity First, Entity Second)
        {
            mHandle.add(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Attach(Any<Type> Component)
        {
            mHandle.set<Type>(Move(Component));
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Obtain()
        {
            return mHandle.get_mut<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Obtain(Entity Component)
        {
            mHandle.get_mut<Type>(Component.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Obtain(Entity First, Entity Second)
        {
            mHandle.get_mut<Type>(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Retain() const
        {
            return mHandle.has<Type>();
        }

        // -=(Undocumented)=-
        void Retain(Entity Component)
        {
            mHandle.has(Component.GetID());
        }

        // -=(Undocumented)=-
        void Retain(Entity First, Entity Second)
        {
            mHandle.has(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Detach()
        {
            mHandle.remove<Type>();
        }

        // -=(Undocumented)=-
        void Detach(Entity Component)
        {
            mHandle.remove(Component.GetID());
        }

        // -=(Undocumented)=-
        void Detach(Entity First, Entity Second)
        {
            mHandle.remove(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Notify()
        {
            mHandle.modified<Type>();
        }

        // -=(Undocumented)=-
        void Notify(Entity Component)
        {
            mHandle.modified(Component.GetID());
        }

        // -=(Undocumented)=-
        void Notify(Entity First, Entity Second)
        {
            mHandle.modified(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetParent()
        {
            mHandle.child_of<Type>();
        }

        // -=(Undocumented)=-
        void SetParent(Entity Component)
        {
            mHandle = mHandle.child_of(Component.GetID());
        }

        // -=(Undocumented)=-
        auto GetParent() const
        {
            return Entity(mHandle.parent());
        }

        // -=(Undocumented)=-
        void SetName(CStr Name)
        {
            mHandle.set_name(Name.data());
        }

        // -=(Undocumented)=-
        CStr GetName() const
        {
            const auto Name = mHandle.name();
            return CStr(Name.c_str(), Name.size());
        }

        // -=(Undocumented)=-
        Bool operator==(Ref<const Entity> Other) const
        {
            return GetHandle() == Other.GetHandle();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}