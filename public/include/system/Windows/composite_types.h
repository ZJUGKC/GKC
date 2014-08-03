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

#define BEGIN_NOINLINE __declspec(noinline)
#define END_NOINLINE

//no vtable
#define NOVTABLE  __declspec(novtable)

//export functions for shared library
#ifdef SHARED_LIBRARY
	#ifdef __cplusplus
		#define SA_FUNCTION  extern "C" __declspec(dllexport)
	#else
		#define SA_FUNCTION  __declspec(dllexport)
	#endif
#else
	#ifdef __cplusplus
		#define SA_FUNCTION  extern "C" __declspec(dllimport)
	#else
		#define SA_FUNCTION  __declspec(dllimport)
	#endif
#endif

////////////////////////////////////////////////////////////////////////////////
