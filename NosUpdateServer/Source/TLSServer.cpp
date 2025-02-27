#include "../Header/TLSServer.hpp"

#include "../Header/TLSConnection.hpp"
#include <NosUpdate/Helper.hpp>

#include <iostream>

void TLSServer::AcceptLoop()
{
	printf("Server Started\n");
	printf("Listening on: %s\n", NosUpdate::EndpointAsString(ConnectionAcceptor.local_endpoint()).c_str());

	while (true)
	{
		AcceptNewConnection();
	}
}

void TLSServer::AcceptNewConnection()
{
	/* TLSConnection object which allows for managed of multiple users */
	TLSConnection* newTlsCon = TLSConnection::CreateConnection(IOContext, TLSContext);

	boost::system::error_code error;

	printf("Waiting for connection\n");
	ConnectionAcceptor.accept(newTlsCon->GetSocket(), error);

	/* if no errors, create thread for the new connection */
	if (error)
	{
		delete newTlsCon;
		return;
	}
	printf("Connection Made, Starting New Thread for Connection\n");
	boost::thread* ClientThread = new boost::thread(boost::bind(&TLSConnection::start, newTlsCon));
}