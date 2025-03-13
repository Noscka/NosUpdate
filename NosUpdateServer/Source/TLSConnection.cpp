#include "../Header/TLSConnection.hpp"

#include <NosUpdate/Helper.hpp>
#include <NosUpdate/FileNet/FileSend.hpp>

TLSConnection* TLSConnection::CreateConnection(boost::asio::io_context& io_context, bSSL::context& ssl_context)
{
	return new TLSConnection(io_context, ssl_context);
}

TLSConnection::TLSStream& TLSConnection::GetTLSSocket()
{
	return TLSSocket;
}

TLSConnection::TCPStream& TLSConnection::GetSocket()
{
	return TLSSocket.next_layer();
}

std::string TLSConnection::GetLocalEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().local_endpoint());
}

std::string TLSConnection::GetRemoteEndpoint()
{
	return NosUpdate::EndpointAsString(GetSocket().remote_endpoint());
}

void TLSConnection::start()
{
	boost::system::error_code error;
	TLSSocket.handshake(bSSL::stream_base::server, error);
	if (error)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Closing connection with {} | Handshake error: {}", GetRemoteEndpoint(), error.message());
		return;
	}

	NosLog::CreateLog(NosLog::Severity::Info, "Client succesfully connected from {}", GetRemoteEndpoint());

	while (!error)
	{
		NosUpdate::Request::Ptr clientsRequest = GetClientsRequest();
		HandleRequest(clientsRequest);
	}
}

NosUpdate::Request::Ptr TLSConnection::GetClientsRequest()
{
	NosUpdate::Request::Ptr clientsRequest;

	boost::asio::streambuf reqBuf;
	NosUpdate::SimpleRead(TLSSocket, reqBuf);
	NosLog::CreateLog(NosLog::Severity::Debug, "Got Client Request");
	clientsRequest = NosUpdate::Request::Deserialize(&reqBuf);

	return clientsRequest;
}

void TLSConnection::HandleRequest(NosUpdate::Request::Ptr& clientsRequest)
{
	using rqTp = NosUpdate::Request::RequestTypes;
	std::string acknowledgement;

	switch (clientsRequest->GetRequestType())
	{
	case rqTp::Update:
		HandleUpdateRequest(clientsRequest);
		break;

	case rqTp::Version:
		HandleVersionRequest(clientsRequest);
		break;
	}
}

void TLSConnection::HandleVersionRequest(NosUpdate::Request::Ptr& clientsRequest)
{
	NosUpdate::VersionRequest::Ptr versionReq = NosLib::Pointer::DynamicUniquePtrCast<NosUpdate::VersionRequest, NosUpdate::Request>(std::move(clientsRequest));

	if (!versionReq)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to cast to Version Request");
		return;
	}
	NosLog::CreateLog(NosLog::Severity::Info, "Client Requested {} Version", versionReq->GetVersionTypeName());

	NosUpdate::Version updateVersion(0, 0, 1);
	NosUpdate::SerializeSend<NosUpdate::VersionResponse>(TLSSocket, updateVersion);

	NosLog::CreateLog(NosLog::Severity::Info, "Responded Newest Version | version: {}", updateVersion.GetVersion());
}

void TLSConnection::HandleUpdateRequest(NosUpdate::Request::Ptr& clientsRequest)
{
	NosUpdate::UpdateRequest::Ptr updateReq = NosLib::Pointer::DynamicUniquePtrCast<NosUpdate::UpdateRequest, NosUpdate::Request>(std::move(clientsRequest));

	if (!updateReq)
	{
		NosLog::CreateLog(NosLog::Severity::Error, "Unable to cast to Update Request");
		return;
	}
	
	NosUpdate::ProgramInfo progInfo = updateReq->GetProgramInfo();

	NosLog::CreateLog(NosLog::Severity::Info, "Client Requested Update | Version: {} | Program Name: {}",
					  updateReq->GetUpdateVersion().GetVersion(),
					  progInfo.GetNormalizedName());

	NosUpdate::Version updateVersion = updateReq->GetUpdateVersion();

	NosUpdate::UpdateResponse::Ptr updatRes(new NosUpdate::UpdateResponse(updateVersion, updateReq));
	NosUpdate::SerializeSendPre<NosUpdate::UpdateResponse>(TLSSocket, updatRes);

	SendUpdateFiles(updatRes);
}

void TLSConnection::SendUpdateFiles(const NosUpdate::UpdateResponse::Ptr& updateRes)
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

		NosLog::CreateLog(NosLog::Severity::Info, "File: File Name: {} | File Action: {}", fileInfo.GetName(), fileAction);
		NosLog::CreateLog(NosLog::Severity::Debug, "File Hash: {} | File Size: {}", fileInfo.GetHashString(), fileInfo.GetSize());

		if (fileInfo.GetAction() != NosUpdate::FileInfo::FileActions::Update)
		{
			continue;
		}

		NosUpdate::SendFile(TLSSocket, fileInfo.GetName());
	}
}