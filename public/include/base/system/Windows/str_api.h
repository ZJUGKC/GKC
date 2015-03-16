/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//Windows

//------------------------------------------------------------------------------
//tools

// __SFILE__
#define __SFILE__   __WFILE__

// calc_string_length

inline uintptr calc_string_length(const CharH* s) throw()
{
	return ::wcslen(s);
}

inline uintptr calc_string_length(const CharL* s) throw()
{
	const CharL* p = s;
	while( *p != 0 ) p ++;
	return p - s;
}

// calc_char_length
//   in current locale
inline uintptr calc_char_length(const CharA* s) throw()
{
	return ::_mbclen((const unsigned char*)s);
}

// calc_string_char_length
//   in current locale
inline uintptr calc_string_char_length(const CharA* s) throw()
{
	return ::_mbslen((const unsigned char*)s);
}

// compare_string

inline int compare_string(const CharH* s1, const CharH* s2) throw()
{
	return ::wcscmp(s1, s2);
}
inline int compare_string(const CharL* s1, const CharL* s2) throw()
{
	const CharL* p1 = s1;
	const CharL* p2 = s2;
	int n = 0;
	do {
		int n = (int)(*p1) - (int)(*p2);
		if( n != 0 )
			break;
		if( (*p1 == 0) || (*p2 == 0) )
			break;
		++ p1;  ++ p2;
	} while( 1 );
	return n;
}

// compare_string_case_insensitive

inline int compare_string_case_insensitive(const CharA* s1, const CharA* s2) throw()
{
	return ::_stricmp(s1, s2);
}

inline int compare_string_case_insensitive(const CharH* s1, const CharH* s2) throw()
{
	return ::_wcsicmp(s1, s2);
}

inline int compare_string_case_insensitive(const CharL* s1, const CharL* s2) throw()
{
	const CharL* p1 = s1;
	const CharL* p2 = s2;
	int n = 0;
	do {
		n = (int)char_lower(*p1) - (int)char_lower(*p2);
		if( n != 0 )
			break;
		if( (*p1 == 0) || (*p2 == 0) )
			break;
		++ p1;  ++ p2;
	} while( 1 );
	return n;
}

// value_to_string
//   uSize : buffer size in typed characters.
//   return value : the number of typed characters, -1 means fail.
inline int value_to_string(CharA* szBuffer, uintptr uSize, const CharA* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::_vsnprintf_s(szBuffer, uSize, _TRUNCATE, szFormat, ap);
	va_end(ap);
	return ret;
}
inline int value_to_string(CharH* szBuffer, uintptr uSize, const CharH* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::_vsnwprintf_s(szBuffer, uSize, _TRUNCATE, szFormat, ap);
	va_end(ap);
	return ret;
}

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, CharA* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	//no check
	::FormatMessageA(0, NULL, cr.GetResult(), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					szBuffer, (DWORD)uSize, NULL);
}
inline void result_to_string(const call_result& cr, CharH* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	//no check
	::FormatMessageW(0, NULL, cr.GetResult(), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					szBuffer, (DWORD)uSize, NULL);
}

////////////////////////////////////////////////////////////////////////////////
