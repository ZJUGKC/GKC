/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains memory manager functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

#include "ComSACache.h"
#include "Globals.h"

////////////////////////////////////////////////////////////////////////////////

//functions

//CrtMemoryManager

GKC::IMemoryManager* _CrtMemoryManager_Get() throw()
{
	return GKC::get_crt_mem_mgr();
}

//Mutex

GKC::Mutex* _MemoryMutex_Get() throw()
{
	return GKC::get_mem_mutex();
}

////////////////////////////////////////////////////////////////////////////////
