#include <NosUpdate/Request/UpdateRequest.hpp>

#include <filesystem>

namespace NosUpdate
{
	void UpdateRequest::GetCurrentFiles(const std::string& rootPath)
	{
		namespace filesys = std::filesystem;

		for (filesys::recursive_directory_iterator i(rootPath), end; i != end; ++i)
		{
			if (is_directory(i->path()))
			{
				continue;
			}

			std::string filePath = i->path().string();

			NosLib::Net::FileInfo newFile(filePath);

			CurrentFiles.push_back(newFile);
		}
	}

	Version UpdateRequest::GetUpdateVersion() const
	{
		return UpdateVersion;
	}

	ProgramInfo UpdateRequest::GetProgramInfo() const
	{
		return ProgramInfoObj;
	}

	std::vector<NosLib::Net::FileInfo> UpdateRequest::GetCurrentFileInfo() const
	{
		return CurrentFiles;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateRequest)