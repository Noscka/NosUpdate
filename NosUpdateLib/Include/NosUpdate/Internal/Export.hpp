#pragma once

#if defined(_WIN32) && defined(NOSUPDATE_SHARED)
	#ifdef NOSUPDATE_EXPORT
		#define NOSUPDATE_API __declspec(dllexport)
	#else
		#define NOSUPDATE_API __declspec(dllimport)
	#endif
#else
	#define NOSUPDATE_API
#endif