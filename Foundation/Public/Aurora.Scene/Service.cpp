// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Context> Context)
        : AbstractSubsystem(Context)
    {
        RegisterDefaultComponents();
        RegisterDefaultSystems();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTick(ConstRef<Time> Time)
    {
        mWorld.progress(Time.GetDelta());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entity Service::Create()
    {
        return Entity(mWorld.entity());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entity Service::Find(UInt64 ID)
    {
        const Entity::Handle Handle = mWorld.entity(ID);
        return mWorld.is_alive(Handle) ? Entity(Handle) : Entity(flecs::entity::null());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Load(Ref<Reader> Reader, Entity Actor)
    {
        const flecs::entity Entity = Actor.GetHandle();

        while (Reader.GetAvailable() > 0)
        {
            // Read Component's ID
            const flecs::untyped_component Component = mWorld.component(Reader.ReadInt<UInt64>());

            // Read Component's Data
            if (ecs_id_is_tag(mWorld, Component))
            {
                Entity.add(Component);
            }
            else
            {
                if (const Ptr<Component::TEcsFactory> Factory = Component.get_mut<Component::TEcsFactory>())
                {
                    if (const Ptr<void> Data = Entity.ensure(Component))
                    {
                        Factory->Read(Reader, Data);
                    }
                    Entity.modified(Component);
                }
                else
                {
                    Log::Warn("World: Trying to load an invalid component '{}'", Component.name().c_str());
                }
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Save(Ref<Writer> Writer, Entity Actor)
    {
        const flecs::query Query = mWorld.query_builder()
                .with<Component::TEcsFactory>().src("$component")
                .with("$component").src(Actor.GetHandle())
                .build();

        const auto OnIterate = [& Writer](Ref<flecs::iter> Iterator)
        {
            while (Iterator.next())
            {
                const flecs::entity Component = Iterator.src(0);
                const flecs::entity Entity    = Iterator.src(1);

                // Write Component's ID
                Writer.WriteInt<UInt64>(Component.raw_id());

                // Write Component's Data
                Iterator.field<Component::TEcsFactory>(0)->Write(Writer, Entity.get_mut(Component));
            }
        };
        Query.run(OnIterate);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponents()
    {
        Register<Component::TEcsTint>();
        Register<Component::TEcsTransform, Component::TEcsMatrix>().add(flecs::CanToggle);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultSystems()
    {
        static constexpr void (* OnEnableTransform)(flecs::entity Entity) = [](flecs::entity Entity)
        {
            Entity.enable<Component::TEcsTransform>();
            Entity.children(OnEnableTransform);
        };
        mWorld.observer().with<Component::TEcsTransform>().self().event(flecs::OnSet).each(OnEnableTransform);
        mWorld.system<const Component::TEcsTransform, Ptr<const Component::TEcsMatrix>, Component::TEcsMatrix>()
            .term_at(1).parent().cascade()
            .kind(flecs::PreUpdate)
            .each([](Entity::Handle Entity,
                     ConstRef<Component::TEcsTransform> Local,
                     Ptr<const Component::TEcsMatrix> Parent,
                     Ref<Component::TEcsMatrix> World)
            {
                World = Parent ? (* Parent) * Local.Compute() : Local.Compute();
                Entity.disable<Component::TEcsTransform>();
            });
    }
}