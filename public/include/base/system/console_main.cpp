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

/*
This file contains main function for console application.
*/

////////////////////////////////////////////////////////////////////////////////

#include "_sys_types.h"

////////////////////////////////////////////////////////////////////////////////

#if defined(OS_WINDOWS)
	#include "Windows/console_main.cpp"
#elif defined(OS_LINUX)
	#include "Linux/console_main.cpp"
#else
	#error Error OS type!
#endif

////////////////////////////////////////////////////////////////////////////////
