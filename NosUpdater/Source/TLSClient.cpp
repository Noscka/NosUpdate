#include "../Header/TLSClient.hpp"

#include <NosUpdate/Helper.hpp>
#include <NosUpdate/Requests.hpp>
#include <NosUpdate/Responses.hpp>

#include <iostream>

TLSClient::TLSStream& TLSClient::GetTLSSocket()
{
	return TLSSocket;
}

TLSClient::TCPStream& TLSClient::GetSocket()
{
	return TLSSocket.next_layer();
}

std::string TLSClient::GetLocalEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().local_endpoint());
}

std::string TLSClient::GetRemoteEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().remote_endpoint());
}

void TLSClient::Connect()
{
	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/ip address
	Service - Service(Hostname for ports)/Port number
	*/
	boost::asio::connect(GetSocket(), boost::asio::ip::tcp::resolver(IOContext).resolve(Hostname.c_str(), std::to_string(Port)));

	TLSSocket.set_verify_mode(boost::asio::ssl::verify_peer);
	TLSSocket.handshake(boost::asio::ssl::stream_base::client);

	NosLog::CreateLog(NosLog::Severity::Info, "Succesfully connected to: {}:{}", Hostname, Port);
	NosLog::CreateLog(NosLog::Severity::Debug, "Endpoint: {}", GetRemoteEndpoint());
}

void TLSClient::UpdateProgram()
{
	std::string newestVersion = GetNewestVersion();

	


	/* If newest version is newer then current version */
	/* Update */
}

std::string TLSClient::GetNewestVersion()
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::VersionRequest(NosUpdate::VersionRequest::VersionTypes::Newest));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(TLSSocket, reqBuf);
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Newest Version");

	NosUpdate::Response::Ptr serverResponse;

	NosUpdate::SimpleRead(TLSSocket, reqBuf);
	serverResponse = NosUpdate::Response::DeserializeResponse(&reqBuf);
	NosUpdate::VersionResponse* versionRes = dynamic_cast<NosUpdate::VersionResponse*>(serverResponse.get());

	if (versionRes == nullptr)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to deserialize Version Response");
		return "FAILED";
	}
	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded with {} Version", versionRes->GetRequestedVersion());

	return versionRes->GetRequestedVersion();
}
