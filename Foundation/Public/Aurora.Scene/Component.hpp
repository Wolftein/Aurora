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
    // -=(Undocumented)=-
    template<typename Base>
    class Component final
    {
    public:

        // -=(Undocumented)=-
        using Handle = flecs::component<Base>;

    public:

        // -=(Undocumented)=-
        Component()
            : mHandle { Handle::null() }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        Component(Type Handle)
            : mHandle { Handle }
        {
        }

        // -=(Undocumented)=-
        UInt64 GetID() const
        {
            return mHandle.raw_id();
        }

        // -=(Undocumented)=-
        Handle GetHandle() const
        {
            return mHandle;
        }

        // -=(Undocumented)=-
        void Destruct()
        {
            mHandle.destruct();
        }

        // -=(Undocumented)=-
        Bool IsValid() const
        {
            return mHandle.is_valid();
        }

        // -=(Undocumented)=-
        Bool IsNull() const
        {
            return mHandle == 0;
        }

        // -=(Undocumented)=-
        Bool IsArchetype() const
        {
            return mHandle.has(flecs::Prefab);
        }

        // -=(Undocumented)=-
        Bool IsTag() const
        {
            return ecs_id_is_tag(mHandle.world(), mHandle.id());
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Component> Attach()
        {
            mHandle.template add<Type>();
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename First>
        Ref<Component> Attach(Component Component)
        {
            mHandle.template add<First>(Component.GetID());
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Component> Attach(Component First, Component Second)
        {
            mHandle.add(First.GetID(), Second.GetID());
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Component> Attach(Any<Type> Component)
        {
            if constexpr (std::is_empty_v<Type>)
            {
                mHandle.template add<Type>();
            }
            else
            {
                mHandle.template set<Type>(Move(Component));
            }
            return (* this);
        }

        // -=(Undocumented)=-
        template<UInt32 Trait>
        Ref<Component> Attach()
        {
            if constexpr (HasBit(Trait, CastEnum(Trait::Sparse)))
            {
                mHandle.add(flecs::OnInstantiate, flecs::Sparse);
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Serializable)))
            {
                mHandle.template set<Factory>(Factory::Create<Base>());
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Inheritable)))
            {
                mHandle.add(flecs::OnInstantiate, flecs::Inherit);
            }
            else
            {
                mHandle.add(flecs::Final);
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Toggleable)))
            {
                mHandle.add(flecs::CanToggle);
            }
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Ref<Component> With()
        {
            mHandle.add(EcsWith, mHandle.world().template component<Type>());
            return (* this);
        }

        // -=(Undocumented)=-
        Ref<Component> With(Component Component)
        {
            mHandle.add(EcsWith, Component.GetHandle());
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find() const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.template get<Type>();
            }
            else
            {
                return mHandle.template get_mut<Type>();
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find(Component Component) const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.template get<Type>(Component.GetID());
            }
            else
            {
                return mHandle.template get_mut<Type>(Component.GetID());
            }
        }

        // -=(Undocumented)=-
        auto Find(Component Component) const
        {
            return mHandle.get_mut(Component.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        auto Find(Component First, Component Second) const
        {
            if constexpr (std::is_const_v<Type>)
            {
                return mHandle.template get<Type>(First.GetID(), Second.GetID());
            }
            else
            {
                return mHandle.template get_mut<Type>(First.GetID(), Second.GetID());
            }
        }

        // -=(Undocumented)=-
        auto Find(Component First, Component Second) const
        {
            return mHandle.get_mut(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Contains() const
        {
            return mHandle.template has<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Contains(Type Value) const
        {
            return mHandle.has(Value);
        }

        // -=(Undocumented)=-
        Bool Contains(Component Component) const
        {
            return mHandle.has(Component.GetID());
        }

        // -=(Undocumented)=-
        Bool Contains(Component First, Component Second) const
        {
            return mHandle.has(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Detach()
        {
            mHandle.template remove<Type>();
        }

        // -=(Undocumented)=-
        void Detach(Component Component)
        {
            mHandle.remove(Component.GetID());
        }

        // -=(Undocumented)=-
        void Detach(Component First, Component Second)
        {
            mHandle.remove(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<UInt32 Trait>
        Ref<Component> Detach()
        {
            if constexpr (HasBit(Trait, CastEnum(Trait::Sparse)))
            {
                Detach(flecs::OnInstantiate, flecs::Sparse);
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Serializable)))
            {
                Detach<Factory>();
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Inheritable)))
            {
                Detach(flecs::OnInstantiate, flecs::Inherit);
            }
            else
            {
                Detach(flecs::Final);
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Toggleable)))
            {
                Detach(flecs::CanToggle);
            }

            if constexpr (HasBit(Trait, CastEnum(Trait::Associative)))
            {
                Detach(flecs::PairIsTag);
            }
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Notify()
        {
            mHandle.template modified<Type>();
        }

        // -=(Undocumented)=-
        void Notify(Component Component)
        {
            mHandle.modified(Component.GetID());
        }

        // -=(Undocumented)=-
        void Notify(Component First, Component Second)
        {
            mHandle.modified(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Enable()
        {
            mHandle.template enable<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Enable(Type Value)
        {
            mHandle.enable(Value);
        }

        // -=(Undocumented)=-
        void Enable(Component Component)
        {
            mHandle.enable(Component.GetID());
        }

        // -=(Undocumented)=-
        void Enable(Component First, Component Second)
        {
            mHandle.enable(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Disable()
        {
            mHandle.template disable<Type>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Disable(Type Value)
        {
            mHandle.disable(Value);
        }

        // -=(Undocumented)=-
        void Disable(Component Component)
        {
            mHandle.disable(Component.GetID());
        }

        // -=(Undocumented)=-
        void Disable(Component First, Component Second)
        {
            mHandle.disable(First.GetID(), Second.GetID());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetParent()
        {
            mHandle.template child_of<Type>();
        }

        // -=(Undocumented)=-
        void SetParent(Component Parent)
        {
            mHandle.child_of(Parent.GetHandle());
        }

        // -=(Undocumented)=-
        auto GetParent() const
        {
            return Component(mHandle.parent());
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetArchetype()
        {
            mHandle.template is_a<Type>();
        }

        // -=(Undocumented)=-
        void SetArchetype(Component Archetype)
        {
            mHandle.is_a(Archetype.GetHandle());
        }

        // -=(Undocumented)=-
        auto GetArchetype() const
        {
            return Component(mHandle.target(flecs::IsA));
        }

        // -=(Undocumented)=-
        template<typename Function>
        void Children(Any<Function> Callback) const
        {
            mHandle.children(Callback);
        }

        // -=(Undocumented)=-
        Ref<Component> SetName(CStr Name)
        {
            mHandle.set_name(Name.data());
            return (* this);
        }

        // -=(Undocumented)=-
        CStr GetName() const
        {
            const flecs::string_view Name = mHandle.name();
            return CStr(Name.c_str(), Name.size());
        }

        // -=(Undocumented)=-
        template<typename Function>
        Ref<Component> OnAdd(Any<Function> Callback)
        {
            mHandle.on_add(Callback);
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Function>
        Ref<Component> OnSet(Any<Function> Callback)
        {
            mHandle.on_set(Callback);
            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Function>
        Ref<Component> OnRemove(Any<Function> Callback)
        {
            mHandle.on_remove(Callback);
            return (* this);
        }

        // -=(Undocumented)=-
        Bool operator==(Ref<const Component> Other) const
        {
            return GetHandle() == Other.GetHandle();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}