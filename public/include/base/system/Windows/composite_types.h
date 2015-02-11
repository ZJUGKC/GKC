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

//------------------------------------------------------------------------------
//shared library

//global variables in shared library

//in header file
#define DECLARE_SA_GLOBAL_VARIABLE(T, x)    extern T g_##x;

//in cpp file
#define BEGIN_SA_GLOBAL_VARIABLE(T, x)    T g_##x

#define BEGIN_SA_GLOBAL_VARIABLE_CONSTRUCTOR()         (
#define SA_GLOBAL_VARIABLE_CONSTRUCTOR_PARAM(z)        z,
#define SA_GLOBAL_VARIABLE_CONSTRUCTOR_PARAM_LAST(z)   z
#define END_SA_GLOBAL_VARIABLE_CONSTRUCTOR()           )

#define END_SA_GLOBAL_VARIABLE(x)    ;

//in other files
#define GET_SA_GLOBAL_VARIABLE(x)    g_##x

////////////////////////////////////////////////////////////////////////////////
