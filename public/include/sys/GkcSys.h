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

/*
This file contains GkcSys helper classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_SYS_H__
#define __GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////

#include "gkc_sys.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// SharedPtrBlockHelper

class SharedPtrBlockHelper
{
public:
	static SharedPtrBlock* Allocate() throw()
	{
		return ::SpbPool_Allocate();
	}
	static void Free(SharedPtrBlock* p) throw()
	{
		::SpbPool_Free(p);
	}
};

// SharedArrayBlockHelper

class SharedArrayBlockHelper
{
public:
	static SharedArrayBlock* Allocate() throw()
	{
		return ::SabPool_Allocate();
	}
	static void Free(SharedArrayBlock* p) throw()
	{
		::SabPool_Free(p);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
