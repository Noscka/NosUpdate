#pragma once
#include "Response.hpp"
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API VersionResponse : public Response
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Response>(*this);
			archive& RequestedVersion;
		}

	protected:
		std::string RequestedVersion;

	public:
		VersionResponse() = default;
		VersionResponse(const std::string& requestedVersion) : Response(ResponseTypes::Version), RequestedVersion(requestedVersion) {}
		~VersionResponse() override = default;

		std::string GetResponseName() const override
		{
			return "VersionResponse";
		}

		std::string GetRequestedVersion() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::VersionResponse)