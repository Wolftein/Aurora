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

#include "Aurora.Base/IO/Stream.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<UInt32 Capacity>
    class Handle final
    {
    public:

        // -=(Undocumented)=-
        constexpr static UInt32 k_Invalid = 0;

    public:

        // -=(Undocumented)=-
        explicit Handle()
            : mHead { 0 }
        {
        }

        // -=(Undocumented)=-
        Bool IsFull() const
        {
            return mPool.empty() && mHead == Capacity;
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mHead = 0;
            mPool.clear();
        }

        // -=(Undocumented)=-
        UInt32 Allocate()
        {
            if (mPool.empty())
            {
                return mHead >= Capacity ? k_Invalid : ++mHead;
            }

            const UInt32 Handle = mPool.empty() ? k_Invalid : mPool.back();

            if (Handle != k_Invalid)
            {
                mPool.pop_back();
            }
            return Handle;
        }

        // -=(Undocumented)=-
        UInt32 Free(UInt32 Handle)
        {
            if (Handle < mHead)
            {
                mPool.emplace_back(Handle);
            }
            else
            {
                mHead = Handle;
            }
            return Handle;
        }

        // -=(Undocumented)=-
        UInt32 Back() const
        {
            return mHead;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeVector(mPool);
            Archive.SerializeNumber(mHead);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<UInt32> mPool;
        UInt32         mHead;
    };
}