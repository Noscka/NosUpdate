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
	typedef unsigned char Byte;

	inline const std::string UpdateHostname = "update.nosteck.com";
	inline constexpr uint16_t UpdatePort = 8100;
}