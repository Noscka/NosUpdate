#pragma once

#include <boost/asio.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

namespace NosUpdate
{
	class Request
	{
	public:
		using Ptr = std::unique_ptr<Request>;

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
			archive& RequestType;
		}

		RequestTypes RequestType;	/* What the client wants */
		
	public:
		Request() = default;
		Request(boost::asio::streambuf*);
		Request(const RequestTypes& requestType) : RequestType(requestType) {}

		virtual ~Request(){}

		virtual RequestTypes GetRequestType() const;

		static void SerializeRequest(Request*, boost::asio::streambuf*);
		static Ptr DeserializeRequest(boost::asio::streambuf*);
	};
}

BOOST_SERIALIZATION_ASSUME_ABSTRACT(NosUpdate::Request)