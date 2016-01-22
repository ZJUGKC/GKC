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
This file contains main function for Shared Assembly.
*/

////////////////////////////////////////////////////////////////////////////////

#if defined(GKC_OS_WINDOWS)
	#include "Windows/sa_main.cpp"
#elif defined(GKC_OS_LINUX)
	#include "Linux/sa_main.cpp"
#else
	#error Error OS type!
#endif

//others
#include "_sys_base.cpp"

////////////////////////////////////////////////////////////////////////////////
