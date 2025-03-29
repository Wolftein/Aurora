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

#include "Entity.hpp"
#include "Factory.hpp"
#include "Query.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class Service final : public AbstractSubsystem<Service>, public Tickable
    {
    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // \see Tickable::OnTick
        void OnTick(ConstRef<Time> Time) override;

        // -=(Undocumented)=-
        template<UInt32 Trait = Trait::k_Final>
        Entity Spawn(Entity Archetype = Entity())
        {
            Entity Actor = Allocate<Trait>();

            if constexpr (Trait & Trait::k_Final)
            {
                Actor.Attach(flecs::Final);
            }

            if (Archetype.IsValid())
            {
                Actor.SetArchetype(Archetype);
            }
            return Actor;
        }

        // -=(Undocumented)=-
        Entity Fetch(UInt64 ID)
        {
            const Entity::Handle Handle = mWorld.entity(ID);
            return mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        // -=(Undocumented)=-
        Entity Fetch(CStr Name)
        {
            const Entity::Handle Handle = mWorld.lookup(Name.data());
            return mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        // -=(Undocumented)=-
        template<typename Type, UInt32 Traits = k_Default, typename Dependency = void>
        auto Register()
        {
            flecs::entity Handle = mWorld.component<Type>();

            if constexpr (Traits & Trait::k_Sparse)
            {
                Handle.add(flecs::OnInstantiate, flecs::Sparse);
            }

            if constexpr (Traits & Trait::k_Serializable)
            {
                Handle.template set<Factory>(Factory::Create<Type>());
            }

            if constexpr (Traits & Trait::k_Inheritable)
            {
                Handle.add(flecs::OnInstantiate, flecs::Inherit);
            }

            if constexpr (Traits & Trait::k_Toggleable)
            {
                Handle.add(flecs::CanToggle);
            }

            if constexpr (Traits & Trait::k_Final)
            {
                Handle.add(flecs::Final);
            }

            if constexpr (! std::is_void_v<Dependency>)
            {
                Handle.add(flecs::With, mWorld.component<Dependency>());
            }

            return Component;
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Component()
        {
            return Entity(mWorld.component<Type>());
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Observer()
        {
            return mWorld.template observer<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Query()
        {
            return mWorld.template query_builder<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto System()
        {
            return mWorld.template system<Components...>();
        }

        // -=(Undocumented)=-
        void LoadArchetypes(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void SaveArchetypes(Ref<Writer> Writer);

        // -=(Undocumented)=-
        void LoadComponents(Ref<Reader> Reader, Entity Actor);

        // -=(Undocumented)=-
        void SaveComponents(Ref<Writer> Writer, Entity Actor);

    private:

        // -=(Undocumented)=-
        void RegisterDefaultComponents();

        // -=(Undocumented)=-
        void RegisterDefaultSystems();

        // -=(Undocumented)=-
        template<UInt32 Trait>
        Entity Allocate(UInt64 Identifier = 0)
        {
            Entity::Handle Handle;

            if constexpr (Trait & Trait::k_Archetype)
            {
                Handle = mWorld.entity(Identifier ? Identifier : k_MinRangeArchetypes + mArchetypes.Allocate());
            }
            else
            {
                Handle = (Identifier ? mWorld.entity(Identifier) : mWorld.entity());
            }

            mWorld.make_alive(Handle);

            if constexpr (Trait & Trait::k_Archetype)
            {
                Handle.add(flecs::Prefab);
            }
            return Handle;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world            mWorld;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<k_MaxArchetypes> mArchetypes;
    };
}