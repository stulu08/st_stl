#pragma once
#include <stdio.h>
#include <assert.h>

#ifndef _DEBUG
#ifdef ST_DEBUG
#define _DEBUG
#endif
#endif // !_DEBUG

#ifndef ST_INLINE
#define ST_INLINE inline
#endif // !ST_INLINE

#ifndef ST_F_INLINE
#define ST_F_INLINE __forceinline
#endif // !ST_INLINE


#ifndef ST_INLINE_VAR
#define ST_INLINE_VAR inline
#endif // !ST_INLINE_VAR

#ifndef ST_NODISCARD
#define ST_NODISCARD [[nodiscard]]
#endif // !ST_NODISCARD
#ifndef ST_NODISCARD_MSG
#define ST_NODISCARD_MSG(Msg) [[nodiscard(Msg)]]
#endif // !ST_NODISCARD_MSG

#ifndef ST_NODISCARD_MEMLEAK
#define ST_NODISCARD_MEMLEAK ST_NODISCARD_MSG("This function allocates memory, discrading the return value will result in a memory leak.")
#endif // !ST_NODISCARD_MEMLEAK

#ifndef ST_VIRTUAL
#define ST_VIRTUAL virtual
#endif // !ST_VIRTUAL

#ifndef ST_OVERRIDE
#define ST_OVERRIDE override
#endif // !ST_OVERRIDE

#ifndef ST_DEBUG_LEVEL
#ifdef ST_DEBUG
#define ST_DEBUG_LEVEL 2
#elif defined(ST_RELEASE)
#define ST_DEBUG_LEVEL 1
#elif defined(ST_DIST)
#define ST_DEBUG_LEVEL 0
#endif
#endif // !ST_DEBUG_LEVEL

#ifndef ST_STL_BEGIN
#define ST_STL_BEGIN namespace Stulu {
#endif // !ST_STL_BEGIN

#ifndef ST_STL_END
#define ST_STL_END }
#endif // !ST_STL_END

#ifndef ST_STL
#define ST_STL ::Stulu::
#endif // !ST_STL

#ifndef ST_WIDE_
#define ST_WIDE_(s) L ## s
#endif // !ST_WIDE_

#ifndef ST_WIDE
#define ST_WIDE(s) ST_WIDE_(s)
#endif // !ST_WIDE

#ifndef ST_REPORT_ERROR
#ifdef _DEBUG
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

#ifndef ST_ALIGNOF
#define ST_ALIGNOF(x) alignof(x)
#endif // !ST_ALIGNOF

#ifndef ST_NOEXCEPT
#define ST_NOEXCEPT noexcept
#endif // !ST_NOEXCEPT

#ifndef ST_DEFAULT_SIZE_TYPE
#define ST_DEFAULT_SIZE_TYPE size_t
#endif // !ST_DEFAULT_SIZE_TYPE

#ifndef ST_C_CALL
#define ST_C_CALL ::
#endif // !ST_DEFAULT_SIZE_TYPE

ST_STL_BEGIN

	template <class, class>
	ST_INLINE_VAR constexpr bool is_same = false;
	template <class _Ty>
	ST_INLINE_VAR constexpr bool is_same<_Ty, _Ty> = true;

	struct EmptyType { 
	public:
		EmptyType() = default;
	};

ST_STL_END
