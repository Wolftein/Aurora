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
        mWorld.set_entity_range(k_MinRangeDynamics, k_MaxRangeDynamics);

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

        while (Reader.Peek<UInt32>() != k_Terminator)
        {
            // Read Component's ID
            const SStr Name(Reader.ReadString8());
            const flecs::untyped_component Component = mWorld.component(Name.c_str());

            // Read Component's Bundle
            CPtr<UInt8> Bundle = Reader.ReadBlock<UInt8>();

            // Read Component's Data
            if (Core::Reader Data(Bundle); Data.GetAvailable() > 0)
            {
                if (const ConstPtr<Factory> Serializer = Component.get_mut<Factory>())
                {
                    if (const Ptr<void> Memory = Entity.ensure(Component))
                    {
                        Serializer->Read(Data, Memory);
                    }
                    Entity.modified(Component);
                }
                else
                {
                    Log::Warn("World: Trying to load an invalid component '{}'", Component.name().c_str());
                }
            }
            else
            {
                Entity.add(Component);
            }
        }

        // Read Component's separator
        Reader.Skip(sizeof(UInt32));
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
                const Entity Component = Iterator.src(0);
                const Entity Actor     = Iterator.src(1);

                // Write Component's ID
                Writer.WriteString8(Component.GetName());

                // Write Component's Data
                Core::Writer Bundle;
                Iterator.field<const Factory>(0)->Write(Bundle, Actor.Find(Component));

                // Flush Component's Data
                Writer.WriteBlock(Bundle.GetData());
            }

            // Write terminator
            Writer.WriteUInt32(k_Terminator);
        };
        Query.run(OnIterate);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponentsAndSystems()
    {
        // Observer to keep archetype(s) storage updated.
        mArchetypesObserver = React<>("_Archetypes::OnDelete").with(EcsPrefab).event(EcsOnRemove)
            .each([this](Entity Actor)
            {
                mArchetypes.Free(Actor.GetID() - k_MinRangeArchetypes);
            });

        // Register default component(s).
        using TransformDirty = Tag<Hash("Worldspace_Dirty")>;
        Register<TransformDirty>("Worldspace_Dirty");

        Register<Factory,    k_Final>("Serializer");
        Register<Pivot,      k_Inheritable | k_Serializable>("Pivot");
        Register<Color,      k_Inheritable | k_Serializable>("Color");
        Register<Matrix4f,   k_Default>("Worldspace");
        Register<Transformf, k_Serializable>("Localspace").With<TransformDirty>();

        // Register default system(s).
        React<>("_Default::UpdateTransformDirty").with<Transformf>().event(EcsOnSet)
                .each(Entity::ToggleComponentInHierarchy<TransformDirty, true>);

        Execute<const Transformf, ConstPtr<Matrix4f>>()
                .term_at(1)
                    .cascade()
                .with<TransformDirty>()
                .write<Matrix4f>()
                .each([](Entity Actor, ConstRef<Transformf> Local, ConstPtr<Matrix4f> Parent)
                      {
                          Actor.Attach<Matrix4f>(Parent ? (* Parent) * Local.Compute() : Local.Compute());
                          Actor.Disable<TransformDirty>();
                      });
    }
}