// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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
    class Service final : public EnableSmartPointer<Service>, public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Service(Ref<Context> Context);

        // \see Subsystem::OnTick
        void OnTick() override;

        // -=(Undocumented)=-
        void AddLoader(ConstSPtr<Loader> Loader);

        // -=(Undocumented)=-
        void RemoveLoader(CStr Extension);

        // -=(Undocumented)=-
        void RemoveLoader(List<CStr> Extensions)
        {
            for (const CStr Extension : Extensions)
            {
                RemoveLoader(STRING_FIX(Extension));
            }
        }

        // -=(Undocumented)=-
        void AddLocator(CStr Schema, ConstSPtr<Locator> Locator);

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
        Chunk Find(Ref<const Uri> Key);

        // -=(Undocumented)=-
        void Register(ConstSPtr<Resource> Asset, Bool Cacheable)
        {
            Register(Asset, Cacheable, false);
        }

        // -=(Undocumented)=-
        void Register(ConstSPtr<Resource> Asset, Bool Cacheable, Bool Async);

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> Retrieve(Ref<const Uri> Key, Bool CreateIfMissing)
        {
            return GetFactory(Type::RTTI_CATEGORY)->template GetOrCreate<Type>(Key, CreateIfMissing);
        }

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> Load(Ref<const Uri> Key)
        {
            return Load<Type>(Key, false);
        }

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> Load(Ref<const Uri> Key, Bool Async)
        {
            ConstSPtr<Type> Asset = GetFactory(Type::RTTI_CATEGORY)->template GetOrCreate<Type>(Key, true);

            Load(Asset, Async);

            return Asset;
        }

        // -=(Undocumented)=-
        void Load(ConstSPtr<Resource> Asset)
        {
            Load(Asset, false);
        }

        // -=(Undocumented)=-
        void Load(ConstSPtr<Resource> Asset, Bool Async);

        // -=(Undocumented)=-
        void Reload(ConstSPtr<Resource> Asset)
        {
            Reload(Asset, false);
        }

        // -=(Undocumented)=-
        void Reload(ConstSPtr<Resource> Asset, Bool Async);

        // -=(Undocumented)=-
        void Unload(ConstSPtr<Resource> Asset);

        // -=(Undocumented)=-
        template<typename Type>
        Bool Exist(Ref<const Uri> Key)
        {
            return (GetFactory(Type::RTTI_CATEGORY)->template GetOrCreate<Type>(Key, false) != nullptr);
        }

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
        void RegisterDefaultResources();

        // -=(Undocumented)=-
        ConstSPtr<Factory> GetFactory(UInt Category);

        // -=(Undocumented)=-
        Bool Parse(ConstSPtr<Resource> Asset);

        // -=(Undocumented)=-
        void Process(ConstSPtr<Resource> Asset, Bool Loaded);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Table<UInt, SPtr<Factory>> mFactories;
        Table<SStr, SPtr<Loader>>  mLoaders;
        Table<SStr, SPtr<Locator>> mLocators;
    };
}