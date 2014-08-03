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
This file contains shared-object-header-block pool functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "gkc_sys.h"

////////////////////////////////////////////////////////////////////////////////

//global variables
GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>  g_spbMgr;
GKC::Mutex  g_spbMutex;

//functions
GKC::SharedPtrBlock* SpbPool_Allocate() throw()
{
	GKC::SharedPtrBlock* p = NULL;
	uintptr _p = 0;
	try {
		GKC::SyncLock<GKC::Mutex> lock(g_spbMutex);
		p = (GKC::SharedPtrBlock*)g_spbMgr.Allocate(_p);
	}
	catch(...) {
		return NULL;
	}
	return p;
}

void SpbPool_Free(GKC::SharedPtrBlock* p) throw()
{
	if( p == NULL )
		return ;
	uintptr _p = (uintptr)p;
	try {
		GKC::SyncLock<GKC::Mutex> lock(g_spbMutex);
		g_spbMgr.Free(_p);
	}
	catch(...) {
	}
}

////////////////////////////////////////////////////////////////////////////////
