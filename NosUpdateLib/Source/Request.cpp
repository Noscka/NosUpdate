#include <NosUpdate/Request.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include <memory>

namespace NosUpdate
{
	/*Request::Request(boost::asio::streambuf* StreamBuf)
	{
		DeserializeObject(StreamBuf);
	}*/

	Request::RequestTypes Request::GetRequestType() const
	{
		return RequestType;
	}

	/*void Request::SerializeRequest(boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::polymorphic_portable_binary_oarchive oa(os);
		oa << *this;
	}

	void Request::DeserializeRequest(boost::asio::streambuf* StreamBuf)
	{
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::polymorphic_portable_binary_iarchive ia(is);
		ia >> *this;
	}*/

	void Request::SerializeRequest(Request* objectPtr, boost::asio::streambuf* StreamBuf)
	{
		std::ostream os(StreamBuf);
		NosUpdate::BoostExpand::polymorphic_portable_binary_oarchive oa(os);
		oa << objectPtr;
	}

	Request::Ptr Request::DeserializeRequest(boost::asio::streambuf* StreamBuf)
	{
		Ptr newObjectPtr;
		std::istream is(StreamBuf);
		NosUpdate::BoostExpand::polymorphic_portable_binary_iarchive ia(is);
		ia >> newObjectPtr;
		return newObjectPtr;
	}
}