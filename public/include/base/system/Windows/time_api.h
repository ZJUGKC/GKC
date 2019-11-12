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

//Windows

//------------------------------------------------------------------------------
// time

extern LARGE_INTEGER  _os_g_time_liFreq;

inline void time_initialize() throw()
{
	BOOL bRet = ::QueryPerformanceFrequency(&_os_g_time_liFreq);
	(void)bRet;
	assert( bRet );  //FALSE, ::GetLastError()
}

inline void get_tick_count(time_value& tv) throw()
{
	LARGE_INTEGER liCount;
	BOOL bRet = ::QueryPerformanceCounter(&liCount);
	(void)bRet;
	assert( bRet );  //FALSE, ::GetLastError()
	tv.set_Seconds(liCount.QuadPart / _os_g_time_liFreq.QuadPart);
	tv.set_Nanoseconds(((int64)((double)TIME_SECOND_FULL_NS / (double)(_os_g_time_liFreq.QuadPart) * (double)(liCount.QuadPart % _os_g_time_liFreq.QuadPart))) % TIME_SECOND_FULL_NS);
}

#define _TIME_DELTA_1601_1970  (11644473600LL)

//FILETIME <-> 100ns
inline int64 _os_filetime_to_100ns(const FILETIME& ft) throw()
{
	LARGE_INTEGER liValue;
	liValue.LowPart  = ft.dwLowDateTime;
	liValue.HighPart = ft.dwHighDateTime;
	return liValue.QuadPart;
}
inline void _os_100ns_to_filetime(int64 v, FILETIME& ft) throw()
{
	LARGE_INTEGER liValue;
	liValue.QuadPart  = v;
	ft.dwLowDateTime  = liValue.LowPart;
	ft.dwHighDateTime = liValue.HighPart;
}

//FILETIME <-> time_value
inline void _os_timevalue_to_filetime(const time_value& tv, FILETIME& ft) throw()
{
	int64 v = (tv.get_Seconds() + _TIME_DELTA_1601_1970) * TIME_SECOND_100_NS
				+ tv.get_Nanoseconds() / 100;
	_os_100ns_to_filetime(v, ft);
}
inline void _os_filetime_to_timevalue(const FILETIME& ft, time_value& tv) throw()
{
	int64 v = _os_filetime_to_100ns(ft);
	tv.Initialize(v / TIME_SECOND_100_NS - _TIME_DELTA_1601_1970,
				(v % TIME_SECOND_100_NS) * 100);
}

inline bool _os_is_valid_filetime(const FILETIME& ft) throw()
{
	FILETIME ftLocal;
	if( !::FileTimeToLocalFileTime(&ft, &ftLocal) )
		return false;  //FALSE, ::GetLastError()
	SYSTEMTIME st;
	if( !::FileTimeToSystemTime(&ftLocal, &st) )
		return false;  //FALSE, ::GetLastError()
	return true;
}
inline void _os_fill_tm_from_systemtime(const SYSTEMTIME& st, struct tm& tms) throw()
{
	_fill_tm_from_detail((int)(st.wYear), (int)(st.wMonth), (int)(st.wDay),
						(int)(st.wHour), (int)(st.wMinute), (int)(st.wSecond),
						tms);
}

inline void get_current_time(time_value& tv) throw()
{
	FILETIME ft;
	::GetSystemTimePreciseAsFileTime(&ft);
	_os_filetime_to_timevalue(ft, tv);
}

inline bool make_local_time(int iYear, int iMonth, int iDay,
							int iHour, int iMinute, int iSecond, int64 iNanoseconds,
							time_value& tv) throw()
{
	assert( iSecond >= 0 && iSecond <= 59 );
	assert( iNanoseconds >= 0 && iNanoseconds < TIME_SECOND_FULL_NS );

	struct tm tms;
	_fill_tm_from_detail(iYear, iMonth, iDay,
						iHour, iMinute, iSecond,
						tms);
	__time64_t tmu = ::_mktime64(&tms);
	if( tmu == (__time64_t)-1 )
		return false;  //errno

	tv.Initialize(tmu, iNanoseconds);
	return true;
}

inline bool _os_make_local_time_from_systemtime(const SYSTEMTIME& st, time_value& tv) throw()
{
	return make_local_time((int)(st.wYear), (int)(st.wMonth), (int)(st.wDay),
							(int)(st.wHour), (int)(st.wMinute), (int)(st.wSecond),
							(int64)(st.wMilliseconds) * 1000000LL,
							tv);
}

inline bool time_to_gmt_detail(const time_value& tv, time_detail& td) throw()
{
	struct tm tms;
	if( ::_gmtime64_s(&tms, &(tv.get_Seconds())) != 0 )
		return false;
	_fill_time_detail_from_tm(tms, td);
	td.iNanoseconds = tv.get_Nanoseconds();
	return true;
}

inline bool time_to_local_detail(const time_value& tv, time_detail& td) throw()
{
	struct tm tms;
	if( ::_localtime64_s(&tms, &(tv.get_Seconds())) != 0 )
		return false;
	_fill_time_detail_from_tm(tms, td);
	td.iNanoseconds = tv.get_Nanoseconds();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
