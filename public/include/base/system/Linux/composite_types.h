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

// for composite types

//no inline

#define BEGIN_NOINLINE
#define END_NOINLINE __attribute__((noinline))

//no vtable
#define NOVTABLE

//export functions for shared library
#ifdef SHARED_LIBRARY
	#ifdef __cplusplus
		#define SA_FUNCTION  extern "C" __attribute__((visibility("default")))
	#else
		#define SA_FUNCTION  __attribute__((visibility("default")))
	#endif
#else
	#ifdef __cplusplus
		#define SA_FUNCTION  extern "C"
	#else
		#define SA_FUNCTION
	#endif
#endif

////////////////////////////////////////////////////////////////////////////////
