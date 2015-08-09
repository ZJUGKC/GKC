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
#define __WFILE__   _WIDEN(__FILE__)

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

// copy_string
//  no buffer overruns
inline CharA* copy_string(const CharA* szSrc, uintptr uCopyLen, CharA* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::strncpy_s(szDest, uDestLen, szSrc, n);
	szDest[n] = '\0';
	return szDest;
}
inline CharH* copy_string(const CharH* szSrc, uintptr uCopyLen, CharH* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::wcsncpy_s(szDest, uDestLen, szSrc, n);
	szDest[n] = L'\0';
	return szDest;
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

// find_string_char

inline CharH* find_string_char(const CharH* s, CharH c) throw()
{
	return (CharH*)::wcschr(s, c);
}

// find_string_last_char

inline CharH* find_string_last_char(const CharH* s, CharH c) throw()
{
	return (CharH*)::wcsrchr(s, c);
}

// find_string_string

inline CharH* find_string_string(const CharH* s, const CharH* z) throw()
{
	return (CharH*)::wcsstr(s, z);
}

//------------------------------------------------------------------------------

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

// string_to_value
//   return value: the number of input items successfully matched and assigned. <0 means fail.
inline int string_to_value(const CharA* szString, const CharA* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsscanf(szString, szFormat, ap);
	va_end(ap);
	return ret;
}
inline int string_to_value(const CharH* szString, const CharH* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswscanf(szString, szFormat, ap);
	va_end(ap);
	return ret;
}

//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline CharH* string_to_value(const CharH* szString, float& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = ::wcstof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline CharH* string_to_value(const CharH* szString, double& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = ::wcstod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline CharH* string_to_value(const CharH* szString, int iBase, int& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = (int)::wcstol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline CharH* string_to_value(const CharH* szString, int iBase, uint& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = (uint)::wcstoul(szString, &pN, iBase);  // v may be ULONG_MAX
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline CharA* string_to_value(const CharA* szString, int iBase, int64& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::_strtoi64(szString, &pN, iBase);  // v may be _I64_MAX or _I64_MIN
	bOK = (errno == 0);  // EINVAL
	return pN;
}
inline CharH* string_to_value(const CharH* szString, int iBase, int64& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = ::_wcstoi64(szString, &pN, iBase);  // v may be _I64_MAX or _I64_MIN
	bOK = (errno == 0);  // EINVAL
	return pN;
}

inline CharA* string_to_value(const CharA* szString, int iBase, uint64& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::_strtoui64(szString, &pN, iBase);  // v may be _UI64_MAX
	bOK = (errno == 0);  // EINVAL
	return pN;
}
inline CharH* string_to_value(const CharH* szString, int iBase, uint64& v, bool& bOK) throw()
{
	CharH* pN = NULL;
	errno = 0;
	v = ::_wcstoui64(szString, &pN, iBase);  // v may be _UI64_MAX
	bOK = (errno == 0);  // EINVAL
	return pN;
}

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, CharA* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	DWORD dwRet = 0;
	dwRet = ::FormatMessageA(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, cr.GetResult(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szBuffer, (DWORD)uSize, NULL);
	if( dwRet == 0 ) {
		//no check
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, cr.GetResult(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szBuffer, (DWORD)uSize, NULL);
	}
}
inline void result_to_string(const call_result& cr, CharH* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	DWORD dwRet = 0;
	dwRet = ::FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, cr.GetResult(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szBuffer, (DWORD)uSize, NULL);
	if( dwRet == 0 ) {
		//no check
		::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, cr.GetResult(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szBuffer, (DWORD)uSize, NULL);
	}
}

//------------------------------------------------------------------------------
// path to platform
inline void cvt_path_string_to_platform(CharA* szBuffer) throw()
{
	CharA* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == '/' )
			*ps = '\\';
		++ ps;
	}
}
inline void cvt_path_string_to_platform(CharH* szBuffer) throw()
{
	CharH* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == L'/' )
			*ps = L'\\';
		++ ps;
	}
}

////////////////////////////////////////////////////////////////////////////////
