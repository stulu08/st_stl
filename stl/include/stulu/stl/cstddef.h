#pragma once
#include "stulu/core/core.h"
#include "stulu/core/platform.h"
#include "stulu/core/config.h"

#include <stddef.h>

ST_STL_BEGIN

typedef decltype(__nullptr) nullptr_t;
using ST_STL nullptr_t;

template <class... Types>
using void_t = void;

using ST_C_STD ptrdiff_t;
using ST_C_STD size_t;

struct mbstate_t;

ST_STL_END