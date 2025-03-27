#pragma once
#include "Response.hpp"

#include <boost/serialization/vector.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosLib/Net/FileInfo.hpp>
#include <NosUpdate/Info/ProgramInfo.hpp>
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
			archive& UpdateFiles;
		}

	protected:
		using FileInfoVec = std::vector<NosLib::Net::FileInfo>;

		Version UpdateVersion;
		FileInfoVec UpdateFiles;

		FileInfoVec GetLocalFiles(const UpdateRequest::Ptr&);
		FileInfoVec GetFilesDifference(const FileInfoVec&, const FileInfoVec&);
		void GetUpdateFiles(const UpdateRequest::Ptr&);
	public:
		UpdateResponse() = default;
		UpdateResponse(const Version& updateVersion, const UpdateRequest::Ptr& updateRequest) :
			Response(ResponseTypes::Version),
			UpdateVersion(updateVersion)
		{
			GetUpdateFiles(updateRequest);
		}

		~UpdateResponse() override = default;

		std::string GetResponseName() const override
		{
			return "UpdateResponse";
		}

		Version GetUpdateVersion() const;
		std::vector<NosLib::Net::FileInfo> GetUpdateFileInfo() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateResponse)