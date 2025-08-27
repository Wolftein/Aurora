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

#include "Trait.hpp"
#include <array>
#include <format>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Casts an object to an rvalue, enabling move semantics.
    /// 
    /// \param Object The object to move.
    /// \return An rvalue reference to the object.
    template<typename Type>
    constexpr Unref<Type>&& Move(Type&& Object)
    {
        return static_cast<Unref<Type>&&>(Object);
    }

    /// \brief Perfectly forwards an object to another function.
    /// 
    /// \param Object The object to forward.
    /// \return An lvalue or rvalue reference depending on the input type.
    template<typename Type>
    constexpr Type&& Forward(Unref<Type>& Object)
    {
        return static_cast<Type&&>(Object);
    }

    /// \brief Perfectly forwards an object to another function.
    /// 
    /// \param Object The object to forward.
    /// \return An lvalue or rvalue reference depending on the input type.
    template<typename Type>
    constexpr Type&& Forward(Unref<Type>&& Object)
    {
        return static_cast<Type&&>(Object);
    }

    /// \brief Exchanges the value of an object with a new one, returning the old value.
    /// 
    /// \param First  The object to update.
    /// \param Second The new value to assign.
    /// \return The previous value held by the object.
    template<typename Type, typename Other = Type>
    constexpr Type Exchange(Ref<Type> First, AnyRef<Other> Second)
    {
        Type Temp = Move(First);
        First     = Forward<Other>(Second);
        return Temp;
    }

    /// \brief Swaps the values of two objects using move semantics.
    /// 
    /// \param First  The first object.
    /// \param Second The second object.
    template<typename Type>
    void Swap(Ref<Type> First, Ref<Type> Second)
    {
        Second = Exchange(First, Move(Second));
    }

    /// \brief Computes a 64-bit FNV-1a hash of a string view at compile or run time.
    /// 
    /// \param Block   The string data to hash.
    /// \param Counter Initial hash value (defaults to FNV offset basis).
    /// \return The 64-bit FNV-1a hash.
    constexpr auto Hash(ConstText Block, UInt64 Counter = 14695981039346656037ull)
    {
        for (const Char Character : Block)
        {
            Counter = (Counter ^ Character) * 1099511628211ull;
        }
        return Counter;
    }

    /// \brief Computes a compile-time hash for the type \c Type using its decorated name.
    /// 
    /// leverages the type name to compute a unique 64-bit FNV-1a hash at compile time.
    /// It is typically used for type-based identifiers or registration systems.
    /// 
    /// \tparam Type The type to compute the hash for.
    /// \return The 64-bit FNV-1a hash of the type's decorated name.
    template<typename Type>
    constexpr auto Hash()
    {
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
        return Hash(__PRETTY_FUNCTION__);
#else
        return Hash(__FUNCSIG__);
#endif
    }

    /// \brief Combines an existing hash value with a new value using a mixing function.
    ///
    /// \param Seed   Reference to the current hash seed. Updated in-place.
    /// \param Value  The value to incorporate into the hash.
    /// \return Reference to the updated seed, for chaining.
    template<typename Base, typename Type>
    constexpr auto Hash(Ref<Base> Seed, ConstRef<Type> Value)
    {
        std::hash<std::decay_t<decltype(Value)>> Hasher;

        Seed ^= Hasher(Value) + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
        return Seed;
    }

    /// \brief Extracts an element of the specified type from a tuple-like container.
    /// 
    /// \tparam Type The type of the element to retrieve.
    /// \param Tuple The tuple-like container to fetch from.
    /// \return A reference (or value, if rvalue) to the extracted element.
    template<typename Type, typename Container>
    constexpr auto Fetch(AnyRef<Container> Tuple)
    {
        return std::get<Type>(Forward<Container>(Tuple));
    }

    /// \brief Extracts an element of the specified type from a tuple-like container.
    ///
    /// \tparam Type The index of the element to retrieve.
    /// \param Tuple The tuple-like container to fetch from.
    /// \return A reference (or value, if rvalue) to the extracted element.
    template<UInt Type, typename Container>
    constexpr auto Fetch(AnyRef<Container> Tuple)
    {
        return std::get<Type>(Forward<Container>(Tuple));
    }

    /// \brief Formats a UTF-8 string using the specified arguments.
    /// 
    /// \param Format     The format string, following std::format syntax.
    /// \param Parameters The parameters to substitute into the format string.
    /// \return A formatted UTF-8 string.
    template<typename... Arguments>
    auto Format(ConstText Format, AnyRef<Arguments>... Parameters)
    {
        static thread_local std::array<Char, 2048> Buffer;

        const auto Result = std::vformat_to(Buffer.begin(), Format, std::make_format_args(Parameters...));
        (* Result) = '\0';

        return ConstText(Buffer.data(), std::distance(Buffer.begin(), Result));
    }

    /// \brief Binds the first N arguments of a callable and returns a new invocable.
    /// 
    /// creates a callable object with the specified parameters bound to the front
    /// of the original function's argument list. When the resulting invocable is called, it will
    /// prepend the captured arguments to any additional arguments provided at invocation time.
    /// 
    /// \tparam Type      The type of the callable (function, lambda, member function pointer, etc.).
    /// \tparam Arguments The types of the arguments to bind to the front.
    /// \param Function   The callable object to bind to.
    /// \param Parameters The arguments to capture and bind to the front of the callable.
    /// \return A new callable object with the bound arguments.
    /// 
    template<typename Type, typename... Arguments>
    constexpr auto Capture(AnyRef<Type> Function, AnyRef<Arguments>... Parameters)
    {
        return std::bind_front(Forward<Type>(Function), Forward<Arguments>(Parameters)...);
    }

    /// \brief Constructs an object of type `Type` in-place at the specified memory location.
    /// 
    /// performs a placement `new` using the provided memory address and constructor parameters.
    /// It is intended for use with manual memory management systems such as custom allocators or object pools.
    /// 
    /// \tparam Type The type of object to construct.
    /// \tparam Arguments The types of the constructor arguments.
    /// \param Memory     Pointer to the memory where the object will be constructed.
    /// \param Parameters The constructor arguments to forward.
    /// \return A reference to the newly constructed object.
    template<typename Type, typename... Arguments>
    constexpr decltype(auto) InPlaceCreate(Ptr<void> Memory, AnyRef<Arguments>... Parameters)
    {
        return * new (static_cast<Ptr<Type>>(Memory)) Type(Forward<Arguments>(Parameters)...);
    }

    /// \brief Constructs an object of type `Type` in-place at the specified memory location.
    ///
    /// performs a placement `new` using the provided memory address and constructor parameters.
    /// It is intended for use with manual memory management systems such as custom allocators or object pools.
    ///
    /// \tparam Type The type of object to construct.
    /// \tparam Arguments The types of the constructor arguments.
    /// \param Memory     Reference to the memory where the object will be constructed.
    /// \param Parameters The constructor arguments to forward.
    /// \return A reference to the newly constructed object.
    template<typename Type, typename... Arguments>
    constexpr decltype(auto) InPlaceConstruct(Ref<Type> Memory, AnyRef<Arguments>... Parameters)
    {
       return * ::new (static_cast<Ptr<Type>>(&Memory)) Type(Forward<Arguments>(Parameters)...);
    }

    /// \brief Destroys an object in-place if its type is non-trivially destructible.
    /// 
    /// explicitly calls the destructor of the given object, but only if the type
    /// is not trivially destructible. It is safe to use with objects allocated in custom memory
    /// pools or manually managed storage.
    /// 
    /// \tparam Type The type of the object.
    /// \param Object Reference to the object to destroy.
    template<typename Type>
    constexpr void InPlaceDelete(Ref<Type> Object) noexcept
    {
        if constexpr (!std::is_trivially_destructible_v<Type>)
        {
            Object.~Type();
        }
    }

    /// \brief Creates a \c Unique pointer to a newly constructed object.
    /// 
    /// \param Arguments The arguments to forward to the constructor.
    /// \return A \c Unique pointer managing the newly constructed object.
    template<typename Type, typename... Args>
    auto NewUniquePtr(AnyRef<Args> ... Arguments)
    {
        return std::make_unique<Type>(Forward<Args>(Arguments)...);
    }

    /// \brief Aligns an integer offset to the specified alignment.
    ///
    /// \tparam Alignment Alignment value in bytes.
    /// \param  Offset    Input value to align.
    /// \return Aligned value, greater than or equal to \p Offset.
    template<UInt Alignment>
    constexpr auto Align(UInt Offset)
    {
        constexpr Bool IsPow2 = (Alignment && ((Alignment & (Alignment - 1)) == 0));

        if constexpr (IsPow2)
        {
            return (Offset + (Alignment - 1)) & ~(Alignment - 1);
        }
        else
        {
            return ((Offset + Alignment - 1) / Alignment) * Alignment;
        }
    }

    /// \brief Aligns an integer offset to the specified alignment.
    ///
    /// \param  Offset    Input value to align.
    /// \param  Alignment Alignment value in bytes.
    /// \return Aligned value, greater than or equal to \p Offset.
    constexpr auto Align(UInt Offset, UInt Alignment)
    {
        return ((Offset + Alignment - 1) / Alignment) * Alignment;
    }

    /// \brief Converts 2D coordinates into a linear index.
    ///
    /// \param X     Horizontal coordinate.
    /// \param Y     Vertical coordinate.
    /// \param Width Number of elements per row.
    /// \return The 1D index corresponding to (X, Y).
    template<typename Type>
    constexpr auto Flatten2D(Type X, Type Y, Type Width)
    {
        return Y * Width + X;
    }

    /// \brief Converts 3D coordinates into a linear index.
    ///
    /// \param X      Horizontal coordinate.
    /// \param Y      Vertical coordinate.
    /// \param Z      Depth coordinate.
    /// \param Width  Number of elements per row.
    /// \param Height Number of rows per layer.
    /// \return The 1D index corresponding to (X, Y, Z).
    template<typename Type>
    constexpr auto Flatten3D(Type X, Type Y, Type Z, Type Width, Type Height)
    {
        return Z * (Width * Height) + Y * Width + X;
    }
}