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

#include "Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    class Factory final
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

            if (const auto Iterator = mAssets.find(STRING_FIX(Key.GetPath())); Iterator != mAssets.end())
            {
                Result = CastPtr<Type>(Iterator->second);
            }
            else if (CreateIfNeeded)
            {
                Result = NewPtr<Type>(Key);

                mAssets.try_emplace(STRING_FIX(Key.GetPath()), Result);
            }
            return Result;
        }

        // -=(Undocumented)=-
        Bool Insert(Ref<const SPtr<Resource>> Resource);

        // -=(Undocumented)=-
        Bool Unload(Ref<const Uri> Key);

        // -=(Undocumented)=-
        Vector<SPtr<Resource>> Prune(Bool Force);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Table<SStr, SPtr<Resource>> mAssets;
        UInt64                      mAssetsBudget;
        UInt64                      mAssetsUsage;
    };
}