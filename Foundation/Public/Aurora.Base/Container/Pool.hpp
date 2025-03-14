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
    template<typename Type, UInt32 Count>
    class Pool final
    {
    public:

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
        auto GetData() const
        {
            return CPtr<const Type>(mPool.data(), mAllocator.Head());
        }

        // -=(Undocumented)=-
        UInt32 Allocate()
        {
            return mAllocator.Allocate();
        }

        // -=(Undocumented)=-
        UInt32 Free(UInt32 Handle)
        {
            return mAllocator.Free(Handle);
        }

        // -=(Undocumented)=-
        Ref<Type> operator[](UInt32 Handle)
        {
            return mPool[Handle];
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeObject(mAllocator);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<Count>      mAllocator;
        Array<Type, Count> mPool;
    };
}