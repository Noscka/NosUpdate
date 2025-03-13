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
		case RequestTypes::Version:
			return "Version";

		case RequestTypes::Update:
			return "Update";
		}

		return "UNDEFINED";
	}

	void Request::Serialize(const Request::Base* objectPtr, boost::asio::streambuf* streamBuf)
	{
		std::ostream os(streamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa << objectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Serialized Request | Class Name: {} | Type Name: {}", objectPtr->GetRequestName(), objectPtr->GetRequestTypeName());
	}

	void Request::Serialize(const Ptr& objectPtr, boost::asio::streambuf* streamBuf)
	{
		return Serialize(objectPtr.get(), streamBuf);
	}

	Request::Ptr Request::Deserialize(boost::asio::streambuf* streamBuf)
	{
		return Ptr(DeserializeRaw(streamBuf));
	}

	Request::Base* Request::DeserializeRaw(boost::asio::streambuf* streamBuf)
	{
		Request::Base* newObjectPtr;
		std::istream is(streamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia >> newObjectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Deserialized Request | Class Name: {} | Type Name: {}", newObjectPtr->GetRequestName(), newObjectPtr->GetRequestTypeName());

		return newObjectPtr;
	}
}