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

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////

//functions

//SPB

GKC::SharedPtrBlock* SpbPool_Allocate() throw()
{
	GKC::SharedPtrBlock* p = NULL;
	try {
		GKC::SyncLock<GKC::Mutex> lock(GET_SA_GLOBAL_VARIABLE(spb_mutex));
		p = (GKC::SharedPtrBlock*)(GET_SA_GLOBAL_VARIABLE(spb_mgr).Allocate(0));
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
	try {
		GKC::SyncLock<GKC::Mutex> lock(GET_SA_GLOBAL_VARIABLE(spb_mutex));
		GET_SA_GLOBAL_VARIABLE(spb_mgr).Free((uintptr)p);
	}
	catch(...) {
	}
}

//SAB

GKC::SharedArrayBlock* SabPool_Allocate() throw()
{
	GKC::SharedArrayBlock* p = NULL;
	try {
		GKC::SyncLock<GKC::Mutex> lock(GET_SA_GLOBAL_VARIABLE(sab_mutex));
		p = (GKC::SharedArrayBlock*)(GET_SA_GLOBAL_VARIABLE(sab_mgr).Allocate(0));
	}
	catch(...) {
		return NULL;
	}
	return p;
}

void SabPool_Free(GKC::SharedArrayBlock* p) throw()
{
	if( p == NULL )
		return ;
	try {
		GKC::SyncLock<GKC::Mutex> lock(GET_SA_GLOBAL_VARIABLE(sab_mutex));
		GET_SA_GLOBAL_VARIABLE(sab_mgr).Free((uintptr)p);
	}
	catch(...) {
	}
}

////////////////////////////////////////////////////////////////////////////////
