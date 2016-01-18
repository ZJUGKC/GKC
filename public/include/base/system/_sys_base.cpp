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
// internal
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/_sys_base.cpp"
#elif defined(GKC_OS_LINUX)
	#include "Linux/_sys_base.cpp"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
