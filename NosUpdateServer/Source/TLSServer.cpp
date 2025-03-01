#include "../Header/TLSServer.hpp"
#include "../Header/TLSConnection.hpp"

#include <NosUpdate/Helper.hpp>

#include <iostream>

void TLSServer::AcceptLoop()
{
	NosLog::CreateLog(NosLog::Severity::Info, "Server Started");
	std::string listenEndpoint = NosUpdate::EndpointAsString(ConnectionAcceptor.local_endpoint());
	NosLog::CreateLog(NosLog::Severity::Debug, "Listening on: {}", listenEndpoint);

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

	NosLog::CreateLog(NosLog::Severity::Debug, "Waiting for connection");
	ConnectionAcceptor.accept(newTlsCon->GetSocket(), error);

	/* if no errors, create thread for the new connection */
	if (error)
	{
		delete newTlsCon;
		return;
	}
	NosLog::CreateLog(NosLog::Severity::Debug, "Connection Made, Starting New Thread for Connection");
	boost::thread* ClientThread = new boost::thread(boost::bind(&TLSConnection::start, newTlsCon));
}