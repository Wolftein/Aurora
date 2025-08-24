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

#include "Utility.hpp"
#include <atomic>
#include <condition_variable>
#include <mutex>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Defines a platform-independent atomic type for lock-free synchronization.
    template<typename Type>
    using Atomic    = std::atomic<Type>;

    /// \brief Lock-free test-and-set flag for tiny critical sections.
    using Flag      = std::atomic_flag;

    /// \brief Alias for a standard condition variable used for thread synchronization.
    using Condition = std::condition_variable;

    /// \brief Defines standard mutual exclusion primitive.
    using Mutex     = std::mutex;

    /// \brief Lock guard for a \c Mutex, with optional RAII-style behavior.
    /// 
    /// This alias selects the locking strategy based on the \c Unique flag:
    /// - If \c Unique is `false` (default), uses `std::lock_guard<Mutex>`, which provides simple RAII-style locking.
    /// - If \c Unique is `true`, uses `std::unique_lock<Mutex>`, which allows deferred locking, manual unlock/relock,
    /// and is not strictly RAII-based.
    /// 
    /// \tparam Unique Whether to use a unique lock (`true`) or a scoped lock (`false`).
    template<Bool Unique = false>
    using Lock      = Switch<Unique, std::unique_lock<Mutex>, std::lock_guard<Mutex>>;

    /// \brief Alias for a standard joinable thread with automatic cleanup.
    using Thread    = std::jthread;
}