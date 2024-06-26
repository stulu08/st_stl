#pragma once
#include "stulu/stl/cstddef.h"

#include <string.h>
ST_STL_BEGIN

using ST_C_STD memchr;
using ST_C_STD memcmp;
using ST_C_STD memcpy;
using ST_C_STD memmove;
using ST_C_STD memset;
using ST_C_STD strcat;
using ST_C_STD strchr;
using ST_C_STD strcmp;
using ST_C_STD strcoll;
using ST_C_STD strcpy;
using ST_C_STD strcspn;
using ST_C_STD strerror;
using ST_C_STD strlen;
using ST_C_STD strncat;
using ST_C_STD strncmp;
using ST_C_STD strncpy;
using ST_C_STD strpbrk;
using ST_C_STD strrchr;
using ST_C_STD strspn;
using ST_C_STD strstr;
using ST_C_STD strtok;
using ST_C_STD strxfrm;


// When destination is not null but an error occured, destination will be filled with \0 using memset
// possible returns:
// 0 - executed memcpy
// 1 - destination is null
// 2 - source is null
// 3 - destinationSize < sourceSize
ST_INLINE int memcpy_s(void* destination, size_t destinationSize, const void* source, size_t sourceSize) {
    if (sourceSize == 0)
        return 0;
    if (destination == nullptr)
        return 1;

    if (source == nullptr || destinationSize < sourceSize) {
        memset(destination, '\0', (size_t)destinationSize);
        if (source == nullptr)
            return 2;
        if (destinationSize < sourceSize)
            return 3;
    }
    // can copy
    memcpy(destination, source, (size_t)sourceSize);
    return 0;
}

// possible returns:
// 0 - executed memmove
// 1 - destination is null
// 2 - source is null
// 3 - destinationSize < sourceSize
ST_INLINE int memmove_s(void* destination, size_t destinationSize, const void* source, size_t sourceSize) {
    if (sourceSize == 0)
        return 0;
    if (destination == nullptr)
        return 1;
    if (source == nullptr)
        return 2;
    if (destinationSize < sourceSize)
        return 3;
    // can move
    memmove(destination, source, (size_t)sourceSize);
    return 0;
}

ST_STL_END