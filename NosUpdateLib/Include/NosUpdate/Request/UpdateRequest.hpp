#pragma once
#include "Request.hpp"
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

#include <NosUpdate/Version.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API UpdateRequest : public Request
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& boost::serialization::base_object<Request>(*this);
			archive& UpdateVersion;
		}

	protected:
		Version UpdateVersion;
		//uint64_t AmountByteLeft;	/* Currently unused: how much the client already downloaded (where to continue from) */

	public:
		UpdateRequest() : Request(RequestTypes::Update) {}
		UpdateRequest(const Version& updateVersion) : Request(RequestTypes::Update), UpdateVersion(updateVersion) {}
		~UpdateRequest() override = default;

		std::string GetRequestName() const override
		{
			return "UpdateRequest";
		}

		Version GetUpdateVersion() const;
	};
}

BOOST_CLASS_EXPORT_KEY(NosUpdate::UpdateRequest)