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

#define __OS_WFILE__   _OS_WIDEN(__FILE__)

// __SFILE__
#define __SFILE__   __OS_WFILE__

// calc_string_length

inline uintptr calc_string_length(const char_h* s) throw()
{
	return ::wcslen(s);
}

inline uintptr calc_string_length(const char_l* s) throw()
{
	const char_l* p = s;
	while( *p != 0 ) p ++;
	return p - s;
}

// calc_char_length
//   in current locale
inline uintptr calc_char_length(const char_a* s) throw()
{
	return ::_mbclen((const unsigned char*)s);
}

// calc_string_char_length
//   in current locale
inline uintptr calc_string_char_length(const char_a* s) throw()
{
	return ::_mbslen((const unsigned char*)s);
}

// copy_string
//  no buffer overruns
inline char_a* copy_string(const char_a* szSrc, uintptr uCopyLen, char_a* szDest, uintptr uDestLen) throw()
{
	assert( uDestLen > 0 );
	uintptr n = uCopyLen;
	if( n > uDestLen - 1 )
		n = uDestLen - 1;
	::strncpy_s(szDest, uDestLen, szSrc, n);
	szDest[n] = '\0';
	return szDest;
}
inline char_h* copy_string(const char_h* szSrc, uintptr uCopyLen, char_h* szDest, uintptr uDestLen) throw()
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

