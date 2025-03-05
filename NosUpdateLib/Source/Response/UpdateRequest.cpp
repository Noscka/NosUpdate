#include <NosUpdate/Response/UpdateResponse.hpp>

namespace NosUpdate
{
	Version UpdateResponse::GetUpdateVersion() const
	{
		return UpdateVersion;
	}

	FileInfo UpdateResponse::GetFileInfo() const
	{
		return FileInfoObj;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateResponse)