// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Type.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename Type, UInt32 Count>
    class Stack final
    {
    public:

        // -=(Undocumented)=-
      	Stack()
          	: mSize { 0 }
     	{
        }

        // -=(Undocumented)=-
        Bool IsEmpty() const
        {
            return mSize == 0;
        }

        // -=(Undocumented)=-
        Bool IsFull() const
        {
            return mSize == Count;
        }

        // -=(Undocumented)=-
        auto GetData()
      	{
      	    return CPtr<Type>(mPool.data(), mSize);   
      	}
        
        // -=(Undocumented)=-
        auto GetData() const
      	{
      	    return CPtr<const Type>(mPool.data(), mSize);   
      	}

    	// -=(Undocumented)=-
    	auto GetSize() const
      	{
      		return mSize;
      	}

        // -=(Undocumented)=-
        void Clear(Bool Dispose)
        {
      		if (Dispose)
      		{
      			Destroy();
      		}
            mSize = 0;
        }

        // -=(Undocumented)=-
        Ptr<Type> Allocate()
        {
      	    return IsFull() ? nullptr : AddressOf(mPool[mSize++]);
        }

        // -=(Undocumented)=-
        Ref<Type> operator[](UInt Handle)
        {
            return mPool[Handle];
        }

    	// -=(Undocumented)=-
    	Ref<const Type> operator[](UInt Handle) const
    	{
    		return mPool[Handle];
    	}

    private:

    	// -=(Undocumented)=-
    	void Destroy()
    	{
    		for (Ref<Type> Object : GetData())
    		{
    			Object.~Type();
    		}
    	}

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Type, Count> mPool;
        UInt32			   mSize;
    };
}