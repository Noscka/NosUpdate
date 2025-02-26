#include "../Header/TCPConnection.hpp"

#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosUpdate/Helper.hpp>

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
		NosUpdate::Request clientsRequest = GetClientsRequest();
		HandleRequest(clientsRequest);
	}
}

NosUpdate::Request tcp_connection::GetClientsRequest()
{
	NosUpdate::Request clientsRequest;

	boost::asio::streambuf RequestBuf;

	boost::asio::read_until(socket, RequestBuf, NosUpdate::GetDelimiter());
	clientsRequest.DeserializeRequest(&RequestBuf);

	return clientsRequest;
}

void tcp_connection::HandleRequest(NosUpdate::Request& clientsRequest)
{
	using rqTp = NosUpdate::Request::RequestTypes;
	std::string acknowledgement;

	switch (clientsRequest.GetRequestType())
	{
	case rqTp::Update:
	{
		NosUpdate::UpdateRequest* updateReq = dynamic_cast<NosUpdate::UpdateRequest*>(&clientsRequest);

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