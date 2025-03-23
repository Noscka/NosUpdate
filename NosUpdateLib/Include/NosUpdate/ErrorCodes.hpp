#pragma once

#include <NosLib/ErrorCodes.hpp>

namespace NosUpdate
{
	enum class NetErrors : uint8_t
	{
		Successful,
		Unresolve,
		Unconnectable,
		Handshake,
	};

	class NetErrorCategory : public std::error_category
	{
	public:
	public:
		static const NetErrorCategory& instance()
		{
			static NetErrorCategory inst; // Guaranteed to be instantiated once.
			return inst;
		}

		const char* name() const noexcept override
		{
			return "NetErrorCategory";
		}

		std::string message(int ev) const override
		{
			switch (static_cast<NetErrors>(ev))
			{
			case NetErrors::Successful:
				return "Successful";

			case NetErrors::Unresolve:
				return "Unable to Resolve";

			case NetErrors::Unconnectable:
				return "Unable to Connect";

			case NetErrors::Handshake:
				return "Problem with Handshake";
			}

			return "Unknown error";
		}
	};

	inline std::error_code make_error_code(NosUpdate::NetErrors e) noexcept
	{
		return { static_cast<int>(e), NosUpdate::NetErrorCategory::instance() };
	}
}

namespace std
{
	template <>
	struct is_error_code_enum<NosUpdate::NetErrors> : true_type {};
}
