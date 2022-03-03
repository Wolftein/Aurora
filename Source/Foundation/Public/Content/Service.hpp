// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Factory.hpp"
#include "Loader.hpp"
#include "Locator.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    class Service final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Service(Ref<Context> Context);

        // \see Subsystem::OnTick
        void OnTick() override;

        // -=(Undocumented)=-
        void AddLoader(CStr Extension, Ref<const SPtr<Loader>> Loader);

        // -=(Undocumented)=-
        void RemoveLoader(CStr Extension);

        // -=(Undocumented)=-
        void AddLocator(CStr Schema, Ref<const SPtr<Locator>> Locator);

        // -=(Undocumented)=-
        void RemoveLocator(CStr Schema);

        // -=(Undocumented)=-
        template<typename Type>
        void SetMemoryBudget(UInt Budget)
        {
            SetMemoryBudget(Type::RTTI_CATEGORY, Budget);
        }

        // -=(Undocumented)=-
        void SetMemoryBudget(UInt Category, UInt Budget);

        // -=(Undocumented)=-
        template<typename Type>
        UInt GetMemoryBudget()
        {
            return GetMemoryBudget(Type::RTTI_CATEGORY);
        }

        // -=(Undocumented)=-
        UInt GetMemoryBudget(UInt Category);

        // -=(Undocumented)=-
        template<typename Type>
        UInt GetMemoryUsage()
        {
            return GetMemoryUsage(Type::RTTI_CATEGORY);
        }

        // -=(Undocumented)=-
        UInt GetMemoryUsage(UInt Category);

        // -=(Undocumented)=-
        Reader Find(Ref<const Uri> Key);

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> Load(Ref<const Uri> Key, Bool Async)
        {
            Ref<const SPtr<Type>> Asset = GetFactory(Type::RTTI_CATEGORY)->template GetOrCreate<Type>(Key, true);

            Load(Asset, Async);

            return Asset;
        }

        // -=(Undocumented)=-
        void Load(Ref<const SPtr<Resource>> Asset, Bool Async);

        // -=(Undocumented)=-
        void Register(Ref<const SPtr<Resource>> Asset, Bool Async);

        // -=(Undocumented)=-
        void Reload(Ref<const SPtr<Resource>> Asset, Bool Async);

        // -=(Undocumented)=-
        void Unload(Ref<const SPtr<Resource>> Asset);

        // -=(Undocumented)=-
        template<typename Type>
        void Prune(Bool Force)
        {
            Prune(Type::RTTI_CATEGORY, Force);
        }

        // -=(Undocumented)=-
        void Prune(UInt Category, Bool Force);

    private:

        // -=(Undocumented)=-
        Ref<const SPtr<Factory>> GetFactory(UInt Category);

        // -=(Undocumented)=-
        Bool Parse(Ref<const SPtr<Resource>> Asset);

        // -=(Undocumented)=-
        void Process(Ref<const SPtr<Resource>> Asset, Bool Loaded);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Table<UInt, SPtr<Factory>> mFactories;
        Table<CStr, SPtr<Loader>>  mLoaders;
        Table<CStr, SPtr<Locator>> mLocators;
    };
}