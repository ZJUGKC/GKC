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
inline int print_format(const char_a* szFormat, ...) throw()
{
	char_a* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vprintf(szV, ap);
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}
inline int print_format(const char_l* szFormat, ...) throw()
{
	char_l* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwprintf(szV, ap);
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}

// print string
inline void print_string(const char_a* sz) throw()
{
	//no check
	::printf("%s", sz);
}
inline void print_string(const char_l* sz) throw()
{
	//no check
	::wprintf(L"%ls", sz);
}

//------------------------------------------------------------------------------
//stdout attributes

// Macros
#define STDOUT_ATTR_FORE_BLUE          (0x00000004)
#define STDOUT_ATTR_FORE_GREEN         (0x00000002)
#define STDOUT_ATTR_FORE_RED           (0x00000001)
#define STDOUT_ATTR_FORE_INTENSITY     (0x00000008)
#define STDOUT_ATTR_BACK_BLUE          (0x00000040)
#define STDOUT_ATTR_BACK_GREEN         (0x00000020)
#define STDOUT_ATTR_BACK_RED           (0x00000010)
#define STDOUT_ATTR_BACK_INTENSITY     (0x00000080)
#define STDOUT_ATTR_UNDERSCORE         (0x00000100)
#define STDOUT_ATTR_REVERSE            (0x00000200)

#define STDOUT_ATTR_FORE_KEEP          (0x00100000)
#define STDOUT_ATTR_BACK_KEEP          (0x00200000)

// stdout_attr
class stdout_attr
{
public:
	stdout_attr() throw()
	{
	}
	~stdout_attr() throw()
	{
	}

	//initialize
	void Init() throw()
	{
	}

	//restore
	void Restore() throw()
	{
		::printf("\033[0m");
	}

	//set attribute (one or more STDOUT_ATTR_*)
	void SetAttribute(uint uAttrs) throw()
	{
		char szFormat[256];
		szFormat[0] = 0;
		bool bUnderScore = (uAttrs & STDOUT_ATTR_UNDERSCORE) != 0;
		int ret = value_to_string(szFormat, sizeof(szFormat) / sizeof(char),
								"\033[%d;%d;%dm",
								(uAttrs & STDOUT_ATTR_REVERSE) ? (7) : (bUnderScore ? (4) : ((uAttrs & (STDOUT_ATTR_FORE_INTENSITY | STDOUT_ATTR_BACK_INTENSITY)) ? (1) : (0))),       //mode
								(uAttrs & STDOUT_ATTR_FORE_KEEP) ? (bUnderScore ? 38 : 39) : (uAttrs & (STDOUT_ATTR_FORE_RED | STDOUT_ATTR_FORE_GREEN | STDOUT_ATTR_FORE_BLUE)) + 30,  //foreground
								(uAttrs & STDOUT_ATTR_BACK_KEEP) ? (49) : ((uAttrs & (STDOUT_ATTR_BACK_RED | STDOUT_ATTR_BACK_GREEN | STDOUT_ATTR_BACK_BLUE)) >> 4) + 40               //background
								);
		if( ret >= 0 )
			szFormat[ret] = 0;
		print_string(szFormat);
	}

private:
	//noncopyable
	stdout_attr(const stdout_attr&) throw();
	stdout_attr& operator=(const stdout_attr&) throw();
};

//------------------------------------------------------------------------------
//input

// scan_format
//  return value: the number of input items successfully matched and assigned. <0 means fail.
inline int scan_format(const char_a* szFormat, ...) throw()
{
	char_a* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vscanf(szV, ap);
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}
inline int scan_format(const char_l* szFormat, ...) throw()
{
	char_l* szV = _os_convert_unified_format_string(szFormat);
	if( szV == NULL )
		return -1;
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwscanf(szV, ap);
	va_end(ap);
	_os_free_unified_format_convert_string(szV);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
