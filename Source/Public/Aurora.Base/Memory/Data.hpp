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

#include "Aurora.Base/Trait.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class Data final
    {
    public:

        // -=(Undocumented)=-
        using Deleter = FPtr<void(Ptr<void>)>;

        // -=(Undocumented)=-
        using Pointer = Ptr<void>;

        // -=(Undocumented)=-
        static constexpr auto EMPTY_DELETER  = [](Ptr<void>)
        {

        };

        // -=(Undocumented)=-
        template<typename Type>
        static constexpr auto BASIC_DELETER  = [](Ptr<void> Address)
        {
            delete static_cast<Ptr<Type>>(Address);
        };

    public:

        // -=(Undocumented)=-
        Data()
            : mDeleter { nullptr },
              mData    { nullptr },
              mSize    { 0 }
        {
        }

        // -=(Undocumented)=-
        Data(UInt Size)
            : mDeleter { BASIC_DELETER<UInt8> },
              mData    { new UInt8[Size] },
              mSize    { Size }
        {
        }

        // -=(Undocumented)=-
        Data(Pointer Data, UInt Size, Deleter Destructor)
            : mDeleter { Destructor },
              mData    { Data },
              mSize    { Size }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        Data(Type Data, UInt Size, Deleter Destructor)
            : mDeleter { Destructor },
              mData    { Data },
              mSize    { Size }
        {
        }

        // -=(Undocumented)=-
        Data(Any<Data> Other)
            : mDeleter { Move(Other.mDeleter) },
              mData    { std::exchange(Other.mData, nullptr) },
              mSize    { std::exchange(Other.mSize, 0) }
        {
        }

        // -=(Undocumented)=-
        ~Data()
        {
            Clear();
        }

        // -=(Undocumented)=-
        template<typename Type = void>
        Ptr<Type> GetData()
        {
            return static_cast<Ptr<Type>>(mData);
        }

        // -=(Undocumented)=-
        template<typename Type = void>
        Ptr<const Type> GetData() const
        {
            return static_cast<Ptr<const Type>>(mData);
        }

        // -=(Undocumented)=-
        template<typename Type>
        CPtr<Type> GetSpan(UInt Offset = 0) const
        {
            return CPtr<Type>(static_cast<Ptr<Type>>(mData) + Offset, mSize / sizeof(Type));
        }

        // -=(Undocumented)=-
        CStr GetText() const
        {
            return CStr(static_cast<CStr::pointer>(mData), mSize);
        }

        // -=(Undocumented)=-
        Bool HasData() const
        {
            return GetSize() > 0;
        }

        // -=(Undocumented)=-
        UInt GetSize() const
        {
            return mSize;
        }

        // -=(Undocumented)=-
        void Clear()
        {
            if (mData)
            {
                mDeleter(mData);
                mData = nullptr;
            }
            mSize = 0;
        }

        // -=(Undocumented)=-
        Ref<Data> operator=(Any<Data> Other)
        {
            mDeleter = Move(Other.mDeleter);
            mData    = std::exchange(Other.mData, nullptr);
            mSize    = std::exchange(Other.mSize, 0);

            return (* this);
        }

        // -=(Undocumented)=-
        template<typename Type>
        operator CPtr<Type>() const
        {
            return CPtr<Type>(static_cast<Ptr<Type>>(mData), mSize / sizeof(Type));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Deleter mDeleter;
        Pointer mData;
        UInt    mSize;
    };
}