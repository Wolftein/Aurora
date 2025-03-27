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
    template<typename Type, UInt32 Size, Bool Serializable = true>
    class Pool final
    {
    public:

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
        void Clear()
        {
            mAllocator.Clear();
        }

        // -=(Undocumented)=-
        Bool IsFull() const
        {
            return mAllocator.IsFull();
        }

        // -=(Undocumented)=-
        Bool IsEmpty() const
        {
            return mAllocator.IsEmpty();
        }

        // -=(Undocumented)=-
        UInt32 GetBack() const
        {
            return mAllocator.GetBack();
        }

        // -=(Undocumented)=-
        UInt32 GetSize() const
        {
            return mAllocator.GetSize();
        }

        // -=(Undocumented)=-
        CPtr<Type> GetContent()
        {
            return CPtr<Type>(mPool.data(), mAllocator.GetBack());
        }

        // -=(Undocumented)=-
        CPtr<const Type> GetContent() const
        {
            return CPtr<const Type>(mPool.data(), mAllocator.GetBack());
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

            if constexpr (Serializable)
            {
                for (UInt32 Element = 0, Limit = mAllocator.GetBack(); Element < Limit; ++Element)
                {
                    Archive.SerializeObject(mPool[Element]);
                }
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle<Size>      mAllocator;
        Array<Type, Size> mPool;
    };
}