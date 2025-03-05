#pragma once
#include "Response.hpp"
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

#include <NosUpdate/Version.hpp>
#include <NosUpdate/FileInfo.hpp>

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
			archive& FileInfoObj;
		}

	protected:
		Version UpdateVersion;
		FileInfo FileInfoObj;

	public:
		UpdateResponse() = default;
		UpdateResponse(const Version& updateVersion, const std::string& FileName) :
			Response(ResponseTypes::Version),
			UpdateVersion(updateVersion),
			FileInfoObj(FileName, FileInfo::FileActions::Update)
		{}
		~UpdateResponse() override = default;

		std::string GetResponseName() const override
		{
			return "UpdateResponse";
		}

		Version GetUpdateVersion() const;
		FileInfo GetFileInfo() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateResponse)