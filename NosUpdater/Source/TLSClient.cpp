#include "../Header/TLSClient.hpp"

#include <NosUpdate/Helper.hpp>
#include <NosUpdate/Requests.hpp>

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
	/* Check newest version */

	/* If newest version is newer then current version */
	/* Update */
}





using SSLSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
using ReqType = NosUpdate::Request::RequestTypes;
using NosLog = NosLib::Logging;

void SendRequest(SSLSocket& socket, const ReqType& reqType)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);

	NosLog::CreateLog(NosLog::Severity::Debug, "Client Request Deserialized | Class Name: {} | Type Name: {}", req->GetRequestName(), req->GetRequestTypeName());
}

void SendUpdateRequest(SSLSocket& socket)
{
	boost::asio::streambuf reqBuf;
	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	NosUpdate::Request::SerializeRequest(req, &reqBuf);
	NosUpdate::SimpleWrite(socket, reqBuf);

	NosLog::CreateLog(NosLog::Severity::Debug, "Client Request Deserialized | Class Name: {} | Type Name: {}", req->GetRequestName(), req->GetRequestTypeName());
}

void ReceiveResponse(SSLSocket& socket)
{
	boost::asio::streambuf streamBuffer;
	size_t bytesReceived = boost::asio::read_until(socket, streamBuffer, NosUpdate::GetDelimiter());

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Replied: {}", NosUpdate::StreamBufferToString(streamBuffer, bytesReceived));
}

void TLSClient::SendRequests()
{
	SendRequest(TLSSocket, ReqType::NewestVersion);
	ReceiveResponse(TLSSocket);

	SendUpdateRequest(TLSSocket);
	ReceiveResponse(TLSSocket);
}