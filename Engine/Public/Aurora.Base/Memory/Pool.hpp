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

#include "Handle.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A fixed-size memory pool for efficient allocation and management of objects.
    template<typename Type, UInt32 Size>
    class Pool final
    {
    public:

        /// \brief Allocates a new handle and constructs an object in place.
        ///
        /// \param Parameters The arguments to forward to the constructor of the new object.
        /// \return A unique handle value if successful, or 0 if the pool is exhausted.
        template<typename... Arguments>
        AURORA_INLINE UInt32 Allocate(AnyRef<Arguments>... Parameters)
        {
            const UInt32 Handle = mAllocator.Allocate();

            if (Handle)
            {
                InPlaceConstruct<Type>(mPool[Handle - 1], Forward<Arguments>(Parameters)...);
            }
            return Handle;
        }

        /// \brief Frees a previously allocated handle for reuse.
        ///
        /// \param Handle The handle to release.
        /// \return The released handle if valid, or \ref kInvalid if rejected.
        AURORA_INLINE UInt32 Free(UInt32 Handle)
        {
            return mAllocator.Free(Handle);
        }

        /// \brief Resets the allocator, clearing all handles.
        AURORA_INLINE void Clear()
        {
            mAllocator.Clear();
        }

        /// \brief Returns whether all handles have been allocated and none are free.
        ///
        /// \return `true` if the allocator is full, `false` otherwise.
        AURORA_INLINE Bool IsFull() const
        {
            return mAllocator.IsFull();
        }

        /// \brief Returns whether no handles have been allocated or all have been freed.
        ///
        /// \return `true` if the allocator is empty, `false` otherwise.
        AURORA_INLINE Bool IsEmpty() const
        {
            return mAllocator.IsEmpty();
        }

        /// \brief Returns the highest allocated handle.
        ///
        /// \return The highest handle allocated so far.
        AURORA_INLINE UInt32 GetBack() const
        {
            return mAllocator.GetBack();
        }

        /// \brief Returns the number of currently active (allocated) handles.
        ///
        /// \return The total count of handles in use.
        AURORA_INLINE UInt32 GetSize() const
        {
            return mAllocator.GetSize();
        }

        /// \brief Accesses the element associated with the given handle.
        ///
        /// \param Handle The handle of the element to access. Must be a valid, allocated handle.
        /// \return A reference to the element associated with the handle.
        AURORA_INLINE Ref<Type> operator[](UInt32 Handle)
        {
            return mPool[Handle - 1];
        }

        /// \brief Accesses the element associated with the given handle.
        ///
        /// \param Handle The handle of the element to access. Must be a valid, allocated handle.
        /// \return A reference to the element associated with the handle.
        AURORA_INLINE ConstRef<Type> operator[](UInt32 Handle) const
        {
            return mPool[Handle - 1];
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        ///
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeObject(mAllocator);

            for (UInt32 Element = 0, Limit = mAllocator.GetBack(); Element < Limit; ++Element)
            {
                Archive.SerializeObject(mPool[Element]);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<Size>      mAllocator;
        Array<Type, Size> mPool;
    };
}