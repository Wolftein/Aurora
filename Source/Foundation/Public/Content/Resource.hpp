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

#include "Uri.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=(Undocumented)=-
    class Resource
    {
    public:

        // -=(Undocumented)=-
        enum class Status
        {
            None, Pending, Loaded, Failed, Unloaded
        };

    public:

        // -=(Undocumented)=-
        Resource(Ref<const Uri> Key, UInt Category)
            : mKey      { Key },
              mCategory { Category },
              mMemory   { 0 },
              mStatus   { Status::None }
        {
        }

        // -=(Undocumented)=-
        virtual ~Resource() = default;

        // -=(Undocumented)=-
        Bool Create(Ref<Subsystem::Context> Context)
        {
            const Bool Result = OnCreate(Context);

            if (Result)
            {
                SetStatus(Status::Loaded);
            }
            else
            {
                SetStatus(Status::Failed);
            }
            return Result;
        }

        // -=(Undocumented)=-
        void Dispose(Ref<Subsystem::Context> Context)
        {
            OnDispose(Context);

            SetStatus(Status::Unloaded);
            SetMemory(0);
        }

        // -=(Undocumented)=-
        Ref<const Uri> GetKey() const
        {
            return mKey;
        }

        // -=(Undocumented)=-
        UInt GetCategory() const
        {
            return mCategory;
        }

        // -=(Undocumented)=-
        void SetMemory(UInt Memory)
        {
            mMemory = Memory;
        }

        // -=(Undocumented)=-
        UInt GetMemory() const
        {
            return mMemory;
        }

        // -=(Undocumented)=-
        void SetStatus(Status Status)
        {
            mStatus = Status;
        }

        // -=(Undocumented)=-
        Status GetStatus() const
        {
            return mStatus;
        }

        // -=(Undocumented)=-
        Bool HasCreated() const
        {
            return mStatus == Status::None;
        }

        // -=(Undocumented)=-
        Bool HasUnloaded() const
        {
            return mStatus == Status::Unloaded;
        }

        // -=(Undocumented)=-
        Bool HasFinished() const
        {
            return HasLoaded() | HasFailed();
        }

        // -=(Undocumented)=-
        Bool HasFailed() const
        {
            return mStatus == Status::Failed;
        }

        // -=(Undocumented)=-
        Bool HasLoaded() const
        {
            return mStatus == Status::Loaded;
        }

    protected:

        // -=(Undocumented)=-
        virtual Bool OnCreate(Ref<Subsystem::Context> Context)
        {
            return false;
        }

        // -=(Undocumented)=-
        virtual void OnDispose(Ref<Subsystem::Context> Context)
        {
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const Uri  mKey;
        const UInt mCategory;
        UInt       mMemory;
        Status     mStatus;
    };

    // -=(Undocumented)=-
    template<UInt Category>
    class AbstractResource : public Resource
    {
    public:

        // -=(Undocumented)=-
        static constexpr inline UInt RTTI_CATEGORY = Category;

    public:

        // -=(Undocumented)=-
        AbstractResource(Ref<const Uri> Key)
            : Resource(Key, RTTI_CATEGORY)
        {
        }
    };
}