#pragma once

#include <NosUpdate/WinVersion.hpp>

#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
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
		Request(const RequestTypes& requestType) : RequestType(requestType) {}

		virtual ~Request() = default;

		RequestTypes GetRequestType() const;
	};
}
BOOST_SERIALIZATION_ASSUME_ABSTRACT(NosUpdate::Request)