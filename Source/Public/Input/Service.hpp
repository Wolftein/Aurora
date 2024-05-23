// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Listener.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=(Undocumented)=-
    class Service final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Service(Ref<Context> System);

        // -=(Undocumented)=-
        void OnTick(UInt64 Tick) override;

        // -=(Undocumented)=-
        template<typename T>
        void Register(ConstSPtr<T> Device)
        {
            if constexpr (std::is_base_of_v<Keyboard, T>)
            {
                mKeyboard = Device;
            }
            else if constexpr (std::is_base_of_v<Mouse, T>)
            {
                mMouse = Device;
            }

            mDevices.emplace_back(Device);
        }

        // -=(Undocumented)=-
        template<typename T>
        void Unregister(ConstSPtr<T> Device)
        {
            if constexpr (std::is_base_of_v<Keyboard, T>)
            {
                mKeyboard = nullptr;
            }
            else if constexpr (std::is_base_of_v<Mouse, T>)
            {
                mMouse = nullptr;
            }

            mDevices.erase(std::remove(mDevices.begin(), mDevices.end(), Device));
        }

        // -=(Undocumented)=-
        void AddListener(ConstSPtr<Listener> Listener)
        {
            mListeners.emplace_back(Listener);
        }

        // -=(Undocumented)=-
        void RemoveListener(ConstSPtr<Listener> Listener)
        {
            mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), Listener));
        }

        // -=(Undocumented)=-
        bool IsKeyPressed(Key Key) const
        {
            return mKeyboard->IsKeyPressed(Key);
        }

        // -=(Undocumented)=-
        bool IsKeyHeld(Key Key) const
        {
            return mKeyboard->IsKeyHeld(Key);
        }

        // -=(Undocumented)=-
        Bool IsKeyReleased(Key Key) const
        {
            return mKeyboard->IsKeyReleased(Key);
        }

        // -=(Undocumented)=-
        Vector2i GetMousePosition() const
        {
            return mMouse->GetPosition();
        }

        // -=(Undocumented)=-
        Vector2i GetMouseDelta() const
        {
            return mMouse->GetDelta();
        }

        // -=(Undocumented)=-
        Vector2i GetMouseScroll() const
        {
            return mMouse->GetScroll();
        }

        // -=(Undocumented)=-
        bool IsMouseButtonPressed(Button Button) const
        {
            return mMouse->IsButtonPressed(Button);
        }

        // -=(Undocumented)=-
        bool IsMouseButtonHeld(Button Button) const
        {
            return mMouse->IsButtonHeld(Button);
        }

        // -=(Undocumented)=-
        Bool IsMouseButtonReleased(Button Button) const
        {
            return mMouse->IsButtonReleased(Button);
        }

        // TODO: Action, Binding and Setting serialization

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<SPtr<Device>>   mDevices;
        SPtr<Keyboard>         mKeyboard;
        SPtr<Mouse>            mMouse;
        Vector<SPtr<Listener>> mListeners;
    };
}