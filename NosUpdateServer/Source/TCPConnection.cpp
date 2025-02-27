#include "../Header/TCPConnection.hpp"

#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>

#include <NosUpdate/Helper.hpp>

#include <boost/serialization/unique_ptr.hpp>

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

void TLSConnection::start()
{
	boost::system::error_code ec;
	TLSSocket.handshake(bSSL::stream_base::server, ec);
	if (ec)
	{
		std::cerr << "Handshake error: " << ec.message() << "\n";
		return;
	}

	printf("Client Connected from %s\n", NosUpdate::EndpointAsString(GetSocket().local_endpoint()).c_str());

	boost::system::error_code error;

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