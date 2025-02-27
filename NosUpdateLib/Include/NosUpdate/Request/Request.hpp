#pragma once

#include "../WinVersion.hpp"

#include <boost/asio.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
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
		Request(const RequestTypes& requestType) : RequestType(requestType) {}

		virtual ~Request() = default;

		RequestTypes GetRequestType() const;

		static void SerializeRequest(const Ptr&, boost::asio::streambuf*);
		static Ptr DeserializeRequest(boost::asio::streambuf*);
	};
}

BOOST_SERIALIZATION_ASSUME_ABSTRACT(NosUpdate::Request)