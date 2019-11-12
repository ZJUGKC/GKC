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

//Linux

//------------------------------------------------------------------------------
// time

inline void time_initialize() throw()
{
}

inline void _os_timespec_to_timevalue(const struct timespec& tc, time_value& tv) throw()
{
	tv.set_Seconds(tc.tv_sec);
	tv.set_Nanoseconds(tc.tv_nsec);
}

inline void get_tick_count(time_value& tv) throw()
{
	struct timespec tc;
	int ret = ::clock_gettime(CLOCK_MONOTONIC_RAW, &tc);
	(void)ret;
	assert( ret == 0 );  //-1, errno
	_os_timespec_to_timevalue(tc, tv);
}

inline void get_current_time(time_value& tv) throw()
{
	struct timespec tc;
	int ret = ::clock_gettime(CLOCK_REALTIME, &tc);
	(void)ret;
	assert( ret == 0 );  //-1, errno
	_os_timespec_to_timevalue(tc, tv);
}

inline bool make_local_time(int iYear, int iMonth, int iDay,
							int iHour, int iMinute, int iSecond, int64 iNanoseconds,
							time_value& tv) throw()
{
	assert( iNanoseconds >= 0 && iNanoseconds < TIME_SECOND_FULL_NS );

	struct tm tms;
	_fill_tm_from_detail(iYear, iMonth, iDay,
						iHour, iMinute, iSecond,
						tms);
	time_t tmu = ::mktime(&tms);
	if( tmu == (time_t)-1 )
		return false;  //errno

	tv.Initialize(tmu, iNanoseconds);
	return true;
}

inline bool time_to_gmt_detail(const time_value& tv, time_detail& td) throw()
{
	struct tm tms;
	time_t sec = (time_t)tv.get_Seconds();
	if( ::gmtime_r(&sec, &tms) == NULL )
		return false;  //errno
	_fill_time_detail_from_tm(tms, td);
	td.iNanoseconds = tv.get_Nanoseconds();
	return true;
}

inline bool time_to_local_detail(const time_value& tv, time_detail& td) throw()
{
	struct tm tms;
	time_t sec = (time_t)tv.get_Seconds();
	if( ::localtime_r(&sec, &tms) == NULL )
		return false;  //errno
	_fill_time_detail_from_tm(tms, td);
	td.iNanoseconds = tv.get_Nanoseconds();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
