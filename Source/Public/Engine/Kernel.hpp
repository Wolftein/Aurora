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

#include "Audio/Service.hpp"

#include "Content/Service.hpp"

#include "Activity.hpp"

#include "Properties.hpp"

#include "Graphic/Service.hpp"

#include "Input/Service.hpp"

#include "Network/Service.hpp"

#include "Platform/Service.hpp"

#include "UI/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Engine
{
    // -=(Undocumented)=-
    class Kernel final : public Subsystem::Context
    {
    public:

        // -=(Undocumented)=-
        enum class State
        {
            Idle,
            Running,
            Exiting,
        };

    public:

        // -=(Undocumented)=-
        Kernel();

        // -=(Undocumented)=-
        ~Kernel();

        // -=(Undocumented)=-
        void Initialize(Mode Mode, Ref<const Properties> Properties, SPtr<class Host> Host);

        // -=(Undocumented)=-
        void Run();

        // -=(Undocumented)=-
        void Exit();

        // -=(Undocumented)=-
        void Goto(ConstSPtr<Activity> Foreground);

        // -=(Undocumented)=-
        void Back();

        // -=(Undocumented)=-
        template<typename Type = Activity>
        SPtr<Type> GetForeground()
        {
            return mActivities.empty() ? nullptr : CastPtr<Type>(mActivities.back());
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        State                  mState;
        Vector<SPtr<Activity>> mActivities;
        SPtr<class Host>       mHost;
    };
}