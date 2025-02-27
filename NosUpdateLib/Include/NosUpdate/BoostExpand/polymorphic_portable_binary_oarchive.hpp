#ifndef BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP

// For MS compilers:
#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/archive/detail/polymorphic_oarchive_route.hpp>
#include "portable_binary_oarchive.hpp"

namespace NosUpdate::BoostExpand
{
	typedef boost::archive::detail::polymorphic_oarchive_route<
		portable_binary_oarchive
	> polymorphic_portable_binary_oarchive;
}

#include <boost/version.hpp>
#if BOOST_VERSION > 103401
BOOST_SERIALIZATION_REGISTER_ARCHIVE(
	NosUpdate::BoostExpand::polymorphic_portable_binary_oarchive
)
#endif

#endif // BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP
