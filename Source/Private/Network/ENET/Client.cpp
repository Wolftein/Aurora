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

#include "Client.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    EnetClient::EnetClient(Ptr<ENetPeer> Peer)
        : mHost { nullptr },
          mPeer { Peer }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    EnetClient::~EnetClient()
    {
        OnClose(true);

        enet_host_destroy(mHost);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::Handle(Ref<ENetEvent> Event)
    {
        switch (Event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            Char Buffer[MAX_PATH];
            enet_peer_get_ip(mPeer, Buffer, MAX_PATH);

            if (mProtocol)
            {
                mProtocol->OnConnect(shared_from_this());
            }
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (mProtocol)
            {
                mProtocol->OnRead(shared_from_this(), CPtr<UInt08>(Event.packet->data, Event.packet->dataLength));
            }

            enet_packet_destroy(Event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            if (mProtocol)
            {
                mProtocol->OnDisconnect(shared_from_this());
            }
            break;
        case ENET_EVENT_TYPE_NONE:
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool EnetClient::Connect(CStr Address, UInt16 Port)
    {
        mHost = enet_host_create(nullptr, 1, 0, 0, 0);

        ENetAddress SocketAddress { { 0 }, Port, 0 };
        enet_address_set_host(& SocketAddress, Address.data());

        mPeer = enet_host_connect(mHost, & SocketAddress, 2, 0);
        return (mPeer != nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::Poll()
    {
        ENetEvent Event;

        while (mHost && enet_host_service(mHost, & Event, 0) > 0)
        {
            Handle(Event);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::OnClose(Bool Immediately)
    {
        if (mPeer)
        {
            if (Immediately)
            {
                enet_peer_disconnect(mPeer, 0);
            }
            else
            {
                enet_peer_disconnect_later(mPeer, 0);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::OnWrite(CPtr<const UInt08> Bytes, Channel Mode)
    {
        if (mPeer)
        {
            if (mProtocol)
            {
                mProtocol->OnWrite(
                    shared_from_this(), CPtr<UInt08>(const_cast<Ptr<UInt08>>(Bytes.data()), Bytes.size()));
            }

            const UInt32 Flag = (Mode == Channel::Reliable ? ENET_PACKET_FLAG_RELIABLE
                              : (Mode == Channel::Unsequenced ? ENET_PACKET_FLAG_UNSEQUENCED : 0));
            const UInt32 Slot = (Mode == Channel::Reliable ? 1 : 0);

            Ptr<ENetPacket> Packet = enet_packet_create(Bytes.data(), Bytes.size(), Flag);
            enet_peer_send(mPeer, Slot, Packet);
        }
    }
}