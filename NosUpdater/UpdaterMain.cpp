#include <NosUpdate/WinVersion.hpp>

#include <boost/asio.hpp>

#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosUpdate/Definitions.hpp>
#include <NosUpdate/Helper.hpp>

#include <iostream>

#include <boost/serialization/unique_ptr.hpp>

using tcpSocket = boost::asio::ip::tcp::socket;
using ReqType = NosUpdate::Request::RequestTypes;

void SendRequest(tcpSocket& socket, const ReqType& reqType)
{
	//NosUpdate::Request testRequest(reqType);
	//NosUpdate::Request::SerializeRequest(&testRequest, &RequestBuf);

	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	boost::asio::streambuf RequestBuf;
	std::ostream os(&RequestBuf);
	boost::archive::polymorphic_binary_oarchive oa(os);
	oa << req;

	NosUpdate::SimpleWrite(socket, RequestBuf);
}

void SendUpdateRequest(tcpSocket& socket)
{
	//NosUpdate::UpdateRequest testRequest(130);
	//NosUpdate::Request::SerializeRequest(&testRequest, &RequestBuf);

	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	boost::asio::streambuf RequestBuf;
	std::ostream os(&RequestBuf);
	boost::archive::polymorphic_binary_oarchive oa(os);
	oa << req;
	NosUpdate::SimpleWrite(socket, RequestBuf);
}

void ReceiveResponse(tcpSocket& socket)
{
	boost::asio::streambuf streamBuffer;
	size_t bytesReceived = boost::asio::read_until(socket, streamBuffer, NosUpdate::GetDelimiter());
	printf("%s\n", NosUpdate::StreamBufferToString(streamBuffer, bytesReceived).c_str());
}

void SerializeRequest(boost::asio::streambuf* streamBuf, const ReqType& reqType)
{
	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	std::ostream os(streamBuf);
	boost::archive::polymorphic_binary_oarchive oa(os);
	oa << req;
}

void SerializeUpdateRequest(boost::asio::streambuf* streamBuf)
{
	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	std::ostream os(streamBuf);
	boost::archive::polymorphic_binary_oarchive oa(os);
	oa << req;
}

void HandleRequest(const NosUpdate::Request::Ptr& req)
{
	std::string acknowledgement;
	switch (req->GetRequestType())
	{
	case ReqType::Update:
	{
		NosUpdate::UpdateRequest* updateReq = dynamic_cast<NosUpdate::UpdateRequest*>(req.get());

		acknowledgement = "Deserializing Failed";
		if (updateReq != nullptr)
		{
			acknowledgement = std::format("Requested Update | bytes left: {}", updateReq->GetDataLeft());
		}


		printf("Client %s\n", acknowledgement.c_str());
		break;
	}

	case ReqType::NewestVersion:
		acknowledgement = "Requested Newest Version";

		printf("Client %s\n", acknowledgement.c_str());
		break;
	}
}

void DeserializeRequest(boost::asio::streambuf* streamBuf)
{
	NosUpdate::Request::Ptr req;

	std::istream is(streamBuf);
	boost::archive::polymorphic_binary_iarchive ia(is);
	ia >> req;

	HandleRequest(req);
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