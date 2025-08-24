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

#include "Scope.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    /// \brief Forward declaration of the content service manager.
    class Service;

    /// \brief Interface for decoding binary content into resource objects.
    ///
    /// A loader interprets a binary \ref Blob and initializes a matching \ref Resource instance with its decoded data.
    class Loader : public Trackable
    {
    public:

        /// \brief Ensures derived class can be destroyed polymorphically.
        virtual ~Loader() = default;

        /// \brief Gets the list of file extensions this loader can decode.
        ///
        /// Extensions should be provided without a leading dot and in lowercase
        /// (e.g., `"png"`, `"obj"`). The loader will typically be registered to
        /// handle these extensions during content system initialization.
        ///
        /// \return A contiguous span of supported file extensions.
        virtual ConstSpan<ConstText> GetExtensions() const = 0;

        /// \brief Loads a resource from serialized binary data.
        ///
        /// Implementations should parse the provided \p Data and populate a resource
        /// instance in the context of the given \p Scope. This may involve creating
        /// dependent resources through \p Service or performing deferred initialization.
        ///
        /// \param Service Reference to the content service, used for loading dependencies.
        /// \param Scope   The load scope managing dependency tracking and finalization.
        /// \param Data    Raw binary data containing the serialized resource.
        /// \return `true` if the resource was successfully loaded, otherwise `false`.
        virtual Bool Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data) = 0;
    };

}