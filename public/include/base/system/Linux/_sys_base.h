/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
//internal header
////////////////////////////////////////////////////////////////////////////////

#ifndef __ILINUX_BASE_H__
#define __ILINUX_BASE_H__

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Time

// _tm_to_system_time

inline void _tm_to_system_time(const struct tm* ptm, GKC::SystemTime& tm) throw()
{
	tm.uYear         = (ushort)(ptm->tm_year + 1900);
	tm.uMonth        = (ushort)(ptm->tm_mon + 1);
	tm.uDayOfWeek    = (ushort)(ptm->tm_wday);
	tm.uDay          = (ushort)(ptm->tm_mday);
	tm.uHour         = (ushort)(ptm->tm_hour);
	tm.uMinute       = (ushort)(ptm->tm_min);
	tm.uSecond       = (ushort)(ptm->tm_sec);
	tm.uMilliseconds = 0;
}

////////////////////////////////////////////////////////////////////////////////
#endif  //__ILINUX_BASE_H__
////////////////////////////////////////////////////////////////////////////////
