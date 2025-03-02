#pragma once

#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <format>
#include <cstdint>

namespace NosUpdate
{
	class NOSUPDATE_API Version
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& Major;
			archive& Minor;
			archive& Patch;
		}

	protected:
		uint16_t Major, Minor, Patch;

	public:
		Version() = default;
		Version(const uint16_t& major, const uint16_t& minor, const uint16_t& patch) :
			Major(major),
			Minor(minor),
			Patch(patch)
		{ }
		virtual ~Version() = default;

		uint16_t GetMajor() { return Major; }
		uint16_t GetMinor() { return Minor; }
		uint16_t GetPatch() { return Patch; }

		std::string GetVersion()
		{
			return std::format("{}.{}.{}", Major, Minor, Patch);
		}
	};
}