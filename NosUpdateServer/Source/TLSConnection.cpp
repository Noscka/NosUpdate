#include "../Header/TLSConnection.hpp"
#include <NosUpdate/Responses.hpp>

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
	NosUpdate::SimpleRead(TLSSocket, reqBuf);
	NosLog::CreateLog(NosLog::Severity::Debug, "Got Client Request");
	clientsRequest = NosUpdate::Request::DeserializeRequest(&reqBuf);

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
			//acknowledgement = std::format("Requested Update | bytes left: {}", updateReq->GetDataLeft());
		}


		printf("Client %s\n", acknowledgement.c_str());
		NosUpdate::SimpleWrite(TLSSocket, boost::asio::buffer(acknowledgement));
		break;
	}

	case rqTp::Version:
		HandleVersionRequest(clientsRequest);
		break;
	}
}

void TLSConnection::HandleVersionRequest(NosUpdate::Request::Ptr& clientsRequest)
{
	NosUpdate::VersionRequest* versionReq = dynamic_cast<NosUpdate::VersionRequest*>(clientsRequest.get());

	if (versionReq == nullptr)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to deserialize Version Request");
		return;
	}
	NosLog::CreateLog(NosLog::Severity::Info, "Client Requested {} Version", versionReq->GetVersionTypeName());

	boost::asio::streambuf resBuf;
	NosUpdate::Version requestedVersion(0, 0, 1);
	NosUpdate::Response::Ptr res(new NosUpdate::VersionResponse(requestedVersion));
	NosUpdate::Response::SerializeResponse(res, &resBuf);
	NosUpdate::SimpleWrite(TLSSocket, resBuf);

	NosUpdate::VersionResponse* versionRes = dynamic_cast<NosUpdate::VersionResponse*>(res.get());
	NosLog::CreateLog(NosLog::Severity::Info, "Responded Newest Version | version: {}", versionRes->GetRequestedVersion().GetVersion());
}