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
//locale

// set_default_locale
inline void set_default_locale() throw()
{
	//no check
	::setlocale(LC_ALL, "");
}

//------------------------------------------------------------------------------
//print

// print_format
//   return value: the number of typed characters, -1 means fail.
inline int print_format(const CharA* szFormat, ...) throw()
{
	CharA* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vprintf(szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}
inline int print_format(const CharL* szFormat, ...) throw()
{
	CharL* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwprintf(szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
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
	::wprintf(L"%ls", sz);
}

//------------------------------------------------------------------------------
//input

// scan_format
//  return value: the number of input items successfully matched and assigned. <0 means fail.
inline int scan_format(const CharA* szFormat, ...) throw()
{
	CharA* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vscanf(szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}
inline int scan_format(const CharL* szFormat, ...) throw()
{
	CharL* szV = _convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwscanf(szV, ap);
	va_end(ap);
	_free_unified_format_convert_string(szV);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
