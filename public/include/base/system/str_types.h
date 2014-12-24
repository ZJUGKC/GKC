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

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//OS

#if defined(OS_WINDOWS)
	#include "Windows/str_types.h"
#elif defined(OS_LINUX)
	#include "Linux/str_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//tools

// calc_string_length

inline uintptr calc_string_length(const CharA* s) throw()
{
	return ::strlen(s);
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
