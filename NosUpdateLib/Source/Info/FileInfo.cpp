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

NosLib::Hash NosUpdate::FileInfo::GetHash() const
{
	return FileHash;
}

std::string NosUpdate::FileInfo::GetHashString() const
{
	return NosLib::GetHashString(FileHash);
}

NosUpdate::FileInfo::FileActions NosUpdate::FileInfo::GetAction() const
{
	return FileAction;
}

uint64_t NosUpdate::FileInfo::GetSize() const
{
	return FileSize;
}