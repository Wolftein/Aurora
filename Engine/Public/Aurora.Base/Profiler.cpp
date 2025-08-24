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

#include "Profiler.hpp"
#include "Primitive.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef AURORA_PROFILE_MODE

Ptr<void> operator new(std::size_t Count)
{
    Ptr<void> Pointer = std::malloc(Count);
    TracyAllocS(Pointer, Count, AURORA_PROFILE_CALLSTACK_DEPTH);
    return Pointer;
}

Ptr<void> operator new[](std::size_t Count)
{
    Ptr<void> Pointer = std::malloc(Count);
    TracyAllocS(Pointer, Count, AURORA_PROFILE_CALLSTACK_DEPTH);
    return Pointer;
}

void operator delete(Ptr<void> Pointer) noexcept
{
    TracyFreeS(Pointer, AURORA_PROFILE_CALLSTACK_DEPTH);
    std::free(Pointer);
}

void operator delete[](Ptr<void> Pointer) noexcept
{
    TracyFreeS(Pointer, AURORA_PROFILE_CALLSTACK_DEPTH);
    std::free(Pointer);
}

void operator delete(Ptr<void> Pointer, std::size_t Count) noexcept
{
    TracyFreeS(Pointer, AURORA_PROFILE_CALLSTACK_DEPTH);
    std::free(Pointer);
}

void operator delete[](Ptr<void> Pointer, std::size_t Count) noexcept
{
    TracyFreeS(Pointer, AURORA_PROFILE_CALLSTACK_DEPTH);
    std::free(Pointer);
}

#endif // AURORA_PROFILE_MODE
