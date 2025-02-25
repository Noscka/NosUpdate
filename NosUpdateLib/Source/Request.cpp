#include <NosUpdate/Request.hpp>
#include <NosUpdate/BoostExpand/portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/portable_binary_iarchive.hpp>

namespace NosUpdate
{
	Request::Request(boost::asio::streambuf* StreamBuf)
	{
		DeserializeObject(StreamBuf);
	}

	Request::RequestTypes Request::GetRequestType() const
	{
		return RequestType;
	}

	uint64_t Request::GetDataLeft() const
	{
		return AmountByteLeft;
	}

	void Request::SerializeObject(boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_oarchive oa(os);
		oa&* (this);
	}

	void Request::DeserializeObject(boost::asio::streambuf* StreamBuf)
	{
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::portable_binary_iarchive ia(is);
		ia&* (this);
	}
}