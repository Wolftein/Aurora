// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
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

        // \see Subsystem::OnTick(UInt64)
        void OnTick(UInt64 Time) override;

        // -=(Undocumented)=-
        void AddLoader(ConstSPtr<Loader> Loader);

        // -=(Undocumented)=-
        void RemoveLoader(CStr Extension);

        // -=(Undocumented)=-
        void RemoveLoader(List<CStr> Extensions)
        {
            for (const CStr Extension : Extensions)
            {
                RemoveLoader(Extension);
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
            Type::GetFactory().SetMemoryBudget(Budget);
        }

        // -=(Undocumented)=-
        template<typename Type>
        UInt GetMemoryBudget()
        {
            return Type::GetFactory().GetMemoryBudget();
        }

        // -=(Undocumented)=-
        template<typename Type>
        UInt GetMemoryUsage()
        {
            return Type::GetFactory().GetMemoryUsage();
        }

        // -=(Undocumented)=-
        Data Find(Ref<const Uri> Key);

        // -=(Undocumented)=-
        Bool Save(Ref<const Uri> Key, CStr Data)
        {
            CPtr<const UInt8> Bytes(reinterpret_cast<Ptr<const UInt8>>(Data.data()), Data.size());

            return Save(Key, Bytes);
        }

        // -=(Undocumented)=-
        Bool Save(Ref<const Uri> Key, CPtr<const UInt8> Data);

        // -=(Undocumented)=-
        Bool Delete(Ref<const Uri> Key);

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> Load(Ref<const Uri> Key, Bool Async = false)
        {
            ConstSPtr<Type> Asset = Type::GetFactory().GetOrCreate(Key, true);

            if (Asset && !Asset->HasFinished())
            {
                // TODO: Asynchronous

                Process(Asset, Parse(Asset));
            }

            return Asset;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Reload(ConstSPtr<Type> Asset, Bool Async = false)
        {
            if (Asset && Asset->HasFinished())
            {
                // TODO: Asynchronous

                Process(Asset, Parse(Asset));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        Bool Exist(Ref<const Uri> Key)
        {
            return (Type::GetFactory().GetOrCreate(Key, false) != nullptr);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Unload(ConstSPtr<Type> Asset)
        {
            if (Asset && Type::GetFactory().Remove(Asset->GetKey()))
            {
                // TODO: Asynchronous

                Process(Asset, false);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Prune(Bool Force)
        {
            const Vector<SPtr<Type>> Assets = Type::GetFactory().Prune(Force);

            for (ConstSPtr<Type> Asset : Assets)
            {
                Process(Asset, false);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Process(ConstSPtr<Type> Asset, Bool Loaded)
        {
            if (Loaded)
            {
                Log::Debug("Resources: Loading '{}'", Asset->GetKey().GetUrl());

                Asset->Create(GetContext());
            }
            else
            {
                Log::Debug("Resources: Unloading '{}'", Asset->GetKey().GetUrl());

                Asset->Delete(GetContext());
            }
        }

    private:

        // -=(Undocumented)=-
        void RegisterDefaultResources();

        // -=(Undocumented)=-
        Bool Parse(ConstSPtr<Resource> Asset);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        StringTable<SPtr<Loader>>  mLoaders;
        StringTable<SPtr<Locator>> mLocators;
    };
}