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
#include <SDL3/SDL_cpuinfo.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Host> Host)
        : AbstractService { Host }
    {
        mWorld.set_threads(SDL_GetNumLogicalCPUCores());

        // Ensures that handles within this range are exclusively for entities created during runtime,
        // preventing conflicts with internal engine objects like components or archetypes.
        mWorld.set_entity_range(kMinRangeEntities, kMaxRangeEntities);

        // Register engine’s built-in components and systems.
        RegisterDefaultComponentsAndSystems();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTick(ConstRef<Time> Time)
    {
        AURORA_PROFILE_SCOPE("Scene::Service::Tick");

        /// Update the world time component.
        mWorld.set<Base::Time>(Time);

        /// Advance the ECS world simulation by the frame delta.
        mWorld.progress(Time.GetDelta());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::LoadArchetypes(Ref<Reader> Reader)
    {
        mArchetypes.OnSerialize(Archive(Reader));

        for (UInt32 Element = 1, Limit = mArchetypes.GetSize(); Element <= Limit; ++Element)
        {
            /// Allocate prefab entity with the serialized identifier.
            Entity Archetype = Allocate<true>(kMinRangeArchetypes + Reader.ReadInt<UInt32>());

            /// Read and set archetype name.
            if (ConstText Name = Reader.ReadText(); !Name.empty())
            {
                Archetype.SetName(Name);
            }

            /// Read and set base archetype reference if present.
            if (const UInt32 Base = Reader.ReadInt<UInt32>(); Base)
            {
                Archetype.SetArchetype(GetEntity(kMinRangeArchetypes + Base));
            }

            /// Load all serialized components for this archetype.
            LoadComponents(Reader, Archetype);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveArchetypes(Ref<Writer> Writer)
    {
        mArchetypes.OnSerialize(Archive(Writer));

        for (UInt32 Element = 1, Limit = mArchetypes.GetBack(); Element <= Limit; ++Element)
        {
            if (Entity Archetype = GetEntity(kMinRangeArchetypes + Element); Archetype.IsValid())
            {
                /// Write archetype identifier.
                Writer.WriteInt(Archetype.GetID() - kMinRangeArchetypes);

                /// Write archetype name.
                Writer.WriteText(Archetype.GetName());

                /// Write base archetype reference or `0` if none.
                const Entity Base = Archetype.GetArchetype();
                Writer.WriteInt(Base.IsValid() ? Base.GetID() - kMinRangeArchetypes : 0);

                /// Write all components of the archetype.
                SaveComponents(Writer, Archetype);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entity Service::LoadEntity(Ref<Reader> Reader)
    {
        Entity Actor = Allocate<false>();

        /// Read and assign entity name if present.
        if (ConstText Name = Reader.ReadText(); !Name.empty())
        {
            Actor.SetName(Name);
        }

        /// Read and assign entity display name if present.
        if (ConstText Name = Reader.ReadText(); !Name.empty())
        {
            Actor.SetDisplayName(Name);
        }

        /// Read and assign archetype reference if present.
        if (const UInt64 Base = Reader.ReadInt<UInt64>(); Base)
        {
            Actor.SetArchetype(GetEntity(kMinRangeArchetypes + Base));
        }

        /// Load all serialized components for the entity.
        LoadComponents(Reader, Actor);
        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Entity Service::LoadEntityHierarchy(Ref<Reader> Reader)
    {
        /// Read and construct the root entity.
        Entity Actor = LoadEntity(Reader);

        /// Recursively read and attach all child entities.
        while (Reader.Peek<UInt32>() != kDelimiter)
        {
            Entity Children = LoadEntityHierarchy(Reader);
            Children.SetParent(Actor);
        }

        /// Skip delimiter marking the end of this hierarchy.
        Reader.Skip(sizeof(UInt32));
        return Actor;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveEntity(Ref<Writer> Writer, Entity Actor)
    {
        if (Actor.Contains<Transient>())
        {
            return;
        }

        /// Write entity name and display name.
        Writer.WriteText(Actor.GetName());
        Writer.WriteText(Actor.GetDisplayName());

        /// Write archetype reference or `0` if none.
        const Entity Archetype = Actor.GetArchetype();
        Writer.WriteInt<UInt64>(Archetype.IsValid() ? Archetype.GetID() - kMinRangeArchetypes : 0);

        /// Write all components of the entity.
        SaveComponents(Writer, Actor);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveEntityHierarchy(Ref<Writer> Writer, Entity Actor)
    {
        if (Actor.Contains<Transient>())
        {
            return;
        }

        /// Write the entity and its components.
        SaveEntity(Writer, Actor);

        /// Recursively write all child entities.
        Actor.Children([&](Entity Children)
        {
            SaveEntityHierarchy(Writer, Children);
        });

        /// Write delimiter marking the end of the hierarchy.
        Writer.WriteUInt32(kDelimiter);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::LoadComponents(Ref<Reader> Reader, Entity Actor)
    {
        while (Reader.Peek<UInt32>() != kDelimiter)
        {
            /// Read first element of the pair (tag/relationship); empty means single component.
            const Text   Pair(Reader.ReadText()); // TODO: Remove heap allocation (Flecs Limitation)
            const Entity First = Pair.empty() ? Entity() : mWorld.component(Pair.c_str());

            /// Read component name and resolve the component entity.
            const Text   Name(Reader.ReadText()); // TODO: Remove heap allocation (Flecs Limitation)
            const Entity Second = mWorld.component(Name.c_str());

            /// Read serialized component payload.
            ConstSpan<Byte> Bundle = Reader.ReadBlock<Byte>();

            /// Apply payload if present; otherwise attach component without data.
            if (Base::Reader Data(Bundle); Data.GetAvailable() > 0)
            {
                if (const ConstPtr<Factory> Serializer = Second.Lookup<const Factory>())
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
                    LOG_WARNING("World: Trying to load an invalid component '{}'", Second.GetName());
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

        /// Skip delimiter marking the end of the component list.
        Reader.Skip(sizeof(UInt32));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::SaveComponents(Ref<Writer> Writer, Entity Actor)
    {
        /// Iterate over each component attached to the entity.
        const auto OnIterateEntityComponents = [&](Entity Component)
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

            ConstPtr<Factory> Serializer = Second.IsValid() ? Second.Lookup<const Factory>() : nullptr;

            if (Serializer && (!First.IsValid() || First.Contains<Factory>()))
            {
                /// Write the name of the relation tag if valid, otherwise an empty string.
                Writer.WriteText(First.IsValid() ? First.GetName() : "");

                /// Write the name of the relation target or component.
                Writer.WriteText(Second.GetName());

                /// Serialize the component data into a temporary bundle.
                // TODO: Prevent heap Allocation.
                Base::Writer Bundle;
                Serializer->Write(Bundle, Actor.Lookup(Component));

                /// Write the serialized component bundle to the output stream.
                Writer.WriteBlock(Bundle.GetData());
            }
        };
        Actor.Iterate(OnIterateEntityComponents);

        /// Write delimiter marking the end of the component list.
        Writer.WriteUInt32(kDelimiter);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultComponentsAndSystems()
    {
#ifdef   AURORA_PROFILE_MODE
        /// Import built-in ECS statistics module.
        mWorld.import<flecs::stats>();

        /// Enable REST service for remote ECS inspection.
        mWorld.set<flecs::Rest>({});
#endif // AURORA_PROFILE_MODE

        /// Create observer to release archetype IDs when prefab entities are removed.
        CreateObserver<>("_Archetypes::OnDelete").with(flecs::Prefab).event(flecs::OnRemove).each([this](Entity Actor)
        {
            mArchetypes.Free(Actor.GetID() - kMinRangeArchetypes);
        });

        /// Registers the engine's default set of components with the ECS.
        RegisterComponent<Transient>("Transient");
        RegisterComponent<Factory>("Factory");
        RegisterComponent<Time>("Time").SetTrait(Trait::Singleton);
    }
}