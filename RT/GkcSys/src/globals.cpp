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

#include "PreComp.h"

#include "Globals.h"

////////////////////////////////////////////////////////////////////////////////

#include "base/GkcDef.cpp"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//global variables

//CrtMemoryManager
BEGIN_SA_GLOBAL_VARIABLE(CrtMemoryManager, g_crt_mem_mgr)
END_SA_GLOBAL_VARIABLE(g_crt_mem_mgr)

//Mutex
BEGIN_SA_GLOBAL_VARIABLE(Mutex, g_mutex)
END_SA_GLOBAL_VARIABLE(g_mutex)

//SPB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_ptr_block)>, g_spb_pool)
END_SA_GLOBAL_VARIABLE(g_spb_pool)

//SAB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_array_block)>, g_sab_pool)
END_SA_GLOBAL_VARIABLE(g_sab_pool)

//SCB
BEGIN_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_com_block)>, g_scb_pool)
END_SA_GLOBAL_VARIABLE(g_scb_pool)

//------------------------------------------------------------------------------
//functions

bool _InitGlobals() throw()
{
	CallResult cr;

	//mutex
	cr = GET_SA_GLOBAL_VARIABLE(g_mutex).Init();
	if( cr.IsFailed() )
		return false;

	//crt
	IMemoryManager* pMgr = get_crt_mem_mgr();

	//spb
	GET_SA_GLOBAL_VARIABLE(g_spb_pool).SetMemoryManager(pMgr);

	//sab
	GET_SA_GLOBAL_VARIABLE(g_sab_pool).SetMemoryManager(pMgr);

	//scb
	GET_SA_GLOBAL_VARIABLE(g_scb_pool).SetMemoryManager(pMgr);

	return true;
}

void _DumpGlobals() throw()
{
	//no destructions
	// scb
	GET_SA_GLOBAL_VARIABLE(g_scb_pool).Clear();
	// sab
	GET_SA_GLOBAL_VARIABLE(g_sab_pool).Clear();
	// spb
	GET_SA_GLOBAL_VARIABLE(g_spb_pool).Clear();

	//mutex
	GET_SA_GLOBAL_VARIABLE(g_mutex).Term();
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
