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

#include "Factory.hpp"

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
            None, Loaded, Failed
        };

    public:

        // -=(Undocumented)=-
        explicit Resource(Any<Uri> Key)
            : mKey      { Move(Key) },
              mMemory   { 0 },
              mStatus   { Status::None }
        {
        }

        // -=(Undocumented)=-
        virtual ~Resource() = default;

        // -=(Undocumented)=-
        Bool Create(Ref<Subsystem::Context> Context)
        {
            Delete(Context);

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
        void Delete(Ref<Subsystem::Context> Context)
        {
            if (HasLoaded())
            {
                OnDelete(Context);
            }

            SetStatus(Status::None);
            SetMemory(0);
        }

        // -=(Undocumented)=-
        ConstRef<Uri> GetKey() const
        {
            return mKey;
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
        Bool HasFinished() const
        {
            return HasLoaded() || HasFailed();
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
            return true;
        }

        // -=(Undocumented)=-
        virtual void OnDelete(Ref<Subsystem::Context> Context)
        {
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const Uri  mKey;
        UInt       mMemory;
        Status     mStatus;
    };

    // -=(Undocumented)=-
    template<typename Impl>
    class AbstractResource : public Resource
    {
    public:

        // -=(Undocumented)=-
        explicit AbstractResource(Any<Uri> Key)
            : Resource(Move(Key))
        {
        }

    protected:

        // -=(Undocumented)=-
        Bool OnCreate(Ref<Subsystem::Context> Context) override
        {
            const Bool Result = static_cast<Ptr<Impl>>(this)->OnCreate(Context);

            if (Result)
            {
                GetFactory().AddMemoryUsage(GetMemory());
            }
            return Result;
        }

        // -=(Undocumented)=-
        void OnDelete(Ref<Subsystem::Context> Context) override
        {
            GetFactory().AddMemoryUsage(-GetMemory());

            static_cast<Ptr<Impl>>(this)->OnDelete(Context);
        }

    public:

        // -=(Undocumented)=-
        static Ref<Factory<Impl>> GetFactory()
        {
            static Factory<Impl> RTTI_FACTORY;
            return RTTI_FACTORY;
        }
    };
}