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

//Linux

//------------------------------------------------------------------------------
//tools

// __SFILE__
#define __SFILE__   __FILE__

// calc_string_length

inline uintptr calc_string_length(const CharH* s) throw()
{
	const CharH* p = s;
	while( *p != 0 ) p ++;
	return p - s;
}

inline uintptr calc_string_length(const CharL* s) throw()
{
	return ::wcslen(s);
}

// calc_char_length
//   in current locale
//   it is not thread-safe
inline uintptr calc_char_length(const CharA* s) throw()
{
	return ::mbrlen(s, MB_LEN_MAX, NULL);
}

// calc_string_char_length
//   in current locale
//   it is not thread-safe
inline uintptr calc_string_char_length(const CharA* s) throw()
{
	const CharA* p = s;
	uintptr n = 0;
	uintptr len;
	while( (intptr)(len = calc_char_length(p)) > 0 ) {
		++ n;
		p += len;
	}
	return n;
}

// copy_string
//  no buffer overruns
inline CharA* copy_string(const CharA* szSrc, uintptr uCopyLen, CharA* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::strncpy(szDest, szSrc, n);
	szDest[n] = '\0';
	return szDest;
}
inline CharL* copy_string(const CharL* szSrc, uintptr uCopyLen, CharL* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::wcsncpy(szDest, szSrc, n);
	szDest[n] = L'\0';
	return szDest;
}

// compare_string

inline int compare_string(const CharH* s1, const CharH* s2) throw()
{
	const CharH* p1 = s1;
	const CharH* p2 = s2;
	int n = 0;
	do {
		n = (int)(*p1) - (int)(*p2);
		if( n != 0 )
			break;
		if( (*p1 == 0) || (*p2 == 0) )
			break;
		++ p1;  ++ p2;
	} while( 1 );
	return n;
}
inline int compare_string(const CharL* s1, const CharL* s2) throw()
{
	return ::wcscmp(s1, s2);
}

// compare_string_case_insensitive

inline int compare_string_case_insensitive(const CharA* s1, const CharA* s2) throw()
{
	return ::strcasecmp(s1, s2);
}

