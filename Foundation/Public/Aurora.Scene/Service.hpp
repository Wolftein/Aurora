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
    // @TODO: 128Bit UUID instead of names.
    class Service final : public AbstractSubsystem<Service>, public Tickable
    {
    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // \see Tickable::OnTick
        void OnTick(ConstRef<Time> Time) override;

        // -=(Undocumented)=-
        template<typename Type>
        void Attach(Any<Type> Component)
        {
            if constexpr (std::is_empty_v<Type>)
            {
                mWorld.add<Type>();
            }
            else
            {
                mWorld.set<Type>(Move(Component));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find() const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mWorld.get<Type>();
            }
            else
            {
                return mWorld.get_mut<Type>();
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Contains() const
        {
            return mWorld.has<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Detach()
        {
            mWorld.remove<Type>();
        }

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
            return Handle.is_valid() && mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        // -=(Undocumented)=-
        Entity Fetch(CStr Name)
        {
            const Entity::Handle Handle = mWorld.lookup(Name.data());
            return Handle.is_valid() && mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto GetComponent()
        {
            return Component<Type>(mWorld.component<Type>());
        }

        // -=(Undocumented)=-
        template<typename Function>
        void GetArchetypes(Any<Function> Callback)
        {
            mWorld.query_builder<>().with(EcsPrefab).each(Callback);
        }

        // -=(Undocumented)=-
        template<typename Tag, typename Function>
        void GetTags(Any<Function> Callback)
        {
            mWorld.query_builder<>().with<Tag>().each(Callback);
        }

        // -=(Undocumented)=-
        template<typename Type, UInt32 Traits = k_Default>
        Component<Type> Register(CStr ID = flecs::_::symbol_name<Type>())
        {
            flecs::component<Type> Actor = mWorld.component<Type>(ID.data());

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
            return Actor;
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto React(CStr Name = "")
        {
            return mWorld.template observer<Components...>(Name.data());
        }

        // -=(Undocumented)=-
        template<typename Query>
        auto Match(CStr Name = "")
        {
            return std::apply([&](auto... Arguments)
            {
                return mWorld.template query_builder<decltype(Arguments)...>(Name.data());
            }, typename Query::Types { });
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
        Handle<k_MaxCountArchetypes> mArchetypes;
    };
}