#pragma once
#include <stdio.h>

#include "cstddef.h"

#include <assert.h>

#ifndef ST_REPORT_ERROR
#if ST_DEBUG_LEVEL
#define ST_REPORT_ERROR(msg)     							\
{							   								\
	::_wassert(ST_WIDE(msg), ST_WIDE(__FILE__), __LINE__);	\
}				
#else
#define ST_REPORT_ERROR(msg) 
#endif
#endif // !ST_REPORT_ERROR

#ifndef ST_VERIFY
#define ST_VERIFY(expr, msg)                                 \
if (!(expr)) {                                               \
	ST_REPORT_ERROR(msg);                                    \
}                                   
#endif // !ST_VERIFY

#ifndef ST_ASSERT
#define ST_ASSERT(expr, msg) ST_VERIFY(expr, msg)
#endif // !ST_ASSERT