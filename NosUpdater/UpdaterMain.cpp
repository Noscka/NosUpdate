#include <boost/asio.hpp>

#include <NosUpdate/Request.hpp>
#include <NosUpdate/Definitions.hpp>
#include <NosUpdate/Helper.hpp>

#include <iostream>

using tcpSocket = boost::asio::ip::tcp::socket;

void SendRequest(tcpSocket& socket)
{
	NosUpdate::Request testRequest(NosUpdate::Request::RequestTypes::Update);

	boost::asio::streambuf RequestBuf;
	testRequest.serializeObject(&RequestBuf);
	boost::asio::write(socket, RequestBuf);
	boost::asio::write(socket, boost::asio::buffer(NosUpdate::GetDelimiter()));
}

int main()
{
	boost::asio::io_context io_context;

	tcpSocket socket(io_context);

	printf("Updater\n");

	printf("Type in hostname: ");
	std::string HostName;
	std::getline(std::cin, HostName);
	if (HostName.empty())
		HostName = "update.nosteck.com";

	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/Ip address
	Service - Service(Hostname for ports)/Port number
	*/
	boost::asio::connect(socket, boost::asio::ip::tcp::resolver(io_context).resolve(HostName.c_str(), "8100"));

	printf("Connected to server\n");

	SendRequest(socket);

	boost::asio::streambuf streamBuffer;
	size_t bytesReceived = boost::asio::read_until(socket, streamBuffer, NosUpdate::GetDelimiter());
	printf("%s\n", NosUpdate::StreamBufferToString(streamBuffer, bytesReceived).c_str());

	printf("Press any button to continue"); getchar();
	return 0;
}