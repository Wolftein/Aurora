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

#include "Cache.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Content
{
    /// \brief Base class for a loadable resource identified by a unique URI.
    class Resource : public Trackable
    {
    public:

        /// \brief Enumerates the states of a resource.
        enum class Status : UInt8
        {
            Idle,       ///< Resource has not been requested or is inactive.
            Queued,     ///< Resource has been requested and is awaiting load or parse.
            Completed,  ///< Resource loaded and parsed successfully.
            Failed,     ///< Resource failed to load.
        };

        /// \brief Enumerates the ownership policies of a resource.
        enum class Policy : UInt8
        {
            Managed,    ///< Owned and managed by the resource manager.
            Exclusive,  ///< Exclusive to a specific owner, not shared.
        };

    public:

        /// \brief Creates a resource with the given unique URI key.
        ///
        /// \param Key Unique URI identifying this resource.
        AURORA_INLINE explicit Resource(AnyRef<Uri> Key)
            : mKey      { Move(Key) },
              mStatus   { Status::Idle },
              mMemory   { 0 },
              mPolicy   { Policy::Managed }
        {
        }

        /// \brief Gets the unique key (URI) identifying this resource.
        ///
        /// \return A constant reference to the URI key.
        AURORA_INLINE ConstRef<Uri> GetKey() const
        {
            return mKey;
        }

        /// \brief Atomically changes status if it matches an expected value.
        ///
        /// \param Comparison Expected current status.
        /// \param Status     New status to set if comparison succeeds.
        /// \return `true` if the status was updated, otherwise `false`.
        AURORA_INLINE Bool Transition(Status Comparison, Status Status)
        {
            return mStatus.compare_exchange_strong(Comparison, Status, std::memory_order_acq_rel);
        }

        /// \brief Sets the current status of the resource.
        ///
        /// \param Status The new status to set.
        AURORA_INLINE void SetStatus(Status Status)
        {
            mStatus.store(Status, std::memory_order_release);
        }

        /// \brief Gets the current status of the resource.
        ///
        /// Loads with acquire semantics to synchronize with \ref SetStatus.
        ///
        /// \return The current status value.
        AURORA_INLINE Status GetStatus() const
        {
            return mStatus.load(std::memory_order_acquire);
        }

        /// \brief Sets the estimated memory usage of the resource in bytes.
        ///
        /// \param Memory The size in bytes to record for this resource.
        AURORA_INLINE void SetMemory(UInt64 Memory)
        {
            mMemory = Memory;
        }

        /// \brief Gets the estimated memory usage of this resource.
        ///
        /// \return Size in bytes currently attributed to this resource.
        AURORA_INLINE UInt64 GetMemory() const
        {
            return mMemory;
        }

        /// \brief Checks whether loading has completed or failed.
        ///
        /// \return `true` if \ref GetStatus is \ref Status::Completed or \ref Status::Failed, otherwise `false`.
        AURORA_INLINE Bool HasFinished() const
        {
            const Status Current = GetStatus();
            return (Current == Status::Failed || Current == Status::Completed);
        }

        /// \brief Checks whether loading failed.
        ///
        /// \return `true` if \ref GetStatus is \ref Status::Failed, otherwise `false`.
        AURORA_INLINE Bool HasFailed() const
        {
            return GetStatus() == Status::Failed;
        }

        /// \brief Checks whether loading completed successfully.
        ///
        /// \return `true` if \ref GetStatus is \ref Status::Completed, otherwise `false`.
        AURORA_INLINE Bool HasCompleted() const
        {
            return GetStatus() == Status::Completed;
        }

        /// \brief Sets the ownership policy for this resource.
        ///
        /// \param Policy The ownership policy to apply.
        AURORA_INLINE void SetPolicy(Policy Policy)
        {
            mPolicy = Policy;
        }

        /// \brief Checks whether the resource uses a specific ownership policy.
        ///
        /// \param Policy The policy to compare against.
        /// \return `true` if the resource’s policy equals \p Policy, otherwise `false`.
        AURORA_INLINE Bool HasPolicy(Policy Policy) const
        {
            return mPolicy == Policy;
        }

        /// \brief Attempts to load the resource using the given service host.
        ///
        /// This method sets up the resource for loading, then invokes \ref OnCreate
        /// in the derived class to perform the actual load logic.
        ///
        /// \param Host Reference to the service host responsible for content management.
        /// \return `true` if the resource was loaded successfully, `false` otherwise.
        Bool Create(Ref<Service::Host> Host);

        /// \brief Unloads the resource using the given service host.
        ///
        /// This method invokes \ref OnDelete in the derived class to release any
        /// allocated memory, handles, or dependent resources.
        ///
        /// \param Host Reference to the service host responsible for content management.
        void Delete(Ref<Service::Host> Host);

    protected:

        /// \brief Called during \ref Create to perform resource-specific load logic.
        ///
        /// Derived classes must override this to decode, initialize, and allocate any
        /// data required by the resource.
        ///
        /// \param Host Reference to the service host that manages content loading.
        /// \return `true` if the resource was loaded successfully, `false` otherwise.
        virtual Bool OnCreate(Ref<Service::Host> Host);

        /// \brief Called during \ref Delete to perform resource-specific unload logic.
        ///
        /// Derived classes must override this to release all memory and handles held
        /// by the resource.
        ///
        /// \param Host Reference to the service host that manages content unloading.
        virtual void OnDelete(Ref<Service::Host> Host);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const Uri      mKey;
        Atomic<Status> mStatus;
        UInt64         mMemory;
        Policy         mPolicy;
    };

    /// \brief Base class template for resources with automatic memory tracking in a shared cache.
    ///
    /// Automatically updates cache memory usage during load and unload.
    template<typename Type>
    class AbstractResource : public Resource
    {
    public:

        /// \brief Constructs a resource with the given URI.
        AURORA_INLINE explicit AbstractResource(AnyRef<Uri> Key)
            : Resource(Move(Key))
        {
        }

    protected:

        /// \brief Wraps load logic with cache memory usage tracking.
        AURORA_INLINE Bool OnCreate(Ref<Service::Host> Host) override
        {
            const Bool Result = static_cast<Ptr<Type>>(this)->OnCreate(Host);

            if (Result)
            {
                GetCache().SetMemoryUsage(GetCache().GetMemoryUsage() + GetMemory());
            }
            return Result;
        }

        /// \brief Wraps unload logic with cache memory usage tracking.
        AURORA_INLINE void OnDelete(Ref<Service::Host> Host) override
        {
            GetCache().SetMemoryUsage(GetCache().GetMemoryUsage() - GetMemory());
            static_cast<Ptr<Type>>(this)->OnDelete(Host);
        }

    public:

        /// \brief Gets the global cache for this resource type.
        AURORA_INLINE constexpr static Ref<Cache<Type>> GetCache()
        {
            static Cache<Type> RTTI_FACTORY;
            return RTTI_FACTORY;
        }
    };
}
