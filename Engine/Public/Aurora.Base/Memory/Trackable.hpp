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

#include "Aurora.Base/Concurrency.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Base class for objects that support intrusive reference counting.
    /// 
    /// `Trackable` is meant to be inherited by objects that need to be shared across systems
    /// while keeping their lifetime automatically managed.
    class Trackable
    {
    public:

        /// \brief Constructs a new instance with a reference count of zero.
        AURORA_INLINE Trackable()
            : mReferences { 0 }
        {
        }

        /// \brief Destructor.
        virtual ~Trackable() = default;

        /// \brief Increments the reference count.
        AURORA_INLINE void Acquire() const
        {
            mReferences.fetch_add(1, std::memory_order_relaxed);
        }

        /// \brief Decrements the reference count and deletes the object when it reaches zero.
        AURORA_INLINE void Release() const
        {
            if (mReferences.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                delete this;
            }
        }

        /// \brief Checks whether the object is still tracked by at least one holder.
        /// 
        /// \return `true` if the reference count is greater than one, `false` otherwise.
        AURORA_INLINE Bool IsTracked() const
        {
            return mReferences.load(std::memory_order_acquire) > 1;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        mutable Atomic<UInt32> mReferences;
    };
}
