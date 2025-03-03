#pragma once
#include "Response.hpp"
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

#include <NosUpdate/Version.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API UpdateResponse : public Response
	{
	public:
		using Ptr = std::unique_ptr<UpdateResponse>;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Response>(*this);
			archive& UpdateVersion;
			archive& FileSize;
		}

	protected:
		Version UpdateVersion;
		uint64_t FileSize;

	public:
		UpdateResponse() = default;
		UpdateResponse(const Version& updateVersion, const uint64_t& fileSize) :
			Response(ResponseTypes::Version),
			UpdateVersion(updateVersion),
			FileSize(fileSize)
		{}
		~UpdateResponse() override = default;

		std::string GetResponseName() const override
		{
			return "UpdateResponse";
		}

		Version GetUpdateVersion() const;
		uint64_t GetFileSize() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateResponse)