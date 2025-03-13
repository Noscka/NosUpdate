#include <NosUpdate/Response/UpdateResponse.hpp>

#include <filesystem>
#include <algorithm>

namespace NosUpdate
{
	UpdateResponse::FileInfoVec UpdateResponse::GetLocalFiles(const UpdateRequest::Ptr& updateRequest)
	{
		namespace filesys = std::filesystem;

		std::string programPath = updateRequest->GetProgramInfo().GetNormalizedName();

		FileInfoVec serverSideFiles;
		for (filesys::recursive_directory_iterator i(programPath), end; i != end; ++i)
		{
			if (is_directory(i->path()))
			{
				continue;
			}

			std::string filePath = i->path().string();

			FileInfo newFile(filePath, FileInfo::FileActions::Update);

			serverSideFiles.push_back(newFile);
		}

		return serverSideFiles;
	}

	UpdateResponse::FileInfoVec UpdateResponse::GetFilesDifference(const FileInfoVec& serverFiles, const FileInfoVec& clientFiles)
	{
		namespace filesys = std::filesystem;

		FileInfoVec updateFiles;

		size_t i = 0, j = 0;
		while (i < clientFiles.size() || j < serverFiles.size())
		{
			/* Only Client Has | Deduced from server being out of files */
			if (i < clientFiles.size() && j >= serverFiles.size())
			{
				updateFiles.push_back(FileInfo(clientFiles[i], FileInfo::FileActions::Delete));
				i++;
				continue;
			}

			/* Only Server Has | Deduced from client being out of files */
			if (j < serverFiles.size() && i >= clientFiles.size())
			{
				updateFiles.push_back(FileInfo(serverFiles[j], FileInfo::FileActions::Update));
				j++;
				continue;
			}

			const FileInfo& clientFile = clientFiles[i];
			const FileInfo& serverFile = serverFiles[j];

			filesys::path clientFilePath(clientFile.GetName());
			filesys::path serverFilePath(serverFile.GetName());

			clientFilePath = clientFilePath.lexically_normal();
			serverFilePath = serverFilePath.lexically_normal();

			/* Only Client Has | Deduced from server file path value being larger the client */
			if (clientFilePath > serverFilePath)
			{
				updateFiles.push_back(FileInfo(clientFiles[i], FileInfo::FileActions::Delete));
				i++;
				continue;
			}

			/* Only Client Has | Deduced from client file path value being larger the server */
			if (clientFilePath < serverFilePath)
			{
				updateFiles.push_back(FileInfo(serverFiles[j], FileInfo::FileActions::Update));
				j++;
				continue;
			}

			/* clientFilePath == serverFilePath */

			int cmp = std::memcmp(clientFile.GetHash().HashBinary.data(),
								  serverFile.GetHash().HashBinary.data(),
								  clientFile.GetHash().HashLenght);

			if (cmp == 0)
			{
				i++; j++;
				continue;
			}

			updateFiles.push_back(FileInfo(serverFiles[i], FileInfo::FileActions::Update));
			i++; j++;
		}

		return updateFiles;
	}

	void UpdateResponse::GetUpdateFiles(const UpdateRequest::Ptr& updateRequest)
	{
		FileInfoVec serverSideFiles = GetLocalFiles(updateRequest);

		auto cmp = [](const FileInfo& a, const FileInfo& b)
		{
			namespace filesys = std::filesystem;
			filesys::path aPath(a.GetName());
			filesys::path bPath(b.GetName());

			return aPath < bPath;
		};

		FileInfoVec clientSideFiles = updateRequest->GetCurrentFileInfo();

		std::sort(clientSideFiles.begin(), clientSideFiles.end(), cmp);
		std::sort(serverSideFiles.begin(), serverSideFiles.end(), cmp);

		UpdateFiles = GetFilesDifference(serverSideFiles, clientSideFiles);
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