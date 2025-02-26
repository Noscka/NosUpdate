#pragma once
#include <NosUpdate/Request.hpp>
#include <boost/serialization/export.hpp>

namespace NosUpdate
{
	class UpdateRequest : public Request
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Request>(*this);
			archive& AmountByteLeft;
		}

	protected:
		uint64_t AmountByteLeft;	/* Currently unused: how much the client already downloaded (where to continue from) */

	public:
		UpdateRequest() = default;
		UpdateRequest(const uint64_t& ByteLeft) : Request(RequestTypes::Update), AmountByteLeft(ByteLeft) {}

		uint64_t GetDataLeft() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateRequest)