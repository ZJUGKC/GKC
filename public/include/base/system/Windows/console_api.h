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
//locale

// set_default_locale
inline void set_default_locale() throw()
{
	//no check
	::_wsetlocale(LC_ALL, L"");
}

//------------------------------------------------------------------------------
//print

// print_format
//   return value: the number of typed characters, -1 means fail.
inline int print_format(const CharA* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vprintf_s(szFormat, ap);
	va_end(ap);
	return ret;
}
inline int print_format(const CharH* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwprintf_s(szFormat, ap);
	va_end(ap);
	return ret;
}

// print string
inline void print_string(const CharA* sz) throw()
{
	//no check
	::printf_s("%s", sz);
}
inline void print_string(const CharH* sz) throw()
{
	//no check
	::wprintf_s(L"%s", sz);
}

////////////////////////////////////////////////////////////////////////////////
