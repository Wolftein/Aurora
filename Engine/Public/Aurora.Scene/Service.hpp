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
#include "Query.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    /// \brief Provides high-level management of the scene subsystem.
    class Service final : public AbstractService<Service>
    {
    public:

        /// \brief Constructs the scene service and registers it with the system host.
        ///
        /// \param Host The system context that owns and manages this service.
        explicit Service(Ref<Host> Host);

        /// \brief Advances the scene once per frame.
        ///
        /// \param Time The time step data for the current frame.
        void OnTick(ConstRef<Time> Time) override;

        /// \brief Defers execution of a callback until the end of the current frame.
        ///
        /// \param Callback   The function to execute deferred.
        /// \param Parameters The arguments to forward to the callback.
        template<typename Function, typename ...Arguments>
        AURORA_INLINE void Schedule(AnyRef<Function> Callback, AnyRef<Arguments>... Parameters)
        {
            mWorld.defer_begin();
            {
                Callback(Forward<Arguments>(Parameters)...);
            }
            mWorld.defer_end();
        }

        /// \brief Registers a component type.
        ///
        /// \tparam Target The component type.
        /// \param ID      Specifies the identifier of the component (defaults to the type’s symbol name).
        /// \return The component object associated with the type.
        template<typename Target>
        AURORA_INLINE Component<Target> RegisterComponent(ConstText ID = flecs::_::symbol_name<Target>())
        {
            return Component<Target>(mWorld.template component<Target>(ID.data()));
        }

        /// \brief Retrieves an existing component type.
        ///
        /// \tparam Target The component type.
        /// \return The component object associated with the type.
        template<typename Target>
        AURORA_INLINE Component<Target> GetComponent()
        {
            return Component<Target>(mWorld.template component<Target>());
        }

        /// \brief Creates a new entity.
        ///
        /// \tparam Archetype If `true`, creates an archetype entity; otherwise, creates a regular entity.
        /// \return The newly created entity object.
        template<Bool Archetype>
        AURORA_INLINE Entity CreateEntity()
        {
            return Allocate<Archetype>();
        }

        /// \brief Retrieves an entity by an identifier.
        ///
        /// \param ID Specifies the entity identifier.
        /// \return The entity object if valid and alive, otherwise an empty entity.
        AURORA_INLINE Entity GetEntity(UInt64 ID)
        {
            const Entity::Handle Handle = mWorld.entity(ID);
            return Handle.is_valid() && mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        /// \brief Retrieves an entity by name.
        ///
        /// \param Name Specifies the entity name.
        /// \return The entity object if valid and alive, otherwise an empty entity.
        AURORA_INLINE Entity GetEntity(ConstText Name)
        {
            const Entity::Handle Handle = mWorld.lookup(Name.data());
            return Handle.is_valid() && mWorld.is_alive(Handle) ? Entity(Handle) : Entity();
        }

        /// \brief Retrieves a relation entity.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        /// \return The entity object associated with the relation.
        template<typename Tag, typename Target>
        AURORA_INLINE Entity GetEntity()
        {
            const Entity::Handle Handle = mWorld.entity(mWorld.template pair<Tag, Target>());
            return Entity(Handle);
        }

        /// \brief Creates a query for entities with the specified components.
        ///
        /// \param Name The query name (can be empty).
        /// \return A query builder object.
        template<typename ...Components>
        AURORA_INLINE auto CreateQuery(ConstText Name = "")
        {
            return mWorld.template query_builder<Components...>(Name.data());
        }

        /// \brief Iterates over all archetype entities.
        ///
        /// \param Callback The function invoked once per archetype entity.
        template<typename Function>
        AURORA_INLINE void QueryArchetypes(AnyRef<Function> Callback)
        {
            CreateQuery<>().with(EcsPrefab).each(Callback);
        }

        /// \brief Iterates over all entities with a given tag.
        ///
        /// \param Callback The function invoked once per matching entity.
        template<typename Tag, typename Function>
        AURORA_INLINE void QueryTag(AnyRef<Function> Callback)
        {
            CreateQuery<>().with<Tag>().each(Callback);
        }

        /// \brief Creates an observer for the specified components.
        ///
        /// \param Name The observer name (can be empty).
        /// \return An observer object.
        template<typename ...Components>
        AURORA_INLINE auto CreateObserver(ConstText Name = "")
        {
            return mWorld.template observer<Components...>(Name.data());
        }

        /// \brief Creates a system for the specified components.
        ///
        /// \param Name The system name (can be empty).
        /// \return A system object.
        template<typename ...Components>
        AURORA_INLINE auto CreateSystem(ConstText Name = "")
        {
            return mWorld.template system<Components...>(Name.data());
        }

        /// \brief Loads all archetypes.
        ///
        /// \param Reader The stream containing serialized archetypes.
        void LoadArchetypes(Ref<Reader> Reader);

        /// \brief Saves all archetypes.
        ///
        /// \param Writer The stream that receives serialized archetypes.
        void SaveArchetypes(Ref<Writer> Writer);

        /// \brief Loads a single entity.
        ///
        /// \param Reader The stream containing the serialized entity.
        /// \return An entity created from the stream.
        Entity LoadEntity(Ref<Reader> Reader);

        /// \brief Loads an entity hierarchy.
        ///
        /// \param Reader The stream containing the serialized hierarchy.
        /// \return An entity created from the stream.
        Entity LoadEntityHierarchy(Ref<Reader> Reader);

        /// \brief Saves a single entity.
        ///
        /// \param Writer The stream that receives the serialized entity.
        /// \param Actor  The entity to serialize.
        void SaveEntity(Ref<Writer> Writer, Entity Actor);

        /// \brief Saves an entity hierarchy.
        ///
        /// \param Writer The stream that receives the serialized hierarchy.
        /// \param Actor  The entity to serialize.
        void SaveEntityHierarchy(Ref<Writer> Writer, Entity Actor);

        /// \brief Loads components of an entity.
        ///
        /// \param Reader The stream containing the serialized components.
        /// \param Actor  The entity to serialize.
        void LoadComponents(Ref<Reader> Reader, Entity Actor);

        /// \brief Saves components of an entity.
        ///
        /// \param Writer The stream that receives the serialized components.
        /// \param Actor  The entity to serialize.
        void SaveComponents(Ref<Writer> Writer, Entity Actor);

    private:

        /// \brief Allocates a new entity.
        ///
        /// \tparam Archetype If `true`, creates an archetype entity; otherwise, creates a regular entity.
        /// \param ID         Specifies the entity identifier. If `0`, a new unique ID is generated.
        /// \return The newly allocated entity object.
        template<Bool Archetype>
        AURORA_INLINE Entity Allocate(UInt64 ID = 0)
        {
            flecs::entity Actor;

            if constexpr(Archetype)
            {
                Actor = mWorld.entity(ID ? ID : kMinRangeArchetypes + mArchetypes.Allocate());
            }
            else
            {
                Actor = (ID ? mWorld.entity(ID) : mWorld.entity());
            }

            mWorld.make_alive(Actor);

            if constexpr(Archetype)
            {
                Actor.add(flecs::Prefab);
            }
            else
            {
                Actor.add(flecs::Final);
            }
            return Actor;
        }

        /// \brief Registers the engine’s built-in components and systems.
        void RegisterDefaultComponentsAndSystems();

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world                mWorld;
        Handle<kMaxCountArchetypes> mArchetypes;
    };
}