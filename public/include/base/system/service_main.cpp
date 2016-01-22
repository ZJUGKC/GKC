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

/*
This file contains main function for Service program.
*/

////////////////////////////////////////////////////////////////////////////////

#include "_sys_cmdline.h"

////////////////////////////////////////////////////////////////////////////////

#if defined(GKC_OS_WINDOWS)
	#include "Windows/service_main.cpp"
#elif defined(GKC_OS_LINUX)
	#include "Linux/service_main.cpp"
#else
	#error Error OS type!
#endif

////////////////////////////////////////////////////////////////////////////////
