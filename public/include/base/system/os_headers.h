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

//OS

#if defined(OS_WINDOWS)
	#include "Windows/os_headers.h"
#elif defined(OS_LINUX)
	#include "Linux/os_headers.h"
#else
	#error Error OS type!
#endif

//CRT

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <stdarg.h>
#include <errno.h>

#if defined(OS_WINDOWS)
	#include <tchar.h>
	#include <mbstring.h>
#elif defined(OS_LINUX)
	#include <wchar.h>
	#include <ctype.h>
	#include <wctype.h>
	#include <iconv.h>
#else
	#error Error OS type!
#endif

#include <locale.h>

#include <math.h>
#include <limits.h>
#if defined(OS_WINDOWS)
	#include <float.h>
#elif defined(OS_LINUX)
	#define FLT_EPSILON   __FLT_EPSILON__
	#define FLT_MIN       __FLT_MIN__
	#define FLT_MAX       __FLT_MAX__
	#define DBL_EPSILON   __DBL_EPSILON__
	#define DBL_MIN       __DBL_MIN__
	#define DBL_MAX       __DBL_MAX__
#else
	#error Error OS type!
#endif

////////////////////////////////////////////////////////////////////////////////
