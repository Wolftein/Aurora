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
#include "Observer.hpp"
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
        Entity Spawn()
        {
            return Allocate<Trait>();
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
        Entity Register()
        {
            flecs::entity Actor = mWorld.component<Type>();

            if constexpr (Traits & Trait::k_Sparse)
            {
                Actor.add(flecs::OnInstantiate, flecs::Sparse);
            }
            if constexpr (Traits & Trait::k_Serializable)
            {
                Actor.template set<Factory>(Factory::Create<Type>());
            }
            if constexpr (Traits & Trait::k_Inheritable)
            {
                Actor.add(flecs::OnInstantiate, flecs::Inherit);
            }
            if constexpr (Traits & Trait::k_Toggleable)
            {
                Actor.add(flecs::CanToggle);
            }
            if constexpr (Traits & Trait::k_Final)
            {
                Actor.add(flecs::Final);
            }
            if constexpr (! std::is_void_v<Dependency>)
            {
                Actor.add(flecs::With, mWorld.component<Dependency>());
            }
            return Actor;
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Observe(CStr Name)
        {
            return mWorld.template observer<Components...>(Name.data());
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Query(CStr Name)
        {
            return mWorld.template query_builder<Components...>(Name.data());
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Execute()
        {
            return mWorld.template system<Components...>();
        }

    public:

        // -=(Undocumented)=-
        void LoadArchetypes(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void SaveArchetypes(Ref<Writer> Writer);

        // -=(Undocumented)=-
        Entity LoadEntity(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void SaveEntity(Ref<Writer> Writer, Entity Actor);

        // -=(Undocumented)=-
        Entity LoadEntityHierarchy(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void SaveEntityHierarchy(Ref<Writer> Writer, Entity Actor);

        // -=(Undocumented)=-
        void LoadComponents(Ref<Reader> Reader, Entity Actor);

        // -=(Undocumented)=-
        void SaveComponents(Ref<Writer> Writer, Entity Actor);

    private:

        // -=(Undocumented)=-
        void RegisterDefaultComponentsAndSystems();

        // -=(Undocumented)=-
        template<UInt32 Trait>
        Entity Allocate(UInt64 ID = 0)
        {
            flecs::entity Actor;

            if constexpr (Trait & Trait::k_Archetype)
            {
                Actor = mWorld.entity(ID ? ID : k_MinRangeArchetypes + mArchetypes.Allocate());
            }
            else
            {
                Actor = (ID ? mWorld.entity(ID) : mWorld.entity());
            }

            mWorld.make_alive(Actor);

            if constexpr (Trait & Trait::k_Archetype)
            {
                Actor.add(flecs::Prefab);
            }

            if constexpr (Trait & Trait::k_Final)
            {
                Actor.add(flecs::Final);
            }
            return Actor;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world                 mWorld;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<k_MaxCountArchetypes> mArchetypes;
        Scene::Observer              mArchetypesOnDelete;
    };
}