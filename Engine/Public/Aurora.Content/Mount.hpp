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

#include "Aurora.Base/Memory/Trackable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Content
{
    /// \brief Interface for a virtual mount point providing read, write, and delete access to asset files.
    ///
    /// A mount abstracts an underlying storage system (e.g., filesystem directory, archive file, or remote source)
    /// and exposes uniform file access methods for the content system.
    class Mount : public Trackable
    {
    public:

        /// \brief Ensures derived mount implementations can be destroyed polymorphically.
        virtual ~Mount() = default;

        /// \brief Reads all bytes from a file located at the specified path.
        ///
        /// \param Path Path to the file within this mount.
        /// \return A \ref Blob containing the file's raw binary data.
        virtual Blob Read(ConstText Path) = 0;

        /// \brief Writes raw bytes to a file at the specified path.
        ///
        /// \param Path  Path to the file within this mount.
        /// \param Bytes Contiguous buffer of bytes to write.
        virtual void Write(ConstText Path, ConstSpan<Byte> Bytes) = 0;

        /// \brief Deletes a file located at the specified path.
        ///
        /// \param Path Path to the file within this mount.
        virtual void Delete(ConstText Path) = 0;
    };
}
