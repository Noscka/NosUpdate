#pragma once
#include "WinVersion.hpp"

#include <boost/algorithm/string.hpp>
#include <string>

/*
Terminology

Sectioning -> collects data about file and puts it in defined sections for the client to later desection and use
|8 bytes       | metadata size                      | 8 bytes      | Delimiter (used for telling the client when to stop reading)
|metadata size | metadata (only filename currently) | content size | Delimiter (used for telling the client when to stop reading)

-------------------------------------------------------------------------------------------------------------------------------

segments -> Used to be named chunks. segements are defined amounts of data (currently 500MB) that get sent.
allows for more optimized data sending with minimal memory usage and quicker sending speeds

for server:
the server used to send the file like this: file(all) -> memory -> send
and now works:                              file(500MB) -> send, repeat untill all sent
*/

namespace Definition
{
	inline const std::string Delimiter = "\n\r\n\r\n\013\x4\n";
	inline constexpr int SegementSize = 524288000;
	typedef unsigned char byte;
}

namespace NosUpdate
{
	inline std::string GetDelimiter()
	{
		return Definition::Delimiter;
	}

	inline std::string GetRawDelimiter()
	{
		std::string returnString = Definition::Delimiter;
		boost::replace_all(returnString, L"\n", L"\\n");
		boost::replace_all(returnString, L"\r", L"\\r");
		boost::replace_all(returnString, L"\013", L"\\013");
		boost::replace_all(returnString, L"\x4", L"\\x4");
		return returnString;
	}
}