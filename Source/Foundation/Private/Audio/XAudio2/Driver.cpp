// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Driver.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   DATA   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    static X3DAUDIO_DISTANCE_CURVE_POINT XAUDIO2_LFE_RAMP[] = {
        { 0.0f, 1.0f }, { 0.25f, 0.0f }, { 1.0f, 0.0f }
    };
    static X3DAUDIO_DISTANCE_CURVE       XAUDIO2_LFE_CURVE  = {
        XAUDIO2_LFE_RAMP, eastl::size(XAUDIO2_LFE_RAMP)
    };
    static X3DAUDIO_DISTANCE_CURVE_POINT XAUDIO2_REV_RAMP[] = {
        { 0.0f, 0.5f }, { 0.75f, 1.0f }, { 1.0f, 0.0f }
    };
    static X3DAUDIO_DISTANCE_CURVE       XAUDIO2_REV_CURVE  = {
        XAUDIO2_REV_RAMP, eastl::size(XAUDIO2_REV_RAMP)
    };
    static X3DAUDIO_DISTANCE_CURVE_POINT XAUDIO2_VOL_RAMP[] = {
        { 0.0f, 1.0f }, { 0.15f, 0.8239f }, { 0.3625f, 0.4406f }, { 0.575f, 0.2403f }, { 0.7875f, 0.1037f }, { 1.0f, 0.0f }
    };
    static X3DAUDIO_DISTANCE_CURVE       XAUDIO2_VOL_CURVE  = {
        XAUDIO2_VOL_RAMP, eastl::size(XAUDIO2_VOL_RAMP)
    };
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Audio
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetKey(Ref<const SPtr<Sound>> Sound)
    {
        return (Sound->GetChannel() << 16) | Sound->GetDepth();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto GetAzimuth(UInt32 Channels)
    {
        constexpr static Real32 K_Left         = 3 * X3DAUDIO_PI / 2;
        constexpr static Real32 k_Right        = X3DAUDIO_PI / 2;
        constexpr static Real32 k_FrontLeft    = 7 * X3DAUDIO_PI / 4;
        constexpr static Real32 k_FrontRight   = X3DAUDIO_PI / 4;
        constexpr static Real32 k_FrontCenter  = 0.0f;
        constexpr static Real32 k_LowFrequency = X3DAUDIO_2PI;
        constexpr static Real32 k_BackLeft     = 5 * X3DAUDIO_PI / 4;
        constexpr static Real32 k_BackRight    = 3 * X3DAUDIO_PI / 4;
        constexpr static Real32 k_BackCenter   = X3DAUDIO_PI;

        constexpr static Real32 k_Mapping[9][8] = {
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

        return & k_Mapping[Channels][0];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    XAudio2Driver::XAudio2Driver()
        : mDevice { nullptr },
          mMaster { nullptr }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    XAudio2Driver::~XAudio2Driver()
    {
        if (mDevice)
        {
            mDevice->Release();
            mDevice = nullptr;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool XAudio2Driver::Initialise(UInt32 Submixes)
    {
        if (!InitialiseLibrary())
        {
            LOG_ERROR("XAudio2: Failed to find XAudio2 libraries, please make sure you have XAudio v2.9 or v2.8.");
            return false;
        }

        if (FAILED(mLibrary.XAudio2Create(& mDevice, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        {
            LOG_ERROR("XAudio2: Failed to initialize.");
            return false;
        }

        if (FAILED(mDevice->CreateMasteringVoice(& mMaster)))
        {
            LOG_ERROR("XAudio2: Failed to create master voice.");
            return false;
        }

        // Initialise Submixes
        XAUDIO2_VOICE_DETAILS Details;
        mMaster->GetVoiceDetails(& Details);

        for (UInt32 Index = 0; Index < Submixes; ++Index)
        {
            mDevice->CreateSubmixVoice(& mSubmixes.push_back(), Details.InputChannels, Details.InputSampleRate);
        }

        // Initialise X3D audio for positional 3D voices
        DWORD Mask;
        mMaster->GetChannelMask(& Mask);
        mLibrary.X3DAudioInitialize(Mask, X3DAUDIO_SPEED_OF_SOUND, m3DAudio);

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Advance()
    {
        for (auto Iterator = mMixes.begin(); Iterator != mMixes.end(); )
        {
            Ref<XAudioInstance> InstanceRef = (* Iterator);

            XAUDIO2_VOICE_STATE State;
            InstanceRef.Source->GetState(& State);

            if (InstanceRef.Finished && State.BuffersQueued == 0)
            {
                mVoices.push_back(XAudioSource { GetKey(InstanceRef.Sound), InstanceRef.Source });

                Iterator = mMixes.erase(Iterator);
            }
            else
            {
                if (InstanceRef.Emitter)
                {
                    Apply3D(InstanceRef);
                }

                ++Iterator;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Suspend()
    {
        mDevice->StopEngine();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Restore()
    {
        mDevice->StartEngine();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::SetListener(Ref<const Vector3f> Position)
    {
        m3DAudioListener.Position    = X3DAUDIO_VECTOR(Position.GetX(), Position.GetY(), Position.GetZ());
        m3DAudioListener.Velocity    = X3DAUDIO_VECTOR(0, 0, 0); // TODO For 3D implementation
        m3DAudioListener.OrientFront = X3DAUDIO_VECTOR(0, 0, 1); // TODO For 3D implementation
        m3DAudioListener.OrientTop   = X3DAUDIO_VECTOR(0, 1, 0); // TODO For 3D implementation
        m3DAudioListener.pCone       = nullptr; // TODO For 3D implementation
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::SetMasterVolume(Real32 Volume)
    {
        mMaster->SetVolume(Volume);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::SetSubmixVolume(UInt Submix, Real32 Volume)
    {
        mSubmixes[Submix]->SetVolume(Volume);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 XAudio2Driver::GetMasterVolume()
    {
        Real32 Volume;
        mMaster->GetVolume(& Volume);

        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Real32 XAudio2Driver::GetSubmixVolume(UInt Submix)
    {
        Real32 Volume;
        mSubmixes[Submix]->GetVolume(& Volume);
        return Volume;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Object XAudio2Driver::Play(UInt Submix, Ref<const SPtr<Sound>> Sound, Ref<const SPtr<Emitter>> Emitter, Bool Repeat)
    {
        if (mMixes.full())
        {
            return 0;
        }

        XAUDIO2_SEND_DESCRIPTOR SendDescriptor[] {
            { 0u, mSubmixes[Submix] },
        };
        XAUDIO2_VOICE_SENDS     SendList;
        SendList.pSends    = SendDescriptor;
        SendList.SendCount = 1;

        // Create or reuse a source voice channel
        Ptr<IXAudio2SourceVoice> Voice = GetOrCreateMix(Sound);
        Voice->SetOutputVoices(& SendList);
        Voice->SetVolume(1.0f);
        Voice->SetFrequencyRatio(1.0f);
        Voice->SetSourceSampleRate(Sound->GetFrequency());

        // Add play instance into the mixer and apply 3D
        Ref<XAudioInstance> Instance = mMixes.push_back();
        Instance.Repeat    = Repeat;
        Instance.Frequency = 1.0f;
        Instance.Sound     = Sound;
        Instance.Emitter   = Emitter;
        Instance.Submix    = mSubmixes[Submix];
        Instance.Source    = Voice;
        Instance.Finished  = false;

        // Reset seek table in-case the sound was already being played
        Sound->Seek(0);

        // Fill initial buffer of the sound instance
        CPtr<UInt08> Data;
        Instance.Finished = !Sound->Read(Data);

        XAUDIO2_BUFFER Buffer { 0 };
        Buffer.pAudioData = Data.data();
        Buffer.AudioBytes = Data.size_bytes();
        Buffer.Flags      = (Instance.Finished) ? XAUDIO2_END_OF_STREAM : 0;
        Buffer.LoopCount  = (Instance.Finished && Repeat) ? XAUDIO2_LOOP_INFINITE : 0;
        Buffer.pContext   = Voice;

        Voice->SubmitSourceBuffer(& Buffer);

        // Uses channel address as unique identifier for the instance
        return reinterpret_cast<UInt>(Voice);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::SetGain(Object Instance, Real32 Gain)
    {
        const Ptr<XAudioInstance> InstancePtr = GetInstance(Instance);

        if (InstancePtr)
        {
            InstancePtr->Source->SetVolume(Gain);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::SetPitch(Object Instance, Real32 Pitch)
    {
        const Ptr<XAudioInstance> InstancePtr = GetInstance(Instance);

        if (InstancePtr)
        {
            Real32 Doppler;
            InstancePtr->Source->GetFrequencyRatio(& Doppler);
            InstancePtr->Source->SetFrequencyRatio(InstancePtr->Frequency = Pitch * (Doppler / InstancePtr->Frequency));
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Start(Object Instance)
    {
        const Ptr<XAudioInstance> InstancePtr = GetInstance(Instance);

        if (InstancePtr)
        {
            InstancePtr->Source->Start();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Stop(Object Instance, Bool Immediately)
    {
        const Ptr<XAudioInstance> InstancePtr = GetInstance(Instance);

        if (InstancePtr)
        {
            InstancePtr->Finished = true;

            if (Immediately)
            {
                InstancePtr->Source->Stop();
                InstancePtr->Source->FlushSourceBuffers();
            }

            if (InstancePtr->Repeat)
            {
                InstancePtr->Source->ExitLoop();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Stop(Ref<const SPtr<Emitter>> Emitter, Bool Immediately)
    {
        for (auto It = mMixes.begin(); It != mMixes.end(); ++It)
        {
            Ref<XAudioInstance> InstanceRef = (* It);

            if (InstanceRef.Emitter == Emitter)
            {
                InstanceRef.Finished = true;

                if (Immediately)
                {
                    InstanceRef.Source->Stop();
                    InstanceRef.Source->FlushSourceBuffers();
                }

                if (InstanceRef.Repeat)
                {
                    InstanceRef.Source->ExitLoop();
                }
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Halt(UInt Submix)
    {
        for (Ref<XAudioInstance> InstanceRef : mMixes)
        {
            if (InstanceRef.Submix == mSubmixes[Submix])
            {
                InstanceRef.Source->Stop();
                InstanceRef.Source->FlushSourceBuffers();
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ptr<IXAudio2SourceVoice> XAudio2Driver::GetOrCreateMix(Ref<const SPtr<Sound>> Sound)
    {
        const auto FindByKey = [Key = GetKey(Sound)](Ref<const XAudioSource> InstanceRef)
        {
            return InstanceRef.Key == Key;
        };

        Ptr<IXAudio2SourceVoice> Mixer = nullptr;

        if (const auto Iterator = eastl::find_if(mVoices.begin(), mVoices.end(), FindByKey); Iterator != mVoices.end())
        {
            Mixer = Iterator->Source;

            mVoices.erase(Iterator);
        }
        else
        {
            WAVEFORMATEX Description { 0 };

            Description.wFormatTag      = (Sound->GetDepth() >= 32 ? WAVE_FORMAT_IEEE_FLOAT : WAVE_FORMAT_PCM);
            Description.wBitsPerSample  = Sound->GetDepth();
            Description.nChannels       = Sound->GetChannel();
            Description.nSamplesPerSec  = Sound->GetFrequency();
            Description.nBlockAlign     = static_cast<DWORD>(Description.nChannels * (Description.wBitsPerSample / 8));
            Description.nAvgBytesPerSec = Description.nSamplesPerSec * Description.nBlockAlign;
            Description.cbSize          = 0;

            mDevice->CreateSourceVoice(& Mixer, & Description, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this);
        }
        return Mixer;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Write(Ptr<XAudioInstance> InstancePtr)
    {
        if (InstancePtr->Finished)
        {
            return;
        }

        // Request some frames from the clip
        CPtr<UInt08> Data;
        InstancePtr->Finished = !InstancePtr->Sound->Read(Data) && !InstancePtr->Repeat;

        // Submix the frames into the source voice
        XAUDIO2_BUFFER Buffer { 0 };
        Buffer.pAudioData = Data.data();
        Buffer.AudioBytes = Data.size_bytes();
        Buffer.Flags      = (InstancePtr->Finished) ? XAUDIO2_END_OF_STREAM : 0;
        Buffer.pContext   = InstancePtr->Source;

        InstancePtr->Source->SubmitSourceBuffer(& Buffer);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void XAudio2Driver::Apply3D(Ref<XAudioInstance> Instance)
    {
        UInt Flags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_DOPPLER;

        XAUDIO2_VOICE_DETAILS Details;
        mMaster->GetVoiceDetails(& Details);

        Ref<const Vector3f> Position = Instance.Emitter->GetPosition();
        Ref<const Vector3f> Velocity = Instance.Emitter->GetVelocity();

        X3DAUDIO_EMITTER Emitter { 0 };
        Emitter.Position            = X3DAUDIO_VECTOR(Position.GetX(), Position.GetY(), Position.GetZ());
        Emitter.Velocity            = X3DAUDIO_VECTOR(Velocity.GetX(), Velocity.GetY(), Velocity.GetZ());
        Emitter.OrientFront         = X3DAUDIO_VECTOR(0, 0, 1); // TODO Expose?
        Emitter.OrientTop           = X3DAUDIO_VECTOR(0, 1, 0); // TODO Expose?
        Emitter.ChannelCount        = Instance.Sound->GetChannel();
        Emitter.ChannelRadius       = 1.0f;
        Emitter.DopplerScaler       = 1.0f;
        Emitter.CurveDistanceScaler = Instance.Emitter->GetAttenuation();
        Emitter.InnerRadius         = Instance.Emitter->GetInnerRadius();
        Emitter.InnerRadiusAngle    = Instance.Emitter->GetInnerRadiusAngle();

        Emitter.pLFECurve           = & XAUDIO2_LFE_CURVE;
        Emitter.pReverbCurve        = & XAUDIO2_REV_CURVE;
        Emitter.pVolumeCurve        = & XAUDIO2_VOL_CURVE;
        Emitter.pChannelAzimuths    = const_cast<Ptr<Real32>>(GetAzimuth(Emitter.ChannelCount));

        X3DAUDIO_DSP_SETTINGS Settings { 0 };
        Real32 Matrix[XAUDIO2_MAX_AUDIO_CHANNELS * 8] = { 0 };
        Settings.SrcChannelCount = Emitter.ChannelCount;
        Settings.DstChannelCount = Details.InputChannels;
        Settings.pMatrixCoefficients = Matrix;

        mLibrary.X3DAudioCalculate(m3DAudio, & m3DAudioListener, & Emitter, Flags, & Settings);

        Instance.Source->SetFrequencyRatio(Instance.Frequency * Settings.DopplerFactor);
        Instance.Source->SetOutputMatrix(Instance.Submix,
            Settings.SrcChannelCount,
            Settings.DstChannelCount,
            Settings.pMatrixCoefficients);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool XAudio2Driver::InitialiseLibrary()
    {
        HMODULE Dll = ::LoadLibrary("XAUDIO2_9.DLL");

        if (Dll == nullptr)
        {
            Dll = ::LoadLibrary("XAUDIO2_9REDIST.DLL");
        }
        if (Dll == nullptr)
        {
            Dll = ::LoadLibrary("XAUDIO2_8.DLL");
        }

        if (Dll != nullptr)
        {
            mLibrary.XAudio2Create
                = (decltype(mLibrary.XAudio2Create)) GetProcAddress(Dll, "XAudio2Create");
            mLibrary.X3DAudioInitialize
                = (decltype(mLibrary.X3DAudioInitialize)) GetProcAddress(Dll, "X3DAudioInitialize");
            mLibrary.X3DAudioCalculate
                = (decltype(mLibrary.X3DAudioCalculate)) GetProcAddress(Dll, "X3DAudioCalculate");
        }
        return (mLibrary.XAudio2Create && mLibrary.X3DAudioInitialize && mLibrary.X3DAudioCalculate);
    }
}