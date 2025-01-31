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

#include "Aurora.Base/Base.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Scene
{
    // -=(Undocumented)=-
    class Factory final
    {
    public:

        // -=(Undocumented)=-
        template<typename Type>
        using Serializer = FPtr<void(Ref<Type>, Ptr<void>)>;

    public:

        // -=(Undocumented)=-
        Factory() = default;

        // -=(Undocumented)=-
        Factory(Serializer<Reader> Reader, Serializer<Writer> Writer)
            : mReader { Move(Reader) },
              mWriter { Move(Writer) }
        {
        }

        // -=(Undocumented)=-
        void Read(Ref<Reader> Reader, Ptr<void> Component)
        {
            mReader(Reader, Component);
        }

        // -=(Undocumented)=-
        void Write(Ref<Writer> Writer, Ptr<void> Component)
        {
            mWriter(Writer, Component);
        }

    public:

        // -=(Undocumented)=-
        template<typename Component>
        static Factory Create()
        {
            return Factory(OnComponentRead<Component>, OnComponentWrite<Component>);
        }

    private:

        // -=(Undocumented)=-
        template<typename Type>
        static void OnComponentRead(Ref<Reader> Reader, Ptr<void> Component)
        {
            if constexpr (not std::is_empty_v<Type>)
            {
                static_cast<Ptr<Type>>(Component)->OnSerialize(Stream(Reader));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        static void OnComponentWrite(Ref<Writer> Writer, Ptr<void> Component)
        {
            if constexpr (not std::is_empty_v<Type>)
            {
                static_cast<Ptr<Type>>(Component)->OnSerialize(Stream(Writer));
            }
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Serializer<Reader> mReader;
        Serializer<Writer> mWriter;
    };
}