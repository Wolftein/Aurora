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

#include "Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    template<typename Type>
    class Accessor final
    {
    public:

        // -=(Undocumented)=-
        using Handle = flecs::ref<Type>;

    public:

        // -=(Undocumented)=-
        Accessor() = default;

        // -=(Undocumented)=-
        Accessor(Handle Handle)
            : mHandle { Handle }
        {
        }

        // -=(Undocumented)=-
        template<auto Function, typename... Arguments>
        void Modify(Any<Arguments>... Parameters)
        {
            if (Ptr<Type> Component = mHandle.get())
            {
                std::invoke(Function, Component, std::forward<Arguments>(Parameters)...);
                Notify();
            }
        }

        // -=(Undocumented)=-
        void Notify()
        {
            if (mHandle.has())
            {
                mHandle.entity().template modified<Type>();
            }
        }

        // -=(Undocumented)=-
        Ptr<Type> operator->()
        {
            return mHandle.get();
        }

        // -=(Undocumented)=-
        Ref<Type> operator*()
        {
            return * mHandle.get();
        }

        // -=(Undocumented)=-
        ConstPtr<Type> operator->() const
        {
            return const_cast<Ptr<Accessor>>(this)->mHandle.get();
        }

        // -=(Undocumented)=-
        ConstRef<Type> operator*() const
        {
            return * const_cast<Ptr<Accessor>>(this)->mHandle.get();
        }

        // -=(Undocumented)=-
        operator Bool() const
        {
            return mHandle.has();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}