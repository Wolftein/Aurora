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

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    /// \brief Represents a unique game object that can hold components and define behavior.
    class Entity final
    {
    public:

        /// \brief Underlying flecs entity handle type.
        using Handle = flecs::entity;

    public:

        /// \brief Constructs a null entity.
        AURORA_INLINE Entity()
            : mHandle { Handle::null() }
        {
        }

        /// \brief Constructs an entity.
        ///
        /// \param Handle The entity handle.
        AURORA_INLINE Entity(Handle Handle)
            : mHandle { Handle }
        {
        }

        /// \brief Constructs an entity.
        ///
        /// \param Id The entity unique identifier.
        AURORA_INLINE Entity(flecs::id Id)
            : mHandle { Id.world(), Id.raw_id() }
        {
        }

        /// \brief Returns the raw entity ID.
        ///
        /// \return A 64-bit unsigned integer representing the entity ID.
        AURORA_INLINE UInt64 GetID() const
        {
            return mHandle.raw_id();
        }

        /// \brief Returns the underlying entity handle.
        ///
        /// \return The entity handle.
        AURORA_INLINE Handle GetHandle() const
        {
            return mHandle;
        }

        /// \brief Destroys the entity and releases all its memory.
        AURORA_INLINE void Destruct()
        {
            mHandle.destruct();
        }

        /// \brief Checks if the entity is valid.
        ///
        /// \return `true` if valid, `false` otherwise.
        AURORA_INLINE Bool IsValid() const
        {
            return mHandle.is_valid();
        }

        /// \brief Checks if the entity is an archetype.
        ///
        /// \return `true` if this entity is an archetype, `false` otherwise.
        AURORA_INLINE Bool IsArchetype() const
        {
            return mHandle.has(flecs::Prefab);
        }

        /// \brief Checks if the entity is a pair relation.
        ///
        /// \return `true` if this entity represents a pair relation, `false` otherwise.
        AURORA_INLINE Bool IsPair() const
        {
            return mHandle.is_pair();
        }

        /// \brief Checks if the entity is a tag.
        ///
        /// \return `true` if this entity is a tag, `false` otherwise.
        AURORA_INLINE Bool IsTag() const
        {
            return ecs_id_is_tag(mHandle.world(), mHandle.id());
        }

        /// \brief Attaches a tag component to this entity.
        ///
        /// \tparam Tag The tag to attach.
        template<typename Tag>
        AURORA_INLINE void Attach()
        {
            mHandle.template add<Tag>();
        }

        /// \brief Attaches a component identified by an entity.
        ///
        /// \param Component The entity that represents the component type.
        AURORA_INLINE void Attach(Entity Component)
        {
            mHandle.add(Component.GetID());
        }

        /// \brief Attaches a relation between a tag component and an entity.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        template<typename Tag>
        AURORA_INLINE void Attach(Entity Target)
        {
            mHandle.template add<Tag>(Target.GetID());
        }

        /// \brief Attaches a relation between two tag components.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Attach()
        {
            mHandle.template add<Tag, Target>();
        }

        /// \brief Attaches a relation between two entities.
        ///
        /// \param Tag       The relation's tag component.
        /// \param Component The relation's target component.
        AURORA_INLINE void Attach(Entity Tag, Entity Component)
        {
            mHandle.add(Tag.GetID(), Component.GetID());
        }

        /// \brief Attaches or modifies a component of type \p Component.
        ///
        /// \param Data The component's data.
        template<typename Component>
        AURORA_INLINE void Attach(AnyRef<Component> Data)
        {
            mHandle.template set<Component>(Move(Data));
        }

        /// \brief Attaches a relation between a tag component and a component of type \p Component.
        ///
        /// \tparam Tag The relation's tag component.
        /// \param Data The relation's target component.
        template<typename Tag, typename Component>
        AURORA_INLINE void Attach(AnyRef<Component> Data)
        {
            mHandle.template set_second<Tag>(Move(Data));
        }

        /// \brief Ensures a component is present.
        ///
        /// \param Component The entity that represents the component type.
        /// \return A pointer to the component’s raw storage.
        AURORA_INLINE Ptr<void> Ensure(Entity Component)
        {
            return mHandle.ensure(Component.GetID());
        }

        /// \brief Ensures a relation between a tag and an entity is present.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        /// \return A pointer to the component’s raw storage.
        template<typename Tag>
        AURORA_INLINE Ptr<void> Ensure(Entity Target)
        {
            return mHandle.template ensure<Tag>(Target.GetID());
        }

        /// \brief Ensures a relation between two entities is present.
        ///
        /// \param Tag       The relation's tag component.
        /// \param Component The relation's target component.
        /// \return A pointer to the component’s raw storage.
        AURORA_INLINE Ptr<void> Ensure(Entity Tag, Entity Component)
        {
            return mHandle.ensure(Tag.GetID(), Component.GetID());
        }

        /// \brief Removes a component of type \p Component from this entity.
        template<typename Component>
        AURORA_INLINE void Detach()
        {
            mHandle.template remove<Component>();
        }

        /// \brief Removes a component from this entity.
        ///
        /// \param Component The entity representing the component type.
        AURORA_INLINE void Detach(Entity Component)
        {
            mHandle.remove(Component.GetID());
        }

        /// \brief Removes a relation component from this entity.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Detach()
        {
            mHandle.template remove<Tag, Target>();
        }

        /// \brief Removes a relation component from this entity.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        template<typename Tag>
        AURORA_INLINE void Detach(Entity Target)
        {
            mHandle.template remove<Tag>(Target.GetID());
        }

        /// \brief Removes a relation component from this entity.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Detach(Entity Tag, Entity Target)
        {
            mHandle.remove(Tag.GetID(), Target.GetID());
        }

        /// \brief Checks if this entity has a component of type \p Component.
        ///
        /// \tparam Component The component type.
        /// \return `true` if the component is present, `false` otherwise.
        template<typename Component>
        AURORA_INLINE Bool Contains() const
        {
            return mHandle.template has<Component>();
        }

        /// \brief Checks if this entity has a component.
        ///
        /// \param Component The entity representing the component type.
        /// \return `true` if the component is present, `false` otherwise.
        AURORA_INLINE Bool Contains(Entity Component) const
        {
            return mHandle.has(Component.GetID());
        }

        /// \brief Checks if this entity has a relation between \p Tag and \p Target.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        /// \return `true` if the relation is present, `false` otherwise.
        template<typename Tag, typename Target>
        AURORA_INLINE Bool Contains() const
        {
            return mHandle.template has<Tag, Target>();
        }

        /// \brief Checks if this entity has a relation between \p Tag and \p Target.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        /// \return `true` if the relation is present, `false` otherwise.
        AURORA_INLINE Bool Contains(Entity Tag, Entity Target) const
        {
            return mHandle.has(Tag.GetID(), Target.GetID());
        }

        /// \brief Lookup a component of type \p Component from this entity.
        ///
        /// \return Pointer to the component, or `nullptr` if not present.
        template<typename Component>
        AURORA_INLINE auto Lookup() const
        {
            if constexpr (std::is_const_v<Component>)
            {
                return mHandle.template try_get<Component>();
            }
            else
            {
                return mHandle.template try_get_mut<Component>();
            }
        }

        /// \brief Lookup a component from this entity.
        ///
        /// \param Component The entity representing the component type.
        /// \return Pointer to the raw component, or `nullptr` if not present.
        AURORA_INLINE auto Lookup(Entity Component) const
        {
            return mHandle.try_get_mut(Component.GetID());
        }

        /// \brief Lookup a component from this entity.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        /// \return Pointer to the raw component, or `nullptr` if not present.
        template<typename Tag>
        AURORA_INLINE auto Lookup(Entity Target) const
        {
            return mHandle.template try_get_mut<Tag>(Target.GetID());
        }

        /// \brief Lookup a component from this entity.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        /// \return Pointer to the raw component, or `nullptr` if not present.
        AURORA_INLINE auto Lookup(Entity Tag, Entity Target) const
        {
            return mHandle.try_get_mut(Tag.GetID(), Target.GetID());
        }

        /// \brief Lookup a component from this entity.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        /// \return Pointer to the component, or `nullptr` if not present.
        template<typename Tag, typename Target>
        AURORA_INLINE auto Lookup() const
        {
            if constexpr (std::is_const_v<Target>)
            {
                return mHandle.template try_get<Tag, Target>();
            }
            else
            {
                return mHandle.template try_get_mut<Tag, Target>();
            }
        }

        /// \brief Marks a component of type \p Component as modified.
        template<typename Component>
        AURORA_INLINE void Notify() const
        {
            mHandle.template modified<Component>();
        }

        /// \brief Marks a component as modified.
        ///
        /// \param Component The entity representing the component type.
        AURORA_INLINE void Notify(Entity Component) const
        {
            mHandle.modified(Component.GetID());
        }

        /// \brief Marks a relation between two entities as modified.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Notify() const
        {
            mHandle.template modified<Tag, Target>();
        }

        /// \brief Marks a relation between two entities as modified.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Notify(Entity Tag, Entity Target) const
        {
            mHandle.modified(Tag.GetID(), Target.GetID());
        }

        /// \brief Enables a component of type \p Component on this entity.
        template<typename Component>
        AURORA_INLINE void Enable()
        {
            mHandle.template enable<Component>();
        }

        /// \brief Enables a component.
        ///
        /// \param Component The entity representing the component type.
        AURORA_INLINE void Enable(Entity Component)
        {
            mHandle.enable(Component.GetID());
        }

        /// \brief Enables a relation between two entities as modified.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Enable()
        {
            mHandle.template enable<Tag, Target>();
        }

        /// \brief Enables a relation component.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Enable(Entity Tag, Entity Target)
        {
            mHandle.enable(Tag.GetID(), Target.GetID());
        }

        /// \brief Disables a component of type \p Component on this entity.
        template<typename Component>
        AURORA_INLINE void Disable()
        {
            mHandle.template disable<Component>();
        }

        /// \brief Disables a component.
        ///
        /// \param Component The entity representing the component type.
        AURORA_INLINE void Disable(Entity Component)
        {
            mHandle.disable(Component.GetID());
        }

        /// \brief Disables a relation between two entities as modified.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Disable()
        {
            mHandle.template disable<Tag, Target>();
        }

        /// \brief Disables a relation component.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Disable(Entity Tag, Entity Target)
        {
            mHandle.disable(Tag.GetID(), Target.GetID());
        }

        /// \brief Sets this entity’s parent to a type.
        ///
        /// \tparam Type The parent type.
        template<typename Type>
        AURORA_INLINE void SetParent()
        {
            mHandle.template child_of<Type>();
        }

        /// \brief Sets this entity’s parent to another entity.
        ///
        /// \param Parent The parent entity.
        AURORA_INLINE void SetParent(Entity Parent)
        {
            mHandle.child_of(Parent.GetHandle());
        }

        /// \brief Gets this entity’s parent.
        ///
        /// \return The parent entity, or null if none.
        AURORA_INLINE Entity GetParent() const
        {
            return Entity(mHandle.parent());
        }

        /// \brief Sets this entity’s archetype to a type.
        ///
        /// \tparam Type The archetype type.
        template<typename Type>
        AURORA_INLINE void SetArchetype()
        {
            mHandle.template is_a<Type>();
        }

        /// \brief Sets this entity’s archetype to another entity.
        ///
        /// \param Archetype The archetype entity.
        AURORA_INLINE void SetArchetype(Entity Archetype)
        {
            mHandle.is_a(Archetype.GetHandle());
        }

        /// \brief Gets this entity’s archetype.
        ///
        /// \return The archetype entity.
        AURORA_INLINE Entity GetArchetype() const
        {
            return Entity(mHandle.target(flecs::IsA));
        }

        /// \brief Iterates over this entity’s children.
        ///
        /// \param Callback The function to invoke for each child.
        template<typename Function>
        AURORA_INLINE void Children(AnyRef<Function> Callback) const
        {
            mHandle.children(Callback);
        }

        /// \brief Iterates over this entity’s components.
        ///
        /// \param Callback The function to invoke for each component.
        template<typename Function>
        AURORA_INLINE void Iterate(AnyRef<Function> Callback) const
        {
            mHandle.each(Callback);
        }

        /// \brief Sets the name of this entity.
        ///
        /// \param Name The new name.
        AURORA_INLINE void SetName(ConstText Name)
        {
            mHandle.set_name(Name.data());
        }

        /// \brief Gets the name of this entity.
        ///
        /// \return The entity’s name.
        AURORA_INLINE ConstText GetName() const
        {
            const flecs::string_view Name = mHandle.name();
            return ConstText(Name.c_str(), Name.size());
        }

        /// \brief Sets the display name of this entity.
        ///
        /// \param Name The new display name.
        AURORA_INLINE void SetDisplayName(ConstText Name)
        {
            mHandle.set_doc_name(Name.data());
        }

        /// \brief Gets the display name of this entity.
        ///
        /// \return The entity’s display name.
        AURORA_INLINE ConstText GetDisplayName() const
        {
            const ConstPtr<Char> Name = mHandle.doc_name();
            return ConstText(Name ? Name : "");
        }

        /// \brief Gets the first element of a pair relation.
        ///
        /// \return The first entity in the pair.
        AURORA_INLINE Entity GetFirst() const
        {
            return mHandle.first();
        }

        /// \brief Gets the second element of a pair relation.
        ///
        /// \return The second entity in the pair.
        AURORA_INLINE Entity GetSecond() const
        {
            return mHandle.second();
        }

        /// \brief Checks if this entity is equal to another entity.
        ///
        /// \param Other The entity to compare to.
        /// \return `true` if the entities are the same, `false` otherwise.
        AURORA_INLINE Bool operator==(ConstRef<Entity> Other) const
        {
            return GetHandle() == Other.GetHandle();
        }

        /// \brief Checks if this entity is not equal to another entity.
        ///
        /// \param Other The entity to compare to.
        /// \return `true` if the entities are not the same, `false` otherwise.
        AURORA_INLINE Bool operator!=(ConstRef<Entity> Other) const
        {
            return !(*this == Other);
        }

    public:

        /// \brief Conditionally enables or disables a component on the specified entity.
        ///
        /// \tparam Component The type of component to toggle.
        /// \tparam Enable    If `true`, the component will be enabled. If `false`, the component will be disabled.
        /// \param Actor The target entity on which to toggle the component state.
        template<typename Component, Bool Enable>
        static void ToggleComponent(Entity Actor)
        {
            if constexpr(Enable)
            {
                Actor.template Enable<Component>();
            }
            else
            {
                Actor.template Disable<Component>();
            }
        }

        /// \brief Recursively enables or disables a component on the specified entity and all its children.
        ///
        /// \tparam Component The type of component to toggle throughout the hierarchy.
        /// \tparam Enable    If `true`, the component will be enabled. If `false`, the component will be disabled.
        /// \param Actor The root entity of the hierarchy on which to start the operation.
        template<typename Component, Bool Enable>
        static void ToggleComponentInHierarchy(Entity Actor)
        {
            if constexpr(Enable)
            {
                Actor.template Enable<Component>();
            }
            else
            {
                Actor.template Disable<Component>();
            }
            Actor.Children(&ToggleComponentInHierarchy<Component, Enable>);
        }

        /// \brief Recursively ensures a component is present and enabled on the specified entity and all its children.
        ///
        /// \tparam Component The type of component to add or enable throughout the hierarchy.
        /// \param Actor      The root entity of the hierarchy on which to start the operation.
        template<typename Component>
        static void AddOrEnableComponentInHierarchy(Entity Actor)
        {
            if (Actor.Contains<Component>())
            {
                Actor.Enable<Component>();
            }
            else
            {
                Actor.Attach<Component>();
            }
            Actor.Children(&AddOrEnableComponentInHierarchy<Component>);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::entity mHandle;
    };
}