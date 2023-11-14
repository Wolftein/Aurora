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

#include "Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Input
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Context> System)
        : Subsystem(System)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTick()
    {
        // Poll all raw-input event(s) from the input-devices.
        Vector<Event> Collector;

        for(ConstSPtr<Device> Device : mDevices)
        {
            Device->Poll(Collector);
        }

        // Sort all raw-input event(s) by its time
        std::sort(Collector.begin(), Collector.end(), [](Ref<const Event> First, Ref<const Event> Second)
        {
           return First.Time < Second.Time;
        });

        // Dispatch all event(s)
        for (Ref<const Event> Event : Collector)
        {
            for (auto Iterator = mListeners.begin(); Iterator != mListeners.end(); /* Unused */)
            {
                if (ConstSPtr<Listener> Listener = Iterator->lock())
                {
                    if (Listener->OnEvent(Event))
                    {
                        break;
                    }

                    ++Iterator;
                }
                else
                {
                    Iterator = mListeners.erase(Iterator);
                }
            }
        }
    }
}