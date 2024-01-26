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

#include "Session.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class Server : public EnableSmartPointer<Server>, public Protocol
    {
    public:

        // -=(Undocumented)=-
        virtual void Close() = 0;

        // -=(Undocumented)=-
        void Attach(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        void Disconnect(Bool Forcibly)
        {
            for (ConstSPtr<Session> Session : mDatabase)
            {
                Session->Close(Forcibly);
            }
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Broadcast(Message && Packet)
        {
            for (ConstSPtr<Session> Session : mDatabase)
            {
                Session->template Write(Packet);
            }
        }

        // -=(Undocumented)=-
        void Flush()
        {
            for (ConstSPtr<Session> Session : mDatabase)
            {
                Session->Flush();
            }
        }

    private:

        // -=(Undocumented)=-
        void OnAttach(ConstSPtr<class Session> Session) override;

        // -=(Undocumented)=-
        void OnDetach(ConstSPtr<class Session> Session) override;

        // -=(Undocumented)=-
        void OnError(ConstSPtr<class Session> Session, UInt Error, CStr Description) override;

        // -=(Undocumented)=-
        void OnRead(ConstSPtr<class Session> Session,  CPtr<UInt08> Bytes) override;

        // -=(Undocumented)=-
        void OnWrite(ConstSPtr<class Session> Session, CPtr<UInt08> Bytes) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Protocol>        mProtocol;
        Vector<SPtr<Session>> mDatabase;
    };
}