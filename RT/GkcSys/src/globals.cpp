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
This file contains global variables.
*/

////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////

#include "base/GkcDef.cpp"
#include "base/GkcBase.cpp"

//------------------------------------------------------------------------------
//global variables

//SPB
BEGIN_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>, spb_mgr)
END_SA_GLOBAL_VARIABLE(spb_mgr)

BEGIN_SA_GLOBAL_VARIABLE(GKC::Mutex, spb_mutex)
END_SA_GLOBAL_VARIABLE(spb_mutex)

//SAB
BEGIN_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedArrayBlock)>, sab_mgr)
END_SA_GLOBAL_VARIABLE(sab_mgr)

BEGIN_SA_GLOBAL_VARIABLE(GKC::Mutex, sab_mutex)
END_SA_GLOBAL_VARIABLE(sab_mutex)

//------------------------------------------------------------------------------
//functions

bool init_globals() throw()
{
	GKC::RefPtr<GKC::IMemoryManager> mgr(GKC::MemoryHelper::GetCrtMemoryManager());
	GKC::CallResult cr;

	//spb
	cr = GET_SA_GLOBAL_VARIABLE(spb_mutex).Init();
	if( cr.IsFailed() )
		return false;
	GET_SA_GLOBAL_VARIABLE(spb_mgr).SetMemoryManager(mgr);

	//sab
	cr = GET_SA_GLOBAL_VARIABLE(sab_mutex).Init();
	if( cr.IsFailed() ) {
		GET_SA_GLOBAL_VARIABLE(spb_mutex).Term();
		return false;
	}
	GET_SA_GLOBAL_VARIABLE(sab_mgr).SetMemoryManager(mgr);

	return true;
}

void dump_globals() throw()
{
	//sab
	GET_SA_GLOBAL_VARIABLE(sab_mutex).Term();
	//spb
	GET_SA_GLOBAL_VARIABLE(spb_mutex).Term();
}

////////////////////////////////////////////////////////////////////////////////
