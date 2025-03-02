#include <NosUpdate/Response/VersionResponse.hpp>
namespace NosUpdate
{
	std::string VersionResponse::GetRequestedVersion() const
	{
		return RequestedVersion;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::VersionResponse)