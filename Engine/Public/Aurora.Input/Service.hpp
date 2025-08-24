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

#include "Listener.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Poller.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Input
{
    /// \brief Provides high-level management of the input subsystem.
    class Service final : public AbstractService<Service>
    {
    public:

        /// \brief Constructs the input service and registers it with the system host.
        ///
        /// \param Host The system context that owns and manages this service.
        explicit Service(Ref<Host> Host);

        /// \brief Updates all input devices and dispatches events to registered listeners.
        ///
        /// This method is called once per frame by the engine and is responsible for
        /// polling devices via \ref Poller and updating mouse and keyboard states.
        ///
        /// \param Time The time step data for the current frame.
        void OnTick(ConstRef<Time> Time) override;

        /// \brief Adds an input listener to receive events from this service.
        ///
        /// \param Listener The listener instance to attach.
        void Attach(ConstTracker<Listener> Listener);

        /// \brief Removes an input listener so it no longer receives events.
        ///
        /// \param Listener The listener instance to detach.
        void Detach(ConstTracker<Listener> Listener);

        /// \brief Checks if a key was pressed in the current frame.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key transitioned from released to pressed this frame, otherwise `false`.
        AURORA_INLINE Bool IsKeyPressed(Key Key) const
        {
            return mKeyboard.IsKeyPressed(Key);
        }

        /// \brief Checks if a key is currently held down.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key is currently pressed, otherwise `false`.
        AURORA_INLINE Bool IsKeyHeld(Key Key) const
        {
            return mKeyboard.IsKeyHeld(Key);
        }

        /// \brief Checks if a key was released in the current frame.
        ///
        /// \param Key The key to check.
        /// \return `true` if the key transitioned from pressed to released this frame, otherwise `false`.
        AURORA_INLINE Bool IsKeyReleased(Key Key) const
        {
            return mKeyboard.IsKeyReleased(Key);
        }

        /// \brief Gets the current mouse position.
        ///
        /// \return The mouse position in window or screen coordinates.
        AURORA_INLINE Vector2 GetMousePosition() const
        {
            return mMouse.GetPosition();
        }

        /// \brief Gets the mouse scroll delta since the last frame.
        ///
        /// \return The mouse scroll delta vector.
        AURORA_INLINE Vector2 GetMouseScroll() const
        {
            return mMouse.GetScroll();
        }

        /// \brief Checks if a mouse button was pressed in the current frame.
        ///
        /// \param Button The mouse button to check.
        /// \return `true` if the button transitioned from released to pressed this frame, otherwise `false`.
        AURORA_INLINE Bool IsMouseButtonPressed(Button Button) const
        {
            return mMouse.IsButtonPressed(Button);
        }

        /// \brief Checks if a mouse button is currently held down.
        ///
        /// \param Button The mouse button to check.
        /// \return `true` if the button is currently pressed, otherwise `false`.
        AURORA_INLINE Bool IsMouseButtonHeld(Button Button) const
        {
            return mMouse.IsButtonHeld(Button);
        }

        /// \brief Checks if a mouse button was released in the current frame.
        ///
        /// \param Button The mouse button to check.
        /// \return `true` if the button transitioned from pressed to released this frame, otherwise `false`.
        AURORA_INLINE Bool IsMouseButtonReleased(Button Button) const
        {
            return mMouse.IsButtonReleased(Button);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Poller                                   mPoller;
        Mouse                                    mMouse;
        Keyboard                                 mKeyboard;
        Vector<Tracker<Listener>, kMaxListeners> mListeners;
    };
}
