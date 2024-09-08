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
#include "Poller.hpp"

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

        // \see Subsystem::OnTick(Real64)
        void OnTick(Real64 Time) override;

        // -=(Undocumented)=-
        bool IsKeyPressed(Key Key) const
        {
            return mKeyboard.IsKeyPressed(Key);
        }

        // -=(Undocumented)=-
        bool IsKeyHeld(Key Key) const
        {
            return mKeyboard.IsKeyHeld(Key);
        }

        // -=(Undocumented)=-
        Bool IsKeyReleased(Key Key) const
        {
            return mKeyboard.IsKeyReleased(Key);
        }

        // -=(Undocumented)=-
        Vector2i GetMousePosition() const
        {
            return mMouse.GetPosition();
        }

        // -=(Undocumented)=-
        Vector2i GetMouseDelta() const
        {
            return mMouse.GetDelta();
        }

        // -=(Undocumented)=-
        Vector2i GetMouseScroll() const
        {
            return mMouse.GetScroll();
        }

        // -=(Undocumented)=-
        bool IsMouseButtonPressed(Button Button) const
        {
            return mMouse.IsButtonPressed(Button);
        }

        // -=(Undocumented)=-
        bool IsMouseButtonHeld(Button Button) const
        {
            return mMouse.IsButtonHeld(Button);
        }

        // -=(Undocumented)=-
        Bool IsMouseButtonReleased(Button Button) const
        {
            return mMouse.IsButtonReleased(Button);
        }

        // -=(Undocumented)=-
        void Attach(ConstSPtr<Listener> Listener)
        {
            mDispatcher.emplace_back(Listener);
        }

        // -=(Undocumented)=-
        void Detach(ConstSPtr<Listener> Listener)
        {
            mDispatcher.erase(std::remove(mDispatcher.begin(), mDispatcher.end(), Listener));
        }

        // TODO: Action, Binding and Setting serialization

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Poller                 mPoller;
        Mouse                  mMouse;
        Keyboard               mKeyboard;
        Vector<SPtr<Listener>> mDispatcher;
    };
}