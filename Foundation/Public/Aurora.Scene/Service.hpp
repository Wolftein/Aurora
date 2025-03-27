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
        template<UInt32 Trait = k_Final>
        Entity Create()
        {
            Entity Actor(mWorld.entity());

            if constexpr (Trait & k_Final)
            {
                Actor.Attach(flecs::Final);
            }

            if constexpr (Trait & k_Prefab)
            {
                Actor.Attach(flecs::Prefab);
            }

            return Actor;
        }

        // -=(Undocumented)=-
        Entity Find(UInt64 ID);

        // -=(Undocumented)=-
        void Load(Ref<Reader> Reader, Entity Actor);

        // -=(Undocumented)=-
        void Save(Ref<Writer> Writer, Entity Actor);

        // -=(Undocumented)=-
        template<typename Type, UInt32 Traits = k_Serializable, typename Dependency = void>
        auto Register()
        {
            flecs::entity Component = mWorld.component<Type>();

            if constexpr (Traits & k_Sparse)
            {
                Component.add(flecs::OnInstantiate, flecs::Sparse);
            }

            if constexpr (Traits & k_Serializable)
            {
                Component.template set<Factory>(Factory::Create<Type>());
            }

            if constexpr (Traits & k_Inheritable)
            {
                Component.add(flecs::OnInstantiate, flecs::Inherit);
            }

            if constexpr (Traits & k_Toggleable)
            {
                Component.add(flecs::CanToggle);
            }

            if constexpr (Traits & k_Final)
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
        void RegisterDefaultComponents();

        // -=(Undocumented)=-
        void RegisterDefaultSystems();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world mWorld;
    };
}