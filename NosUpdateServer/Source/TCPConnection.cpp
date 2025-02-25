#include "../Header/TCPConnection.hpp"

#include <NosUpdate/Request.hpp>
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
	NosUpdate::Request clientsRequest = GetClientsRequest();
	HandleRequest(clientsRequest);
}

NosUpdate::Request tcp_connection::GetClientsRequest()
{
	NosUpdate::Request clientsRequest;

	boost::asio::streambuf RequestBuf;

	boost::asio::read_until(socket, RequestBuf, NosUpdate::GetDelimiter());
	clientsRequest.DeserializeObject(&RequestBuf);

	return clientsRequest;
}

void tcp_connection::HandleRequest(const NosUpdate::Request& clientsRequest)
{
	using rqTp = NosUpdate::Request::RequestTypes;

	switch (clientsRequest.GetRequestType())
	{
	case rqTp::Update:
	{
		boost::asio::write(socket, boost::asio::buffer("Requested Update"));
		boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));

		/*NosUpdate::SimpleWrite(socket, boost::asio::buffer("requested update"));*/
		break;
	}
	case rqTp::NewestVersion:
	{
		boost::asio::write(socket, boost::asio::buffer("Requested Newest Version"));
		boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));
		break;
	}
	}
}