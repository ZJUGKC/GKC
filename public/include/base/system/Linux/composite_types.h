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

#include <new>

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////

// for composite types

//no inline

#define BEGIN_NOINLINE
#define END_NOINLINE __attribute__((noinline))

//no vtable
#define NOVTABLE

//------------------------------------------------------------------------------
//shared library

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

//global variables in shared library
/*
Because the global variables are initialized after calling __attribute__((constructor)) function,
they must be defined as static variables in helper functions
if their methods should be called in __attribute__((constructor)) function.
Be careful to use static variables of classes in __attribute__((constructor)) function.
__attribute__((destructor)) function is called before destroying the global variables.

Other global variables can be defined normally.
*/

//in header file
#define DECLARE_SA_GLOBAL_VARIABLE(T, x)    \
	BEGIN_NOINLINE T & __gv_get_##x () throw() END_NOINLINE ;

//in cpp file
#define BEGIN_SA_GLOBAL_VARIABLE(T, x)    \
	T & __gv_get_##x () throw() { static T l_##x

#define BEGIN_SA_GLOBAL_VARIABLE_CONSTRUCTOR()         (
#define SA_GLOBAL_VARIABLE_CONSTRUCTOR_PARAM(z)        z,
#define SA_GLOBAL_VARIABLE_CONSTRUCTOR_PARAM_LAST(z)   z
#define END_SA_GLOBAL_VARIABLE_CONSTRUCTOR()           )

#define END_SA_GLOBAL_VARIABLE(x)    ;  return l_##x;  }

//in other files
#define GET_SA_GLOBAL_VARIABLE(x)    __gv_get_##x ()

////////////////////////////////////////////////////////////////////////////////
