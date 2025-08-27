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

#include "Aurora.Base/Collection.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A lightweight, bounded handle allocator with support for recycling.
    /// 
    /// \tparam Capacity Maximum number of active handles.
    template<UInt32 Capacity>
    class Handle final
    {
    public:

        /// \brief Invalid handle value.
        constexpr static UInt32 kInvalid = 0;

    public:

        /// \brief Constructs a new handle allocator with empty state.
        AURORA_INLINE explicit Handle()
            : mHead { 0 }
        {
        }

        /// \brief Allocates a new handle from the pool.
        /// 
        /// \return A unique handle value, or \ref kInvalid if the pool is exhausted.
        AURORA_INLINE UInt32 Allocate()
        {
            if (mPool.empty())
            {
                return mHead >= Capacity ? kInvalid : ++mHead;
            }

            const UInt32 Handle = mPool.back();

            if (Handle != kInvalid)
            {
                mPool.pop_back();
            }
            return Handle;
        }

        /// \brief Frees a previously allocated handle for reuse.
        /// 
        /// \param Handle The handle to release.
        /// \return The released handle if valid, or \ref kInvalid if rejected.
        AURORA_INLINE UInt32 Free(UInt32 Handle)
        {
            if (Handle == kInvalid || Handle > mHead)
            {
                return kInvalid;
            }

            if (Handle == mHead)
            {
                --mHead;
            }
            else
            {
                mPool.emplace_back(Handle);
            }
            return Handle;
        }

        /// \brief Resets the allocator, clearing all handles.
        AURORA_INLINE void Clear()
        {
            mHead = 0;
            mPool.clear();
        }

        /// \brief Returns whether all handles have been allocated and none are free.
        /// 
        /// \return `true` if the allocator is full, `false` otherwise.
        AURORA_INLINE Bool IsFull() const
        {
            return mPool.empty() && mHead == Capacity;
        }

        /// \brief Returns whether no handles have been allocated or all have been freed.
        /// 
        /// \return `true` if the allocator is empty, `false` otherwise.
        AURORA_INLINE Bool IsEmpty() const
        {
            return mPool.empty() && mHead == 0;
        }

        /// \brief Returns the highest allocated handle.
        /// 
        /// \return The highest handle allocated so far.
        AURORA_INLINE UInt32 GetBack() const
        {
            return mHead;
        }

        /// \brief Returns the number of currently active (allocated) handles.
        /// 
        /// \return The total count of handles in use.
        AURORA_INLINE UInt32 GetSize() const
        {
            return mHead - mPool.size();
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeVector(mPool);
            Archive.SerializeInt(mHead);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<UInt32, Capacity> mPool;
        UInt32                   mHead;
    };
}