#pragma once
#include "WinVersion.hpp"
#include <NosUpdate/Internal/Export.hpp>

#include <boost/algorithm/string.hpp>
#include <string>

namespace Definitions
{
	constexpr uint64_t FileSegmentSize = 256000; /* Segments for sending/reading files */
	typedef unsigned char Byte;

	inline const std::string UpdateHostname = "update.nosteck.com";
	inline constexpr uint16_t UpdatePort = 8100;
}