#include <NosUpdate/Request.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>

namespace NosUpdate
{
	Request::RequestTypes Request::GetRequestType() const
	{
		return RequestType;
	}

	void Request::SerializeRequest(const Request::Ptr& objectPtr, boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa << objectPtr;
	}

	Request::Ptr Request::DeserializeRequest(boost::asio::streambuf* StreamBuf)
	{
		Ptr newObjectPtr;
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia >> newObjectPtr;
		return newObjectPtr;
	}
}