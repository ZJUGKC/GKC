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

inline uintptr calc_string_length(const char_h* s) throw()
{
	const char_h* p = s;
	while( *p != 0 ) p ++;
	return p - s;
}

inline uintptr calc_string_length(const char_l* s) throw()
{
	return ::wcslen(s);
}

// calc_char_length
//   in current locale
//   it is not thread-safe
inline uintptr calc_char_length(const char_a* s) throw()
{
	return ::mbrlen(s, MB_LEN_MAX, NULL);
}

// calc_string_char_length
//   in current locale
//   it is not thread-safe
inline uintptr calc_string_char_length(const char_a* s) throw()
{
	const char_a* p = s;
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
inline char_a* copy_string(const char_a* szSrc, uintptr uCopyLen, char_a* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::strncpy(szDest, szSrc, n);
	szDest[n] = '\0';
	return szDest;
}
inline char_l* copy_string(const char_l* szSrc, uintptr uCopyLen, char_l* szDest, uintptr uDestLen) throw()
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

inline int compare_string(const char_h* s1, const char_h* s2) throw()
{
	const char_h* p1 = s1;
	const char_h* p2 = s2;
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
inline int compare_string(const char_l* s1, const char_l* s2) throw()
{
	return ::wcscmp(s1, s2);
}

// compare_string_case_insensitive

inline int compare_string_case_insensitive(const char_a* s1, const char_a* s2) throw()
{
	return ::strcasecmp(s1, s2);
}

inline int compare_string_case_insensitive(const char_h* s1, const char_h* s2) throw()
{
	const char_h* p1 = s1;
	const char_h* p2 = s2;
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

inline int compare_string_case_insensitive(const char_l* s1, const char_l* s2) throw()
{
	return ::wcscasecmp(s1, s2);
}

// find_string_char

inline char_l* find_string_char(const char_l* s, char_l c) throw()
{
	return (char_l*)::wcschr(s, c);
}

// find_string_last_char

inline char_l* find_string_last_char(const char_l* s, char_l c) throw()
{
	return (char_l*)::wcsrchr(s, c);
}

// find_string_charset

inline char_l* find_string_charset(const char_l* s, const char_l* z) throw()
{
	return (char_l*)::wcspbrk(s, z);
}

// find_string_string

inline char_l* find_string_string(const char_l* s, const char_l* z) throw()
{
	return (char_l*)::wcsstr(s, z);
}

// char_is_*
inline bool char_is_alpha(const char_l& ch) throw()
{
	return ::iswalpha((wint_t)ch) != 0;
}
inline bool char_is_lower(const char_l& ch) throw()
{
	return ::iswlower((wint_t)ch) != 0;
}
inline bool char_is_upper(const char_l& ch) throw()
{
	return ::iswupper((wint_t)ch) != 0;
}
inline bool char_is_digit(const char_l& ch) throw()
{
	return ::iswdigit((wint_t)ch) != 0;
}
inline bool char_is_xdigit(const char_l& ch) throw()
{
	return ::iswxdigit((wint_t)ch) != 0;
}
inline bool char_is_blank(const char_l& ch) throw()
{
	return ::iswblank((wint_t)ch) != 0;
}
inline bool char_is_space(const char_l& ch) throw()
{
	return ::iswspace((wint_t)ch) != 0;
}
inline bool char_is_print(const char_l& ch) throw()
{
	return ::iswprint((wint_t)ch) != 0;
}

//------------------------------------------------------------------------------

// convert format string from unification to linux specification

inline char_a* _os_convert_unified_format_string(const char_a* szFormat) throw()
{
	uintptr uDestLen = calc_string_length(szFormat);
	char_a* pd = (char_a*)crt_alloc(sizeof(char_a) * (uDestLen + 1));
	if( pd == NULL )
		return NULL;
	char_a* pdo = pd;
	const char_a* ps = szFormat;
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
			mem_copy(ps, len * sizeof(char_a), pd);
			ps += len;
			pd += len;
			}
			break;
		}
	} //end while
	*pd = 0;
	return pdo;
}
inline char_l* _os_convert_unified_format_string(const char_l* szFormat) throw()
{
	uintptr uDestLen = calc_string_length(szFormat);
	char_l* pd = (char_l*)crt_alloc(sizeof(char_l) * (uDestLen + 1));
	if( pd == NULL )
		return NULL;
	char_l* pdo = pd;
	const char_l* ps = szFormat;
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
inline void _os_free_unified_format_convert_string(void* p) throw()
{
	crt_free(p);
}

// value_to_string
//   uSize : buffer size in typed characters.
//   return value : the number of typed characters, <0 means fail.
inline int value_to_string(char_a* szBuffer, uintptr uSize, const char_a* szFormat, ...) throw()
{
	assert( uSize > 0 );
	char_a* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsnprintf(szBuffer, uSize, szV, ap);
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	//check
	if( (ret >= 0) && (((uintptr)ret) >= uSize) ) {
		ret = (int)(uSize - 1);
		szBuffer[ret] = 0;
	}
	return ret;
}
inline int value_to_string(char_l* szBuffer, uintptr uSize, const char_l* szFormat, ...) throw()
{
	assert( uSize > 0 );
	char_l* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswprintf(szBuffer, uSize, szV, ap);  // -1 as error
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}

// string_to_value
//   return value: the number of input items successfully matched and assigned. <0 means fail.
inline int string_to_value(const char_a* szString, const char_a* szFormat, ...) throw()
{
	char_a* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsscanf(szString, szV, ap);  //EOF as error
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}
inline int string_to_value(const char_l* szString, const char_l* szFormat, ...) throw()
{
	char_l* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswscanf(szString, szV, ap);  //EOF as error
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}

//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline char_l* string_to_value(const char_l* szString, float& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = ::wcstof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline char_l* string_to_value(const char_l* szString, double& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = ::wcstod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline char_l* string_to_value(const char_l* szString, int iBase, int& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = (int)::wcstol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline char_l* string_to_value(const char_l* szString, int iBase, uint& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = (uint)::wcstoul(szString, &pN, iBase);  // v may be ULONG_MAX (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline char_a* string_to_value(const char_a* szString, int iBase, int64& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::strtoll(szString, &pN, iBase);  // v may be LLONG_MAX or LLONG_MIN
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline char_l* string_to_value(const char_l* szString, int iBase, int64& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = ::wcstoll(szString, &pN, iBase);  // v may be LLONG_MAX or LLONG_MIN
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

inline char_a* string_to_value(const char_a* szString, int iBase, uint64& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::strtoull(szString, &pN, iBase);  // v may be ULLONG_MAX
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline char_l* string_to_value(const char_l* szString, int iBase, uint64& v, bool& bOK) throw()
{
	char_l* pN = NULL;
	errno = 0;
	v = ::wcstoull(szString, &pN, iBase);  // v may be ULLONG_MAX
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

//------------------------------------------------------------------------------
// guid
//   XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX

inline bool string_to_guid(const char_a* szString, guid& id) throw()
{
	return ::uuid_parse(szString, id) == 0;  // -1
}

// The length of szBuffer must be not less than 37.
inline bool guid_to_string(const guid& id, char_a* szBuffer) throw()
{
	::uuid_unparse(id, szBuffer);
	return true;
}

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, char_a* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	char* szRet = ::strerror_r(cr.GetResult() & (~0x80000000), szBuffer, uSize);
	if( szRet != szBuffer ) {
		//copy
		uintptr uLen = ::strlen(szRet);
		if( uLen > uSize - 1 )
			uLen = uSize - 1;
		mem_copy(szRet, uLen * sizeof(char_a), szBuffer);
		szBuffer[uLen] = 0;
	}
}

//------------------------------------------------------------------------------
// path to platform
inline void cvt_path_string_to_platform(char_a* szBuffer) throw()
{
}
inline void cvt_path_string_to_platform(char_l* szBuffer) throw()
{
}

//------------------------------------------------------------------------------
// path

//including the terminating null character
#define MAX_FULL_PATH  (PATH_MAX)

inline void get_path_separator(char_a& ch) throw()
{
	ch = '/';
}
inline void get_path_separator(char_l& ch) throw()
{
	ch = L'/';
}

inline bool check_path_separator(const char_a& ch) throw()
{
	return ch == '/';
}
inline bool check_path_separator(const char_l& ch) throw()
{
	return ch == L'/';
}
inline bool check_drive_separator(const char_a& ch) throw()
{
	return false;
}
inline bool check_drive_separator(const char_l& ch) throw()
{
	return false;
}

inline void get_path_extension_start(char_l& ch) throw()
{
	ch = L'.';
}
inline bool check_path_extension_start(const char_l& ch) throw()
{
	return ch == L'.';
}

template <typename t_char>
inline int get_path_type(const t_char* szPath) throw()
{
	int iRes = path_types::None;
	int iState = 1;
	const t_char* ps = szPath;
	while( *ps != 0 ) {
		switch(iState) {
		case 1:
			if( check_path_separator(*ps) ) {
				iState = 2;
			}
			else if( check_path_extension_start(*ps) ) {
				iState = 3;
			}
			else {
				iState = 4;
			}
			break;
		case 2:
			if( check_path_separator(*ps) ) {
				iState = 5;
			}
			break;
		case 3:
			if( check_path_separator(*ps) ) {
				// "./..."
				iRes = path_types::Relative;
				return iRes;
			}
			else if( check_path_extension_start(*ps) ) {
				iState = 6;
			}
			else {
				iState = 4;
			}
			break;
		case 4:
			if( check_path_separator(*ps) ) {
				// "X.../..."
				iRes = path_types::Relative;
				return iRes;
			}
			break;
		case 5:
			if( check_path_extension_start(*ps) ) {
				iState = 7;
			}
			else if( check_path_separator(*ps) ) {
			}
			else {
				iState = 2;
			}
			break;
		case 6:
			if( check_path_separator(*ps) ) {
				// "../..."
				iRes = path_types::Relative;
				return iRes;
			}
			else {
				iState = 4;
			}
			break;
		case 7:
			if( check_path_separator(*ps) ) {
				// "..././..."
				iRes = path_types::Relative;
				return iRes;
			}
			else if( check_path_extension_start(*ps) ) {
				iState = 8;
			}
			else {
				iState = 2;
			}
			break;
		case 8:
			if( check_path_separator(*ps) ) {
				// ".../../..."
				iRes = path_types::Relative;
				return iRes;
			}
			else {
				iState = 2;
			}
			break;
		default:
			assert(false);
			break;
		} //end switch
		ps ++;
	} //end while
	//middle states
	if( iState != 1 && iState != 4 ) {
		if( iState == 3 || iState == 6 || iState == 7 || iState == 8 )
			iRes = path_types::Relative;
		else
			iRes = path_types::Absolute;
	}
	return iRes;
}

inline void get_current_path_prefix(const char_a*& sz, uintptr& len) throw()
{
	sz = "./";
	len = 2;
}
inline void get_current_path_prefix(const char_l*& sz, uintptr& len) throw()
{
	sz = L"./";
	len = 2;
}

inline void get_absolute_path_prefix(const char_a*& sz, uintptr& len) throw()
{
	sz = "";
	len = 0;
}
inline void get_absolute_path_prefix(const char_l*& sz, uintptr& len) throw()
{
	sz = L"";
	len = 0;
}

inline void get_sa_extention(const char_a*& sz, uintptr& len) throw()
{
	sz = ".so";
	len = 3;
}
inline void get_sa_extention(const char_l*& sz, uintptr& len) throw()
{
	sz = L".so";
	len = 3;
}

////////////////////////////////////////////////////////////////////////////////
