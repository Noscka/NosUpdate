#pragma once
#include "Request.hpp"

#include <NosUpdate/Version.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API VersionRequest : public Request
	{
	public:
		using Ptr = std::unique_ptr<VersionRequest>;

	public:
		enum class VersionTypes : uint8_t
		{
			Newest,	/* Request to get newest version */
		};

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Request>(*this);
			archive& VersionType;
		}

	protected:
		VersionTypes VersionType;	/* What the client wants */

	public:
		VersionRequest() = default;
		VersionRequest(const VersionTypes& versionType) : Request(RequestTypes::Version), VersionType(versionType) {}
		~VersionRequest() override = default;

		std::string GetRequestName() const override
		{
			return "VersionRequest";
		}

		VersionTypes GetVersionType() const;
		std::string GetVersionTypeName() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::VersionRequest)