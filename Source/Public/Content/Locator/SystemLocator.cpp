// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "SystemLocator.hpp"

#ifdef    EA_PLATFORM_WINDOWS
    #include <shlobj_core.h>
#endif // EA_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SystemLocator::SystemLocator(CStr Path)
        : mPath(Path)
    {
#ifdef    EA_PLATFORM_WINDOWS
        ::SHCreateDirectoryEx(nullptr, Path.data(), nullptr);
#endif // EA_PLATFORM_WINDOWS
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Chunk SystemLocator::Read(CStr Path)
    {
        Chunk Output;

#ifdef EA_PLATFORM_WINDOWS
        constexpr UInt Access     = GENERIC_READ;
        constexpr UInt Attributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;

        const SStr   File   = Format("{}/{}", mPath, Path);
        const HANDLE Handle = ::CreateFile(File.c_str(), Access, 0, 0, OPEN_EXISTING, Attributes, 0);

        if (Handle != INVALID_HANDLE_VALUE)
        {
            Output = Chunk(::GetFileSize(Handle, nullptr));

            DWORD lpNumberOfBytesRead;
            ::ReadFile(Handle, Output.GetData(), Output.GetSize(), & lpNumberOfBytesRead, 0);

            ::CloseHandle(Handle);
        }
#endif // EA_PLATFORM_WINDOWS

        return Output;
	}

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SystemLocator::Write(CStr Path, CPtr<const UInt08> Data)
    {
#ifdef EA_PLATFORM_WINDOWS
        constexpr UInt Access     = GENERIC_WRITE;
        constexpr UInt Attributes = FILE_ATTRIBUTE_NORMAL;

        const SStr   File   = Format("{}/{}", mPath, Path);
        const HANDLE Handle = ::CreateFile(File.c_str(), Access, 0, 0, CREATE_ALWAYS, Attributes, 0);

        if (Handle != INVALID_HANDLE_VALUE)
        {
            DWORD lpNumberOfBytesWritten;
            ::WriteFile(Handle, Data.data(), Data.size_bytes(), & lpNumberOfBytesWritten, 0);

            ::CloseHandle(Handle);
        }
#endif // EA_PLATFORM_WINDOWS
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SystemLocator::Delete(CStr Path)
    {
#ifdef    EA_PLATFORM_WINDOWS
        const SStr   File   = Format("{}/{}", mPath, Path);

        ::DeleteFileA(File.c_str());
#endif // EA_PLATFORM_WINDOWS
    }
}