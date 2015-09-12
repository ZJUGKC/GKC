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

//basic types

//bool
//char
//byte
typedef unsigned char   byte;
//short
//ushort
typedef unsigned short  ushort;
//int
//uint
typedef unsigned int    uint;
//float
//double

#if defined(GKC_OS_WINDOWS)

#include "Windows/base_types.h"

#elif defined(GKC_OS_LINUX)

#include "Linux/base_types.h"

#else
	#error error OS type!
#endif

#if defined(CM_X86)

//intptr
typedef int      intptr;
//uintptr
typedef uint     uintptr;

#elif defined(CM_X64)

//intptr
typedef int64    intptr;
//uintptr
typedef uint64   uintptr;

#else
	#error error machine type!
#endif

////////////////////////////////////////////////////////////////////////////////
