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
//character

typedef char            CharA;  //ANSI or UTF8
typedef unsigned short  CharH;  //word or UTF16
typedef wchar_t         CharL;  //long or UTF32

typedef CharA  CharS;  //system type, UTF8
//for const string
#define _S(x)  x

typedef CharL  CharW;  //for wide type, L"..."

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

////////////////////////////////////////////////////////////////////////////////
