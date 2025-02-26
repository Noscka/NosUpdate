#ifndef BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP

#include <boost/archive/detail/polymorphic_iarchive_route.hpp>
#include "portable_binary_iarchive.hpp"

namespace NosUpdate::BoostExpand
{
	typedef boost::archive::detail::polymorphic_iarchive_route<
		portable_binary_iarchive
	> polymorphic_portable_binary_iarchive;
}
#include <boost/version.hpp>
#if BOOST_VERSION > 103401
BOOST_SERIALIZATION_REGISTER_ARCHIVE(
	NosUpdate::BoostExpand::polymorphic_portable_binary_iarchive
)
#endif

#endif // BOOST_ARCHIVE_POLYMORPHIC_PORTABLE_BINARY_OARCHIVE_HPP