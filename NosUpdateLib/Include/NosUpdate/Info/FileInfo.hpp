#pragma once
#include <NosUpdate/WinVersion.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/filesystem.hpp>

#include <NosLib/File.hpp>

#include <string>
#include <array>

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
			archive& FileHash;
			archive& FileAction;
			archive& FileSize;
		}

	protected:
		std::string FileName;
		NosLib::Hash FileHash;
		FileActions FileAction;
		uint64_t FileSize;

	public:
		FileInfo() = default;
		FileInfo(const std::string& fileName, const FileActions& fileAction = FileActions::Update) :
			FileName(fileName),
			FileHash(NosLib::GetHash(fileName, EVP_sha256())),
			FileAction(fileAction),
			FileSize(boost::filesystem::file_size(boost::filesystem::path(fileName)))
		{}

		FileInfo(const FileInfo& other, const FileActions& fileAction) :
			FileName(other.FileName),
			FileHash(other.FileHash),
			FileAction(fileAction),
			FileSize(other.FileSize)
		{ }

		std::string GetName() const;
		NosLib::Hash GetHash() const;
		std::string GetHashString() const;
		FileActions GetAction() const;
		uint64_t GetSize() const;
	};
}