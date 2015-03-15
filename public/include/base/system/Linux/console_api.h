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
//print

// print_format
//   return value: the number of typed characters, -1 means fail.
inline int print_format(const CharA* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vprintf(szFormat, ap);
	va_end(ap);
	return ret;
}
inline int print_format(const CharL* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwprintf(szFormat, ap);
	va_end(ap);
	return ret;
}

// print string
inline void print_string(const CharA* sz) throw()
{
	//no check
	::printf("%s", sz);
}
inline void print_string(const CharL* sz) throw()
{
	//no check
	::wprintf(L"%s", sz);
}

////////////////////////////////////////////////////////////////////////////////
