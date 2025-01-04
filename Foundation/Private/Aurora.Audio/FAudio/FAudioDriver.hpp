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
    // -=(Undocumented)=-
    class FAudioDriver final : public Driver
    {
    public:

        // -=(Undocumented)=-
        static constexpr Real32 k_SpeedOfSound = 343.5f;

    public:

        // -=(Undocumented)=-
        FAudioDriver();

        // -=(Undocumented)=-
        ~FAudioDriver() override;

        // \see Driver::Initialize
        Bool Initialize(CStr Device, UInt8 Submixes) override;

        // \see Driver::GetCapabilities
        ConstRef<Capabilities> GetCapabilities() const override;

        // \see Driver::Tick
        void Tick() override;

        // \see Driver::Suspend
        void Suspend() override;

        // \see Driver::Restore
        void Restore() override;

        // \see Driver::SetListener
        void SetListener(Ref<Listener> Listener) override;

        // \see Driver::SetMasterVolume
        void SetMasterVolume(Real32 Volume) override;

        // \see Driver::SetMasterVolume
        void SetSubmixVolume(UInt8 Submix, Real32 Volume) override;

        // \see Driver::GetMasterVolume
        Real32 GetMasterVolume() override;

        // \see Driver::GetSubmixVolume
        Real32 GetSubmixVolume(UInt8 Submix) override;

        // \see Driver::Play
        Object Play(UInt8 Submix, ConstSPtr<Sound> Sound, ConstSPtr<Emitter> Emitter, Bool Repeat) override;

        // \see Driver::SetGain
        void SetGain(Object Instance, Real32 Gain) override;

        // \see Driver::Start
        void Start(Object Instance) override;

        // \see Driver::Pause
        void Pause(Object Instance) override;

        // \see Driver::Stop
        void Stop(Object Instance, Bool Immediately) override;

        // \see Driver::Stop
        void Stop(ConstSPtr<Emitter> Emitter, Bool Immediately) override;

    private:

        // -=(Undocumented)=-
        struct FAudioChannel
        {
            // -=(Undocumented)=-
            UInt32                 Key;

            // -=(Undocumented)=-
            Ptr<FAudioSourceVoice> Voice;
        };

        // -=(Undocumented)=-
        struct FAudioInstance
        {
            // -=(Undocumented)=-
            Object                 ID;

            // -=(Undocumented)=-
            Bool                   Finish;

            // -=(Undocumented)=-
            Bool                   Loop;

            // -=(Undocumented)=-
            SPtr<Emitter>          Emitter;

            // -=(Undocumented)=-
            SPtr<Sound>            Sound;

            // -=(Undocumented)=-
            Ptr<FAudioSourceVoice> Source;

            // -=(Undocumented)=-
            Ptr<FAudioSubmixVoice> Submix;
        };

        // -=(Undocumented)=-
        using Submixes  = Array<Ptr<FAudioSubmixVoice>, k_MaxSubmixes>;

        // -=(Undocumented)=-
        using Instances = Pool<FAudioInstance, k_MaxMixes>;

        // -=(Undocumented)=-
        using Mixes     = Vector<Ptr<FAudioInstance>>;

        // -=(Undocumented)=-
        using Voices    = Vector<FAudioChannel>;

    private:

        // -=(Undocumented)=-
        Ptr<FAudioInstance> Find(Object Instance)
        {
            for (Ptr<FAudioInstance> InstancePtr : mMixes)
            {
                if (InstancePtr->ID == Instance)
                {
                    return InstancePtr;
                }
            }
            return nullptr;
        }

        // -=(Undocumented)=-
        Ptr<FAudioSourceVoice> GetOrCreateVoice(ConstSPtr<Sound> Sound);

        // -=(Undocumented)=-
        static void Write(Ref<FAudioInstance> Instance);

        // -=(Undocumented)=-
        void Process(Ref<FAudioInstance> Instance, Ref<F3DAUDIO_LISTENER> Listener);

        // -=(Undocumented)=-
        UInt32 FindDeviceAndLoadCapabilities(CStr Device);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Capabilities              mCapabilities;
        Ptr<FAudio>               mDevice;
        Ptr<FAudioMasteringVoice> mMaster;
        Submixes                  mSubmixes;
        Voices                    mVoices;
        F3DAUDIO_HANDLE           mProcessor;
        F3DAUDIO_LISTENER         mListener;
        Bool                      mDirty;
        Mixes                     mMixes;
        Instances                 mInstances;
    };
}