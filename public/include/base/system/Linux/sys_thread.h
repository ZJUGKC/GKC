/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
// Thread

// thread_sleep
//  uTimeout: ms
inline void thread_sleep(uint uTimeout) throw()
{
	uint uSecond = uTimeout / 1000;
	uint uRest = uTimeout % 1000;
	//no check
	if( uSecond != 0 )
		::sleep(uSecond);
	::usleep(uRest * 1000);
}

////////////////////////////////////////////////////////////////////////////////
