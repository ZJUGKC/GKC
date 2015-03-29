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
inline uintptr calc_char_length(const CharA* s) throw()
{
	return ::mbrlen(s, MB_LEN_MAX, NULL);
}

// calc_string_char_length
//   in current locale
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

//------------------------------------------------------------------------------
// result_to_string
//   uSize : buffer size in typed characters. It must be large than 0.
inline void result_to_string(const call_result& cr, CharA* szBuffer, uintptr uSize) throw()
{
	szBuffer[0] = 0;
	char* szRet = ::strerror_r(cr.GetResult() & (~0x10000000), szBuffer, uSize);
	if( szRet != szBuffer ) {
		//copy
		uintptr uLen = ::strlen(szRet);
		if( uLen > uSize - 1 )
			uLen = uSize - 1;
		mem_copy(szRet, uLen * sizeof(CharA), szBuffer);
		szBuffer[uLen] = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
