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

#include "Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    /// \brief Represents the context for a resource load operation.
    ///
    /// Manages the resource being loaded and its dependent resources.
    /// A scope is considered "ready" when all dependencies have completed loading.
    class Scope final
    {
    public:

        /// \brief Maximum number of dependencies tracked by a single scope.
        constexpr static UInt32 kMaxDependencies = 14;

    public:

        /// \brief Creates an empty scope with no associated resource.
        AURORA_INLINE Scope()
            : mResource { nullptr }
        {
        }

        /// \brief Creates a scope for a specific resource.
        ///
        /// \param Resource The resource instance to associate with this scope.
        AURORA_INLINE explicit Scope(ConstTracker<Resource> Resource)
            : mResource { Resource }
        {
        }

        /// \brief Gets the resource associated with this scope.
        ///
        /// \return A reference-counted handle to the resource linked to this scope.
        AURORA_INLINE ConstTracker<Resource> GetResource() const
        {
            return mResource;
        }

        /// \brief Checks whether all dependent resources have finished loading.
        ///
        /// \return `true` if no dependencies remain, otherwise `false`.
        AURORA_INLINE Bool Poll()
        {
            for (UInt32 Element = 0; Element < mDependencies.size(); ++Element)
            {
                if (mDependencies[Element]->HasCompleted())
                {
                    Swap(mDependencies[Element], mDependencies.back());
                    mDependencies.pop_back();
                }
            }
            return mDependencies.empty();
        }

        /// \brief Adds a dependent resource to this scope.
        ///
        /// \param Dependency A reference-counted handle to the dependent resource.
        AURORA_INLINE void Mark(ConstTracker<Resource> Dependency)
        {
            mDependencies.emplace_back(Dependency);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Tracker<Resource>                           mResource;
        Vector<Tracker<Resource>, kMaxDependencies> mDependencies;
    };
}