inline int compare_string_case_insensitive(const CharH* s1, const CharH* s2) throw()
{
	const CharH* p1 = s1;
	const CharH* p2 = s2;
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

inline int compare_string_case_insensitive(const CharL* s1, const CharL* s2) throw()
{
	return ::wcscasecmp(s1, s2);
}

// find_string_char

inline CharL* find_string_char(const CharL* s, CharL c) throw()
{
	return (CharL*)::wcschr(s, c);
}

// find_string_last_char

inline CharL* find_string_last_char(const CharL* s, CharL c) throw()
{
	return (CharL*)::wcsrchr(s, c);
}

// find_string_string

inline CharL* find_string_string(const CharL* s, const CharL* z) throw()
{
	return (CharL*)::wcsstr(s, z);
}

// char_is_*
inline bool char_is_alpha(const CharL& ch) throw()
{
	return ::iswalpha((wint_t)ch) != 0;
}
inline bool char_is_lower(const CharL& ch) throw()
{
	return ::iswlower((wint_t)ch) != 0;
}
inline bool char_is_upper(const CharL& ch) throw()
{
	return ::iswupper((wint_t)ch) != 0;
}
inline bool char_is_digit(const CharL& ch) throw()
{
	return ::iswdigit((wint_t)ch) != 0;
}
inline bool char_is_xdigit(const CharL& ch) throw()
{
	return ::iswxdigit((wint_t)ch) != 0;
}
inline bool char_is_blank(const CharL& ch) throw()
{
	return ::iswblank((wint_t)ch) != 0;
}
inline bool char_is_space(const CharL& ch) throw()
{
	return ::iswspace((wint_t)ch) != 0;
}
inline bool char_is_print(const CharL& ch) throw()
{
	return ::iswprint((wint_t)ch) != 0;
}

//------------------------------------------------------------------------------

// convert format string from unification to linux specification

inline CharA* _convert_unified_format_string(const CharA* szFormat) throw()
{
	uintptr uDestLen = calc_string_length(szFormat);
	CharA* pd = (CharA*)crt_alloc(sizeof(CharA) * (uDestLen + 1));
	if( pd == NULL )
		return NULL;
	CharA* pdo = pd;
	const CharA* ps = szFormat;
	// state : 1 -- after %  2 -- h  3 -- I  4 -- 6
	int state = 0;
	uintptr len;
	while( (intptr)(len = calc_char_length(ps)) > 0 ) {
		switch( *ps ) {
		case '%':
			{
			assert( len == 1 );
			if( state == 0 )
				state = 1;
			else
				state = 0;
			*pd ++ = *ps ++;
			}
			break;
		case 'h':
			{
			assert( len == 1 );
			if( state != 0 )
				state = 2;
			*pd ++ = *ps ++;
			}
			break;
		case 'I':
			{
			assert( len == 1 );
			if( state != 0 )
				state = 3;
			*pd ++ = *ps ++;
			}
			break;
		case 'c':
		case 's':
		case '[':
			{
			assert( len == 1 );
			if( state == 2 )
				pd --;
			state = 0;
			*pd ++ = *ps ++;
			}
			break;
		case '6':
			{
			assert( len == 1 );
			if( state == 3 )
				state = 4;
			else if( state != 0 )
				state = 1;
			*pd ++ = *ps ++;
			}
			break;
		case '4':
			{
			assert( len == 1 );
			if( state == 4 ) {
				pd -= 2;
				*pd ++ = 'l';
				*pd ++ = 'l';
				state = 1;
				ps ++;
			}
			else {
				if( state != 0 )
					state = 1;
				*pd ++ = *ps ++;
			}
			}
			break;
		case 'd':
		case 'o':
		case 'u':
		case 'x':
		case 'X':
			{
			if( state == 3 ) {
				pd --;
				*pd ++ = 'z';
			}
			}
		case 'a':
		case 'A':
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
		case 'p':
			{
			assert( len == 1 );
			state = 0;
			*pd ++ = *ps ++;
			}
			break;
		default:
			{
			if( state != 0 )
				state = 1;
			mem_copy(ps, len * sizeof(CharA), pd);
			ps += len;
			pd += len;
			}
			break;
		}
	} //end while
	*pd = 0;
	return pdo;
}
inline CharL* _convert_unified_format_string(const CharL* szFormat) throw()
{
	uintptr uDestLen = calc_string_length(szFormat);
	CharL* pd = (CharL*)crt_alloc(sizeof(CharL) * (uDestLen + 1));
	if( pd == NULL )
		return NULL;
	CharL* pdo = pd;
	const CharL* ps = szFormat;
	// state : 1 -- after %  2 -- h  3 -- I  4 -- 6
	int state = 0;
	while( *ps != 0 ) {
		switch( *ps ) {
		case L'%':
			{
			if( state == 0 )
				state = 1;
			else
				state = 0;
			*pd ++ = *ps ++;
			}
			break;
		case L'h':
			{
			if( state != 0 )
				state = 2;
			*pd ++ = *ps ++;
			}
			break;
		case L'I':
			{
			if( state != 0 )
				state = 3;
			*pd ++ = *ps ++;
			}
			break;
		case L'c':
		case L's':
		case L'[':
			{
			if( state == 2 )
				pd --;
			state = 0;
			*pd ++ = *ps ++;
			}
			break;
		case L'6':
			{
			if( state == 3 )
				state = 4;
			else if( state != 0 )
				state = 1;
			*pd ++ = *ps ++;
			}
			break;
		case L'4':
			{
			if( state == 4 ) {
				pd -= 2;
				*pd ++ = L'l';
				*pd ++ = L'l';
				state = 1;
				ps ++;
			}
			else {
				if( state != 0 )
					state = 1;
				*pd ++ = *ps ++;
			}
			}
			break;
		case L'd':
		case L'o':
		case L'u':
		case L'x':
		case L'X':
			{
			if( state == 3 ) {
				pd --;
				*pd ++ = L'z';
			}
			}
		case L'a':
		case L'A':
		case L'e':
		case L'E':
		case L'f':
		case L'g':
		case L'G':
		case L'p':
			{
			state = 0;
			*pd ++ = *ps ++;
			}
			break;
		default:
			{
			if( state != 0 )
				state = 1;
			*pd ++ = *ps ++;
			}
			break;
		}
	} //end while
	*pd = 0;
	return pdo;
}
inline void _free_unified_format_convert_string(void* p) throw()
{
	crt_free((uintptr)p);
}

// value_to_string
//   uSize : buffer size in typed characters.
//   return value : the number of typed characters, -1 means fail.
inline int value_to_string(CharA* szBuffer, uintptr uSize, const CharA* szFormat, ...) throw()
{
	assert( uSize > 0 );
	CharA* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsnprintf(szBuffer, uSize, szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	//check
	if( (ret >= 0) && (((uintptr)ret) >= uSize) ) {
		ret = (int)(uSize - 1);
		szBuffer[ret] = 0;
	}
	return ret;
}
inline int value_to_string(CharL* szBuffer, uintptr uSize, const CharL* szFormat, ...) throw()
{
	assert( uSize > 0 );
	CharL* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswprintf(szBuffer, uSize, szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}

// string_to_value
//   return value: the number of input items successfully matched and assigned. <0 means fail.
inline int string_to_value(const CharA* szString, const CharA* szFormat, ...) throw()
{
	CharA* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsscanf(szString, szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}
inline int string_to_value(const CharL* szString, const CharL* szFormat, ...) throw()
{
	CharL* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswscanf(szString, szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}

//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline CharL* string_to_value(const CharL* szString, float& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = ::wcstof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline CharL* string_to_value(const CharL* szString, double& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = ::wcstod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline CharL* string_to_value(const CharL* szString, int iBase, int& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = (int)::wcstol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline CharL* string_to_value(const CharL* szString, int iBase, uint& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = (uint)::wcstoul(szString, &pN, iBase);  // v may be ULONG_MAX (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline CharA* string_to_value(const CharA* szString, int iBase, int64& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::strtoll(szString, &pN, iBase);  // v may be LLONG_MAX or LLONG_MIN
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline CharL* string_to_value(const CharL* szString, int iBase, int64& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = ::wcstoll(szString, &pN, iBase);  // v may be LLONG_MAX or LLONG_MIN
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline CharA* string_to_value(const CharA* szString, int iBase, uint64& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::strtoull(szString, &pN, iBase);  // v may be ULLONG_MAX
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline CharL* string_to_value(const CharL* szString, int iBase, uint64& v, bool& bOK) throw()
{
	CharL* pN = NULL;
	errno = 0;
	v = ::wcstoull(szString, &pN, iBase);  // v may be ULLONG_MAX
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, CharA* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	char* szRet = ::strerror_r(cr.GetResult() & (~0x80000000), szBuffer, uSize);
	if( szRet != szBuffer ) {
		//copy
		uintptr uLen = ::strlen(szRet);
		if( uLen > uSize - 1 )
			uLen = uSize - 1;
		mem_copy(szRet, uLen * sizeof(CharA), szBuffer);
		szBuffer[uLen] = 0;
	}
}

//------------------------------------------------------------------------------
// path to platform
inline void cvt_path_string_to_platform(CharA* szBuffer) throw()
{
}
inline void cvt_path_string_to_platform(CharL* szBuffer) throw()
{
}

//------------------------------------------------------------------------------
// path

inline bool path_is_relative(const CharA* szPath) throw()
{
	return *szPath != '/';
}
inline bool path_is_relative(const CharL* szPath) throw()
{
	return *szPath != L'/';
}

inline void get_path_separator(CharA& ch) throw()
{
	ch = '/';
}
inline void get_path_separator(CharL& ch) throw()
{
	ch = L'/';
}

inline bool check_path_separator(const CharA& ch) throw()
{
	return ch == '/';
}
inline bool check_path_separator(const CharL& ch) throw()
{
	return ch == L'/';
}
inline bool check_drive_separator(const CharA& ch) throw()
{
	return false;
}
inline bool check_drive_separator(const CharL& ch) throw()
{
	return false;
}

inline void get_path_extension_start(CharL& ch) throw()
{
	ch = L'.';
}
inline bool check_path_extension_start(const CharL& ch) throw()
{
	return ch == L'.';
}

inline void get_current_path_prefix(CharA*& sz, uintptr& len) throw()
{
	sz = "./";
	len = 2;
}
inline void get_current_path_prefix(CharL*& sz, uintptr& len) throw()
{
	sz = L"./";
	len = 2;
}

inline void get_absolute_path_prefix(CharA*& sz, uintptr& len) throw()
{
	sz = "";
	len = 0;
}
inline void get_absolute_path_prefix(CharL*& sz, uintptr& len) throw()
{
	sz = L"";
	len = 0;
}

////////////////////////////////////////////////////////////////////////////////
