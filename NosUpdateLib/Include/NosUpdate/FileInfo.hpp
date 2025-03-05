#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <boost/filesystem.hpp>

#include <string>

namespace NosUpdate
{
	class NOSUPDATE_API FileInfo
	{
	public:
		enum class FileActions : uint8_t
		{
			Update,
			Delete
		};

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& FileName;
			archive& Hash;
			archive& FileAction;
			archive& FileSize;
		}

	protected:
		std::string FileName;
		std::string Hash;
		FileActions FileAction;
		uint64_t FileSize;

	public:
		FileInfo() = default;
		FileInfo(const std::string& fileName, const FileActions& fileAction) :
			FileName(fileName),
			Hash(CreateSHA256Hash(fileName)),
			FileAction(fileAction),
			FileSize(boost::filesystem::file_size(boost::filesystem::path(fileName)))
		{}

		std::string GetName();
		std::string GetHash();
		FileActions GetAction();
		uint64_t GetSize();

	private:
		std::string CreateSHA256Hash(const std::string& filename);

	};
}