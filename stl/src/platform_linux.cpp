#include "stulu/stl/platform.h"

#ifdef ST_LINUX
#include <unistd.h>
ST_STL_BEGIN
int usleep(const size_t microsends) {
	// uses micro seconds
	return ::usleep(static_cast<useconds_t>(microsends));
}
ST_STL_END
#endif