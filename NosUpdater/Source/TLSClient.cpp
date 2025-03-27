#include "../Header/TLSClient.hpp"

#include <NosLib/Net/Helper.hpp>
#include <NosLib/Net/FileReceive.hpp>

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
	return NosLib::Net::EndpointAsString(GetSocket().local_endpoint());
}

std::string TLSClient::GetRemoteEndpoint()
{
	return NosLib::Net::EndpointAsString(GetSocket().remote_endpoint());
}

NosLib::Result<void> TLSClient::Connect()
{
	boost::system::error_code error;

	/*
	Connects to the function using `resolver` which resolves the address e.g. (Noscka.com -> 123.123.123.123)
	Host - Hostname/ip address
	Service - Service(Hostname for ports)/Port number
	*/
	auto resolvedEndpoint = boost::asio::ip::tcp::resolver(IOContext).resolve(Hostname.c_str(), std::to_string(Port), error);

	NOSLOG_ASSERT(error, return NosUpdate::NetErrors::Unresolve, NosLog::Severity::Error, "Unable to resolve endpoint | {}", error.message());

	boost::asio::connect(GetSocket(), resolvedEndpoint, error);

	NOSLOG_ASSERT(error, return NosUpdate::NetErrors::Unconnectable, NosLog::Severity::Error, "Unabled to connect to endpoint | {}", error.message());

	TLSSocket.set_verify_mode(boost::asio::ssl::verify_peer);
	TLSSocket.handshake(boost::asio::ssl::stream_base::client, error);

	NOSLOG_ASSERT(error, return NosUpdate::NetErrors::Handshake, NosLog::Severity::Error, "Closing connection with {} | Handshake error: {}", GetRemoteEndpoint(), error.message());

	NosLog::CreateLog(NosLog::Severity::Info, "Succesfully connected to: {}:{}", Hostname, Port);
	NosLog::CreateLog(NosLog::Severity::Debug, "Endpoint: {}", GetRemoteEndpoint());

	return {};
}

using UpRes = NosUpdate::UpdateResponse;

NosLib::Result<void> TLSClient::UpdateProgram()
{
	NosLib::Result<NosUpdate::Version> newestVersion = GetNewestVersion();
	NOS_ASSERT(!newestVersion, return newestVersion.ErrorCode());

	NosLib::Result<UpRes::Ptr> updateRes = RequestUpdate(newestVersion);
	NOS_ASSERT(!updateRes, return updateRes.ErrorCode());

	ReceivedUpdateFiles(updateRes);
	return {};
}

NosLib::Result<NosUpdate::Version> TLSClient::GetNewestVersion()
{
	NosLib::Net::SerializeSend<NosUpdate::VersionRequest>(TLSSocket, NosUpdate::VersionRequest::VersionTypes::Newest);
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Newest Version");

	NosUpdate::VersionResponse::Ptr versionRes = NosLib::Net::DeserializeRead<NosUpdate::VersionResponse>(TLSSocket);

	NOSLOG_ASSERT(!versionRes, return NosLib::GenericErrors::Casting, NosLog::Severity::Error, "Unable to cast to Version Response");

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded with {} Version", versionRes->GetRequestedVersion().GetVersion());

	return versionRes->GetRequestedVersion();
}

NosLib::Result<UpRes::Ptr> TLSClient::RequestUpdate(const NosUpdate::Version& version)
{
	NosLib::Net::SerializeSend<NosUpdate::UpdateRequest>(TLSSocket, version, "TestProgram", "./TestProgram");
	NosLog::CreateLog(NosLog::Severity::Info, "Requested Update Version");

	UpRes::Ptr updateRes = NosLib::Net::DeserializeRead<NosUpdate::UpdateResponse>(TLSSocket);

	NOSLOG_ASSERT(!updateRes, return NosLib::GenericErrors::Casting, NosLog::Severity::Error, "Unable to cast to Update Response");

	NosLog::CreateLog(NosLog::Severity::Debug, "Server Responded Update | Version: {}", updateRes->GetUpdateVersion().GetVersion());
	
	return updateRes;
}

NosLib::Result<void> TLSClient::ReceivedUpdateFiles(const NosUpdate::UpdateResponse::Ptr& updateRes)
{
	std::vector<NosLib::Net::FileInfo> fileInfos = updateRes->GetUpdateFileInfo();

	for (NosLib::Net::FileInfo& fileInfo : fileInfos)
	{
		NosLib::Result<void> processingRes = ProcessUpdateFile(fileInfo);

		/* If there was some problem */
		if (!processingRes)
		{
			return processingRes;
		}
	}

	return {};
}


NosLib::Result<void> TLSClient::ProcessUpdateFile(const NosLib::Net::FileInfo& updateFile)
{
	NosLog::CreateLog(NosLog::Severity::Info, "Receiving file: File Name: {} | File Action: {}", updateFile.GetName(), updateFile.GetActionName());
	NosLog::CreateLog(NosLog::Severity::Debug, "File Hash: {} | File Size: {}", updateFile.GetHashString(), updateFile.GetSize());

	std::error_code ec;

	switch (updateFile.GetAction())
	{
	case NosLib::Net::FileInfo::FileActions::Update:
		NosLib::Net::ReceiveFile(TLSSocket, updateFile.GetName(), updateFile.GetSize());
		break;

	case NosLib::Net::FileInfo::FileActions::Delete:
		std::filesystem::remove(updateFile.GetName(), ec);
		break;

	default:
		NosLog::CreateLog(NosLog::Severity::Warning, "File Action {} doesn't have a case", updateFile.GetActionName());
		break;
	}

	return {};
}