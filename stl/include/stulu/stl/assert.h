#pragma once
#include <crtdefs.h>

#undef assert
void _CRTIMP __cdecl _wassert(const wchar_t* _Message, const wchar_t* _File, unsigned _Line);
void _CRTIMP __cdecl _assert(const char* _Message, const char* _File, unsigned _Line);

#ifdef _DEBUG
	#if defined(UNICODE )|| defined(_UNICODE)
		#define assert(expr) if(!(expr)) { _wassert(_CRT_WIDE(#expr), _CRT_WIDE(__FILE__), __LINE__); _debugbreak(); }
	#else
		#define assert(expr) if(!(expr)) { _assert(#expr, __FILE__, __LINE__); }
	#endif
#else
	#define assert(expr) ((void)0)
#endif