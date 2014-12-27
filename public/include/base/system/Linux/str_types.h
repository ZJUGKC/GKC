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

////////////////////////////////////////////////////////////////////////////////
