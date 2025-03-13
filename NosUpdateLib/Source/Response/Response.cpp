#include <NosUpdate/Response/Response.hpp>
#include <NosUpdate/Responses.hpp>

#include <NosLib/Logging.hpp>

namespace NosUpdate
{
	using NosLog = NosLib::Logging;

	Response::ResponseTypes Response::GetResponseType() const
	{
		return ResponseType;
	}

	std::string Response::GetResponseTypeName() const
	{
		switch (ResponseType)
		{
		case ResponseTypes::Version:
			return "Version";

		case ResponseTypes::Update:
			return "Update";
		}

		return "UNDEFINED";
	}

	void Response::Serialize(const Response::Base* objectPtr, boost::asio::streambuf* streamBuf)
	{
		std::ostream os(streamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa << objectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Serialized Response | Class Name: {} | Type Name: {}", objectPtr->GetResponseName(), objectPtr->GetResponseTypeName());
	}

	void Response::Serialize(const Ptr& objectPtr, boost::asio::streambuf* streamBuf)
	{
		return Serialize(objectPtr.get(), streamBuf);
	}

	Response::Ptr Response::Deserialize(boost::asio::streambuf* streamBuf)
	{
		return Ptr(DeserializeRaw(streamBuf));
	}

	Response::Base* Response::DeserializeRaw(boost::asio::streambuf* streamBuf)
	{
		Response::Base* newObjectPtr;
		std::istream is(streamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia >> newObjectPtr;

		NosLog::CreateLog(NosLog::Severity::Debug, "Deserialized Response | Class Name: {} | Type Name: {}", newObjectPtr->GetResponseName(), newObjectPtr->GetResponseTypeName());

		return newObjectPtr;
	}
}