/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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

// Time

//------------------------------------------------------------------------------

// is_leap_year

inline bool is_leap_year(uint uYear) throw()
{
	return (uYear % 4) == 0 && ((uYear % 100) != 0 || (uYear % 400) == 0);
}

#pragma pack(push, 1)

class _time_def
{
public:
	explicit _time_def(int64 iS = 0, int64 iNS = 0) throw() : m_iSeconds(iS), m_iNanoseconds(iNS)
	{
	}
	_time_def(const _time_def& src) throw() : m_iSeconds(src.m_iSeconds), m_iNanoseconds(src.m_iNanoseconds)
	{
	}
	~_time_def() throw()
	{
	}

//operators
	_time_def& operator=(const _time_def& src) throw()
	{
		if( this != &src ) {
			m_iSeconds     = src.m_iSeconds;
			m_iNanoseconds = src.m_iNanoseconds;
		}
		return *this;
	}

	bool operator==(const _time_def& right) const throw()
	{
		return m_iSeconds == right.m_iSeconds && m_iNanoseconds == right.m_iNanoseconds;
	}
	bool operator!=(const _time_def& right) const throw()
	{
		return m_iSeconds != right.m_iSeconds || m_iNanoseconds != right.m_iNanoseconds;
	}
	bool operator<(const _time_def& right) const throw()
	{
		if( m_iSeconds == right.m_iSeconds )
			return m_iNanoseconds < right.m_iNanoseconds;
		return m_iSeconds < right.m_iSeconds;
	}
	bool operator>(const _time_def& right) const throw()
	{
		return right < *this;
	}
	bool operator<=(const _time_def& right) const throw()
	{
		return !operator>(right);
	}
	bool operator>=(const _time_def& right) const throw()
	{
		return !operator<(right);
	}

//properties
	int64 get_Seconds() const throw()
	{
		return m_iSeconds;
	}
	void set_Seconds(int64 v) throw()
	{
		m_iSeconds = v;
	}
	int64 get_Nanoseconds() const throw()
	{
		return m_iNanoseconds;
	}
	void set_Nanoseconds(int64 v) throw()
	{
		m_iNanoseconds = v;
	}

//operations
	_time_def& operator+=(const _time_def& td) throw()
	{
		m_iSeconds     += td.m_iSeconds;
		m_iNanoseconds += td.m_iNanoseconds;
		return *this;
	}
	_time_def& operator-=(const _time_def& td) throw()
	{
		m_iSeconds     -= td.m_iSeconds;
		m_iNanoseconds -= td.m_iNanoseconds;
		return *this;
	}

protected:
	void normalize() throw()
	{
		const int64 c_iNS = 1000000000LL;
		m_iSeconds += (m_iNanoseconds / c_iNS);
		m_iNanoseconds %= c_iNS;
		if( m_iSeconds > 0 && m_iNanoseconds < 0 ) {
			-- m_iSeconds;
			m_iNanoseconds += c_iNS;
		}
		else if( m_iSeconds < 0 && m_iNanoseconds > 0 ) {
			++ m_iSeconds;
			m_iNanoseconds -= c_iNS;
		}
	}

protected:
	int64 m_iSeconds;
	int64 m_iNanoseconds;
};

// time_span

class time_span : public _time_def
{
public:
	time_span() throw()
	{
	}
	time_span(int64 iS, int64 iNS) throw() : _time_def(iS, iNS)
	{
		normalize();
	}
	time_span(const time_span& src) throw() : _time_def(src)
	{
	}
	~time_span() throw()
	{
	}

//operators
	time_span& operator=(const time_span& src) throw()
	{
		_time_def::operator=(src);
		return *this;
	}

//operations
	time_span& operator+=(const time_span& ts) throw()
	{
		_time_def::operator+=(ts);
		normalize();
		return *this;
	}
	time_span& operator-=(const time_span& ts) throw()
	{
		_time_def::operator-=(ts);
		normalize();
		return *this;
	}
	time_span operator+(const time_span& ts) throw()
	{
		time_span tmp(*this);
		tmp += ts;
		return tmp;
	}
	time_span operator-(const time_span& ts) throw()
	{
		time_span tmp(*this);
		tmp -= ts;
		return tmp;
	}
};

// time_value

class time_value : public _time_def
{
public:
	time_value() throw()
	{
	}
	time_value(int64 iS, int64 iNS) throw() : _time_def(iS, iNS)
	{
		normalize();
	}
	time_value(const time_value& src) throw() : _time_def(src)
	{
	}
	~time_value() throw()
	{
	}

//operators
	time_value& operator=(const time_value& src) throw()
	{
		_time_def::operator=(src);
		return *this;
	}

//operations
	time_value& operator+=(const time_span& ts) throw()
	{
		_time_def::operator+=(ts);
		normalize();
		return *this;
	}
	time_value& operator-=(const time_span& ts) throw()
	{
		_time_def::operator-=(ts);
		normalize();
		return *this;
	}
	time_value operator+(const time_span& ts) throw()
	{
		time_value tmp(*this);
		tmp += ts;
		return tmp;
	}
	time_value operator-(const time_span& ts) throw()
	{
		time_value tmp(*this);
		tmp -= ts;
		return tmp;
	}
};

// time_detail

typedef struct _tag_time_detail
{
	int iYear;           // years since 1900
	int iMonth;          // [-11 --- 11] or [0 --- 11]
	int iDay;            // [-31 --- 31] or [1 --- 31]
	int iDayOfWeek;      // [0 --- 6]
	int iDayOfYear;      // [0 --- 365]
	int iHour;           // [-23 --- 23] or [0 --- 23]
	int iMinute;         // [-59 --- 59] or [0 --- 59]
	int iSecond;         // [-60 --- 60] or [0 --- 60]
	int64 iNanoseconds;  // [-999999999 --- 999999999] or [0 --- 999999999]
} time_detail;

#pragma pack(pop)

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/time_api.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/time_api.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
