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
	#include "Windows/other_types.h"
#elif defined(OS_LINUX)
	#include "Linux/other_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//functions

//memory
inline uintptr crt_alloc(const uintptr& uBytes) throw()
{
	return (uintptr)::malloc(uBytes);
}

inline uintptr crt_realloc(const uintptr& p, const uintptr& uBytes) throw()
{
	return (uintptr)::realloc((void*)p, uBytes);
}

inline void    crt_free(const uintptr& p) throw()
{
	::free((void*)p);
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
