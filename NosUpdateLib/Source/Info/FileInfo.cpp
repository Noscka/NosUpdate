#include <NosUpdate/Info/FileInfo.hpp>

#include <openssl/evp.h>
#include <openssl/err.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdexcept>

std::string NosUpdate::FileInfo::GetName() const
{
	return FileName;
}

std::string NosUpdate::FileInfo::GetHash() const
{
	return Hash;
}

NosUpdate::FileInfo::FileActions NosUpdate::FileInfo::GetAction() const
{
	return FileAction;
}

uint64_t NosUpdate::FileInfo::GetSize() const
{
	return FileSize;
}

std::string NosUpdate::FileInfo::CreateSHA256Hash(const std::string& filename) const
{
	const size_t bufferSize = 4096;
	std::vector<unsigned char> buffer(bufferSize);
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLen = 0;

	// Create and initialize a digest context
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx)
		throw std::runtime_error("Failed to create EVP_MD_CTX");

	if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1)
	{
		EVP_MD_CTX_free(mdctx);
		throw std::runtime_error("EVP_DigestInit_ex failed");
	}

	// Open the file in binary mode
	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		EVP_MD_CTX_free(mdctx);
		throw std::runtime_error("Could not open file: " + filename);
	}

	// Read the file in chunks and update the digest
	while (file.good())
	{
		file.read(reinterpret_cast<char*>(buffer.data()), bufferSize);
		std::streamsize bytesRead = file.gcount();
		if (bytesRead > 0)
		{
			if (EVP_DigestUpdate(mdctx, buffer.data(), bytesRead) != 1)
			{
				EVP_MD_CTX_free(mdctx);
				throw std::runtime_error("EVP_DigestUpdate failed");
			}
		}
	}

	// Finalize the digest
	if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1)
	{
		EVP_MD_CTX_free(mdctx);
		throw std::runtime_error("EVP_DigestFinal_ex failed");
	}
	EVP_MD_CTX_free(mdctx);

	// Convert the hash to a hexadecimal string
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (unsigned int i = 0; i < hashLen; ++i)
		oss << std::setw(2) << static_cast<int>(hash[i]);
	return oss.str();
}