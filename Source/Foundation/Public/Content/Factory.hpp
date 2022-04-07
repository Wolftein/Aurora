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

#include "Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    class Factory
    {
    public:

        // -=(Undocumented)=-
        Factory();

        // -=(Undocumented)=-
        void SetMemoryBudget(UInt64 Budget);

        // -=(Undocumented)=-
        UInt64 GetMemoryBudget() const;

        // -=(Undocumented)=-
        void SetMemoryUsage(UInt64 Usage);

        // -=(Undocumented)=-
        UInt64 GetMemoryUsage() const;

        // -=(Undocumented)=-
        template<typename Type>
        SPtr<Type> GetOrCreate(Ref<const Uri> Key, Bool CreateIfNeeded)
        {
            SPtr<Type> Result = nullptr;

            if (const auto Iterator = mAssets.find(Key.GetPath()); Iterator != mAssets.end())
            {
                Result = eastl::static_shared_pointer_cast<Type>(Iterator->second);
            }
            else if (CreateIfNeeded)
            {
                Result = eastl::make_shared<Type>(Key);

                mAssets.try_emplace(Key.GetPath(), Result);
            }
            return Result;
        }

        // -=(Undocumented)=-
        void Insert(Ref<const SPtr<Resource>> Resource);

        // -=(Undocumented)=-
        Bool Unload(Ref<const Uri> Key);

        // -=(Undocumented)=-
        Array<SPtr<Resource>> Prune(Bool Force);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Table<CStr, SPtr<Resource>> mAssets;
        UInt64                      mAssetsBudget;
        UInt64                      mAssetsUsage;
    };
}