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
        // Ensures that handles within this range are exclusively for entities created during runtime,
        // preventing conflicts with internal engine objects like component(s) or archetype(s).
        mWorld.set_entity_range(k_MinRangeEntities, k_MaxRangeEntities);

        // Initializes and registers all core components.
        RegisterDefaultComponents();

        // Initializes and registers all core systems.
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

    void Service::LoadArchetypes(Ref<Reader> Reader)
    {
        mArchetypes = Reader.ReadObject<decltype(mArchetypes)>();

        for (UInt32 Element = 1, Limit = mArchetypes.GetSize(); Element <= Limit; ++Element)
        {
            Entity Archetype = Allocate<Trait::k_Archetype>(k_MinRangeArchetypes + Reader.ReadInt<UInt64>());
            Archetype.SetName(Reader.ReadString8());

            if (const UInt64 Base = Reader.ReadInt<UInt64>(); Base)
            {
                Archetype.SetArchetype(Fetch(k_MinRangeArchetypes + Base));
            }
            LoadComponents(Reader, Archetype);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveArchetypes(Ref<Writer> Writer)
    {
        Writer.WriteObject(mArchetypes);

        for (UInt32 Element = 1, Limit = mArchetypes.GetBack(); Element <= Limit; ++Element)
        {
            if (Entity Archetype = Fetch(k_MinRangeArchetypes + Element); Archetype.IsValid())
            {
                Writer.WriteInt<UInt64>(Archetype.GetID() - k_MinRangeArchetypes);
                Writer.WriteString8(Archetype.GetName());

                const Entity Base = Archetype.GetArchetype();
                Writer.WriteInt<UInt64>(Base.IsValid() ? Base.GetID() - k_MinRangeArchetypes : 0);

                SaveComponents(Writer, Archetype);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::LoadComponents(Ref<Reader> Reader, Entity Actor)
    {
        const flecs::entity Entity = Actor.GetHandle();

        for (UInt64 Handle = Reader.ReadInt<UInt64>(); Handle; Handle = Reader.ReadInt<UInt64>())
        {
            // Read Component's ID
            const flecs::untyped_component Component = mWorld.component(Handle);

            // Read Component's Data
            if (ecs_id_is_tag(mWorld, Component))
            {
                Entity.add(Component);
            }
            else
            {
                if (const ConstPtr<Factory> Serializer = Component.get_mut<Factory>())
                {
                    if (const Ptr<void> Data = Entity.ensure(Component))
                    {
                        Serializer->Read(Reader, Data);
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

    void Service::SaveComponents(Ref<Writer> Writer, Entity Actor)
    {
        // TODO Cache query somewhere
        const flecs::query Query = mWorld.query_builder()
                .with<Factory>().src("$component")
                .with("$component").src(Actor.GetHandle())
                .build();

        const auto OnIterate = [&](Ref<flecs::iter> Iterator)
        {
            while (Iterator.next())
            {
                const flecs::entity Component = Iterator.src(0);
                const flecs::entity Entity    = Iterator.src(1);

                // Write Component's ID
                Writer.WriteInt<UInt64>(Component.raw_id());

                // Write Component's Data
                Iterator.field<const Factory>(0)->Write(Writer, Entity.get_mut(Component));
            }

            // Write terminator
            Writer.WriteInt<UInt64>(0);
        };
        Query.run(OnIterate);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponents()
    {
        Register<Component::Pivot,      k_Inheritable | k_Serializable>();
        Register<Component::Tint,       k_Inheritable | k_Serializable>();
        Register<Component::Dirty,      k_Toggleable>();
        Register<Component::Worldspace, k_Default,      Component::Dirty>();
        Register<Component::Localspace, k_Serializable, Component::Worldspace>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultSystems()
    {
        using namespace Component;

        // Observes when an archetype is destroyed.
        Observer<>().with(flecs::Prefab).self().event(flecs::OnRemove).each([&](Entity Actor)
        {
            if (const UInt32 ID = Actor.GetID(); ID >= k_MinRangeArchetypes && ID <= k_MaxRangeArchetypes)
            {
                mArchetypes.Free(ID - k_MinRangeArchetypes);
            }
        });

        // Observes changes to local transforms and marks affected hierarchies as dirty.
        Observer<>().with<Localspace>().self().event(flecs::OnSet).each(Entity::ToggleComponentInHierarchy<Dirty, true>);

        // Updates world transforms for dirty entities in hierarchy order.
        System<const Localspace, ConstPtr<Worldspace>, Worldspace>()
            .with<Dirty>()
            .term_at(1)
                .cascade()
            .each([](Entity Actor, ConstRef<Localspace> Local, ConstPtr<Worldspace> Parent, Ref<Worldspace> World)
            {
                World = Parent ? (* Parent) * Local.Compute() : Local.Compute();
                Actor.Notify<Worldspace>();
                Actor.Disable<Dirty>();
            });
    }
}