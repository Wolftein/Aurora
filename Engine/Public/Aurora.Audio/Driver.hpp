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

#include "Emitter.hpp"
#include "Sound.hpp"
#include "Listener.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    /// \brief Abstract interface for low-level audio driver backends.
    class Driver
    {
    public:

        /// \brief Ensures derived drivers can be destroyed polymorphically.
        virtual ~Driver() = default;

        /// \brief Initializes the audio driver.
        ///
        /// \param Device   Identifier of the audio device to use.
        /// \param Submixes Number of submix channels to support.
        /// \return `true` if the driver initialized successfully, `false` otherwise.
        virtual Bool Initialize(ConstText Device, UInt8 Submixes) = 0;

        /// \brief Returns the hardware or driver capabilities.
        ///
        /// \return A reference to the capabilities structure describing supported features.
        virtual ConstRef<Capabilities> GetCapabilities() const = 0;

        /// \brief Advances the audio engine by one update cycle.
        ///
        /// \param Time The elapsed time since the last update cycle.
        virtual void Advance(ConstRef<Time> Time) = 0;

        /// \brief Suspends audio output.
        virtual void Suspend() = 0;

        /// \brief Resumes audio output after being suspended.
        virtual void Restore() = 0;

        /// \brief Sets the active listener used for spatial audio.
        ///
        /// \param Listener The active audio listener in world space.
        virtual void SetListener(Ref<Listener> Listener) = 0;

        /// \brief Sets the master volume.
        ///
        /// \param Volume Volume multiplier (`1.0` = default, `0.0` = mute).
        virtual void SetMasterVolume(Real32 Volume) = 0;

        /// \brief Sets the volume of a specific submix channel.
        ///
        /// \param Submix Index of the submix channel.
        /// \param Volume Volume multiplier (`1.0` = default, `0.0` = mute).
        virtual void SetSubmixVolume(UInt8 Submix, Real32 Volume) = 0;

        /// \brief Gets the current master volume.
        ///
        /// \return The current master volume multiplier.
        virtual Real32 GetMasterVolume() = 0;

        /// \brief Gets the volume of a specific submix channel.
        ///
        /// \param Submix Index of the submix channel.
        /// \return The current submix volume multiplier.
        virtual Real32 GetSubmixVolume(UInt8 Submix) = 0;

        /// \brief Begins playback of a sound instance.
        ///
        /// \param Submix  Index of the submix channel to route the audio through.
        /// \param Sound   Handle to the sound asset to play.
        /// \param Emitter Initial spatial and audio parameters for playback.
        /// \param Repeat  `true` to loop the sound, `false` to play once.
        /// \return An opaque handle representing the playback instance.
        virtual Object Prepare(UInt8 Submix, ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter, Bool Repeat) = 0;

        /// \brief Sets the playback volume for a sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        /// \param Gain     Linear volume multiplier (`1.0` = default, `0.0` = mute).
        virtual void SetGain(Object Instance, Real32 Gain) = 0;

        /// \brief Sets the playback pitch for a sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        /// \param Ratio    Playback rate multiplier (`1.0` = default, `0.5` = one octave lower, `2.0` = one octave higher).
        virtual void SetPitch(Object Instance, Real32 Ratio) = 0;

        /// \brief Resumes playback of a paused sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        virtual void Resume(Object Instance) = 0;

        /// \brief Pauses playback of an active sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        virtual void Pause(Object Instance) = 0;

        /// \brief Stops playback of a sound instance.
        ///
        /// \param Instance    Handle to the active sound instance.
        /// \param Immediately `true` to stop abruptly, `false` to allow fade-out.
        virtual void Stop(Object Instance, Bool Immediately) = 0;

        /// \brief Stops all sound instances associated with a given emitter.
        ///
        /// \param Emitter     Emitter associated with one or more active sound instances.
        /// \param Immediately `true` to stop abruptly, `false` to allow fade-out.
        virtual void Stop(ConstTracker<Emitter> Emitter, Bool Immediately) = 0;
    };
}
