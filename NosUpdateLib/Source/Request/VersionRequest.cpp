#include <NosUpdate/Request/VersionRequest.hpp>
namespace NosUpdate
{
	VersionRequest::VersionTypes VersionRequest::GetVersionType() const
	{
		return VersionType;
	}

	std::string VersionRequest::GetVersionTypeName() const
	{
		switch (VersionType)
		{
			using enum VersionTypes;
		case Newest:
			return "Newest";

		default:
			return "UNDEFINED";
		}
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::VersionRequest)