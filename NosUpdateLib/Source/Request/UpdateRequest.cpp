#include <NosUpdate/Request/UpdateRequest.hpp>

namespace NosUpdate
{
	uint64_t UpdateRequest::GetDataLeft() const
	{
		return AmountByteLeft;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateRequest)