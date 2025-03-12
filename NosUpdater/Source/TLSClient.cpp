#include "../Header/TLSClient.hpp"

#include <NosUpdate/Helper.hpp>
#include <NosUpdate/Requests.hpp>
#include <NosUpdate/Responses.hpp>
#include <NosUpdate/FileNet/FileReceive.hpp>

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
	NosUpdate::Version newestVersion = GetNewestVersion();
	RequestUpdate(newestVersion);
}

NosUpdate::Version TLSClient::GetNewestVersion()
{
	NosUpdate::SerializeSend<NosUpdate::VersionRequest>(TLSSocket, NosUpdate::VersionRequest::VersionTypes::Newest);
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Newest Version");

	NosUpdate::VersionResponse::Ptr versionRes = NosUpdate::DeserializeRead<NosUpdate::VersionResponse>(TLSSocket);

	if (!versionRes)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to cast to Version Response");
		return NosUpdate::Version();
	}

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded with {} Version", versionRes->GetRequestedVersion().GetVersion());

	return versionRes->GetRequestedVersion();
}

void TLSClient::RequestUpdate(const NosUpdate::Version& version)
{
	NosUpdate::SerializeSend<NosUpdate::UpdateRequest>(TLSSocket, NosUpdate::Version(0, 0, 1), "TestProgram", "./");
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Update Version");

	NosUpdate::UpdateResponse::Ptr updateRes = NosUpdate::DeserializeRead<NosUpdate::UpdateResponse>(TLSSocket);

	if (!updateRes)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to cast to Update Response");
		return;
	}
	std::vector<NosUpdate::FileInfo> fileInfos = updateRes->GetUpdateFileInfo();

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded Update | Version: {}", updateRes->GetUpdateVersion().GetVersion());

	for (NosUpdate::FileInfo& fileInfo : fileInfos)
	{
		NosLog::CreateLog(NosLog::Severity::Debug, "File Name: {} | File Hash: {} | File Size: {}",
						  fileInfo.GetName(),
						  fileInfo.GetHash(),
						  fileInfo.GetSize());
	}

	//NosUpdate::ReceiveFile(TLSSocket, fileInfo.GetName(), fileInfo.GetSize());
	//NosLog::CreateLog(NosLog::Severity::Debug, "Received File");
}