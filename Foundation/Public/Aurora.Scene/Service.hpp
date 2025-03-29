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
        static constexpr UInt32 k_MinRangeComponents = 0;

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxRangeComponents = 1'023;

        // -=(Undocumented)=-
        static constexpr UInt32 k_MinRangeArchetypes = 1'024;

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxRangeArchetypes = 65'534;

        // -=(Undocumented)=-
        static constexpr UInt32 k_MinRangeDynamic    = 65'535;

        // -=(Undocumented)=-
        static constexpr UInt32 k_MaxRangeDynamic    = 4'294'967'295;

    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // \see Tickable::OnTick
        void OnTick(ConstRef<Time> Time) override;

        // -=(Undocumented)=-
        template<UInt32 Trait = Trait::k_Final>
        Entity Create(Entity Archetype = Entity())
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
        Entity Find(UInt64 ID)
        {
            const Entity::Handle Handle = mWorld.entity(ID);
            return mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        // -=(Undocumented)=-
        template<typename Type, UInt32 Traits = k_Default, typename Dependency = void>
        auto Register()
        {
            flecs::entity Component = mWorld.component<Type>();

            if constexpr (Traits & Trait::k_Sparse)
            {
                Component.add(flecs::OnInstantiate, flecs::Sparse);
            }

            if constexpr (Traits & Trait::k_Serializable)
            {
                Component.template set<Factory>(Factory::Create<Type>());
            }

            if constexpr (Traits & Trait::k_Inheritable)
            {
                Component.add(flecs::OnInstantiate, flecs::Inherit);
            }

            if constexpr (Traits & Trait::k_Toggleable)
            {
                Component.add(flecs::CanToggle);
            }

            if constexpr (Traits & Trait::k_Final)
            {
                Component.add(flecs::Final);
            }

            if constexpr (! std::is_void_v<Dependency>)
            {
                Component.add(flecs::With, mWorld.component<Dependency>());
            }

            return Component;
        }

        // -=(Undocumented)=-
        void LoadArchetypes(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void SaveArchetypes(Ref<Writer> Writer);

        // -=(Undocumented)=-
        void LoadComponents(Ref<Reader> Reader, Entity Actor);

        // -=(Undocumented)=-
        void SaveComponents(Ref<Writer> Writer, Entity Actor);

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Observe()
        {
            return mWorld.template observer<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Select()
        {
            return mWorld.template query_builder<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Execute()
        {
            return mWorld.template system<Components...>();
        }

    private:

        // -=(Undocumented)=-
        using Archetypes = Handle<k_MaxRangeArchetypes - k_MinRangeArchetypes>;

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

            if constexpr (Trait::k_Archetype)
            {
                Handle.set(flecs::Prefab);
            }
            return Handle;
        }

        // -=(Undocumented)=-
        void RegisterDefaultComponents();

        // -=(Undocumented)=-
        void RegisterDefaultSystems();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world mWorld;
        Archetypes   mArchetypes;
    };
}