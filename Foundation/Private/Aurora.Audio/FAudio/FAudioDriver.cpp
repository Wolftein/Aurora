// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
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

    static auto As(CStr16 Value)
    {
        SStr Result;
        Result.reserve(Value.size() * 4);

        for (auto Iterator = Value.begin(); Iterator != Value.end(); ++Iterator)
        {
            const UInt16 Character = (* Iterator);
            UInt32 Codepoint = 0;

            if (Character >= 0xD800 && Character <= 0xDBFF)
            {
                const UInt16 Surrogate = * (Iterator++);
                Codepoint = ((Character - 0xD800) << 10) | (Surrogate - 0xDC00);
                Codepoint += 0x10000;
            }
            else
            {
                Codepoint = Character;
            }

            if (Codepoint <= 0x7F)
            {
                Result.push_back(static_cast<char>(Codepoint));
            }
            else if (Codepoint <= 0x7FF)
            {
                Result.push_back(0xC0 | (Codepoint >> 6));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
            else if (Codepoint <= 0xFFFF)
            {
                Result.push_back(0xE0 | (Codepoint >> 12));
                Result.push_back(0x80 | ((Codepoint >> 6) & 0x3F));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
            else if (Codepoint <= 0x10FFFF)
            {
                Result.push_back(0xF0 | (Codepoint >> 18));
                Result.push_back(0x80 | ((Codepoint >> 12) & 0x3F));
                Result.push_back(0x80 | ((Codepoint >> 6) & 0x3F));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
        }
        return Result;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetAzimuth(UInt32 Channels)
    {
        constexpr static Real32 K_Left         = 3 * k_PI / 2;
        constexpr static Real32 k_Right        = k_PI / 2;
        constexpr static Real32 k_FrontLeft    = 7 * k_PI / 4;
        constexpr static Real32 k_FrontRight   = k_PI / 4;
        constexpr static Real32 k_FrontCenter  = 0.0f;
        constexpr static Real32 k_LowFrequency = k_PI;
        constexpr static Real32 k_BackLeft     = 5 * k_PI / 4;
        constexpr static Real32 k_BackRight    = 3 * k_PI / 4;
        constexpr static Real32 k_BackCenter   = k_PI;

        constexpr static Real32 k_Mapping[9][8] =
            {
                /* 0 */   { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
                /* 1 */   { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
                /* 2 */   { K_Left, k_Right, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
                /* 2.1 */ { K_Left, k_Right, k_LowFrequency, 0.f, 0.f, 0.f, 0.f, 0.f },
                /* 4.0 */ { K_Left, k_Right, k_BackLeft, k_BackRight, 0.f, 0.f, 0.f, 0.f },
                /* 4.1 */ { K_Left, k_Right, k_LowFrequency, k_BackLeft, k_BackRight, 0.f, 0.f, 0.f },
                /* 5.1 */ { K_Left, k_Right, k_FrontCenter, k_LowFrequency, k_BackLeft, k_BackRight, 0.f, 0.f },
                /* 6.1 */ { K_Left, k_Right, k_FrontCenter, k_LowFrequency, k_BackLeft, k_BackRight, k_BackCenter, 0.f },
                /* 7.1 */ { K_Left, k_Right, k_FrontCenter, k_LowFrequency, k_BackLeft, k_BackRight, K_Left, k_Right }
            };
        return const_cast<Ptr<Real32>>(& k_Mapping[Channels][0]);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetKey(ConstSPtr<Sound> Sound)
    {
        const UInt32 Key = Sound->GetChannel() << 16 | Sound->GetDepth() << 8 | CastEnum(Sound->GetKind());
        return Key;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetFormat(Kind Value)
    {
        static constexpr SInt32 k_Mapping[] = {
            FAUDIO_FORMAT_MSADPCM,                   // Format::ADPCM
            FAUDIO_FORMAT_PCM,                       // Format::PCM
            FAUDIO_FORMAT_IEEE_FLOAT,                // Format::IEEE
        };
        return k_Mapping[CastEnum(Value)];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetVector(ConstRef<Vector3f> Vector)
    {
        F3DAUDIO_VECTOR Conversion;
        Conversion.x = Vector.GetX();
        Conversion.y = Vector.GetY();
        Conversion.z = Vector.GetZ();
        return Conversion;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    FAudioDriver::FAudioDriver()
        : mDevice { nullptr },
          mMaster { nullptr },
          mDirty  { false }
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

    Bool FAudioDriver::Initialize(CStr Device, UInt8 Submixes)
    {
        if (FAudioCreate(& mDevice, 0, FAUDIO_DEFAULT_PROCESSOR))
        {
            Log::Error("FAudio: Failed to initialize");
            return false;
        }

        const UInt32 DeviceID = FindDeviceAndLoadCapabilities(Device);

        if (FAudio_CreateMasteringVoice(
            mDevice, & mMaster, FAUDIO_DEFAULT_CHANNELS, FAUDIO_DEFAULT_SAMPLERATE, 0, DeviceID, nullptr))
        {
            Log::Error("FAudio: Failed to create master voice");
            return false;
        }

        // Initialize Submixes
        FAudioVoiceDetails Details;
        FAudioVoice_GetVoiceDetails(mMaster, & Details);

        for (UInt32 Index = 0; Index < Submixes; ++Index)
        {
            FAudio_CreateSubmixVoice(
                mDevice, & mSubmixes[Index], Details.InputChannels, Details.InputSampleRate, 0, 0, nullptr, nullptr);
        }

        // Initialize X3D audio for positional 3D voices
        UInt32 Mask;
        FAudioMasteringVoice_GetChannelMask(mMaster, & Mask);
        F3DAudioInitialize(Mask, k_SpeedOfSound, mProcessor);

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

    void FAudioDriver::Tick()
    {
        // Ensures that the listener configuration remains consistent throughout the entire tick,
        // preserving the audio environment's state for accurate 3D audio processing during this period.
        F3DAUDIO_LISTENER Listener = mListener;

        for (auto Iterator = mMixes.begin(); Iterator != mMixes.end(); )
        {
            const Ptr<FAudioInstance> InstancePtr = * Iterator;

            FAudioVoiceState State;
            FAudioSourceVoice_GetState(InstancePtr->Source, & State, FAUDIO_VOICE_NOSAMPLESPLAYED);

            if (InstancePtr->Finish && State.BuffersQueued == 0)
            {
                mVoices.push_back(FAudioChannel { GetKey(InstancePtr->Sound), InstancePtr->Source });

                mInstances.Free(InstancePtr->ID);

                Iterator = mMixes.erase(Iterator);
            }
            else
            {
                if (InstancePtr->Emitter && (InstancePtr->Emitter->GetDirty() || mDirty))
                {
                    Process(* InstancePtr, Listener);
                }
                ++Iterator;
            }
        }
        mDirty = false;
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
        mListener.Position    = GetVector(Listener.GetPosition());
        mListener.Velocity    = GetVector(Listener.GetVelocity());
        mListener.OrientFront = GetVector(Listener.GetOrientation().GetForward());
        mListener.OrientTop   = GetVector(Listener.GetOrientation().GetUp());
        mDirty                = mDirty || Listener.GetDirty();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetMasterVolume(Real32 Volume)
    {
        FAudioVoice_SetVolume(mMaster, Volume, FAUDIO_COMMIT_NOW);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetSubmixVolume(UInt8 Submix, Real32 Volume)
    {
        FAudioVoice_SetVolume(mSubmixes[Submix], Volume, FAUDIO_COMMIT_NOW);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 FAudioDriver::GetMasterVolume()
    {
        Real32 Volume;
        FAudioVoice_GetVolume(mMaster, & Volume);
        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 FAudioDriver::GetSubmixVolume(UInt8 Submix)
    {
        Real32 Volume;
        FAudioVoice_GetVolume(mSubmixes[Submix], & Volume);
        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object FAudioDriver::Play(UInt8 Submix, ConstSPtr<Sound> Sound, ConstSPtr<Emitter> Emitter, Bool Repeat)
    {
        const Object ID = mInstances.Allocate();

        if (ID > 0)
        {
            // Create a \see FAudioSourceVoice from a pool of reusable object(s).
            FAudioSendDescriptor   SendDescriptor[]
            {
                { FAUDIO_SEND_USEFILTER, mSubmixes[Submix] },
            };
            const FAudioVoiceSends SendList
            {
                1, SendDescriptor
            };

            const Ptr<FAudioSourceVoice> Source = GetOrCreateVoice(Sound);
            FAudioVoice_SetOutputVoices(Source, AddressOf(SendList));
            FAudioVoice_SetVolume(Source, 1.0f, FAUDIO_COMMIT_NOW);
            FAudioSourceVoice_SetFrequencyRatio(Source, 1.0f, FAUDIO_COMMIT_NOW);
            FAudioSourceVoice_SetSourceSampleRate(Source, Sound->GetFrequency());

            // Create a \see FAudioInstance from a pool of reusable object(s).
            Ref<FAudioInstance> Instance = mInstances[ID];
            Instance.ID      = ID;
            Instance.Finish  = false;
            Instance.Loop    = Repeat;
            Instance.Emitter = Emitter;
            Instance.Sound   = Sound;
            Instance.Submix  = mSubmixes[Submix];
            Instance.Source  = Source;

            // Fill initial buffer(s) from the start of the audio clip
            Sound->Seek(0);

            for (UInt Stream = 0; Stream < k_MaxBuffers && !Instance.Finish; ++Stream)
            {
                Write(Instance);
            }

            mMixes.push_back(& Instance);
        }
        return ID;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::SetGain(Object Instance, Real32 Gain)
    {
        if (const Ptr<FAudioInstance> InstancePtr = Find(Instance))
        {
            FAudioVoice_SetVolume(InstancePtr->Source, Gain, FAUDIO_COMMIT_NOW);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Start(Object Instance)
    {
        if (const Ptr<FAudioInstance> InstancePtr = Find(Instance))
        {
            FAudioSourceVoice_Start(InstancePtr->Source, 0, FAUDIO_COMMIT_NOW);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Pause(Object Instance)
    {
        if (const Ptr<FAudioInstance> InstancePtr = Find(Instance))
        {
            FAudioSourceVoice_Stop(InstancePtr->Source, 0, FAUDIO_COMMIT_NOW);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Stop(Object Instance, Bool Immediately)
    {
        if (const Ptr<FAudioInstance> InstancePtr = Find(Instance))
        {
            InstancePtr->Finish = true;

            if (Immediately)
            {
                FAudioSourceVoice_Stop(InstancePtr->Source, 0, FAUDIO_COMMIT_NOW);
                FAudioSourceVoice_FlushSourceBuffers(InstancePtr->Source);
            }

            FAudioSourceVoice_ExitLoop(InstancePtr->Source, FAUDIO_COMMIT_NOW);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Stop(ConstSPtr<Emitter> Emitter, Bool Immediately)
    {
        for (const Ptr<FAudioInstance> InstancePtr : mMixes)
        {
            if (InstancePtr->Emitter == Emitter)
            {
                InstancePtr->Finish = true;

                if (Immediately)
                {
                    FAudioSourceVoice_Stop(InstancePtr->Source, 0, FAUDIO_COMMIT_NOW);
                    FAudioSourceVoice_FlushSourceBuffers(InstancePtr->Source);
                }

                FAudioSourceVoice_ExitLoop(InstancePtr->Source, FAUDIO_COMMIT_NOW);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<FAudioSourceVoice> FAudioDriver::GetOrCreateVoice(ConstSPtr<Sound> Sound)
    {
        const auto FindByKey = [Key = GetKey(Sound)](ConstRef<FAudioChannel> Channel)
        {
            return Channel.Key == Key;
        };

        Ptr<FAudioSourceVoice> Voice = nullptr;

        if (const auto Iterator = std::ranges::find_if(mVoices, FindByKey); Iterator != mVoices.end())
        {
            Voice = Iterator->Voice;

            mVoices.erase(Iterator);
        }
        else
        {
            FAudioWaveFormatEx Description { };
            Description.wFormatTag      = GetFormat(Sound->GetKind());
            Description.wBitsPerSample  = Sound->GetDepth();
            Description.nChannels       = Sound->GetChannel();
            Description.nSamplesPerSec  = Sound->GetFrequency();
            Description.nBlockAlign     = Description.nChannels * (Description.wBitsPerSample / 8);
            Description.nAvgBytesPerSec = Description.nSamplesPerSec * Description.nBlockAlign;
            Description.cbSize          = 0;

            static FAudioVoiceCallback s_Callback =
            {
                [](Ptr<FAudioVoiceCallback>, Ptr<void> Context)
                {
                    Write(* static_cast<Ptr<FAudioInstance>>(Context));
                },
                [](Ptr<FAudioVoiceCallback>, Ptr<void>) {},
                [](Ptr<FAudioVoiceCallback>, Ptr<void>) {},
                [](Ptr<FAudioVoiceCallback>) {},
                [](Ptr<FAudioVoiceCallback>, Ptr<void>, UInt32) {},
                [](Ptr<FAudioVoiceCallback>) {},
                [](Ptr<FAudioVoiceCallback>, UInt32) {}
            };

            FAudio_CreateSourceVoice(
                mDevice, & Voice, & Description, 0, FAUDIO_DEFAULT_FREQ_RATIO, & s_Callback, nullptr, nullptr);
        }

        if (Voice)
        {
            FAudioSourceVoice_SetSourceSampleRate(Voice, Sound->GetFrequency());
        }
        return Voice;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Write(Ref<FAudioInstance> Instance)
    {
        if (Instance.Finish)
        {
            return;
        }

        // Decode from the \see Audio::Decoder of the \see Audio::Sound
        Decoder::Frame Frame;
        Instance.Finish = (Instance.Sound)->Read(Frame) && !Instance.Loop;

        // Submit the frame into the \see FAudioSourceVoice
        FAudioBuffer Buffer { };
        Buffer.pAudioData = Frame.data();
        Buffer.AudioBytes = Frame.size_bytes();
        Buffer.Flags      = Instance.Finish ? FAUDIO_END_OF_STREAM : 0;
        Buffer.pContext   = AddressOf(Instance);
        Buffer.LoopCount  = Instance.Finish && Instance.Loop ? FAUDIO_LOOP_INFINITE : 0;
        FAudioSourceVoice_SubmitSourceBuffer(Instance.Source, & Buffer, nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void FAudioDriver::Process(Ref<FAudioInstance> Instance, Ref<F3DAUDIO_LISTENER> Listener)
    {
        constexpr UInt Flags = F3DAUDIO_CALCULATE_MATRIX | F3DAUDIO_CALCULATE_DOPPLER | F3DAUDIO_CALCULATE_LPF_DIRECT;

        FAudioVoiceDetails Details;
        FAudioVoice_GetVoiceDetails(mMaster, & Details);

        F3DAUDIO_EMITTER Emitter { };
        Emitter.Position            = GetVector(Instance.Emitter->GetPosition());
        Emitter.Velocity            = GetVector(Instance.Emitter->GetVelocity());
        Emitter.OrientFront         = GetVector(Instance.Emitter->GetOrientation().GetForward());
        Emitter.OrientTop           = GetVector(Instance.Emitter->GetOrientation().GetUp());
        Emitter.ChannelCount        = Instance.Sound->GetChannel();
        Emitter.ChannelRadius       = 1.0f;
        Emitter.DopplerScaler       = 1.0f;
        Emitter.CurveDistanceScaler = 1.0f;
        Emitter.InnerRadius         = Instance.Emitter->GetRadius();
        Emitter.InnerRadiusAngle    = k_PI / 4.0;
        Emitter.pChannelAzimuths    = GetAzimuth(Emitter.ChannelCount);

        F3DAUDIO_DSP_SETTINGS Settings { };
        Real32 Matrix[FAUDIO_MAX_AUDIO_CHANNELS * 8] = { };
        Settings.SrcChannelCount = Emitter.ChannelCount;
        Settings.DstChannelCount = Details.InputChannels;
        Settings.pMatrixCoefficients = Matrix;

        F3DAudioCalculate(mProcessor, & Listener, AddressOf(Emitter), Flags, & Settings);

        FAudioSourceVoice_SetFrequencyRatio(Instance.Source, Settings.DopplerFactor, FAUDIO_COMMIT_NOW);
        FAudioVoice_SetOutputMatrix(
            Instance.Source,
            Instance.Submix,
            Settings.SrcChannelCount,
            Settings.DstChannelCount,
            Settings.pMatrixCoefficients,
            FAUDIO_COMMIT_NOW);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt32 FAudioDriver::FindDeviceAndLoadCapabilities(CStr Device)
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
            Adapter.Name    = As(reinterpret_cast<Ptr<Char16>>(Details.DisplayName));
            Adapter.Default = Details.Role & FAudioDefaultGameDevice;

            if (Adapter.Name == Device)
            {
                Result = Element;
            }
            else
            {
                // If we didn't find the device, use the default one
                if (Adapter.Default && Result == 0)
                {
                    Result = Element;
                }
            }
        }

        mCapabilities.Backend = Backend::FAudio;
        mCapabilities.Device  = mCapabilities.Adapters[Result].Name;
        return Result;
    }
}