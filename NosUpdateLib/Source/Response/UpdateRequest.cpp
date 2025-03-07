#include <NosUpdate/Response/UpdateResponse.hpp>

#include <filesystem>

namespace NosUpdate
{
	void UpdateResponse::GetUpdateFiles(const ProgramInfo& programInfo)
	{
		using namespace std::filesystem;
		using namespace std;

		std::string programPath = programInfo.GetNormalizedName();

		for (recursive_directory_iterator i(programPath), end; i != end; ++i)
		{
			if (is_directory(i->path()))
			{
				continue;
			}

			std::string filePath = i->path().string();

			FileInfo newFile(filePath, FileInfo::FileActions::Update);

			UpdateFiles.push_back(newFile);
		}
	}

	Version UpdateResponse::GetUpdateVersion() const
	{
		return UpdateVersion;
	}

	std::vector<FileInfo> UpdateResponse::GetUpdateFileInfo() const
	{
		return UpdateFiles;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateResponse)