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
        OnPoll();

        enet_host_destroy(mHost);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::Handle(Ref<ENetEvent> Event)
    {
        switch (Event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            if (mProtocol)
            {
                mProtocol->OnAttach(shared_from_this());
            }

            Char Buffer[MAX_PATH];
            enet_peer_get_ip(mPeer, Buffer, MAX_PATH);

            mStatistics         = { };
            mStatistics.Address = Buffer;
            mStatistics.Port    = enet_peer_get_port(mPeer);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (mProtocol)
            {
                mProtocol->OnRead(shared_from_this(), CPtr<UInt08>(Event.packet->data, Event.packet->dataLength));
            }

            ++mStatistics.TotalPacketReceived;

            enet_packet_destroy(Event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            if (mProtocol)
            {
                mProtocol->OnDetach(shared_from_this());
            }
            break;
        case ENET_EVENT_TYPE_NONE:
            break;
        }

        mStatistics.TotalBytesReceived  = enet_peer_get_bytes_received(mPeer);
        mStatistics.TotalBytesSent      = enet_peer_get_bytes_sent(mPeer);
        mStatistics.TotalPacketLost     = enet_peer_get_packets_lost(mPeer);
        mStatistics.TotalPacketSent     = enet_peer_get_packets_sent(mPeer);
        mStatistics.Latency             = enet_peer_get_rtt(mPeer);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool EnetClient::Connect(CStr Address, UInt16 Port, UInt32 InBandwidth, UInt32 OutBandwidth)
    {
        mHost = enet_host_create(nullptr, 1, 0, InBandwidth, OutBandwidth);

        ENetAddress SocketAddress { { 0 }, Port, 0 };
        enet_address_set_host(& SocketAddress, Address.data());

        mPeer = enet_host_connect(mHost, & SocketAddress, 2, 0);
        return (mPeer != nullptr);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::OnPoll()
    {
        ENetEvent Event;

        while (mHost && enet_host_service (mHost, & Event, 0) > 0)
        {
            Handle(Event);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::OnClose(Bool Immediately)
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

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void EnetClient::OnWrite(CPtr<const UInt08> Bytes, UInt32 Channel, Bool Reliable)
    {
        if (mProtocol)
        {
            mProtocol->OnWrite(shared_from_this(), CPtr<UInt08>(const_cast<Ptr<UInt08>>(Bytes.data()), Bytes.size()));
        }

        Ptr<ENetPacket> Packet = enet_packet_create(Bytes.data(), Bytes.size(),
            Reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);
        enet_peer_send(mPeer, Channel, Packet);
    }
}