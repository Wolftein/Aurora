// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Tracy/Tracy.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
#define AURORA_PROFILE_CALLSTACK_DEPTH          15

/// \def AURORA_PROFILE
/// \brief Marks a profiling zone in the profiler.
#ifdef    AURORA_PROFILE_MODE
    #define AURORA_PROFILE ZoneScopedS(AURORA_PROFILE_CALLSTACK_DEPTH)
#else
    #define AURORA_PROFILE
#endif // AURORA_PROFILE_MODE

/// \def AURORA_PROFILE_SCOPE(Name)
/// \brief Marks a named profiling zone in the profiler.
#ifdef    AURORA_PROFILE_MODE
    #define AURORA_PROFILE_SCOPE(Name) ZoneScopedNS(Name, AURORA_PROFILE_CALLSTACK_DEPTH)
#else
    #define AURORA_PROFILE_SCOPE(Name)
#endif // AURORA_PROFILE_MODE

/// \def AURORA_PROFILE_FRAME(x)
/// \brief Marks the beginning of a new frame in the profiler.
#ifdef    AURORA_PROFILE_MODE
    #define AURORA_PROFILE_FRAME FrameMark
#else
    #define AURORA_PROFILE_FRAME
#endif // AURORA_PROFILE_MODE

/// \def AURORA_THREAD_NAME(x)
/// \brief
#ifdef    AURORA_PROFILE_MODE
    #define AURORA_THREAD_NAME(Name) tracy::SetThreadName(Name)
#else
    #define AURORA_THREAD_NAME(Name)
#endif // AURORA_PROFILE_MODE

}