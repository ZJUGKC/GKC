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

#define TIME_SECOND_FULL_NS  (1000000000LL)
#define TIME_SECOND_100_NS   (10000000LL)

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
		m_iSeconds     = src.m_iSeconds;
		m_iNanoseconds = src.m_iNanoseconds;
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
	const int64& get_Seconds() const throw()
	{
		return m_iSeconds;
	}
	int64& get_Seconds() throw()
	{
		return m_iSeconds;
	}
	void set_Seconds(int64 v) throw()
	{
		m_iSeconds = v;
	}
	const int64& get_Nanoseconds() const throw()
	{
		return m_iNanoseconds;
	}
	int64& get_Nanoseconds() throw()
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
		const int64 c_iNS = TIME_SECOND_FULL_NS;
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
	static constexpr const int c_MaxDaysInSpan = 3615897L;

public:
// Constructors
	time_span() throw()
	{
	}
	time_span(int64 iS, int64 iNS) throw() : _time_def(iS, iNS)
	{
		normalize();
	}
	time_span(int iDays, int iHours, int iMinutes, int iSeconds, int64 iNanoseconds = 0) throw()
	{
		//overflow is not checked
		Initialize(iSeconds + 60 * (iMinutes + 60 * (iHours + (int64)24 * iDays)), iNanoseconds);
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
	time_span operator+(const time_span& ts) const throw()
	{
		time_span tmp(*this);
		tmp += ts;
		return tmp;
	}
	time_span operator-(const time_span& ts) const throw()
	{
		time_span tmp(*this);
		tmp -= ts;
		return tmp;
	}

	time_span operator-() const throw()
	{
		return time_span(-m_iSeconds, -m_iNanoseconds);
	}

//methods
	void Initialize(int64 iS, int64 iNS) throw()
	{
		set_Seconds(iS);
		set_Nanoseconds(iNS);
		normalize();
	}

// Attributes

	// component days in span
	int64 GetDays() const throw()
	{
		return m_iSeconds / (24 * 3600);
	}
	// component hours in span (-23 to 23)
	int64 GetHours() const throw()
	{
		return m_iSeconds / 3600;
	}
	// component minutes in span (-59 to 59)
	int64 GetMinutes() const throw()
	{
		return m_iSeconds / 60;
	}
	// component seconds in span (-59 to 59)
	int64 GetSeconds() const throw()
	{
		return m_iSeconds;
	}
	void GetDetail(int& iHours, int& iMinutes, int& iSeconds) const throw()
	{
		iHours = GetHours() % 24;
		iMinutes = GetMinutes() % 60;
		iSeconds = m_iSeconds % 60;
	}
};

// time_value

class time_value : public _time_def
{
public:
// Constructors
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
	time_value operator+(const time_span& ts) const throw()
	{
		time_value tmp(*this);
		tmp += ts;
		return tmp;
	}
	time_value operator-(const time_span& ts) const throw()
	{
		time_value tmp(*this);
		tmp -= ts;
		return tmp;
	}

	time_span operator-(const time_value& tv) const throw()
	{
		return time_span(m_iSeconds - tv.m_iSeconds, m_iNanoseconds - tv.m_iNanoseconds);
	}

//methods
	void Initialize(int64 iS, int64 iNS) throw()
	{
		set_Seconds(iS);
		set_Nanoseconds(iNS);
		normalize();
	}
};

// time_detail

typedef struct _tag_time_detail
{
	int iYear;           // year
	int iMonth;          // [1 --- 12] Month of year (1 = January)
	int iDay;            // [1 --- 31] Day of month (1-31)
	int iDayOfWeek;      // [0 --- 6] (since Sunday) Day of week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
	int iDayOfYear;      // [0 --- 365] Days since start of year (0 = January 1)
	int iHour;           // [0 --- 23] Hour in day (0-23)
	int iMinute;         // [0 --- 59] Minute in hour (0-59)
	int iSecond;         // [0 --- 60] (leap seconds) Second in minute (0-59)
	int64 iNanoseconds;  // [0 --- 999999999]
} time_detail;

#pragma pack(pop)

inline void _fill_time_detail_from_tm(const struct tm& tms, time_detail& td) throw()
{
	td.iYear = tms.tm_year + 1900;
	td.iMonth = tms.tm_mon + 1;
	td.iDay = tms.tm_mday;
	td.iDayOfWeek = tms.tm_wday;
	td.iDayOfYear = tms.tm_yday;
	td.iHour = tms.tm_hour;
	td.iMinute = tms.tm_min;
	td.iSecond = tms.tm_sec;
}

inline void _fill_tm_from_detail(int iYear, int iMonth, int iDay,
								int iHour, int iMinute, int iSecond,
								struct tm& tms) throw()
{
	assert( iYear >= 1900 );
	assert( iMonth >= 1 && iMonth <= 12 );
	assert( iDay >= 1 && iDay <= 31 );
	assert( iHour >= 0 && iHour <= 23 );
	assert( iMinute >= 0 && iMinute <= 59 );
	assert( iSecond >= 0 && iSecond <= 60 );  //leap seconds

	tms.tm_hour  = iHour;
	tms.tm_mday  = iDay;
	tms.tm_min   = iMinute;
	tms.tm_mon   = iMonth - 1;
	tms.tm_sec   = iSecond;
	tms.tm_year  = iYear - 1900;
	tms.tm_isdst = -1;
	// ignore
	tms.tm_wday  = 0;
	tms.tm_yday  = 0;
}

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
