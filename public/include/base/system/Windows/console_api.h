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
inline int print_format(const char_a* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vprintf_s(szFormat, ap);
	va_end(ap);
	return ret;
}
inline int print_format(const char_h* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwprintf_s(szFormat, ap);
	va_end(ap);
	return ret;
}

// print string
inline void print_string(const char_a* sz) throw()
{
	//no check
	::printf_s("%s", sz);
}
inline void print_string(const char_h* sz) throw()
{
	//no check
	::wprintf_s(L"%s", sz);
}

//------------------------------------------------------------------------------
//stdout attributes

// Macros
#define STDOUT_ATTR_FORE_BLUE          FOREGROUND_BLUE
#define STDOUT_ATTR_FORE_GREEN         FOREGROUND_GREEN
#define STDOUT_ATTR_FORE_RED           FOREGROUND_RED
#define STDOUT_ATTR_FORE_INTENSITY     FOREGROUND_INTENSITY
#define STDOUT_ATTR_BACK_BLUE          BACKGROUND_BLUE
#define STDOUT_ATTR_BACK_GREEN         BACKGROUND_GREEN
#define STDOUT_ATTR_BACK_RED           BACKGROUND_RED
#define STDOUT_ATTR_BACK_INTENSITY     BACKGROUND_INTENSITY
#define STDOUT_ATTR_UNDERSCORE         COMMON_LVB_UNDERSCORE
#define STDOUT_ATTR_REVERSE            COMMON_LVB_REVERSE_VIDEO

#define STDOUT_ATTR_FORE_KEEP          (0x00100000)
#define STDOUT_ATTR_BACK_KEEP          (0x00200000)

// stdout_attr
class stdout_attr
{
public:
	stdout_attr() throw() : m_bInit(false), m_wOldAttr(0), m_hStdOutput(NULL)
	{
	}
	~stdout_attr() throw()
	{
	}

	//initialize
	void Init() throw()
	{
		assert( !m_bInit );
		m_bInit = true;
		m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if( m_hStdOutput == INVALID_HANDLE_VALUE || m_hStdOutput == NULL ) {
			m_bInit = false;
			return ;
		}
		m_bInit = get_console_text_attribute(m_wOldAttr);
	}

	//restore
	void Restore() throw()
	{
		//restore automatically
		if( m_bInit ) {
			::SetConsoleTextAttribute(m_hStdOutput, m_wOldAttr);  //no check
		}
	}
	//set attribute (one or more STDOUT_ATTR_*)
	void SetAttribute(uint uAttrs) throw()
	{
		if( m_bInit ) {
			// keep
			if( uAttrs & (STDOUT_ATTR_FORE_KEEP | STDOUT_ATTR_BACK_KEEP) ) {
				WORD wAttr;
				get_console_text_attribute(wAttr);  //no check
				if( uAttrs & STDOUT_ATTR_FORE_KEEP ) {
					uint uForeMask = STDOUT_ATTR_FORE_RED | STDOUT_ATTR_FORE_GREEN | STDOUT_ATTR_FORE_BLUE | STDOUT_ATTR_FORE_INTENSITY;
					uAttrs = ((uAttrs & ~STDOUT_ATTR_FORE_KEEP) & ~uForeMask) | (uint)(wAttr & uForeMask);
				}
				if( uAttrs & STDOUT_ATTR_BACK_KEEP ) {
					uint uBackMask = STDOUT_ATTR_BACK_RED | STDOUT_ATTR_BACK_GREEN | STDOUT_ATTR_BACK_BLUE | STDOUT_ATTR_BACK_INTENSITY;
					uAttrs = ((uAttrs & ~STDOUT_ATTR_BACK_KEEP) & ~uBackMask) | (uint)(wAttr & uBackMask);
				}
			}
			// only lower word-bits are used.
			::SetConsoleTextAttribute(m_hStdOutput, (WORD)uAttrs);  //no check
		}
	}

private:
	bool get_console_text_attribute(WORD& wAttr) throw()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		if( !::GetConsoleScreenBufferInfo(m_hStdOutput, &csbiInfo) )
			return false;
		wAttr = csbiInfo.wAttributes;
		return true;
	}

private:
	bool   m_bInit;
	WORD   m_wOldAttr;
	HANDLE m_hStdOutput;

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
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vscanf(szFormat, ap);
	va_end(ap);
	return ret;
}
inline int scan_format(const char_h* szFormat, ...) throw()
{
	va_list ap;
	va_start(ap, szFormat);
	int ret = ::vwscanf(szFormat, ap);
	va_end(ap);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
