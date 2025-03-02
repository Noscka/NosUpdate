#include <NosUpdate/Request/UpdateRequest.hpp>
namespace NosUpdate
{
	Version UpdateRequest::GetUpdateVersion() const
	{
		return UpdateVersion;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateRequest)