#ifndef BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/detail/polymorphic_oarchive_route.hpp>
#include "portable_binary_oarchive.hpp"

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace NosUpdate::BoostExpand
{
	class BOOST_SYMBOL_VISIBLE polymorphic_portable_binary_oarchive :
		public boost::archive::detail::polymorphic_oarchive_route<portable_binary_oarchive>
	{
	public:
		polymorphic_portable_binary_oarchive(std::ostream& os, unsigned int flags = 0) :
			boost::archive::detail::polymorphic_oarchive_route<portable_binary_oarchive>(os, flags)
		{
		}
		~polymorphic_portable_binary_oarchive() BOOST_OVERRIDE {}
	};
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(
	NosUpdate::BoostExpand::polymorphic_portable_binary_oarchive
)

#endif // BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP
