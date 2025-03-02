#include <NosUpdate/Response/Response.hpp>
#include <NosUpdate/Responses.hpp>

namespace NosUpdate
{
	Response::ResponseTypes Response::GetResponseType() const
	{
		return ResponseType;
	}

	std::string Response::GetResponseTypeName() const
	{
		switch (ResponseType)
		{
			using enum ResponseTypes;
		case NewestVersion:
			return "NewestVersion";

		case Update:
			return "Update";

		default:
			return "UNDEFINED";
		}
	}

	void Response::SerializeResponse(const Ptr& objectPtr, boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa << objectPtr;
	}

	Response::Ptr Response::DeserializeResponse(boost::asio::streambuf* StreamBuf)
	{
		Ptr newObjectPtr;
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia >> newObjectPtr;
		return newObjectPtr;
	}
}