inline int compare_string(const char_h* s1, const char_h* s2) throw()
{
	return ::wcscmp(s1, s2);
}
inline int compare_string(const char_l* s1, const char_l* s2) throw()
{
	const char_l* p1 = s1;
	const char_l* p2 = s2;
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

inline int compare_string_case_insensitive(const char_a* s1, const char_a* s2) throw()
{
	return ::_stricmp(s1, s2);
}

inline int compare_string_case_insensitive(const char_h* s1, const char_h* s2) throw()
{
	return ::_wcsicmp(s1, s2);
}

inline int compare_string_case_insensitive(const char_l* s1, const char_l* s2) throw()
{
	const char_l* p1 = s1;
	const char_l* p2 = s2;
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

inline char_h* find_string_char(const char_h* s, char_h c) throw()
{
	return (char_h*)::wcschr(s, c);
}

// find_string_last_char

inline char_h* find_string_last_char(const char_h* s, char_h c) throw()
{
	return (char_h*)::wcsrchr(s, c);
}

// find_string_charset

inline char_h* find_string_charset(const char_h* s, const char_h* z) throw()
{
	return (char_h*)::wcspbrk(s, z);
}

// find_string_string

inline char_h* find_string_string(const char_h* s, const char_h* z) throw()
{
	return (char_h*)::wcsstr(s, z);
}

// char_is_*
inline bool char_is_alnum(const char_h& ch) throw()
{
	return ::iswalnum((wint_t)ch) != 0;
}
inline bool char_is_alpha(const char_h& ch) throw()
{
	return ::iswalpha((wint_t)ch) != 0;
}
inline bool char_is_cntrl(const char_h& ch) throw()
{
	return ::iswcntrl((wint_t)ch) != 0;
}
inline bool char_is_graph(const char_h& ch) throw()
{
	return ::iswgraph((wint_t)ch) != 0;
}
inline bool char_is_lower(const char_h& ch) throw()
{
	return ::iswlower((wint_t)ch) != 0;
}
inline bool char_is_upper(const char_h& ch) throw()
{
	return ::iswupper((wint_t)ch) != 0;
}
inline bool char_is_digit(const char_h& ch) throw()
{
	return ::iswdigit((wint_t)ch) != 0;
}
inline bool char_is_xdigit(const char_h& ch) throw()
{
	return ::iswxdigit((wint_t)ch) != 0;
}
inline bool char_is_blank(const char_h& ch) throw()
{
	return ::iswblank((wint_t)ch) != 0;
}
inline bool char_is_space(const char_h& ch) throw()
{
	return ::iswspace((wint_t)ch) != 0;
}
inline bool char_is_print(const char_h& ch) throw()
{
	return ::iswprint((wint_t)ch) != 0;
}
inline bool char_is_punct(const char_h& ch) throw()
{
	return ::iswpunct((wint_t)ch) != 0;
}

//------------------------------------------------------------------------------

// value_to_string
//   uSize : buffer size in typed characters.
//   return value : the number of typed characters, <0 means fail.
inline int value_to_string(char_a* szBuffer, uintptr uSize, const char_a* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::_vsnprintf_s(szBuffer, uSize, _TRUNCATE, szFormat, ap);
	va_end(ap);
	return ret;
}
inline int value_to_string(char_h* szBuffer, uintptr uSize, const char_h* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::_vsnwprintf_s(szBuffer, uSize, _TRUNCATE, szFormat, ap);
	va_end(ap);
	return ret;
}

// string_to_value
//   return value: the number of input items successfully matched and assigned. <0 means fail.
inline int string_to_value(const char_a* szString, const char_a* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vsscanf(szString, szFormat, ap);  //EOF or -1 as error
	va_end(ap);
	return ret;
}
inline int string_to_value(const char_h* szString, const char_h* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vswscanf(szString, szFormat, ap);  //EOF or -1 as error
	va_end(ap);
	return ret;
}

//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline char_h* string_to_value(const char_h* szString, float& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = ::wcstof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline char_h* string_to_value(const char_h* szString, double& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = ::wcstod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline char_h* string_to_value(const char_h* szString, int iBase, int& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = (int)::wcstol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline char_h* string_to_value(const char_h* szString, int iBase, uint& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = (uint)::wcstoul(szString, &pN, iBase);  // v may be ULONG_MAX
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline char_a* string_to_value(const char_a* szString, int iBase, int64& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::_strtoi64(szString, &pN, iBase);  // v may be _I64_MAX or _I64_MIN
	bOK = (errno == 0);  // EINVAL
	return pN;
}
inline char_h* string_to_value(const char_h* szString, int iBase, int64& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = ::_wcstoi64(szString, &pN, iBase);  // v may be _I64_MAX or _I64_MIN
	bOK = (errno == 0);  // EINVAL
	return pN;
}

inline char_a* string_to_value(const char_a* szString, int iBase, uint64& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::_strtoui64(szString, &pN, iBase);  // v may be _UI64_MAX
	bOK = (errno == 0);  // EINVAL
	return pN;
}
inline char_h* string_to_value(const char_h* szString, int iBase, uint64& v, bool& bOK) throw()
{
	char_h* pN = NULL;
	errno = 0;
	v = ::_wcstoui64(szString, &pN, iBase);  // v may be _UI64_MAX
	bOK = (errno == 0);  // EINVAL
	return pN;
}

//------------------------------------------------------------------------------
// guid
//   XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX

inline bool string_to_guid(const char_a* szString, guid& id) throw()
{
	return ::UuidFromStringA((RPC_CSTR)szString, (UUID*)(&id)) == RPC_S_OK;
}
inline bool string_to_guid(const char_h* szString, guid& id) throw()
{
	return ::UuidFromStringW((RPC_WSTR)szString, (UUID*)(&id)) == RPC_S_OK;
}

// The length of szBuffer must be not less than 37.
inline bool guid_to_string(const guid& id, char_a* szBuffer) throw()
{
	szBuffer[0] = 0;
	RPC_CSTR str;
	if( ::UuidToStringA((const UUID*)(&id), &str) != RPC_S_OK )
		return false;
	uintptr uLen = calc_string_length((const char_a*)str);
	mem_copy(str, uLen * sizeof(char_a), szBuffer);
	szBuffer[uLen] = 0;
	RPC_STATUS rs = ::RpcStringFreeA(&str);
	(void)rs;
	assert( rs == RPC_S_OK );
	return true;
}
inline bool guid_to_string(const guid& id, char_h* szBuffer) throw()
{
	szBuffer[0] = 0;
	RPC_WSTR str;
	if( ::UuidToStringW((const UUID*)(&id), &str) != RPC_S_OK )
		return false;
	uintptr uLen = calc_string_length((const char_h*)str);
	mem_copy(str, uLen * sizeof(char_h), szBuffer);
	szBuffer[uLen] = 0;
	RPC_STATUS rs = ::RpcStringFreeW(&str);
	(void)rs;
	assert( rs == RPC_S_OK );
	return true;
}

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, char_a* szBuffer, uintptr uSize) throw()
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
inline void result_to_string(const call_result& cr, char_h* szBuffer, uintptr uSize) throw()
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
inline void cvt_path_string_to_platform(char_a* szBuffer) throw()
{
	char_a* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == '/' )
			*ps = '\\';
		++ ps;
	}
}
inline void cvt_path_string_to_platform(char_h* szBuffer) throw()
{
	char_h* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == L'/' )
			*ps = L'\\';
		++ ps;
	}
}
inline void cvt_path_string_to_ufs(char_a* szBuffer) throw()
{
	char_a* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == '\\' )
			*ps = '/';
		++ ps;
	}
}
inline void cvt_path_string_to_ufs(char_h* szBuffer) throw()
{
	char_h* ps = szBuffer;
	while( *ps != 0 ) {
		if( *ps == L'\\' )
			*ps = L'/';
		++ ps;
	}
}

