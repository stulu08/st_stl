#pragma once
#include "core.h"

#ifdef _WIN32
	#ifdef _WIN64
		#define ST_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define ST_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define ST_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define ST_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define ST_LINUX
#else
	#error "Unknown platform!"
#endif
#ifdef _WIN32
	#if ST_STL_DYNAMIC_LINK
		#ifdef ST_STL_DYNAMIC_BUILD
			#define ST_STL_API __declspec(dllexport)
		#else
			#define ST_STL_API __declspec(dllimport)
		#endif
	#else
		#define ST_STATIC_LINK
		#define ST_STL_API
	#endif
#else
#define ST_STL_API
#endif