#include <NosUpdate/WinVersion.hpp>

#include <boost/asio.hpp>

#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosUpdate/Definitions.hpp>
#include <NosUpdate/Helper.hpp>

#include <iostream>

using tcpSocket = boost::asio::ip::tcp::socket;
using ReqType = NosUpdate::Request::RequestTypes;

void SendRequest(tcpSocket& socket, const ReqType& reqType)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);
}

void SendUpdateRequest(tcpSocket& socket)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);

	NosUpdate::UpdateRequest* updateReq = dynamic_cast<NosUpdate::UpdateRequest*>(req.get());
	updateReq->GetDataLeft();
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