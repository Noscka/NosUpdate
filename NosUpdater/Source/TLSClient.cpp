#include "../Header/TLSClient.hpp"

#include <NosUpdate/Helper.hpp>
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

using UpRes = NosUpdate::UpdateResponse;

void TLSClient::UpdateProgram()
{
	NosUpdate::Version newestVersion = GetNewestVersion();
	UpRes::Ptr updateRes = RequestUpdate(newestVersion);
	ReceivedUpdateFiles(updateRes);
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

UpRes::Ptr TLSClient::RequestUpdate(const NosUpdate::Version& version)
{
	NosUpdate::SerializeSend<NosUpdate::UpdateRequest>(TLSSocket, version, "TestProgram", "./TestProgram");
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Update Version");

	UpRes::Ptr updateRes = NosUpdate::DeserializeRead<NosUpdate::UpdateResponse>(TLSSocket);

	if (!updateRes)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to cast to Update Response");
		return updateRes; /* Will be nullptr */
	}

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded Update | Version: {}", updateRes->GetUpdateVersion().GetVersion());
	
	return updateRes;
}

void TLSClient::ReceivedUpdateFiles(const NosUpdate::UpdateResponse::Ptr& updateRes)
{
	std::vector<NosUpdate::FileInfo> fileInfos = updateRes->GetUpdateFileInfo();

	for (NosUpdate::FileInfo& fileInfo : fileInfos)
	{
		std::string fileAction;

		switch (fileInfo.GetAction())
		{
		case NosUpdate::FileInfo::FileActions::Update:
			fileAction = "Update";
			break;
		case NosUpdate::FileInfo::FileActions::Delete:
			fileAction = "Delete";
			break;
		}

		NosLog::CreateLog(NosLog::Severity::Info, "Receiving file: File Name: {} | File Action: {}", fileInfo.GetName(), fileAction);
		NosLog::CreateLog(NosLog::Severity::Debug, "File Hash: {} | File Size: {}", fileInfo.GetHashString(), fileInfo.GetSize());

		if (fileInfo.GetAction() != NosUpdate::FileInfo::FileActions::Update)
		{
			continue;
		}

		NosUpdate::ReceiveFile(TLSSocket, fileInfo.GetName(), fileInfo.GetSize());
	}
}