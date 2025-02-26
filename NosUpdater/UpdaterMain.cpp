#ifdef WIN32
#include <SDKDDKVer.h>
#endif // WIN32

#include <boost/asio.hpp>

#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosUpdate/Definitions.hpp>
#include <NosUpdate/Helper.hpp>

#include <iostream>

using tcpSocket = boost::asio::ip::tcp::socket;
using ReqType = NosUpdate::Request::RequestTypes;

void SendRequest(tcpSocket& socket, const ReqType& reqType)
{
	NosUpdate::Request testRequest(reqType);

	boost::asio::streambuf RequestBuf;
	NosUpdate::Request::SerializeRequest(&testRequest, &RequestBuf);
	NosUpdate::SimpleWrite(socket, RequestBuf);
}

void SendUpdateRequest(tcpSocket& socket)
{
	NosUpdate::UpdateRequest testRequest(130);

	boost::asio::streambuf RequestBuf;
	NosUpdate::Request::SerializeRequest(&testRequest, &RequestBuf);
	NosUpdate::SimpleWrite(socket, RequestBuf);
}

void ReceiveResponse(tcpSocket& socket)
{
	boost::asio::streambuf streamBuffer;
	size_t bytesReceived = boost::asio::read_until(socket, streamBuffer, NosUpdate::GetDelimiter());
	printf("%s\n", NosUpdate::StreamBufferToString(streamBuffer, bytesReceived).c_str());
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

	SendRequest(socket, ReqType::NewestVersion);
	ReceiveResponse(socket);

	SendUpdateRequest(socket);
	ReceiveResponse(socket);

	printf("Press any button to continue"); getchar();
	return 0;
}