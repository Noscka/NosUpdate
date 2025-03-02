#pragma once
#include "WinVersion.hpp"
#include <NosUpdate/Internal/Export.hpp>

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

namespace Definitions
{
	inline const std::string Delimiter = "\n\r\n\r\n\013\x4\n";
	inline constexpr int SegementSize = 524288000;
	typedef unsigned char Byte;

	inline const std::string UpdateHostname = "update.nosteck.com";
	inline constexpr uint16_t UpdatePort = 8100;
}

namespace NosUpdate
{
	inline std::string GetDelimiter()
	{
		return Definitions::Delimiter;
	}

	inline std::string GetRawDelimiter()
	{
		std::string returnString = Definitions::Delimiter;
		boost::replace_all(returnString, "\n", "\\n");
		boost::replace_all(returnString, "\r", "\\r");
		boost::replace_all(returnString, "\013", "\\013");
		boost::replace_all(returnString, "\x4", "\\x4");
		return returnString;
	}
}