//------------------------------------------------------------------------------
// path

//including the terminating null character
#define MAX_FULL_PATH  (32767)

inline void get_path_separator(char_a& ch) throw()
{
	ch = '\\';
}
inline void get_path_separator(char_h& ch) throw()
{
	ch = L'\\';
}

inline bool check_path_separator(const char_a& ch) throw()
{
	return ch == '\\' || ch == '/';
}
inline bool check_path_separator(const char_h& ch) throw()
{
	return ch == L'\\' || ch == L'/';
}
inline bool check_drive_separator(const char_a& ch) throw()
{
	return ch == ':';
}
inline bool check_drive_separator(const char_h& ch) throw()
{
	return ch == L':';
}

inline void get_path_extension_start(char_h& ch) throw()
{
	ch = L'.';
}
inline bool check_path_extension_start(const char_h& ch) throw()
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
				// "\\"
				iState = 5;
			}
			else {
				// "\X..."
				iRes = path_types::Relative;
				return iRes;
			}
			break;
		case 3:
			if( check_path_separator(*ps) ) {
				// ".\..."
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
				// "X..\.."
				iRes = path_types::Relative;
				return iRes;
			}
			else if( check_drive_separator(*ps) ) {
				iState = 7;
			}
			break;
		case 5:
			if( check_path_separator(*ps) ) {
				iState = 8;
			}
			break;
		case 6:
			if( check_path_separator(*ps) ) {
				// "..\"
				iRes = path_types::Relative;
				return iRes;
			}
			else {
				iState = 4;
			}
			break;
		case 7:
			if( check_path_separator(*ps) ) {
				iState = 5;
			}
			else {
				// "X...:X..."
				iRes = path_types::Relative;
				return iRes;
			}
			break;
		case 8:
			if( check_path_extension_start(*ps) ) {
				iState = 9;
			}
			else if( check_path_separator(*ps) ) {
			}
			else {
				iState = 5;
			}
			break;
		case 9:
			if( check_path_separator(*ps) ) {
				// "...\.\..."
				iRes = path_types::Relative;
				return iRes;
			}
			else if( check_path_extension_start(*ps) ) {
				iState = 10;
			}
			else {
				iState = 5;
			}
			break;
		case 10:
			if( check_path_separator(*ps) ) {
				// "...\..\..."
				iRes = path_types::Relative;
				return iRes;
			}
			else {
				iState = 5;
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
		if( iState == 2 || iState == 3 || iState == 6 || iState == 7 || iState == 9 || iState == 10 )
			iRes = path_types::Relative;
		else
			iRes = path_types::Absolute;
	}
	return iRes;
}

inline void get_current_path_prefix(const char_a*& sz, uintptr& len) throw()
{
	sz = "";
	len = 0;
}
inline void get_current_path_prefix(const char_h*& sz, uintptr& len) throw()
{
	sz = L"";
	len = 0;
}

inline void get_absolute_path_prefix(const char_a*& sz, uintptr& len) throw()
{
	sz = "\\\\?\\";
	len = 4;
}
inline void get_absolute_path_prefix(const char_h*& sz, uintptr& len) throw()
{
	sz = L"\\\\?\\";
	len = 4;
}

inline void get_sa_extention(const char_a*& sz, uintptr& len) throw()
{
	sz = ".dll";
	len = 4;
}
inline void get_sa_extention(const char_h*& sz, uintptr& len) throw()
{
	sz = L".dll";
	len = 4;
}

////////////////////////////////////////////////////////////////////////////////
