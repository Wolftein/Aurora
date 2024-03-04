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

#include "Client.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=(Undocumented)=-
    class Server : public Protocol
    {
    public:

        // -=(Undocumented)=-
        virtual ~Server() = default;

        // -=(Undocumented)=-
        void Poll()
        {
            OnPoll();
        }

        // -=(Undocumented)=-
        void SetProtocol(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        void Disconnect(Bool Forcibly)
        {
            for (ConstSPtr<Client> Session : mConnections)
            {
                Session->Close(Forcibly);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Broadcast(CPtr<const Type> Bytes, UInt32 Channel, Bool Reliable)
        {
            for (ConstSPtr<Client> Session : mConnections)
            {
                Session->Write(Bytes, Channel, Reliable);
            }
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Broadcast(Message && Packet, UInt32 Channel, Bool Reliable)
        {
            for (ConstSPtr<Client> Session : mConnections)
            {
                Session->Write(Packet, Channel, Reliable);
            }
        }

    private:

        // -=(Undocumented)=-
        virtual void OnPoll() = 0;

    private:

        // \see Protocol::OnAttach
        void OnAttach(ConstSPtr<class Client> Session) override;

        // \see Protocol::OnDetach
        void OnDetach(ConstSPtr<class Client> Session) override;

        // \see Protocol::OnError
        void OnError(ConstSPtr<class Client> Session, UInt Error, CStr Description) override;

        // \see Protocol::OnRead
        void OnRead(ConstSPtr<class Client> Session, CPtr<UInt08> Bytes) override;

        // \see Protocol::OnWrite
        void OnWrite(ConstSPtr<class Client> Session, CPtr<UInt08> Bytes) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Protocol>       mProtocol;
        Vector<SPtr<Client>> mConnections;
    };
}