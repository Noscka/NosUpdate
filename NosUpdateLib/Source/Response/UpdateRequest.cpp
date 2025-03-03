#include <NosUpdate/Response/UpdateResponse.hpp>

namespace NosUpdate
{
	Version UpdateResponse::GetUpdateVersion() const
	{
		return UpdateVersion;
	}

	uint64_t UpdateResponse::GetFileSize() const
	{
		return FileSize;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateResponse)