#pragma once

#ifdef ST_LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

ST_STL_BEGIN

int usleep(const size_t microsends) {
	// uses micro seconds
	return ::usleep(static_cast<useconds_t>(microsends));
}

ST_STL_END
#endif