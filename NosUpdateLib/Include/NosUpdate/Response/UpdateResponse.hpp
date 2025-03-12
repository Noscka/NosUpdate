#pragma once
#include "Response.hpp"

#include <boost/serialization/vector.hpp>
#include <NosUpdate/Request/UpdateRequest.hpp>
#include <NosUpdate/Info/FileInfo.hpp>
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
		Version UpdateVersion;
		std::vector<FileInfo> UpdateFiles;

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
		std::vector<FileInfo> GetUpdateFileInfo() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateResponse)