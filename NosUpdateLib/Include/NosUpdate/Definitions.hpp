#pragma once
#include <NosLib/Boost/WinVersion.hpp>
#include <NosUpdate/Internal/Export.hpp>

#include <boost/algorithm/string.hpp>
#include <string>

namespace NosUpdate::Definitions
{
	inline const std::string UpdateHostname = "update.nosteck.com";
	inline constexpr uint16_t UpdatePort = 8100;
}