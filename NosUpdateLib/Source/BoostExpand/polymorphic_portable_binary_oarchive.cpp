#if (defined _MSC_VER) && (_MSC_VER == 1200)
#  pragma warning (disable : 4786) // too long name, harmless warning
#endif

#define BOOST_ARCHIVE_SOURCE
#include <boost/serialization/config.hpp>
#include <NosUpdate/BoostExpand/polymorphic_portable_binary_oarchive.hpp>

// explicitly instantiate for this type of text stream
#include <boost/archive/impl/archive_serializer_map.ipp>

namespace boost::archive::detail
{
	template class archive_serializer_map<NosUpdate::BoostExpand::polymorphic_portable_binary_oarchive>;
}
