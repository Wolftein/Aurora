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
    class Entity final
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
        Entity(Handle Handle)
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
        template<typename Type>
        Bool Retain(Type Value)
        {
            return mHandle.has(Value);
        }

        // -=(Undocumented)=-
        Bool Retain(Entity Component)
        {
            return mHandle.has(Component.GetID());
        }

        // -=(Undocumented)=-
        Bool Retain(Entity First, Entity Second)
        {
            return mHandle.has(First.GetID(), Second.GetID());
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
        void Enable() const
        {
            mHandle.enable<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Enable(Type Value)
        {
            mHandle.enable(Value);
        }

        // -=(Undocumented)=-
        void Enable(Entity Component)
        {
            mHandle.enable(Component.GetID());
        }

        // -=(Undocumented)=-
        void Enable(Entity First, Entity Second)
        {
            mHandle.enable(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Disable() const
        {
            mHandle.disable<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Disable(Type Value)
        {
            mHandle.disable(Value);
        }

        // -=(Undocumented)=-
        void Disable(Entity Component)
        {
            mHandle.disable(Component.GetID());
        }

        // -=(Undocumented)=-
        void Disable(Entity First, Entity Second)
        {
            mHandle.disable(First.GetID(), Second.GetID());
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
        template<typename Function>
        void Children(Any<Function> Callback)
        {
            mHandle.children(Callback);
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

    public:

        // -=(Undocumented)=-
        template<typename Component, Bool Enable>
        static void ToggleComponentInHierarchy(Scene::Entity Entity)
        {
            if constexpr (Enable)
            {
                Entity.template Enable<Component>();
            }
            else
            {
                Entity.template Disable<Component>();
            }
            Entity.Children(& ToggleComponentInHierarchy<Component, Enable>);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}