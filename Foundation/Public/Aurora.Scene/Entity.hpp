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
#include "Link.hpp"

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
        template<typename Type>
        Entity(Type Handle)
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
        Bool IsValid() const
        {
            return mHandle.is_valid();
        }

        // -=(Undocumented)=-
        Bool IsNull() const
        {
            return mHandle == 0;
        }

        // -=(Undocumented)=-
        Bool IsArchetype() const
        {
            return mHandle.has(flecs::Prefab);
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
            if constexpr (std::is_empty_v<Type>)
            {
                mHandle.add<Type>();
            }
            else
            {
                mHandle.set<Type>(Move(Component));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Link() const
        {
            return Scene::Link<Type>(mHandle.get_ref<Type>());
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find() const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.get<Type>();
            }
            else
            {
                return mHandle.get_mut<Type>();
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find(Entity Component) const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.get<Type>(Component.GetID());
            }
            else
            {
                return mHandle.get_mut<Type>(Component.GetID());
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find(Entity First, Entity Second) const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.get<Type>(First.GetID(), Second.GetID());
            }
            else
            {
                return mHandle.get_mut<Type>(First.GetID(), Second.GetID());
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Contains() const
        {
            return mHandle.has<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Contains(Type Value) const
        {
            return mHandle.has(Value);
        }

        // -=(Undocumented)=-
        Bool Contains(Entity Component) const
        {
            return mHandle.has(Component.GetID());
        }

        // -=(Undocumented)=-
        Bool Contains(Entity First, Entity Second) const
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
        void Enable()
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
        void Disable()
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
        void SetParent(Entity Parent)
        {
            mHandle.child_of(Parent.GetHandle());
        }

        // -=(Undocumented)=-
        auto GetParent() const
        {
            return Entity(mHandle.parent());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetArchetype()
        {
            mHandle.is_a<Type>();
        }

        // -=(Undocumented)=-
        void SetArchetype(Entity Archetype)
        {
            mHandle.is_a(Archetype.GetHandle());
        }

        // -=(Undocumented)=-
        auto GetArchetype() const
        {
            ConstPtr<Entity::Handle> Handle = mHandle.get<Entity::Handle>(flecs::IsA);
            return Entity(Handle ? * Handle : Entity::Handle::null());
        }

        // -=(Undocumented)=-
        template<typename Function>
        void Children(Any<Function> Callback) const
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
            const flecs::string_view Name = mHandle.name();
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