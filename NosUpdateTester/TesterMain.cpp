#include <NosUpdate/WinVersion.hpp>

#include <boost/asio/streambuf.hpp>
#include <format>
#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>

using ReqType = NosUpdate::Request::RequestTypes;

void SerializeRequest(boost::asio::streambuf* streamBuf, const ReqType& reqType)
{
	NosUpdate::Request::Ptr req(new NosUpdate::Request(reqType));
	std::ostream os(streamBuf);
	NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
	oa << req;
}

void SerializeUpdateRequest(boost::asio::streambuf* streamBuf)
{
	NosUpdate::Request::Ptr req(new NosUpdate::UpdateRequest(130));
	std::ostream os(streamBuf);
	NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
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
	NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
	ia >> req;

	HandleRequest(req);
}

int main()
{
	{
		boost::asio::streambuf requestBuf;
		SerializeRequest(&requestBuf, ReqType::NewestVersion);
		DeserializeRequest(&requestBuf);
	}

	{
		boost::asio::streambuf requestBuf;
		SerializeUpdateRequest(&requestBuf);
		DeserializeRequest(&requestBuf);
	}
	

	printf("Press any button to continue"); getchar();
	return 0;
}