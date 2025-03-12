#pragma once
#include "Request.hpp"

#include <boost/serialization/vector.hpp>
#include <NosUpdate/Info/ProgramInfo.hpp>
#include <NosUpdate/Info/FileInfo.hpp>
#include <NosUpdate/Version.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API UpdateRequest : public Request
	{
	public:
		using Ptr = std::unique_ptr<UpdateRequest>;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Request>(*this);
			archive& UpdateVersion;
			archive& ProgramInfoObj;
			archive& CurrentFiles;
		}

	protected:
		Version UpdateVersion;
		ProgramInfo ProgramInfoObj;
		std::vector<FileInfo> CurrentFiles;
		//uint64_t AmountByteLeft;	/* Currently unused: how much the client already downloaded (where to continue from) */

		void GetCurrentFiles(const std::string&);
	public:
		UpdateRequest() : Request(RequestTypes::Update) {}
		UpdateRequest(const Version& updateVersion, const std::string& programName, const std::string& rootPath) :
			Request(RequestTypes::Update),
			UpdateVersion(updateVersion),
			ProgramInfoObj(programName)
		{
			GetCurrentFiles(rootPath);
		}

		~UpdateRequest() override = default;

		std::string GetRequestName() const override
		{
			return "UpdateRequest";
		}

		Version GetUpdateVersion() const;
		ProgramInfo GetProgramInfo() const;
		std::vector<FileInfo> GetCurrentFileInfo() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateRequest)