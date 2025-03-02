#include <NosUpdate/Response/VersionResponse.hpp>
namespace NosUpdate
{
	Version VersionResponse::GetRequestedVersion() const
	{
		return RequestedVersion;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::VersionResponse)