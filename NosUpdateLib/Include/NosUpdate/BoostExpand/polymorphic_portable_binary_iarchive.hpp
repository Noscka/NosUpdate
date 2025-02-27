#ifndef BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_IARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/detail/polymorphic_iarchive_route.hpp>
#include "portable_binary_iarchive.hpp"

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace NosUpdate::BoostExpand
{
	class BOOST_SYMBOL_VISIBLE polymorphic_portable_binary_iarchive :
		public boost::archive::detail::polymorphic_iarchive_route<portable_binary_iarchive>
	{
	public:
		polymorphic_portable_binary_iarchive(std::istream& is, unsigned int flags = 0) :
			boost::archive::detail::polymorphic_iarchive_route<portable_binary_iarchive>(is, flags)
		{
		}
		~polymorphic_portable_binary_iarchive() BOOST_OVERRIDE {}
	};
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(
	NosUpdate::BoostExpand::polymorphic_portable_binary_iarchive
)

#endif // BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_IARCHIVE_HPP
