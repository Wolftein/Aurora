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

#include "Aurora.Audio/Driver.hpp"
#include <FAudio.h>
#include <FAudioFX.h>
#include <F3DAudio.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    /// \brief Encapsulate the audio driver implementation using FAudio.
    class FAudioDriver final : public Driver
    {
    public:

        /// \brief Constructs the driver with default settings.
        FAudioDriver();

        /// \brief Destructs the driver and releases all associated resources.
        ~FAudioDriver() override;

        /// \copydoc Driver::Initialize
        Bool Initialize(ConstText Device, UInt8 Submixes) override;

        /// \copydoc Driver::GetCapabilities
        ConstRef<Capabilities> GetCapabilities() const override;

        /// \copydoc Driver::Advance
        void Advance(ConstRef<Time> Time) override;

        /// \copydoc Driver::Suspend
        void Suspend() override;

        /// \copydoc Driver::Restore
        void Restore() override;

        /// \copydoc Driver::SetListener
        void SetListener(Ref<Listener> Listener) override;

        /// \copydoc Driver::SetMasterVolume
        void SetMasterVolume(Real32 Volume) override;

        /// \copydoc Driver::SetSubmixVolume
        void SetSubmixVolume(UInt8 Submix, Real32 Volume) override;

        /// \copydoc Driver::GetMasterVolume
        Real32 GetMasterVolume() override;

        /// \copydoc Driver::GetSubmixVolume
        Real32 GetSubmixVolume(UInt8 Submix) override;

        /// \copydoc Driver::Prepare
        Object Prepare(UInt8 Submix, ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter, Bool Repeat) override;

        /// \copydoc Driver::SetGain
        void SetGain(Object Instance, Real32 Gain) override;

        /// \copydoc Driver::SetPitch
        void SetPitch(Object Instance, Real32 Ratio) override;

        /// \copydoc Driver::Resume
        void Resume(Object Instance) override;

        /// \copydoc Driver::Pause
        void Pause(Object Instance) override;

        /// \copydoc Driver::Stop(Object, Bool)
        void Stop(Object Instance, Bool Immediately) override;

        /// \copydoc Driver::Stop(ConstTracker<Emitter>, Bool)
        void Stop(ConstTracker<Emitter> Emitter, Bool Immediately) override;

    private:

        /// \brief The operation identifier used for asynchronous operations.
        constexpr static UInt32 kOperation = ~0;

        /// \brief A pool of source voices indexed by sound format.
        using Voices   = Vector<Tuple<UInt32, Ptr<FAudioSourceVoice>>>;

        /// \brief Collection of submix voices used to route and mix audio sources.
        using Submixes = Vector<Ptr<FAudioSubmixVoice>, kMaxSubmixes>;

        /// \brief Defines an audio instance that represents a single playback of a sound asset.
        struct FAudioInstance final
        {
            /// \brief Updates the audio stream and submits new data to the playback voice.
            void Tick();

            /// \brief Releases all references held by this audio instance.
            void Release();

            /// The unique identifier for this audio instance.
            Object                 Handle;

            /// True if the instance should restart automatically when reaching the end.
            Bool                   Repeat;

            /// True if playback has reached the end and will stop unless looping.
            Bool                   Finished;

            /// The version of the instance, used to track changes.
            UInt32                 Version;

            /// The emitter that defines the position and properties of the sound.
            Tracker<Emitter>       Emitter;

            /// The sound asset being played.
            Tracker<Sound>         Sound;

            /// The sound asset decoder.
            Unique<Decoder>        Decoder;

            /// The source voice used for playback.
            Ptr<FAudioSourceVoice> Voice;

            /// The submix voice this instance is routed to.
            Ptr<FAudioSubmixVoice> Submix;
        };

        /// \brief Container for managing active and free audio instances.
        struct FAudioInstanceManager final
        {
            /// The allocator of audio instances.
            Handle<kMaxInstances>                      Allocator;

            /// The pool of free audio instances.
            Vector<FAudioInstance, kMaxInstances>      Pool;

            /// The active audio instances currently playing.
            Vector<Ptr<FAudioInstance>, kMaxInstances> Active;
        };

        /// \brief Finds the audio device matching the specified name and loads its capabilities.
        ///
        /// \param Device The name of the preferred audio device to search for.
        /// \return The index of the selected audio device in the device list.
        UInt32 FindDeviceAndLoadCapabilities(ConstText Device);

        /// \brief Retrieves a compatible source voice from the pool or creates a new one.
        ///
        /// \param Sound The sound whose format determines the source voice requirements.
        /// \return A pointer to a valid source voice for playback.
        Ptr<FAudioSourceVoice> GetOrCreateVoice(ConstTracker<Sound> Sound);

        /// \brief Updates 3D spatialization parameters for a playing instance.
        ///
        /// \param Instance The audio instance to process.
        /// \param Listener The 3D audio listener used as the reference point for calculations.
        /// \param Dirty    Whether the listener's properties have changed since the last update.
        void Process(Ref<FAudioInstance> Instance, Ref<F3DAUDIO_LISTENER> Listener, Bool Dirty);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<FAudio>               mDevice;
        Ptr<FAudioMasteringVoice> mMaster;
        Capabilities              mCapabilities;
        Submixes                  mSubmixes;
        Voices                    mVoices;
        FAudioInstanceManager     mInstances;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        F3DAUDIO_HANDLE           mCalculator;
        F3DAUDIO_LISTENER         mListener;
        Bool                      mListenerDirty;
    };
}