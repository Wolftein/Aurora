// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
/// \def AURORA_INLINE
/// \brief Defines a cross-platform inline directive with compiler-specific behavior.
#if defined(_MSC_VER)
    #define AURORA_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define AURORA_INLINE __attribute__((always_inline))
#else
    #define AURORA_INLINE inline
#endif

/// \def AURORA_ALIGN
/// \brief Defines a cross-platform alignment directive for data structures.
#if defined(_MSC_VER)
    #define AURORA_ALIGN(Alignment) __declspec(align(Alignment))
#elif defined(__GNUC__) || defined(__clang__)
    #define AURORA_ALIGN(Alignment) __attribute__((aligned(Alignment)))
#else
    #define AURORA_ALIGN(Alignment)
#endif

/// \def AURORA_DEPRECATED
/// \brief Marks a function or type as deprecated.
#if defined(_MSC_VER)
    #define AURORA_DEPRECATED(Message) __declspec(deprecated(Message))
#elif defined(__GNUC__) || defined(__clang__)
    #define AURORA_DEPRECATED(Message) __attribute__((deprecated(Message)))
#else
    #define AURORA_DEPRECATED(Message)
#endif

/// \def AURORA_PLATFORM_WINDOWS
/// \brief Defined as `1` when compiling for Windows platforms, `0` otherwise.
#if defined(_WIN32)
    #define AURORA_PLATFORM_WINDOWS 1
#else
    #define AURORA_PLATFORM_WINDOWS 0
#endif

/// \def AURORA_PLATFORM_LINUX
/// \brief Defined as `1` when compiling for Linux platforms, `0` otherwise.
#if defined(__linux__) && !defined(__ANDROID__)
    #define AURORA_PLATFORM_LINUX 1
#else
    #define AURORA_PLATFORM_LINUX 0
#endif

/// \def AURORA_PLATFORM_MACOS
/// \brief Defined as `1` when compiling for MacOS platforms, `0` otherwise.
#if defined(__APPLE__) && defined(__MACH__)
    #define AURORA_PLATFORM_MACOS 1
#else
    #define AURORA_PLATFORM_MACOS 0
#endif

/// \def AURORA_LITTLE_ENDIAN
/// \brief Defined as `1` when compiling to Little Endian platform, `0` otherwise.
#if defined(AURORA_PLATFORM_WINDOWS) || defined(AURORA_PLATFORM_LINUX) || defined(AURORA_PLATFORM_MACOS)
    #define AURORA_LITTLE_ENDIAN 1
#else
    #define AURORA_LITTLE_ENDIAN 0
#endif
}