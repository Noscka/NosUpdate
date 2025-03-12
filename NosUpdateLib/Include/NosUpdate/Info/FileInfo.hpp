#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <boost/filesystem.hpp>

#include <NosLib/File.hpp>

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
		FileInfo(const std::string& fileName, const FileActions& fileAction = FileActions::Update) :
			FileName(fileName),
			Hash(NosLib::File::GetHash(fileName, EVP_sha256())),
			FileAction(fileAction),
			FileSize(boost::filesystem::file_size(boost::filesystem::path(fileName)))
		{}

		std::string GetName() const;
		std::string GetHash() const;
		FileActions GetAction() const;
		uint64_t GetSize() const;
	};
}