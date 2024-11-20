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

#include "Uri.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    template<typename Type>
    class Factory final
    {
    public:

        // -=(Undocumented)=-
        Factory()
            : mBudget { 0 },
              mUsage  { 0 }
        {
        }

        // -=(Undocumented)=-
        void SetMemoryBudget(UInt Budget)
        {
            mBudget = Budget;
        }

        // -=(Undocumented)=-
        UInt GetMemoryBudget() const
        {
            return mBudget;
        }

        // -=(Undocumented)=-
        void AddMemoryUsage(UInt Usage)
        {
            mUsage += Usage;
        }

        // -=(Undocumented)=-
        UInt GetMemoryUsage() const
        {
            return mUsage;
        }

        // -=(Undocumented)=-
        SPtr<Type> GetOrCreate(ConstRef<Uri> Key, Bool CreateIfNeeded)
        {
            SPtr<Type> Result;

            if (const auto Iterator = mRegistry.find(Key.GetPath()); Iterator != mRegistry.end())
            {
                Result = Iterator->second;
            }
            else if (CreateIfNeeded)
            {
                Result = NewPtr<Type>(Move(Key));

                mRegistry.try_emplace(Result->GetKey().GetPath(), Result);
            }
            return Result;
        }

        // -=(Undocumented)=-
        Bool Remove(ConstRef<Uri> Key)
        {
            if (const auto Iterator = mRegistry.find(Key.GetPath()); Iterator != mRegistry.end())
            {
                ConstSPtr<Type> Asset = Iterator->second;

                if (Asset->HasFinished())
                {
                    mRegistry.erase(Iterator);
                    return true;
                }
            }
            return false;
        }

        // -=(Undocumented)=-
        Vector<SPtr<Type>> Prune(Bool Force)
        {
            Vector<SPtr<Type>> Collection;

            for (auto Iterator = mRegistry.begin(); Iterator != mRegistry.end();)
            {
                ConstSPtr<Type> Asset = Iterator->second;

                if (Force || (Asset.use_count() == 1 && Asset->HasFinished()))
                {
                    Collection.emplace_back(Asset);

                    Iterator = mRegistry.erase(Iterator);
                }
                else
                {
                    ++Iterator;
                }
            }
            return Collection;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        StringTable<SPtr<Type>> mRegistry;
        UInt                    mBudget;
        UInt                    mUsage;
    };
}