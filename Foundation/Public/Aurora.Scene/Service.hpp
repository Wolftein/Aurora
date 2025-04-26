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
#include "Observer.hpp"
#include "Query.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    // @TODO: 128Bit UUID instead of names.
    class Service final : public AbstractSubsystem<Service>
    {
    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // -=(Undocumented)=-
        void Progress(ConstRef<Time> Time);

        // -=(Undocumented)=-
        template<typename Type>
        void Attach(Any<Type> Component)
        {
            mWorld.set<Type>(Move(Component));
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Attach()
        {
            mWorld.add<Type>();
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
        template<Bool Archetype = false>
        Entity Spawn()
        {
            return Allocate<Archetype>();
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
            return Scene::Component<Type>(mWorld.component<Type>());
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
        template<typename Function, typename ...Arguments>
        void Defer(Any<Function> Callback, Any<Arguments>... Parameters)
        {
            mWorld.defer_begin();
            {
                Callback(std::forward<Arguments>(Parameters)...);
            }
            mWorld.defer_end();
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Register(CStr ID = flecs::_::symbol_name<Type>())
        {
            return Scene::Component<Type>(mWorld.component<Type>(ID.data()));
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Event()
        {
            return mWorld.template event<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto React(CStr Name = "")
        {
            return mWorld.template observer<Components...>(Name.data());
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Match(CStr Name = "")
        {
            return mWorld.template query_builder<Components...>(Name.data());
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto System(CStr Name = "", Entity Phase = EcsOnUpdate)
        {
            auto Builder = mWorld.template system<Components...>(Name.data());
            Builder.kind(Phase.GetHandle());

            return Builder;
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
        template<Bool Archetype>
        Entity Allocate(UInt64 ID = 0)
        {
            flecs::entity Actor;

            if constexpr (Archetype)
            {
                Actor = mWorld.entity(ID ? ID : k_MinRangeArchetypes + mArchetypes.Allocate());
            }
            else
            {
                Actor = (ID ? mWorld.entity(ID) : mWorld.entity());
            }

            mWorld.make_alive(Actor);

            if constexpr (Archetype)
            {
                Actor.add(flecs::Prefab);
            }
            else
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