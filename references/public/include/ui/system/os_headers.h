/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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

//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/os_headers.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/os_headers.h"
#else
	#error Error OS type!
#endif

////////////////////////////////////////////////////////////////////////////////
