// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "FAudioDriver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto GetAzimuth(UInt32 Layout)
    {
        constexpr Real32 kFrontCenter = 0.0f;
        constexpr Real32 kRight       = kPI<Real32> * 0.5f;   // +90°
        constexpr Real32 kLeft        = kPI<Real32> * 1.5f;   // -90° (or 270°)
        constexpr Real32 kBackCenter  = kPI<Real32> * 1.0f;   // 180°
        constexpr Real32 kFrontLeft   = kPI<Real32> * 1.75f;  // -45° (315°)
        constexpr Real32 kFrontRight  = kPI<Real32> * 0.25f;  // +45°
        constexpr Real32 kBackLeft    = kPI<Real32> * 1.25f;  // -135° (225°)
        constexpr Real32 kBackRight   = kPI<Real32> * 0.75f;  // +135°
        constexpr Real32 kLfeDummy    = kPI<Real32> * 1.0f;   // LFE: arbitrary (unused for panning)

        static constexpr Real32 kAzimuthTable[][8] =
        {
            /* Mono   */ { kFrontCenter },
            /* Stereo */ { kLeft,  kRight },
            /* 2.1    */ { kLeft,  kRight,  kLfeDummy },
            /* 4.0    */ { kFrontLeft, kFrontRight, kBackLeft,    kBackRight },
            /* 4.1    */ { kFrontLeft, kFrontRight, kLfeDummy,    kBackLeft, kBackRight },
            /* 5.1    */ { kFrontLeft, kFrontRight, kFrontCenter, kLfeDummy, kLeft,     kRight },
            /* 6.1    */ { kFrontLeft, kFrontRight, kFrontCenter, kLfeDummy, kBackLeft, kBackRight, kBackCenter },
            /* 7.1    */ { kFrontLeft, kFrontRight, kFrontCenter, kLfeDummy, kBackLeft, kBackRight, kLeft, kRight }
        };
        return const_cast<Ptr<Real32>>(kAzimuthTable[Layout - 1]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto GetKey(ConstTracker<Sound> Sound)
    {
        const UInt32 Key = Sound->GetLayout() << 16 | Sound->GetStride() << 8 | Enum::Cast(Sound->GetFormat());
        return Key;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto GetFormat(Format Value)
    {
        static constexpr UInt32 kMapping[] = {
            FAUDIO_FORMAT_MSADPCM,                   // Format::ADPCM
            FAUDIO_FORMAT_PCM,                       // Format::PCM
            FAUDIO_FORMAT_IEEE_FLOAT,                // Format::IEEE
        };
        return kMapping[Enum::Cast(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    auto GetVector(ConstRef<Vector3> Vector)
    {
        return F3DAUDIO_VECTOR { Vector.GetX(), Vector.GetY(), Vector.GetZ() };
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    FAudioDriver::FAudioDriver()
        : mDevice { nullptr },
          mMaster { nullptr }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    FAudioDriver::~FAudioDriver()
    {
        if (mMaster)
        {
            FAudioVoice_DestroyVoice(mMaster);
            mMaster = nullptr;
        }
        if (mDevice)
        {
            FAudio_StopEngine(mDevice);
            FAudio_Release(mDevice);
            mDevice = nullptr;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool FAudioDriver::Initialize(ConstText Device, UInt8 Submixes)
    {
        if (FAudioCreate(&mDevice, 0, FAUDIO_DEFAULT_PROCESSOR))
        {
            LOG_ERROR("FAudio: Failed to initialize");
            return false;
        }

        const UInt32 DeviceIndex = FindDeviceAndLoadCapabilities(Device);

        if (FAudio_CreateMasteringVoice(mDevice, &mMaster, 0, FAUDIO_DEFAULT_SAMPLERATE, 0, DeviceIndex, nullptr))
        {
            LOG_ERROR("FAudio: Failed to create master voice");
            return false;
        }

        FAudioVoiceDetails Details;
        FAudioVoice_GetVoiceDetails(mMaster, & Details);

        for (UInt32 Index = 0; Index < Submixes; ++Index)
        {
            FAudio_CreateSubmixVoice(
                mDevice, &mSubmixes[Index], Details.InputChannels, Details.InputSampleRate, 0, 0, nullptr, nullptr);
        }

        // Initialize the 3D audio calculator.
        UInt32 Mask;
        FAudioMasteringVoice_GetChannelMask(mMaster, &Mask);
        F3DAudioInitialize(Mask, 343.5f, mCalculator);
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    ConstRef<Capabilities> FAudioDriver::GetCapabilities() const
    {
        return mCapabilities;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Advance(ConstRef<Time> Time)
    {
        const Bool IsListenerDirty = Exchange(mListenerDirty, false);

        for (auto Iterator = mInstances.Active.begin(); Iterator != mInstances.Active.end(); )
        {
            const Ptr<FAudioInstance> InstancePtr = (* Iterator);

            FAudioVoiceState State;
            FAudioSourceVoice_GetState(InstancePtr->Voice, &State, FAUDIO_VOICE_NOSAMPLESPLAYED);

            // If the logical instance is marked finished *and* the underlying voice has
            // drained all submitted buffers, recycle the voice and retire the instance.
            if (InstancePtr->Finished && State.BuffersQueued == 0)
            {
                // Return the physical voice to the pool (keyed by the clip’s format/layout).
                mVoices.emplace_back(GetKey(InstancePtr->Sound), InstancePtr->Voice);

                // Reset per-instance state.
                InstancePtr->Release();

                // Release the instance id back to the allocator so it can be reused.
                mInstances.Allocator.Free(InstancePtr->Handle);

                // Remove from the active set; erase returns the next valid iterator.
                Iterator = mInstances.Active.erase(Iterator);
            }
            else
            {
                // Update spatialization only when the instance has an emitter bound.
                if (InstancePtr->Emitter)
                {
                    const UInt32 Version = InstancePtr->Emitter->GetVersion();

                    // Recompute the 3D matrix if the listener changed or the emitter version advanced.
                    Process(* InstancePtr, mListener, IsListenerDirty || (Version != InstancePtr->Version));

                    // Cache the emitter version for next tick to avoid redundant work.
                    InstancePtr->Version = Version;
                }

                // Advance to the next active instance.
                ++Iterator;
            }
        }

        // Apply all deferred commands to the audio thread.
        FAudio_CommitOperationSet(mDevice, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Suspend()
    {
        FAudio_StopEngine(mDevice);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Restore()
    {
        FAudio_StartEngine(mDevice);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetListener(Ref<Listener> Listener)
    {
        const F3DAUDIO_VECTOR Position    = GetVector(Listener.GetPosition());
        const F3DAUDIO_VECTOR Velocity    = GetVector(Listener.GetVelocity());
        const F3DAUDIO_VECTOR OrientFront = GetVector(Listener.GetOrientation().GetForward());
        const F3DAUDIO_VECTOR OrientTop   = GetVector(Listener.GetOrientation().GetUp());

        constexpr auto IsNotEqual = [](ConstRef<F3DAUDIO_VECTOR> First, ConstRef<F3DAUDIO_VECTOR> Second)
        {
            return !IsAlmostEqual(First.x, Second.x)
                || !IsAlmostEqual(First.y, Second.y)
                || !IsAlmostEqual(First.z, Second.z);
        };

        mListenerDirty = IsNotEqual(Position, mListener.Position)
                      || IsNotEqual(Velocity, mListener.Velocity)
                      || IsNotEqual(OrientFront, mListener.OrientFront)
                      || IsNotEqual(OrientTop, mListener.OrientTop);

        if (mListenerDirty)
        {
            mListener.Position    = Position;
            mListener.Velocity    = Velocity;
            mListener.OrientFront = OrientFront;
            mListener.OrientTop   = OrientTop;
            mListener.pCone       = nullptr;
        }
    }

    // -=-=-=-=-=-=-=-  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetMasterVolume(Real32 Volume)
    {
        FAudioVoice_SetVolume(mMaster, Volume, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetSubmixVolume(UInt8 Submix, Real32 Volume)
    {
        FAudioVoice_SetVolume(mSubmixes[Submix], Volume, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 FAudioDriver::GetMasterVolume()
    {
        Real32 Volume;
        FAudioVoice_GetVolume(mMaster, &Volume);
        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 FAudioDriver::GetSubmixVolume(UInt8 Submix)
    {
        Real32 Volume;
        FAudioVoice_GetVolume(mSubmixes[Submix], &Volume);
        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object FAudioDriver::Prepare(UInt8 Submix, ConstTracker<Sound> Sound, ConstTracker<Emitter> Emitter, Bool Repeat)
    {
        const Object Handle = mInstances.Allocator.Allocate();

        if (Handle > 0)
        {
            // Route the source voice to the chosen submix (with output filter enabled).
            FAudioSendDescriptor SendDescriptor[]
            {
                { FAUDIO_SEND_USEFILTER, mSubmixes[Submix] },
            };
            FAudioVoiceSends     SendList { 1, SendDescriptor };

            // Acquire or create a source voice for this clip, then set initial gains/ratio.
            const Ptr<FAudioSourceVoice> Voice = GetOrCreateVoice(Sound);
            FAudioVoice_SetOutputVoices(Voice, &SendList);
            FAudioVoice_SetVolume(Voice, 1.0f, kOperation);
            FAudioSourceVoice_SetFrequencyRatio(Voice, 1.0f, kOperation);

            // Initialize the logical playback instance and bind assets/voice/submix.
            Ref<FAudioInstance> Instance = mInstances.Pool[Handle];
            Instance.Handle   = Handle;
            Instance.Repeat   = Repeat;
            Instance.Finished = false;
            Instance.Version  = (Emitter ? Emitter->GetVersion() : 0);
            Instance.Emitter  = Emitter;
            Instance.Sound    = Sound;
            Instance.Decoder  = Sound::Decode(* Sound);
            Instance.Submix   = mSubmixes[Submix];
            Instance.Voice    = Voice;

            // If the emitter is provided, set up the spatialization parameters.
            if (Emitter)
            {
                Process(Instance, mListener, true);
            }

            // Submit up to kMaxBuffers or until sound ends.
            for (UInt Stream = 0; Stream < kMaxBuffers && !Instance.Finished; ++Stream)
            {
                Instance.Tick();
            }

            mInstances.Active.push_back(&Instance);
        }
        return Handle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetGain(Object Instance, Real32 Gain)
    {
        ConstRef<FAudioInstance> InstanceRef = mInstances.Pool[Instance];
        FAudioVoice_SetVolume(InstanceRef.Voice, Gain, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetPitch(Object Instance, Real32 Ratio)
    {
        ConstRef<FAudioInstance> InstanceRef = mInstances.Pool[Instance];
        FAudioSourceVoice_SetFrequencyRatio(
            InstanceRef.Voice, Clamp(Ratio, FAUDIO_MIN_FREQ_RATIO, FAUDIO_MAX_FREQ_RATIO), kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Resume(Object Instance)
    {
        ConstRef<FAudioInstance> InstanceRef = mInstances.Pool[Instance];
        FAudioSourceVoice_Start(InstanceRef.Voice, 0, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Pause(Object Instance)
    {
        ConstRef<FAudioInstance> InstanceRef = mInstances.Pool[Instance];
        FAudioSourceVoice_Stop(InstanceRef.Voice, 0, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Stop(Object Instance, Bool Immediately)
    {
        Ref<FAudioInstance> InstanceRef = mInstances.Pool[Instance];
        InstanceRef.Finished = true;

        if (Immediately)
        {
            FAudioSourceVoice_Stop(InstanceRef.Voice, 0, kOperation);
            FAudioSourceVoice_FlushSourceBuffers(InstanceRef.Voice);
        }
        FAudioSourceVoice_ExitLoop(InstanceRef.Voice, kOperation);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Stop(ConstTracker<Emitter> Emitter, Bool Immediately)
    {
        for (const Ptr<FAudioInstance> InstancePtr : mInstances.Active)
        {
            if (InstancePtr->Emitter == Emitter)
            {
                Stop(InstancePtr->Handle, Immediately);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt32 FAudioDriver::FindDeviceAndLoadCapabilities(ConstText Device)
    {
        UInt32 Count = 0;
        FAudio_GetDeviceCount(mDevice, & Count);

        UInt32 Result = 0;

        // Query all devices and try to find the best one to use
        for (UInt Element = 0; Element < Count; ++Element)
        {
            FAudioDeviceDetails Details;
            FAudio_GetDeviceDetails(mDevice, Element, & Details);

            Ref<Adapter> Adapter = mCapabilities.Adapters.emplace_back();
            Adapter.Name = ConvertUTF16ToUTF8(reinterpret_cast<Ptr<wchar_t>>(Details.DisplayName));

            if (Adapter.Name == Device)
            {
                Result = Element;
            }
            else
            {
                // If we didn't find the device, use the default one
                if (Result == 0 && (Details.Role & FAudioDefaultGameDevice))
                {
                    Result = Element;
                }
            }
        }

        mCapabilities.Backend = Backend::FAudio;
        mCapabilities.Device  = mCapabilities.Adapters[Result].Name;
        return Result;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<FAudioSourceVoice> FAudioDriver::GetOrCreateVoice(ConstTracker<Sound> Sound)
    {
        const auto FindByKey = [Key = GetKey(Sound)](ConstRef<Tuple<UInt32, Ptr<FAudioSourceVoice>>> Pair)
        {
            return Fetch<UInt32>(Pair) == Key;
        };

        Ptr<FAudioSourceVoice> Voice = nullptr;

        if (const auto Iterator = std::ranges::find_if(mVoices, FindByKey); Iterator != mVoices.end())
        {
            // Reuse an existing voice from the pool.
            Voice = Fetch<Ptr<FAudioSourceVoice>>(* Iterator);

            mVoices.erase(Iterator);
        }
        else
        {
            FAudioWaveFormatEx Description { };
            Description.wFormatTag      = GetFormat(Sound->GetFormat());
            Description.wBitsPerSample  = Sound->GetStride() << 3;
            Description.nChannels       = Sound->GetLayout();
            Description.nSamplesPerSec  = Sound->GetFrequency();
            Description.nBlockAlign     = Description.nChannels * Sound->GetStride();
            Description.nAvgBytesPerSec = Description.nSamplesPerSec * Description.nBlockAlign;
            Description.cbSize          = 0;

            static FAudioVoiceCallback kCallback =
            {
                [](Ptr<FAudioVoiceCallback>, Ptr<void> Context)
                {
                    static_cast<Ptr<FAudioInstance>>(Context)->Tick();
                },
                [](Ptr<FAudioVoiceCallback>, Ptr<void>) {},
                [](Ptr<FAudioVoiceCallback>, Ptr<void>) {},
                [](Ptr<FAudioVoiceCallback>) {},
                [](Ptr<FAudioVoiceCallback>, Ptr<void>, UInt32) {},
                [](Ptr<FAudioVoiceCallback>) {},
                [](Ptr<FAudioVoiceCallback>, UInt32) {}
            };

            // Create a new voice since none were available in the pool.
            FAudio_CreateSourceVoice(mDevice, &Voice, &Description,
                FAUDIO_VOICE_USEFILTER, FAUDIO_MAX_FREQ_RATIO, &kCallback, nullptr, nullptr);
        }

        if (Voice)
        {
            FAudioSourceVoice_SetSourceSampleRate(Voice, Sound->GetFrequency());
        }
        return Voice;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Process(Ref<FAudioInstance> Instance, Ref<F3DAUDIO_LISTENER> Listener, Bool Dirty)
    {
        UInt32 Flags = F3DAUDIO_CALCULATE_DOPPLER | F3DAUDIO_CALCULATE_LPF_DIRECT;
        if (Dirty)
        {
            Flags |= F3DAUDIO_CALCULATE_MATRIX;
        }

        FAudioVoiceDetails Details;
        FAudioVoice_GetVoiceDetails(Instance.Submix, &Details);

        F3DAUDIO_EMITTER Emitter { };
        Emitter.Position            = GetVector(Instance.Emitter->GetPosition());
        Emitter.Velocity            = GetVector(Instance.Emitter->GetVelocity());
        Emitter.OrientFront         = GetVector(Instance.Emitter->GetOrientation().GetForward());
        Emitter.OrientTop           = GetVector(Instance.Emitter->GetOrientation().GetUp());
        Emitter.ChannelCount        = (Instance.Sound)->GetLayout();
        Emitter.DopplerScaler       = 1.0f;
        Emitter.CurveDistanceScaler = 1.0f;
        Emitter.InnerRadius         = Instance.Emitter->GetRadius();
        Emitter.InnerRadiusAngle    = kPI<Real32> * 0.25f;

        if (Emitter.ChannelCount > 1)
        {
            Emitter.ChannelRadius    = 0.25f;
            Emitter.pChannelAzimuths = GetAzimuth(Emitter.ChannelCount);
        }

        F3DAUDIO_DSP_SETTINGS Settings { };
        Real32 Matrix[FAUDIO_MAX_AUDIO_CHANNELS * 8] = { };
        Settings.SrcChannelCount = Emitter.ChannelCount;
        Settings.DstChannelCount = Details.InputChannels;
        Settings.pMatrixCoefficients = Matrix;

        // Calculate the 3D audio parameters
        F3DAudioCalculate(mCalculator, &Listener, &Emitter, Flags, &Settings);

        // Apply Doppler (always)
        FAudioSourceVoice_SetFrequencyRatio(Instance.Voice, Settings.DopplerFactor, kOperation);

        // Apply LPF (always)
        FAudioFilterParameters Filter { };
        Filter.Type      = FAudioFilterType::FAudioLowPassFilter;
        Filter.Frequency = Clamp(Settings.LPFDirectCoefficient, 0.0f, 1.0f);
        Filter.OneOverQ  = 1.0f;
        FAudioVoice_SetFilterParameters(Instance.Voice, &Filter, kOperation);

        // Apply the calculated parameters to the voice
        if (Dirty)
        {
            FAudioVoice_SetOutputMatrix(
                Instance.Voice,
                Instance.Submix,
                Settings.SrcChannelCount,
                Settings.DstChannelCount,
                Settings.pMatrixCoefficients,
                kOperation);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::FAudioInstance::Tick()
    {
        // Already signaled finished? Nothing to do.
        if (Finished)
        {
            return;
        }

        // Pull a chunk of decoded PCM from the decoder.
        if (ConstSpan<Byte> Frames = Decoder->Read(); !Frames.empty())
        {
            // Check if this read positioned the decoder at the end of the asset.
            // If we hit the end and looping is disabled, flag the instance for finalization.
            const Bool HasFinished = Sound->GetDuration() == Decoder->Tell();
            Finished = (HasFinished && !Repeat);

            // Submit the block to the source voice queue.
            FAudioBuffer Buffer { };
            Buffer.pAudioData = Frames.data();
            Buffer.AudioBytes = Frames.size_bytes();
            Buffer.Flags      = Finished ? FAUDIO_END_OF_STREAM : 0;
            Buffer.pContext   = this;
            FAudioSourceVoice_SubmitSourceBuffer(Voice, &Buffer, nullptr);

            // If we reached the end but looping is enabled, rewind the decoder for the next tick.
            if (HasFinished && Repeat)
            {
                Decoder->Seek(0);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::FAudioInstance::Release()
    {
        Emitter = nullptr;
        Sound   = nullptr;
        Decoder = nullptr;
    }
}