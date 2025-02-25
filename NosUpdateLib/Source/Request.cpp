#include "NosUpdate/Request.hpp"

namespace NosUpdate
{
	Request::Request(boost::asio::streambuf* Streambuf)
	{
		DeserializeObject(Streambuf);
	}

	Request::RequestTypes Request::GetRequestType() const
	{
		return RequestType;
	}

	uint64_t Request::GetDataLeft() const
	{
		return AmountByteLeft;
	}

	void Request::serializeObject(std::streambuf* Streambuf)
	{
		boost::archive::binary_oarchive oa(*Streambuf);
		oa&* (this);
	}

	void Request::DeserializeObject(boost::asio::streambuf* Streambuf)
	{
		boost::archive::binary_iarchive ia(*Streambuf);
		ia&* (this);
	}
}