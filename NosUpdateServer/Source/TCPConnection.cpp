#include "../Header/TLSConnection.hpp"

#include <NosUpdate/Helper.hpp>

TLSConnection* TLSConnection::CreateConnection(boost::asio::io_context& io_context, bSSL::context& ssl_context)
{
	return new TLSConnection(io_context, ssl_context);
}

TLSConnection::TLSStream& TLSConnection::GetTLSSocket()
{
	return TLSSocket;
}

TLSConnection::TCPStream& TLSConnection::GetSocket()
{
	return TLSSocket.next_layer();
}

std::string TLSConnection::GetLocalEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().local_endpoint());
}

std::string TLSConnection::GetRemoteEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().remote_endpoint());
}

void TLSConnection::start()
{
	boost::system::error_code error;
	TLSSocket.handshake(bSSL::stream_base::server, error);
	if (error)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Closing connection with {} | Handshake error: {}", GetRemoteEndpoint(), error.message());
		return;
	}

	NosLog::CreateLog(NosLog::Severity::Info, "Client succesfully connected from {}", GetRemoteEndpoint());

	while (!error)
	{
		NosUpdate::Request::Ptr clientsRequest = GetClientsRequest();
		HandleRequest(clientsRequest);
	}
}

NosUpdate::Request::Ptr TLSConnection::GetClientsRequest()
{
	NosUpdate::Request::Ptr clientsRequest;

	boost::asio::streambuf reqBuf;
	boost::asio::read_until(TLSSocket, reqBuf, NosUpdate::GetDelimiter());
	NosLog::CreateLog(NosLog::Severity::Debug, "Got Client Request");
	clientsRequest = NosUpdate::Request::DeserializeRequest(&reqBuf);

	NosLog::CreateLog(NosLog::Severity::Debug, "Client Request Deserialized | Class Name: {} | Type Name: {}", clientsRequest->GetRequestName(), clientsRequest->GetRequestTypeName());

	return clientsRequest;
}

void TLSConnection::HandleRequest(NosUpdate::Request::Ptr& clientsRequest)
{
	using rqTp = NosUpdate::Request::RequestTypes;
	std::string acknowledgement;

	switch (clientsRequest->GetRequestType())
	{
	case rqTp::Update:
	{
		NosUpdate::UpdateRequest* updateReq = dynamic_cast<NosUpdate::UpdateRequest*>(clientsRequest.get());

		acknowledgement = "Deserializing Failed";
		if (updateReq != nullptr)
		{
			acknowledgement = std::format("Requested Update | bytes left: {}", updateReq->GetDataLeft());
		}


		printf("Client %s\n", acknowledgement.c_str());
		NosUpdate::SimpleWrite(TLSSocket, boost::asio::buffer(acknowledgement));
		break;
	}

	case rqTp::NewestVersion:
		acknowledgement = "Requested Newest Version";

		printf("Client %s\n", acknowledgement.c_str());
		NosUpdate::SimpleWrite(TLSSocket, boost::asio::buffer(acknowledgement));
		break;
	}
}