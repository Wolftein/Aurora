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
#include "Factory.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    /// \brief Represents a data container that stores properties or state for an entity.
    template<typename Class>
    class Component final
    {
    public:

        /// \brief Underlying flecs component handle type.
        using Handle = flecs::component<Class>;

    public:

        /// \brief Constructs a null component.
        AURORA_INLINE Component()
            : mHandle { Handle::null() }
        {
        }

        /// \brief Constructs an component.
        ///
        /// \param Handle The component handle.
        AURORA_INLINE Component(Handle Handle)
            : mHandle { Handle }
        {
        }

        /// \brief Returns the raw component ID.
        ///
        /// \return A 64-bit unsigned integer representing the component ID.
        AURORA_INLINE UInt64 GetID() const
        {
            return mHandle.raw_id();
        }

        /// \brief Returns the underlying component handle.
        ///
        /// \return The component handle.
        AURORA_INLINE Handle GetHandle() const
        {
            return mHandle;
        }

        /// \brief Destroys the component and releases all its memory.
        AURORA_INLINE void Destruct()
        {
            mHandle.destruct();
        }

        /// \brief Checks if the component is valid.
        ///
        /// \return `true` if valid, `false` otherwise.
        AURORA_INLINE Bool IsValid() const
        {
            return mHandle.is_valid();
        }

        /// \brief Checks if the component is a pair relation.
        ///
        /// \return `true` if this component represents a pair relation, `false` otherwise.
        AURORA_INLINE Bool IsPair() const
        {
            return mHandle.is_pair();
        }

        /// \brief Checks if the component is a tag.
        ///
        /// \return `true` if this component is a tag, `false` otherwise.
        AURORA_INLINE Bool IsTag() const
        {
            return ecs_id_is_tag(mHandle.world(), mHandle.id());
        }

        /// \brief Attaches a tag component to this component.
        ///
        /// \tparam Tag The tag to attach.
        template<typename Tag>
        AURORA_INLINE void Attach()
        {
            mHandle.template add<Tag>();
        }

        /// \brief Attaches a component identified by an component.
        ///
        /// \param Component The component that represents the component type.
        AURORA_INLINE void Attach(Component Component)
        {
            mHandle.add(Component.GetID());
        }

        /// \brief Attaches a relation between a tag component and an component.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        template<typename Tag>
        AURORA_INLINE void Attach(Component Target)
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
        AURORA_INLINE void Attach(Component Tag, Component Component)
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
            mHandle.template set_second<Component>(Move(Data));
        }

        /// \brief Ensures a component is present.
        ///
        /// \param Component The component that represents the component type.
        /// \return A pointer to the component’s raw storage.
        AURORA_INLINE Ptr<void> Ensure(Component Component)
        {
            return mHandle.ensure(Component.GetID());
        }

        /// \brief Ensures a relation between a tag and an component is present.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        /// \return A pointer to the component’s raw storage.
        template<typename Tag>
        AURORA_INLINE Ptr<void> Ensure(Component Target)
        {
            return mHandle.template ensure<Tag>(Target.GetID());
        }

        /// \brief Ensures a relation between two entities is present.
        ///
        /// \param Tag       The relation's tag component.
        /// \param Component The relation's target component.
        /// \return A pointer to the component’s raw storage.
        AURORA_INLINE Ptr<void> Ensure(Component Tag, Component Component)
        {
            return mHandle.ensure(Tag.GetID(), Component.GetID());
        }

        /// \brief Removes a component of type \p Component from this component.
        template<typename Component>
        AURORA_INLINE void Detach()
        {
            mHandle.template remove<Component>();
        }

        /// \brief Removes a component from this component.
        ///
        /// \param Component The component representing the component type.
        AURORA_INLINE void Detach(Component Component)
        {
            mHandle.remove(Component.GetID());
        }

        /// \brief Removes a relation component from this component.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Detach()
        {
            mHandle.template remove<Tag, Target>();
        }

        /// \brief Removes a relation component from this component.
        ///
        /// \tparam Tag   The relation's tag component.
        /// \param Target The relation's target component.
        template<typename Tag>
        AURORA_INLINE void Detach(Component Target)
        {
            mHandle.template remove<Tag>(Target.GetID());
        }

        /// \brief Removes a relation component from this component.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Detach(Component Tag, Component Target)
        {
            mHandle.remove(Tag.GetID(), Target.GetID());
        }

        /// \brief Checks if this component has a component of type \p Component.
        ///
        /// \tparam Component The component type.
        /// \return `true` if the component is present, `false` otherwise.
        template<typename Component>
        AURORA_INLINE Bool Contains() const
        {
            return mHandle.template has<Component>();
        }

        /// \brief Checks if this component has a component.
        ///
        /// \param Component The component representing the component type.
        /// \return `true` if the component is present, `false` otherwise.
        AURORA_INLINE Bool Contains(Component Component) const
        {
            return mHandle.has(Component.GetID());
        }

        /// \brief Checks if this component has a relation between \p Tag and \p Target.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        /// \return `true` if the relation is present, `false` otherwise.
        template<typename Tag, typename Target>
        AURORA_INLINE Bool Contains() const
        {
            return mHandle.template has<Tag, Target>();
        }

        /// \brief Checks if this component has a relation between \p Tag and \p Target.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        /// \return `true` if the relation is present, `false` otherwise.
        AURORA_INLINE Bool Contains(Component Tag, Component Target) const
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

        /// \brief Lookup a component of type \p Target from this component.
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
        AURORA_INLINE void Notify()
        {
            mHandle.template modified<Component>();
        }

        /// \brief Marks a component as modified.
        ///
        /// \param Component The component representing the component type.
        AURORA_INLINE void Notify(Component Component)
        {
            mHandle.modified(Component.GetID());
        }

        /// \brief Marks a relation between two entities as modified.
        ///
        /// \tparam Tag    The relation's tag component.
        /// \tparam Target The relation's target component.
        template<typename Tag, typename Target>
        AURORA_INLINE void Notify()
        {
            mHandle.template modified<Tag, Target>();
        }

        /// \brief Marks a relation between two entities as modified.
        ///
        /// \param Tag    The relation's tag component.
        /// \param Target The relation's target component.
        AURORA_INLINE void Notify(Component Tag, Component Target)
        {
            mHandle.modified(Tag.GetID(), Target.GetID());
        }

        /// \brief Enables a component of type \p Component on this component.
        template<typename Component>
        AURORA_INLINE void Enable()
        {
            mHandle.template enable<Component>();
        }

        /// \brief Enables a component.
        ///
        /// \param Component The component representing the component type.
        AURORA_INLINE void Enable(Component Component)
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
        AURORA_INLINE void Enable(Component Tag, Component Target)
        {
            mHandle.enable(Tag.GetID(), Target.GetID());
        }

        /// \brief Disables a component of type \p Component on this component.
        template<typename Component>
        AURORA_INLINE void Disable()
        {
            mHandle.template disable<Component>();
        }

        /// \brief Disables a component.
        ///
        /// \param Component The component representing the component type.
        AURORA_INLINE void Disable(Component Component)
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
        AURORA_INLINE void Disable(Component Tag, Component Target)
        {
            mHandle.disable(Tag.GetID(), Target.GetID());
        }

        /// \brief Declares that entities created from this scope will include the specified component type by default.
        ///
        /// \tparam Type Specifies the component type to associate.
        template<typename Type>
        AURORA_INLINE void With()
        {
            mHandle.add(EcsWith, mHandle.world().template component<Type>());
        }

        /// \brief Declares that entities created from this scope will include the specified component by default.
        ///
        /// \param Component Specifies the component handle to associate.
        AURORA_INLINE void With(Component Component)
        {
            mHandle.add(EcsWith, Component.GetHandle());
        }

        /// \brief Sets this component’s parent to a type.
        ///
        /// \tparam Type The parent type.
        template<typename Type>
        AURORA_INLINE void SetParent()
        {
            mHandle.template child_of<Type>();
        }

        /// \brief Sets this component’s parent to another component.
        ///
        /// \param Parent The parent component.
        AURORA_INLINE void SetParent(Component Parent)
        {
            mHandle.child_of(Parent.GetHandle());
        }

        /// \brief Gets this component’s parent.
        ///
        /// \return The parent component, or null if none.
        AURORA_INLINE Component GetParent() const
        {
            return Component(mHandle.parent());
        }

        /// \brief Sets this component’s archetype to a type.
        ///
        /// \tparam Type The archetype type.
        template<typename Type>
        AURORA_INLINE void SetArchetype()
        {
            mHandle.template is_a<Type>();
        }

        /// \brief Sets this component’s archetype to another component.
        ///
        /// \param Archetype The archetype component.
        AURORA_INLINE void SetArchetype(Component Archetype)
        {
            mHandle.is_a(Archetype.GetHandle());
        }

        /// \brief Gets this component’s archetype.
        ///
        /// \return The archetype component.
        AURORA_INLINE Component GetArchetype() const
        {
            return Component(mHandle.target(flecs::IsA));
        }

        /// \brief Applies multiple behavioral traits to the component definition.
        ///
        /// \param Traits The trait flags to apply to the component.
        template<typename... Arguments>
        AURORA_INLINE void SetTrait(Arguments... Traits)
        {
            (InsertTrait(Traits), ...);
        }

        /// \brief Removes multiple behavioral traits from the component definition.
        ///
        /// \param Traits The trait flags to remove to the component.
        template<typename... Arguments>
        AURORA_INLINE void UnsetTrait(Arguments... Traits)
        {
            (RemoveTrait(Traits), ...);
        }

        /// \brief Iterates over this component’s children.
        ///
        /// \param Callback The function to invoke for each child.
        template<typename Function>
        AURORA_INLINE void Children(AnyRef<Function> Callback) const
        {
            mHandle.children(Callback);
        }

        /// \brief Iterates over this component’s components.
        ///
        /// \param Callback The function to invoke for each component.
        template<typename Function>
        AURORA_INLINE void Iterate(AnyRef<Function> Callback) const
        {
            mHandle.each(Callback);
        }

        /// \brief Sets the name of this component.
        ///
        /// \param Name The new name.
        AURORA_INLINE void SetName(ConstText Name)
        {
            mHandle.set_name(Name.data());
        }

        /// \brief Gets the name of this component.
        ///
        /// \return The component’s name.
        AURORA_INLINE ConstText GetName() const
        {
            const flecs::string_view Name = mHandle.name();
            return ConstText(Name.c_str(), Name.size());
        }

        /// \brief Gets the first element of a pair relation.
        ///
        /// \return The first component in the pair.
        AURORA_INLINE Component GetFirst() const
        {
            return mHandle.first();
        }

        /// \brief Gets the second element of a pair relation.
        ///
        /// \return The second component in the pair.
        AURORA_INLINE Component GetSecond() const
        {
            return mHandle.second();
        }

        /// \brief Checks if this component is equal to another component.
        ///
        /// \param Other The component to compare to.
        /// \return `true` if the entities are the same, `false` otherwise.
        AURORA_INLINE Bool operator==(ConstRef<Component> Other) const
        {
            return GetHandle() == Other.GetHandle();
        }

        /// \brief Checks if this component is not equal to another component.
        ///
        /// \param Other The component to compare to.
        /// \return `true` if the entities are not the same, `false` otherwise.
        AURORA_INLINE Bool operator!=(ConstRef<Component> Other) const
        {
            return !(*this == Other);
        }

    private:

        /// \brief Inserts a specific behavioral trait to the component definition.
        ///
        /// \param Trait The behavioral trait to apply to the component.
        AURORA_INLINE void InsertTrait(Trait Trait)
        {
            switch (Trait)
            {
                case Trait::Serializable:
                    mHandle.template set<Factory>(Factory::Create<Class>());
                    break;
                case Trait::Inheritable:
                    mHandle.add(flecs::OnInstantiate, flecs::Inherit);
                    break;
                case Trait::Toggleable:
                    mHandle.add(flecs::CanToggle);
                    break;
                case Trait::Sparse:
                    mHandle.add(flecs::Sparse);
                    break;
                case Trait::Associative:
                    mHandle.add(flecs::PairIsTag);
                    break;
                case Trait::Singleton:
                    mHandle.add(flecs::Singleton);
                    break;
                case Trait::Final:
                    mHandle.add(flecs::Final);
                    break;
            }
        }

        /// \brief Removes a specific behavioral trait from the component definition.
        ///
        /// \param Trait The behavioral trait to remove from the component.
        AURORA_INLINE void RemoveTrait(Trait Trait)
        {
            switch (Trait)
            {
                case Trait::Serializable:
                    Detach<Factory>();
                    break;
                case Trait::Inheritable:
                    Detach(flecs::OnInstantiate, flecs::Inherit);
                    break;
                case Trait::Toggleable:
                    Detach(flecs::CanToggle);
                    break;
                case Trait::Sparse:
                    Detach(flecs::Sparse);
                    break;
                case Trait::Associative:
                    Detach(flecs::PairIsTag);
                    break;
                case Trait::Singleton:
                    Detach(flecs::Singleton);
                    break;
                case Trait::Final:
                    Detach(flecs::Final);
                    break;
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}