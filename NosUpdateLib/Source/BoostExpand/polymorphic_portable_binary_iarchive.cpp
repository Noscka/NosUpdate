//#include <istream>
//
//#define BOOST_ARCHIVE_SOURCE
//#include "NosUpdate/BoostExpand/polymorphic_portable_binary_iarchive.hpp"
//
//// Include necessary implementation files for the underlying archive:
//#include <boost/archive/impl/basic_binary_iarchive.ipp>
//#include <boost/archive/impl/basic_binary_iprimitive.ipp>
//
//namespace boost
//{
//	namespace archive
//	{
//
//// Explicitly instantiate the basic binary iarchive implementation
//		template class binary_iarchive_impl<
//			NosUpdate::BoostExpand::polymorphic_portable_binary_iarchive,
//			std::istream::char_type,
//			std::istream::traits_type
//		>;
//
//		// Explicitly instantiate the archive_serializer_map for the polymorphic iarchive route.
//		// (The error messages refer to the static functions "insert" and "erase" of the map.)
//		template class detail::archive_serializer_map<
//			detail::polymorphic_iarchive_route<NosUpdate::BoostExpand::portable_binary_iarchive>
//		>;
//
//	} // namespace archive
//} // namespace boost
