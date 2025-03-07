#pragma once
#include "Response.hpp"
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

#include <NosUpdate/Version.hpp>
#include <NosUpdate/Info/FileInfo.hpp>
#include <NosUpdate/Info/ProgramInfo.hpp>

#include <NosLib/DynamicArray.hpp>
#include <vector>

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
			archive& UpdateFiles;
		}

	protected:
		Version UpdateVersion;
		std::vector<FileInfo> UpdateFiles;

		void GetUpdateFiles(const ProgramInfo&);
	public:
		UpdateResponse() = default;
		UpdateResponse(const Version& updateVersion, const ProgramInfo& programInf) :
			Response(ResponseTypes::Version),
			UpdateVersion(updateVersion)
		{
			GetUpdateFiles(programInf);
		}

		~UpdateResponse() override = default;

		std::string GetResponseName() const override
		{
			return "UpdateResponse";
		}

		Version GetUpdateVersion() const;
		std::vector<FileInfo> GetUpdateFileInfo() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateResponse)