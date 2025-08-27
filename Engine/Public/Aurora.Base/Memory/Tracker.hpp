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

#include "Trackable.hpp"
#include "Aurora.Base/Utility.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Intrusive smart pointer for managing the lifetime of `Trackable` objects.
    /// 
    /// `Tracker<T>` is a smart pointer that retains a reference to a `Trackable` object.
    /// It increments the reference count on construction and decrements it on destruction.
    /// This provides shared ownership semantics without allocating an external control block.
    template<class Type>
    class Tracker final
    {
        template<typename> friend class Tracker;

    public:

        /// \brief Constructs a null tracker.
        AURORA_INLINE constexpr Tracker()
            : mInstance { nullptr }
        {
        }

        /// \brief Constructs a null tracker from nullptr.
        AURORA_INLINE constexpr Tracker(std::nullptr_t)
            : mInstance { nullptr }
        {
        }

        /// \brief Constructs a tracker from a raw pointer.
        /// 
        /// \param Instance The object to track. Its reference count is incremented.
        AURORA_INLINE Tracker(Ptr<Type> Instance)
            : mInstance { Instance }
        {
            Acquire();
        }

        /// \brief Constructs a tracker from another compatible tracker.
        /// 
        /// \tparam Derived The tracked type, must be derived from `Type`.
        /// \param Other The other tracker to copy from.
        template<typename Derived>
        AURORA_INLINE Tracker(ConstRef<Tracker<Derived>> Other) requires IsDerived<Type, Derived>
            : mInstance(static_cast<Ptr<Type>>(Other.mInstance))
        {
            Acquire();
        }

        /// \brief Copy constructor.
        /// 
        /// \param Other The tracker to copy from.
        AURORA_INLINE Tracker(ConstRef<Tracker> Other)
            : mInstance { Other.mInstance }
        {
            Acquire();
        }

        /// \brief Move constructor.
        /// 
        /// \param Other The tracker to move from.
        AURORA_INLINE constexpr Tracker(AnyRef<Tracker> Other)
            : mInstance { Other.mInstance }
        {
            Other.mInstance = nullptr;
        }

        /// \brief Move constructor from a compatible tracker.
        /// 
        /// \tparam Derived The source type, must be derived from `Type`.
        /// \param Other The tracker to move from.
        template<typename Derived>
        AURORA_INLINE Tracker(AnyRef<Tracker<Derived>> Other) requires IsDerived<Type, Derived>
            : mInstance(static_cast<Ptr<Type>>(Other.mInstance))
        {
            Other.mInstance = nullptr;
        }

        /// \brief Destructor. Decrements the reference count.
        AURORA_INLINE ~Tracker()
        {
            Release();
        }

        /// \brief Copy assignment operator.
        /// 
        /// \param Other The tracker to copy from.
        /// \return Reference to this.
        AURORA_INLINE Ref<Tracker> operator=(ConstRef<Tracker> Other)
        {
            if (this != &Other)
            {
                // Releases the current reference, if any.
                Release();

                // Assigns the pointer from another intrusive pointer (shared ownership).
                mInstance = Other.mInstance;

                // Increments the reference count on the newly assigned instance.
                Acquire();
            }
            return (* this);
        }

        /// \brief Move assignment operator.
        /// 
        /// \param Other The tracker to move from.
        /// \return Reference to this.
        AURORA_INLINE Ref<Tracker> operator=(AnyRef<Tracker> Other)
        {
            if (this != &Other)
            {
                // Releases the current reference, if any.
                Release();

                // Transfers the pointer from the other instance and nulls it in the source.
                mInstance = Exchange(Other.mInstance, nullptr);
            }
            return (* this);
        }

        /// \brief Dereferences the tracked object.
        /// 
        /// \return A reference to the tracked object.
        AURORA_INLINE constexpr Ref<Type> operator*() const
        {
            return * mInstance;
        }

        /// \brief Accesses members of the tracked object.
        /// 
        /// \return The raw pointer to the object.
        AURORA_INLINE constexpr Ptr<Type> operator->() const
        {
            return mInstance;
        }

        /// \brief Checks if two trackers refer to the same object.
        /// 
        /// \param Other The other tracker.
        /// \return `true` if both track the same instance, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Tracker> Other) const
        {
            return mInstance == Other.mInstance;
        }

        /// \brief Checks if two trackers refer to different objects.
        /// 
        /// \param Other The other tracker.
        /// \return `true` if they track different instances, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Tracker> Other) const
        {
            return mInstance != Other.mInstance;
        }

        /// \brief Checks if the tracker is null.
        /// 
        /// \return `true` if it does not track any object, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(std::nullptr_t) const
        {
            return mInstance == nullptr;
        }

        /// \brief Checks if the tracker is not null.
        /// 
        /// \return `true` if it tracks a valid object, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(std::nullptr_t) const
        {
            return mInstance != nullptr;
        }

        /// \brief Checks whether this tracker holds a valid object.
        /// 
        /// \return `true` if valid, `false` otherwise.
        AURORA_INLINE constexpr operator Bool() const
        {
            return mInstance != nullptr;
        }

    private:

        /// \brief Increments the reference count of the tracked object.
        AURORA_INLINE void Acquire()
        {
            if (mInstance)
            {
                mInstance->Acquire();
            }
        }

        /// \brief Decrements the reference count and resets the pointer if necessary.
        AURORA_INLINE void Release()
        {
            if (mInstance)
            {
                mInstance->Release();
                mInstance = nullptr;
            }
        }

    public:

        /// \brief Constructs a new object and returns a tracker managing it.
        /// 
        /// \param Parameters Arguments forwarded to the constructor of `Type`.
        /// \return A tracker managing the new object.
        template<typename... Arguments>
        AURORA_INLINE static Tracker Create(AnyRef<Arguments>... Parameters)
        {
            return Tracker(new Type(Forward<Arguments>(Parameters)...));
        }

        /// \brief Casts a tracker of a derived type to a base tracker type.
        /// 
        /// \param Other The tracker to cast.
        /// \return A tracker of the desired base type.
        template<typename Base>
        AURORA_INLINE static Tracker Cast(ConstRef<Tracker<Base>> Other) requires IsDerived<Base, Type>
        {
            return Tracker(reinterpret_cast<Ptr<Type>>(Other.mInstance));
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<Type> mInstance;
    };

    /// \brief Read-only reference to a `Tracker` smart pointer.
    /// 
    /// `ConstTracker<T>` allows read-only access to tracked objects without increasing
    /// the reference count or taking ownership.
    template<class Type>
    using ConstTracker = ConstRef<Tracker<Type>>;
}