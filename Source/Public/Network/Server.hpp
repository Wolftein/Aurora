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
        void Attach(ConstSPtr<Protocol> Protocol)
        {
            mProtocol = Protocol;
        }

        // -=(Undocumented)=-
        virtual void Close() = 0;

        // -=(Undocumented)=-
        void Disconnect(Bool Forcibly)
        {
            for (ConstSPtr<Client> Session : mDatabase)
            {
                Session->Close(Forcibly);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Broadcast(CPtr<const Type> Bytes)
        {
            for (ConstSPtr<Client> Session : mDatabase)
            {
                Session->Write(Bytes);
            }
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Broadcast(Message && Packet)
        {
            for (ConstSPtr<Client> Session : mDatabase)
            {
                Session->Write(Packet);
            }
        }

        // -=(Undocumented)=-
        void Flush()
        {
            for (ConstSPtr<Client> Session : mDatabase)
            {
                Session->Flush();
            }
        }

    private:

        // -=(Undocumented)=-
        void OnAttach(ConstSPtr<class Client> Session) override;

        // -=(Undocumented)=-
        void OnDetach(ConstSPtr<class Client> Session) override;

        // -=(Undocumented)=-
        void OnError(ConstSPtr<class Client> Session, UInt Error, CStr Description) override;

        // -=(Undocumented)=-
        void OnRead(ConstSPtr<class Client> Session, CPtr<UInt08> Bytes) override;

        // -=(Undocumented)=-
        void OnWrite(ConstSPtr<class Client> Session, CPtr<UInt08> Bytes) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Protocol>       mProtocol;
        Vector<SPtr<Client>> mDatabase;
    };
}