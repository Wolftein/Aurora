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

#include "Uri.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    /// \brief Represents a thread-safe cache and factory for managing loadable assets for \p Type.
    template<typename Type>
    class Cache final
    {
    public:

        /// \brief Constructs an empty cache with no memory budget or usage.
        AURORA_INLINE Cache()
            : mLimit { 0 },
              mUsage { 0 }
        {
        }

        /// \brief Sets the maximum memory budget for stored assets.
        ///
        /// \param Limit The asset memory budget in bytes.
        AURORA_INLINE void SetMemoryLimit(UInt64 Limit)
        {
            mLimit = Limit;
        }

        /// \brief Gets the configured memory budget for stored assets.
        ///
        /// \return Memory budget in bytes.
        AURORA_INLINE UInt64 GetMemoryLimit() const
        {
            return mLimit;
        }

        /// \brief Sets the currently tracked memory usage.
        ///
        /// \param Usage The memory usage in bytes.
        AURORA_INLINE void SetMemoryUsage(UInt64 Usage)
        {
            mUsage = Usage;
        }

        /// \brief Gets the currently tracked memory usage.
        ///
        /// \return Total memory usage in bytes.
        AURORA_INLINE UInt64 GetMemoryUsage() const
        {
            return mUsage;
        }

        /// \brief Retrieves an existing asset or optionally creates it if not found.
        ///
        /// Thread-safe. Acquires a lock on the internal registry.
        ///
        /// \param Key            The asset's URI.
        /// \param CreateIfNeeded If `true`, a new asset is created when not found.
        /// \return A tracker to the asset, or `nullptr` if not found and not created.
        Tracker<Type> GetOrCreate(AnyRef<Uri> Key, Bool CreateIfNeeded)
        {
            Lock<> Guard(mLatch);

            if (const auto Iterator = mRegistry.find(Key.GetPath()); Iterator != mRegistry.end())
            {
                return Iterator->second;
            }

            if (CreateIfNeeded)
            {
                Tracker<Type> Result = Tracker<Type>::Create(Move(Key));
                mRegistry.try_emplace(Result->GetKey().GetPath(), Result);
                return Result;
            }
            return nullptr;
        }

        /// \brief Removes an asset if it has finished loading and is not retained.
        ///
        /// Thread-safe. Acquires a lock on the internal registry.
        ///
        /// \param Key The URI of the asset to remove.
        /// \return `true` if the asset was removed, otherwise `false`.
        Bool Remove(ConstRef<Uri> Key)
        {
            Lock<> Guard(mLatch);

            if (const auto Iterator = mRegistry.find(Key.GetPath()); Iterator != mRegistry.end())
            {
                ConstTracker<Type> Asset = Iterator->second;

                if (Asset->HasFinished())
                {
                    mRegistry.erase(Iterator);
                    return true;
                }
            }
            return false;
        }

        /// \brief Removes finished assets that are not tracked, or all finished assets if forced.
        ///
        /// Thread-safe. Acquires a lock on the internal registry.
        ///
        /// \param Force      If `true`, all finished assets are removed regardless of tracking state.
        /// \param Dispatcher Callback invoked before removing each asset.
        template<typename Function>
        void Prune(Bool Force, AnyRef<Function> Dispatcher)
        {
            Lock<> Guard(mLatch);

            for (auto Iterator = mRegistry.begin(); Iterator != mRegistry.end();)
            {
                ConstTracker<Type> Asset = Iterator->second;

                if (Force || (!Asset->IsTracked() && Asset->HasFinished()))
                {
                    Dispatcher(*Asset);
                    Iterator = mRegistry.erase(Iterator);
                }
                else
                {
                    ++Iterator;
                }
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        mutable Mutex            mLatch;
        TextTable<Tracker<Type>> mRegistry;
        UInt64                   mLimit;
        UInt64                   mUsage;
    };
}
