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

inline void get_tick_count(time_value& tv) throw()
{
	struct timespec tc;
	int ret = ::clock_gettime(CLOCK_MONOTONIC_RAW, &tc);
	(void)ret;
	assert( ret == 0 );  //-1, errno
	tv.set_Seconds(tc.tv_sec);
	tv.set_Nanoseconds(tc.tv_nsec);
}

inline void get_current_time(time_value& tv) throw()
{
	struct timespec tc;
	int ret = ::clock_gettime(CLOCK_REALTIME, &tc);
	(void)ret;
	assert( ret == 0 );  //-1, errno
	tv.set_Seconds(tc.tv_sec);
	tv.set_Nanoseconds(tc.tv_nsec);
}

////////////////////////////////////////////////////////////////////////////////
