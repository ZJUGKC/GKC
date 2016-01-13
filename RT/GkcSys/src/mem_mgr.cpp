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

#include "precomp.h"

#include "gkc_sys.h"

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////

//functions

//CrtMemoryManager

SA_FUNCTION GKC::IMemoryManager* CrtMemoryManager_Get() throw()
{
	return get_crt_mem_mgr();
}

////////////////////////////////////////////////////////////////////////////////
