#pragma once

#ifdef ST_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

ST_STL_BEGIN

int usleep(const size_t microseconds) {
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = - static_cast<LONGLONG>((10 * microseconds)); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0))
        return 1;

    const DWORD re = WaitForSingleObject(timer, INFINITE);

    if (!CloseHandle(timer))
        return 2;

    return static_cast<int>(re);
}

ST_STL_END

#endif