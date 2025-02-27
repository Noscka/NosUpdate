#include <NosUpdate/Request/UpdateRequest.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(NosUpdate::UpdateRequest)
namespace NosUpdate
{
	uint64_t UpdateRequest::GetDataLeft() const
	{
		return AmountByteLeft;
	}
}

