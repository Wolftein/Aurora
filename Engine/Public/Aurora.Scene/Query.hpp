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
    /// \brief Represents a predefined filter for iterating over entities based on their component composition.
    template<typename ...Components>
    class Query final
    {
    public:

        /// \brief Underlying flecs query handle type.
        using Handle = flecs::query<Components...>;

    public:

        /// \brief Constructs an empty, invalid query.
        Query() = default;

        /// \brief Constructs a query from a query handle.
        ///
        /// \param Handle The query handle.
        template<typename Type>
        Query(Type Handle)
            : mHandle { Handle }
        {
        }

        /// \brief Move constructor.
        ///
        /// \param Other The query object to move resources from.
        Query(AnyRef<Query> Other) noexcept
            : mHandle { Exchange(Other.mHandle, Handle()) }
        {
        }

        /// \brief Ensures the underlying query resources are properly released.
        ~Query()
        {
            Destruct();
        }

        /// \brief Explicitly releases the underlying query resources.
        void Destruct()
        {
            if (mHandle)
            {
                mHandle.destruct();
                mHandle = Handle();
            }
        }

        /// \brief Iterates over all entities matching the query, invoking the callback for each one.
        ///
        /// \param Callback The function to invoke.
        template<typename Function>
        void Each(AnyRef<Function> Callback)
        {
            mHandle.each(Callback);
        }

        /// \brief Runs a callback for the entire set of matching entities at once.
        ///
        /// \param Callback The function to invoke.
        template<typename Function>
        void Run(AnyRef<Function> Callback)
        {
            mHandle.run(Callback);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        mutable Handle mHandle;
    };
}