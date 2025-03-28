#ifndef PORTABLE_BINARY_ARCHIVE_HPP
#define PORTABLE_BINARY_ARCHIVE_HPP

/* FOUND HERE: https://www.boost.org/doc/libs/1_87_0/libs/serialization/example/ */

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif
#include <NosLib/Boost/WinVersion.hpp>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

#include <climits>
#if CHAR_BIT != 8
#error This code assumes an eight-bit byte.
#endif

#include <boost/archive/basic_archive.hpp>
#include <boost/predef/other/endian.h>
#include <boost/archive/basic_archive.hpp>

enum portable_binary_archive_flags
{
	endian_big = 0x4000,
	endian_little = 0x8000
};

//#if ( endian_big <= boost::archive::flags_last )
//#error archive flags conflict
//#endif

inline void
reverse_bytes(char size, char* address)
{
	char* first = address;
	char* last = first + size - 1;
	for (; first < last; ++first, --last)
	{
		char x = *last;
		*last = *first;
		*first = x;
	}
}

#endif // PORTABLE_BINARY_ARCHIVE_HPP