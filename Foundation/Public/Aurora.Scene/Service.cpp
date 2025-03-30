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

        // Initializes and registers all core component(s) and system(s).
        RegisterDefaultComponentsAndSystems();
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

    Entity Service::LoadEntity(Ref<Reader> Reader)
    {
        Entity Actor = Spawn<Trait::k_Final>();

        // Read Entity's name
        if (CStr Name = Reader.ReadString8(); !Name.empty())
        {
            Actor.SetName(Name);
        }

        // Read Entity's archetype
        if (const UInt64 Base = Reader.ReadInt<UInt64>(); Base)
        {
            Actor.SetArchetype(Fetch(k_MinRangeArchetypes + Base));
        }

        // Read Entity's component(s)
        LoadComponents(Reader, Actor);

        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveEntity(Ref<Writer> Writer, Entity Actor)
    {
        // Write Entity's name
        Writer.WriteString8(Actor.GetName());

        // Write Entity's archetype
        const Entity Archetype = Actor.GetArchetype();
        Writer.WriteInt<UInt64>(Archetype.IsValid() ? Archetype.GetID() - k_MinRangeArchetypes : 0);

        // Write Entity's component(s)
        SaveComponents(Writer, Actor);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entity Service::LoadEntityHierarchy(Ref<Reader> Reader)
    {
        // Read Entity's data
        Entity Actor = LoadEntity(Reader);

        // Read Entity's hierarchy
        while (Reader.Peek<UInt32>() != k_Terminator)
        {
            Entity Children = LoadEntityHierarchy(Reader);
            Children.SetParent(Actor);
        }

        // Read Entity's hierarchy separator
        Reader.Skip(sizeof(UInt32));

        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveEntityHierarchy(Ref<Writer> Writer, Entity Actor)
    {
        // Write Entity's data
        SaveEntity(Writer, Actor);

        // Write Entity's hierarchy
        Actor.Children([&](Entity Children)
        {
            SaveEntityHierarchy(Writer, Children);
        });

        // Write Entity's hierarchy separator
        Writer.WriteUInt32(k_Terminator);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::LoadComponents(Ref<Reader> Reader, Entity Actor)
    {
        const flecs::entity Entity = Actor.GetHandle();

        for (UInt64 Handle = Reader.ReadInt<UInt64>(); Handle; Handle = Reader.ReadInt<UInt64>())
        {
            // Read Component's ID
            const flecs::untyped_component Component = mWorld.component(Handle + k_MinRangeComponents);

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
                Writer.WriteInt<UInt64>(Component.raw_id() - k_MinRangeComponents);

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

    void Service::RegisterDefaultComponentsAndSystems()
    {
        mArchetypesQueryAll = Query<>("_Archetypes::Query").with(EcsPrefab).build();
        mArchetypesOnDelete = Observe<>("_Archetypes::OnDelete")
                .with(EcsPrefab).self().event(flecs::OnRemove)
                .each([this](Entity Actor)
                {
                    if (const UInt32 ID = Actor.GetID(); ID >= k_MinRangeArchetypes && ID <= k_MaxRangeArchetypes)
                    {
                        mArchetypes.Free(ID - k_MinRangeArchetypes);
                    }
                });

        // TODO REFACTOR
        using namespace Component;

        Register<Pivot,      k_Inheritable | k_Serializable>();
        Register<Tint,       k_Inheritable | k_Serializable>();
        Register<Dirty,      k_Toggleable>();
        Register<Worldspace, k_Default,      Dirty>();
        Register<Localspace, k_Serializable, Worldspace>();

        // Observes changes to local transforms and marks affected hierarchies as dirty.
        Observe<>("Engine::UpdateDirty").with<Localspace>().self().event(flecs::OnSet).each(Entity::ToggleComponentInHierarchy<Dirty, true>);

        // Updates world transforms for dirty entities in hierarchy order.
        Execute<const Localspace, ConstPtr<Worldspace>, Worldspace>()
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