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

#include "Aurora.Audio/Service.hpp"

#include "Aurora.Content/Service.hpp"

#include "Aurora.Graphic/Service.hpp"

#include "Aurora.Input/Service.hpp"

#include "Device.hpp"

#include "Properties.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace Engine
{
    /// \brief Entry point and main coordinator of the application runtime.
    ///
    /// The Kernel initializes core systems, runs the main loop, and handles shutdown.
    class Kernel : public Service::Host, public Input::Listener
    {
    public:

        /// \brief Default constructor.
        Kernel();

        /// \brief Destructs the kernel and releases all associated resources.
        ~Kernel() override;

        /// \brief Initializes the kernel and core services.
        ///
        /// This must be called before entering the main loop. It configures the environment
        /// based on the provided mode and property set.
        ///
        /// \param Mode       Runtime mode (e.g., client, server).
        /// \param Properties Application configuration values.
        void Initialize(Mode Mode, ConstRef<Properties> Properties);

        /// \brief Polls the main execution loop once.
        void Poll();

        /// \brief Runs the main execution loop until the application exits.
        void Run();

        /// \brief Signals the application to exit gracefully.
        void Exit();

    private:

        /// \brief Called once during kernel initialization.
        ///
        /// Override this method to perform custom setup logic.
        ///
        /// \return `true` to continue running, otherwise `false`.
        virtual Bool OnInitialize()
        {
            return true;
        }

        /// \brief Called once during kernel shutdown.
        ///
        /// Override this method to clean up resources before the engine is destroyed.
        virtual void OnTeardown()
        {
        }

        /// \brief Called once per simulation tick.
        ///
        /// Override this method to update game logic or simulation systems.
        ///
        /// \param Tick Time information for the current frame.
        virtual void OnTick(ConstRef<Time> Tick)
        {
        }

    private:

        /// \brief Initializes client-specific services.
        ///
        /// Called when the kernel is running in client mode.
        ///
        /// \param Properties Application configuration values.
        void InitializeClientServices(ConstRef<Properties> Properties);

        /// \brief Initializes server-specific services.
        ///
        /// Called when the kernel is running in server mode.
        ///
        /// \param Properties Application configuration values.
        void InitializeServerServices(ConstRef<Properties> Properties);

        /// \brief Initializes services common to all runtime modes.
        ///
        /// Called when the kernel is running in any mode.
        ///
        /// \param Properties Application configuration values.
        void InitializeCommonServices(ConstRef<Properties> Properties);

    protected:

        /// \copydoc Listener::OnWindowExit
        Bool OnWindowExit() override;

        /// \copydoc Listener::OnWindowResize
        Bool OnWindowResize(UInt32 Width, UInt32 Height) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool           mActive;
        Time           mTime;
        Unique<Device> mDevice;
    };
}
