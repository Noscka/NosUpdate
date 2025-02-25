#pragma once

#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace NosUpdate
{
	class Request
	{
	public:
		enum class RequestTypes
		{
			Update,			/* Request to update to the newest version */
			NewestVersion	/* Request to get newest version */
		};

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& this->RequestType;
			archive& this->AmountByteLeft;
		}

		RequestTypes RequestType;	/* What the client wants */
		uint64_t AmountByteLeft;	/* Currently unused: how much the client already downloaded (where to continue from) */

	public:
		Request() {}
		Request(boost::asio::streambuf* Streambuf);
		Request(const RequestTypes& requestType) : RequestType(requestType) {}
		Request(const RequestTypes& requestType, const uint64_t& ByteLeft) : RequestType(requestType), AmountByteLeft(ByteLeft) {}

		RequestTypes GetRequestType() const;
		uint64_t GetDataLeft() const;

		void serializeObject(std::streambuf* Streambuf);
		void DeserializeObject(boost::asio::streambuf* Streambuf);
	};
}