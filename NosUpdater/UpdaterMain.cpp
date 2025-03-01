#include <NosUpdate/WinVersion.hpp>
#include <NosUpdate/Definitions.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <NosUpdate/Requests.hpp>
#include <NosUpdate/Definitions.hpp>
#include <NosUpdate/Helper.hpp>
#include <NosLib/Logging.hpp>

#include <iostream>

using SSLSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
using ReqType = NosUpdate::Request::RequestTypes;

void SendRequest(SSLSocket& socket, const ReqType& reqType)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);
}

void SendUpdateRequest(SSLSocket& socket)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);

	NosUpdate::UpdateRequest* updateReq = dynamic_cast<NosUpdate::UpdateRequest*>(req.get());
	updateReq->GetDataLeft();
}

void ReceiveResponse(SSLSocket& socket)
{
	boost::asio::streambuf streamBuffer;
	size_t bytesReceived = boost::asio::read_until(socket, streamBuffer, NosUpdate::GetDelimiter());
	printf("%s\n", NosUpdate::StreamBufferToString(streamBuffer, bytesReceived).c_str());
}

std::string GetServerHostName()
{
	printf("Type in hostname: ");
	std::string HostName;
	std::getline(std::cin, HostName);
	if (HostName.empty())
		HostName = "localhost";

	return HostName;
}

int main()
{
	NosLib::Logging::SetVerboseLevel(NosLib::Logging::Verbose::Debug);

	boost::asio::io_context io_context;
	boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23);
	ssl_context.load_verify_file("server.crt");
	ssl_context.set_default_verify_paths();

	SSLSocket socket(io_context, ssl_context);

	printf("Updater\n");

#if 1
	std::string hostName = GetServerHostName();
#else
	std::string hostName = Definitions::UpdateHostname;
#endif

	printf("connecting to: %s\n", hostName.c_str());

	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/Ip address
	Service - Service(Hostname for ports)/Port number
	*/
	boost::asio::connect(socket.lowest_layer(), boost::asio::ip::tcp::resolver(io_context).resolve(hostName.c_str(), std::to_string(Definitions::UpdatePort)));

	socket.set_verify_mode(boost::asio::ssl::verify_peer);
	// You might want to add a verification callback here.
	socket.handshake(boost::asio::ssl::stream_base::client);

	printf("Connected to server with TLS\n");

	SendRequest(socket, ReqType::NewestVersion);
	ReceiveResponse(socket);

	SendUpdateRequest(socket);
	ReceiveResponse(socket);

	printf("Press any button to continue"); getchar();
	return 0;
}