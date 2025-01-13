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
                if (const Ptr<Component::Factory> Factory = Component.get_mut<Component::Factory>())
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
                .with<Component::Factory>().src("$component")
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
                Iterator.field<Component::Factory>(0)->Write(Writer, Entity.get_mut(Component));
            }
        };
        Query.run(OnIterate);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponents()
    {
        Register<Component::LocalTransform, true, Component::WorldTransform>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultSystems()
    {
        System<>()
                .kind(flecs::PreUpdate)
                .with<const Component::LocalTransform>()
                .with<const Component::WorldTransform>().optional().parent().cascade()
                .with<Component::WorldTransform>().out()
                .run([](Ref<flecs::iter> Iterator)
                     {
                         while (Iterator.next())
                         {
                             if (Iterator.changed())
                             {
                                 OnApplyTransformation(Iterator);
                             }
                             else
                             {
                                 Iterator.skip();
                             }
                         }
                     });
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnApplyTransformation(Ref<flecs::iter> Iterator)
    {
        const auto LocalTransformTable  = Iterator.field<const Component::LocalTransform>(0);
        const auto WorldTransformTable  = Iterator.field<Component::WorldTransform>(2);
        const auto ParentTransform      = Iterator.field<const Component::WorldTransform>(1);

        if (Iterator.is_set(1))
        {
            for (const UInt64 Index : Iterator)
            {
                WorldTransformTable[Index] = ParentTransform[0] * LocalTransformTable[Index].Compute();
            }
        }
        else
        {
            for (const UInt64 Index : Iterator)
            {
                WorldTransformTable[Index] = LocalTransformTable[Index].Compute();
            }
        }
    }
}