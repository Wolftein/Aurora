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

#include "Entity.hpp"
#include "Factory.hpp"
#include "Query.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class Service final : public AbstractSubsystem<Service>, public Tickable
    {
    public:

        // -=(Undocumented)=-
        explicit Service(Ref<Context> Context);

        // \see Tickable::OnTick
        void OnTick(ConstRef<Time> Time) override;

        // -=(Undocumented)=-
        Entity Create();

        // -=(Undocumented)=-
        Entity Find(UInt64 ID);

        // -=(Undocumented)=-
        void Load(Ref<Reader> Reader, Entity Actor);

        // -=(Undocumented)=-
        void Save(Ref<Writer> Writer, Entity Actor);

        // -=(Undocumented)=-
        template<typename Type, typename Dependency = void, Bool Serializable = true, Bool Sparse = false>
        auto Register()
        {
            flecs::entity Component = mWorld.component<Type>();

            if constexpr (Serializable)
            {
                Component.template set<Factory>(Factory::Create<Type>());
            }

            if constexpr (Sparse)
            {
                Component.add(flecs::Sparse);
            }

            if constexpr (! std::is_void_v<Dependency>)
            {
                Component.add(flecs::With, mWorld.component<Dependency>());
            }
            return Component;
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Observer()
        {
            return mWorld.template observer<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto Query()
        {
            return mWorld.template query_builder<Components...>();
        }

        // -=(Undocumented)=-
        template<typename ...Components>
        auto System()
        {
            return mWorld.template system<Components...>();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        flecs::world mWorld;
    };
}