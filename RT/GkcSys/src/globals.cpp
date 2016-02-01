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

//------------------------------------------------------------------------------
//global variables

//CrtMemoryManager
BEGIN_SA_GLOBAL_VARIABLE(CrtMemoryManager, crt_mem_mgr)
END_SA_GLOBAL_VARIABLE(crt_mem_mgr)

//Mutex
BEGIN_SA_GLOBAL_VARIABLE(GKC::Mutex, g_mutex)
END_SA_GLOBAL_VARIABLE(g_mutex)

//SPB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_ptr_block)>, spb_pool)
END_SA_GLOBAL_VARIABLE(spb_pool)

//SAB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_array_block)>, sab_pool)
END_SA_GLOBAL_VARIABLE(sab_pool)

//SCB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_com_block)>, scb_pool)
END_SA_GLOBAL_VARIABLE(scb_pool)

//------------------------------------------------------------------------------
//functions

bool init_globals() throw()
{
	GKC::CallResult cr;

	//mutex
	cr = GET_SA_GLOBAL_VARIABLE(g_mutex).Init();
	if( cr.IsFailed() )
		return false;

	//crt
	GKC::IMemoryManager* pMgr = get_crt_mem_mgr();

	//spb
	GET_SA_GLOBAL_VARIABLE(spb_pool).SetMemoryManager(pMgr);

	//sab
	GET_SA_GLOBAL_VARIABLE(sab_pool).SetMemoryManager(pMgr);

	//scb
	GET_SA_GLOBAL_VARIABLE(scb_pool).SetMemoryManager(pMgr);

	return true;
}

void dump_globals() throw()
{
	//no destructions
	// scb
	GET_SA_GLOBAL_VARIABLE(scb_pool).Clear();
	// sab
	GET_SA_GLOBAL_VARIABLE(sab_pool).Clear();
	// spb
	GET_SA_GLOBAL_VARIABLE(spb_pool).Clear();

	//mutex
	GET_SA_GLOBAL_VARIABLE(g_mutex).Term();
}

////////////////////////////////////////////////////////////////////////////////
