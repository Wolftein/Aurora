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

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Type, UInt32 Size>
    class Sparse final
    {
    public:

        // -=(Undocumented)=-
        Sparse() = default;

        // -=(Undocumented)=-
        Sparse(UInt32 Capacity)
        {
            mPool.resize(Capacity);
        }

        // -=(Undocumented)=-
        Bool IsFull() const
        {
            return mAllocator.IsFull();
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mAllocator.Clear();
        }

        // -=(Undocumented)=-
        auto Content() const
        {
            return CPtr<const Type>(mPool.data(), mAllocator.Back());
        }

        // -=(Undocumented)=-
        UInt Capacity() const
        {
            return mPool.size();
        }

        // -=(Undocumented)=-
        UInt32 Allocate()
        {
            const UInt32 Handle = mAllocator.Allocate();

            if (Handle >= mPool.size())
            {
                const UInt32 Limit = mPool.size() + mPool.size() / 2 + 1;
                mPool.resize(Core::Min(Limit, Size));
            }
            return Handle;
        }

        // -=(Undocumented)=-
        UInt32 Free(UInt32 Handle)
        {
            return mAllocator.Free(Handle);
        }

        // -=(Undocumented)=-
        Ref<Type> operator[](UInt32 Handle)
        {
            return mPool[Handle - 1];
        }

        // -=(Undocumented)=-
        ConstRef<Type> operator[](UInt32 Handle) const
        {
            return mPool[Handle - 1];
        }

        // -=(Undocumented)=-
        template<typename Kind>
        void OnSerialize(Stream<Kind> Archive)
        {
            Archive.SerializeObject(mAllocator);
            Archive.SerializeVector(mPool);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<Size> mAllocator;
        Vector<Type> mPool;
    };
}