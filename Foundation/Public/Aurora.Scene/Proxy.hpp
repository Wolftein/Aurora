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
    class Proxy final
    {
    public:

        // -=(Undocumented)=-
        using Handle = flecs::ref<Type>;

    public:

        // -=(Undocumented)=-
        Proxy() = default;

        // -=(Undocumented)=-
        Proxy(Handle Handle)
            : mHandle { Handle }
        {
        }

        // -=(Undocumented)=-
        Bool IsNull() const
        {
            return mHandle == 0;
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
            mHandle.entity().template modified<Type>();
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
            return const_cast<Ptr<Proxy>>(this)->mHandle.get();
        }

        // -=(Undocumented)=-
        ConstRef<Type> operator*() const
        {
            return * const_cast<Ptr<Proxy>>(this)->mHandle.get();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Handle mHandle;
    };
}