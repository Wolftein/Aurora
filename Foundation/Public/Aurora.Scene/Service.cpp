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
        mWorld.set_threads(SDL_GetNumLogicalCPUCores());

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
            Entity Archetype = Allocate<true>(k_MinRangeArchetypes + Reader.ReadInt<UInt64>());
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
        Entity Actor = Allocate<false>();

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
        while (Reader.Peek<UInt32>() != k_Terminator)
        {
            // Read Component's Pair
            const SStr Pair(Reader.ReadString8());
            const Entity First = Pair.empty() ? Entity() : mWorld.component(Pair.c_str());

            // Read Component's ID
            const SStr Name(Reader.ReadString8());
            const Entity Second = mWorld.component(Name.c_str());

            // Read Component's Bundle
            CPtr<UInt8> Bundle = Reader.ReadBlock<UInt8>();

            // Read Component's Data
            if (Core::Reader Data(Bundle); Data.GetAvailable() > 0)
            {
                if (const ConstPtr<Factory> Serializer = Second.Find<const Factory>())
                {
                    if (First.IsValid())
                    {
                        if (const Ptr<void> Memory = Actor.Ensure(First, Second))
                        {
                            Serializer->Read(Data, Memory);
                        }
                        Actor.Notify(First, Second);
                    }
                    else
                    {
                        if (const Ptr<void> Memory = Actor.Ensure(Second))
                        {
                            Serializer->Read(Data, Memory);
                        }
                        Actor.Notify(Second);
                    }
                }
                else
                {
                    Log::Warn("World: Trying to load an invalid component '{}'", Second.GetName());
                }
            }
            else
            {
                if (First.IsValid())
                {
                    Actor.Attach(First, Second);
                }
                else
                {
                    Actor.Attach(Second);
                }
            }
        }

        // Read Component's separator
        Reader.Skip(sizeof(UInt32));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveComponents(Ref<Writer> Writer, Entity Actor)
    {
        // Write Components
        const auto OnIterateComponent = [&](Entity Component)
        {
            Entity First;
            Entity Second;

            if (Component.IsPair())
            {
                First  = Component.GetFirst();
                Second = Component.GetSecond();
            }
            else
            {
                Second = Component;
            }

            ConstPtr<Factory> Serializer = Second.IsValid() ? Second.Find<const Factory>() : nullptr;

            if (Serializer && (First.IsNull() || First.Contains<Factory>()))
            {
                // Write Component's Pair
                Writer.WriteString8(First.IsValid() ? First.GetName() : "");

                // Write Component's ID
                Writer.WriteString8(Second.GetName());

                // Write Component's Data
                Core::Writer Bundle;
                Serializer->Write(Bundle, Actor.Find(Component));

                // Flush Component's Data
                Writer.WriteBlock(Bundle.GetData());
            }
        };
        Actor.Iterate(OnIterateComponent);

        // Write Terminator
        Writer.WriteUInt32(k_Terminator);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponentsAndSystems()
    {
        // Observer to keep archetype(s) storage updated.
        React<>("_Archetypes::OnDelete").with(EcsPrefab).event(EcsOnRemove)
            .each([this](Entity Actor)
            {
                mArchetypes.Free(Actor.GetID() - k_MinRangeArchetypes);
            });

        // Register default component(s).
        Register<Factory>("Factory");
    }
}