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
	tv.set_Nanoseconds(((int64)(1.0e9 / (double)(_os_g_time_liFreq.QuadPart) * (double)(liCount.QuadPart % _os_g_time_liFreq.QuadPart))) % 1000000000LL);
}

inline void get_current_time(time_value& tv) throw()
{
	FILETIME ft;
	::GetSystemTimePreciseAsFileTime(&ft);
	//nanoseconds
	ULARGE_INTEGER uiValue;
	uiValue.LowPart  = ft.dwLowDateTime;
	uiValue.HighPart = ft.dwHighDateTime;
	tv.set_Nanoseconds((uiValue.QuadPart % 10000000ULL) * 100);
	//second
	SYSTEMTIME st;
	BOOL bRet = ::FileTimeToSystemTime(&ft, &st);
	(void)bRet;
	assert( bRet );  //FALSE, ::GetLastError()
	struct tm tms;
	tms.tm_hour = st.wHour;
	tms.tm_mday = st.wDay;
	tms.tm_min  = st.wMinute;
	tms.tm_mon  = st.wMonth - 1;
	tms.tm_sec  = st.wSecond;
	tms.tm_year = st.wYear - 1900;
	tms.isdst   = 0;
	// ignore
	tms.wday    = 0;
	tms.yday    = 0;
	__time64_t tmu = ::_mkgmtime64(&tms);
	assert( tmu != (__time64_t)-1 );
	tv.set_Seconds(tmu);
}

////////////////////////////////////////////////////////////////////////////////
