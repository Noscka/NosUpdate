#include "../Header/TLSClient.hpp"

#include <NosLib/ErrorHandling.hpp>

#include <NosUpdate/Helper.hpp>
#include <NosUpdate/FileNet/FileReceive.hpp>

#include <iostream>
#include <filesystem>

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
	boost::system::error_code error;

	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/ip address
	Service - Service(Hostname for ports)/Port number
	*/
	auto resolvedEndpoint = boost::asio::ip::tcp::resolver(IOContext).resolve(Hostname.c_str(), std::to_string(Port), error);

	NOSLOG_ASSERT(error, return, NosLog::Severity::Error, "Unable to resolve endpoint | {}", error.message());

	boost::asio::connect(GetSocket(), resolvedEndpoint, error);

	NOSLOG_ASSERT(error, return, NosLog::Severity::Error, "Unabled to connect to endpoint | {}", error.message());

	TLSSocket.set_verify_mode(boost::asio::ssl::verify_peer);
	TLSSocket.handshake(boost::asio::ssl::stream_base::client, error);

	NOSLOG_ASSERT(error, return, NosLog::Severity::Error, "Closing connection with {} | Handshake error: {}", GetRemoteEndpoint(), error.message());

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
		ProcessUpdateFile(fileInfo);
	}
}


void TLSClient::ProcessUpdateFile(const NosUpdate::FileInfo& updateFile)
{
	NosLog::CreateLog(NosLog::Severity::Info, "Receiving file: File Name: {} | File Action: {}", updateFile.GetName(), updateFile.GetActionName());
	NosLog::CreateLog(NosLog::Severity::Debug, "File Hash: {} | File Size: {}", updateFile.GetHashString(), updateFile.GetSize());

	switch (updateFile.GetAction())
	{
	case NosUpdate::FileInfo::FileActions::Update:
		NosUpdate::ReceiveFile(TLSSocket, updateFile.GetName(), updateFile.GetSize());
		break;

	case NosUpdate::FileInfo::FileActions::Delete:
		std::filesystem::remove(updateFile.GetName());
		break;

	default:
		NosLog::CreateLog(NosLog::Severity::Warning, "File Action {} doesn't have a case", updateFile.GetActionName());
		break;
	}
}