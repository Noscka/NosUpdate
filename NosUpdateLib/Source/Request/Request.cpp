#include <NosUpdate/Request/Request.hpp>
#include <NosUpdate/Requests.hpp>

#include <NosLib/Logging.hpp>

namespace NosUpdate
{
	using NosLog = NosLib::Logging;

	Request::RequestTypes Request::GetRequestType() const
	{
		return RequestType;
	}

	std::string Request::GetRequestTypeName() const
	{
		switch (RequestType)
		{
			using enum RequestTypes;
		case Version:
			return "Version";

		case Update:
			return "Update";

		default:
			return "UNDEFINED";
		}
	}

	void Request::SerializeRequest(const Ptr& objectPtr, boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa << objectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Serialized Request | Class Name: {} | Type Name: {}", objectPtr->GetRequestName(), objectPtr->GetRequestTypeName());
	}

	Request::Ptr Request::DeserializeRequest(boost::asio::streambuf* StreamBuf)
	{
		Ptr newObjectPtr;
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia >> newObjectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Deserialized Request | Class Name: {} | Type Name: {}", newObjectPtr->GetRequestName(), newObjectPtr->GetRequestTypeName());

		return newObjectPtr;
	}
}