#pragma once
#include "stulu/core/platform.h"
#include "stulu/stl/cstddef.h"

ST_STL_BEGIN

ST_STL_API int usleep(const size_t microseconds);

ST_INLINE void sleep_micros(size_t microseconds) {
	return (void)usleep(microseconds);
}
ST_INLINE void sleep_millis(const size_t milliseconds) {
	return (void)usleep(milliseconds * 1000);
}
ST_INLINE void sleep_seconds(const size_t seconds) {
	return (void)usleep(seconds * 1000 * 1000);
}
ST_INLINE void sleep(const size_t milliseconds) {
	return (void)usleep(milliseconds * 1000);
}

ST_STL_END

#include "linux/platform.h"
#include "windows/platform.h"