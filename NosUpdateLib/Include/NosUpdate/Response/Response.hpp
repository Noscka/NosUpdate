#pragma once

#include "../WinVersion.hpp"

#include <boost/asio.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>

namespace NosUpdate
{
	class NOSUPDATE_API Response
	{
	public:
		using Base = Response;
		using Ptr = std::unique_ptr<Response>;

		enum class ResponseTypes : uint8_t
		{
			Version,	/* Response to get newest version */
			Update,			/* Response to update to the newest version */
		};

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& ResponseType;
		}

		ResponseTypes ResponseType;	/* What the client wants */
		
	public:
		Response() = default;
		Response(const ResponseTypes& requestType) : ResponseType(requestType) {}
		virtual ~Response() = default;

		ResponseTypes GetResponseType() const;
		std::string GetResponseTypeName() const;
		virtual std::string GetResponseName() const
		{
			return "Response";
		}

		static void Serialize(const Ptr&, boost::asio::streambuf*);
		static Ptr Deserialize(boost::asio::streambuf*);
	};
}

BOOST_SERIALIZATION_ASSUME_ABSTRACT(NosUpdate::Response)