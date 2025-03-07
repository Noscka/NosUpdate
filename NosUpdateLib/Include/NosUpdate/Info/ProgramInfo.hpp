#pragma once
#include <NosUpdate/WinVersion.hpp>

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <string>

namespace NosUpdate
{
	class NOSUPDATE_API ProgramInfo
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& ProgramName;
		}

	protected:
		std::string ProgramName;

	public:
		ProgramInfo() = default;
		ProgramInfo(const std::string& programName) : ProgramName(programName)
		{}

		std::string GetName() const;
		std::string GetNormalizedName() const;
	};
}