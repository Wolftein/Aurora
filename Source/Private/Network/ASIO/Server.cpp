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

#include "Server.hpp"
#include "Core/Log/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    AsioServer::AsioServer(Ref<asio::io_context> Context)
        : mAcceptor  { Context },
          mConnector { Context }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool AsioServer::Listen(UInt Capacity, CStr Address, CStr Service)
    {
        try
        {
            const asio::ip::tcp::endpoint Endpoint
                = (* asio::ip::tcp::resolver(mAcceptor.get_executor()).resolve(Address.data(), Service.data()));

            mAcceptor.open(Endpoint.protocol());
            mAcceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
            mAcceptor.bind(Endpoint);
            mAcceptor.listen(Capacity);

            const auto OnCompletion = [Self = shared_from_this()](Ref<const std::error_code> Error)
            {
                Self->WhenAccept(Error);
            };
            mAcceptor.async_accept(mConnector, OnCompletion);
        }
        catch (Ref<std::exception const> Exception)
        {
            LOG_WARNING("Error creating a server: '{}'", Exception.what());
            return false;
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void AsioServer::WhenAccept(Ref<const std::error_code> Error)
    {
        if (Error)
        {
            mAcceptor.close();
        }
        else
        {
            SPtr<AsioClient> Connection = NewPtr<AsioClient>(Move(mConnector));
            Connection->SetProtocol(shared_from_this());
            Connection->Start();

            const auto OnCompletion = [Self = shared_from_this()](Ref<const std::error_code> Error)
            {
                CastPtr<AsioServer>(Self)->WhenAccept(Error);
            };
            mAcceptor.async_accept(mConnector = asio::ip::tcp::socket(mAcceptor.get_executor()), OnCompletion);
        }
    }
}