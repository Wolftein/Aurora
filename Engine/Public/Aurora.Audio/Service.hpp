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

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    /// \brief Provides high-level management of the audio subsystem.
    class Service final : public AbstractService<Service>
    {
    public:

        /// \brief Constructs the audio service and attaches it to the system context.
        ///
        /// \param Host The parent system context managing this subsystem.
        explicit Service(Ref<Host> Host);

        /// \brief Advances the audio driver once per frame.
        ///
        /// \param Time The time step data for the current frame.
        void OnTick(ConstRef<Time> Time) override;

        /// \brief Initializes the audio driver with a backend and output device.
        ///
        /// Must be called before any playback.
        ///
        /// \param Backend The audio backend to use.
        /// \param Device  Name or identifier of the output device to initialize.
        /// \return `true` if initialization succeeded, `false` otherwise.
        Bool Initialize(Backend Backend, ConstText Device);

        /// \brief Sets the active listener used for spatial audio.
        ///
        /// \param Listener The active audio listener in world space.
        AURORA_INLINE void SetListener(Ref<Listener> Listener)
        {
            mDriver->SetListener(Listener);
        }

        /// \brief Sets the master volume applied to all sounds.
        ///
        /// \param Volume Volume multiplier (`1.0` = default, `0.0` = mute).
        AURORA_INLINE void SetMasterVolume(Real32 Volume)
        {
            mDriver->SetMasterVolume(Volume);
        }

        /// \brief Sets the volume for a category’s submix.
        ///
        /// \param Category Logical audio category.
        /// \param Volume   Volume multiplier (`1.0` = default, `0.0` = mute).
        AURORA_INLINE void SetSubmixVolume(Category Category, Real32 Volume)
        {
            mDriver->SetSubmixVolume(Enum::Cast(Category), Volume);
        }

        /// \brief Gets the master audio volume.
        ///
        /// \return Current master volume multiplier.
        AURORA_INLINE Real32 GetMasterVolume() const
        {
            return mDriver->GetMasterVolume();
        }

        /// \brief Gets the volume of a category’s submix.
        ///
        /// \param Category Logical audio category.
        /// \return Current submix volume multiplier.
        AURORA_INLINE Real32 GetCategoryVolume(Category Category) const
        {
            return mDriver->GetSubmixVolume(Enum::Cast(Category));
        }

        /// \brief Plays a music track.
        ///
        /// \param Sound   Handle to the music asset.
        /// \param Emitter Optional emitter for spatial playback.
        /// \param Repeat  `true` to loop, `false` to play once.
        void PlayMusic(ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter = nullptr, Bool Repeat = true);

        /// \brief Stops the currently playing music track.
        void StopMusic();

        /// \brief Plays a sound effect.
        ///
        /// \param Sound   Handle to the effect.
        /// \param Emitter Optional emitter for spatial playback.
        /// \param Repeat  `true` to loop, `false` to play once.
        /// \return Handle representing the playback instance.
        Object PlayEffect(ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter, Bool Repeat = false);

        /// \brief Plays a voice line.
        ///
        /// \param Sound   Handle to the voice line.
        /// \param Emitter Optional emitter for spatial playback.
        /// \return Handle representing the playback instance.
        Object PlayVoice(ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter);

        /// \brief Plays an ambient sound.
        ///
        /// \param Sound   Handle to the ambient track.
        /// \param Emitter Optional emitter for spatial playback.
        /// \param Repeat  `true` to loop, `false` to play once.
        /// \return Handle representing the playback instance.
        Object PlayAmbient(ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter = nullptr, Bool Repeat = true);

        /// \brief Plays an interface sound.
        ///
        /// \param Sound Handle to the sound.
        /// \return Handle representing the playback instance.
        Object PlayInterface(ConstTracker<Sound> Sound);

        /// \brief Sets the playback gain for a sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        /// \param Gain     Linear multiplier (`1.0` = default, `0.0` = mute).
        AURORA_INLINE void SetGain(Object Instance, Real32 Gain)
        {
            mDriver->SetGain(Instance, Gain);
        }

        /// \brief Sets the playback pitch for a sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        /// \param Ratio    Playback rate multiplier (`1.0` = default, `0.5` = one octave lower, `2.0` = one octave higher).
        AURORA_INLINE void SetPitch(Object Instance, Real32 Ratio)
        {
            mDriver->SetPitch(Instance, Ratio);
        }

        /// \brief Pauses playback of an active sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        AURORA_INLINE void Pause(Object Instance)
        {
            mDriver->Pause(Instance);
        }

        /// \brief Resumes playback of a paused sound instance.
        ///
        /// \param Instance Handle to the active sound instance.
        AURORA_INLINE void Resume(Object Instance)
        {
            mDriver->Resume(Instance);
        }

        /// \brief Stops playback of a sound instance.
        ///
        /// \param Instance    Handle to the active sound instance.
        /// \param Immediately `true` to stop abruptly, `false` to allow fade-out.
        AURORA_INLINE void Stop(Object Instance, Bool Immediately = false)
        {
            mDriver->Stop(Instance, Immediately);
        }

        /// \brief Stops all instances associated with an emitter.
        ///
        /// \param Emitter     Emitter associated with one or more active sound instances.
        /// \param Immediately `true` to stop abruptly, `false` to allow fade-out.
        AURORA_INLINE void Stop(ConstTracker<Emitter> Emitter, Bool Immediately = true)
        {
            mDriver->Stop(Emitter, Immediately);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Unique<Driver> mDriver;
        Object         mMusic;
    };
}
