#include <NosUpdate/Request/UpdateRequest.hpp>

namespace NosUpdate
{
	Version UpdateRequest::GetUpdateVersion() const
	{
		return UpdateVersion;
	}

	ProgramInfo UpdateRequest::GetProgramInfo() const
	{
		return ProgramInfoObj;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateRequest)