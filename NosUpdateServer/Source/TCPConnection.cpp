#include "../Header/TCPConnection.hpp"

#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>

#include <NosUpdate/Helper.hpp>

#include <boost/serialization/unique_ptr.hpp>

tcp_connection* tcp_connection::create(boost::asio::io_context& io_context)
{
	return new tcp_connection(io_context);
}

boost::asio::ip::tcp::socket& tcp_connection::ConSocket()
{
	return socket;
}

void tcp_connection::start()
{
	printf("Client Connected from %s\n", EndpointAsString(socket.local_endpoint()).c_str());

	boost::system::error_code error;

	while (!error)
	{
		NosUpdate::Request::Ptr clientsRequest = GetClientsRequest();
		HandleRequest(clientsRequest);
	}
}

NosUpdate::Request::Ptr tcp_connection::GetClientsRequest()
{
	NosUpdate::Request::Ptr clientsRequest;

	boost::asio::streambuf reqBuf;
	boost::asio::read_until(socket, reqBuf, NosUpdate::GetDelimiter());
	clientsRequest = NosUpdate::Request::DeserializeRequest(&reqBuf);

	return clientsRequest;
}

void tcp_connection::HandleRequest(NosUpdate::Request::Ptr& clientsRequest)
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
		NosUpdate::SimpleWrite(socket, boost::asio::buffer(acknowledgement));
		break;
	}

	case rqTp::NewestVersion:
		acknowledgement = "Requested Newest Version";

		printf("Client %s\n", acknowledgement.c_str());
		NosUpdate::SimpleWrite(socket, boost::asio::buffer(acknowledgement));
		break;
	}
}