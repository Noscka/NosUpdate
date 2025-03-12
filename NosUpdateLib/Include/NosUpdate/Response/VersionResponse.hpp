#pragma once
#include "Response.hpp"

#include <NosUpdate/Version.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API VersionResponse : public Response
	{
	public:
		using Ptr = std::unique_ptr<VersionResponse>;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Response>(*this);
			archive& RequestedVersion;
		}

	protected:
		Version RequestedVersion;

	public:
		VersionResponse() = default;
		VersionResponse(const Version& requestedVersion) : Response(ResponseTypes::Version), RequestedVersion(requestedVersion) {}
		~VersionResponse() override = default;

		std::string GetResponseName() const override
		{
			return "VersionResponse";
		}

		Version GetRequestedVersion() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::VersionResponse)