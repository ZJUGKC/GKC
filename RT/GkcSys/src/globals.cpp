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

#include "GkcDef.cpp"
#include "GkcBase.cpp"

//functions

bool init_globals() throw()
{
	GKC::RefPtr<GKC::IMemoryManager> mgr(GKC::MemoryHelper::GetCrtMemoryManager());
	GKC::CallResult cr;

	//spb
	cr = g_spbMutex.Init();
	if( cr.IsFailed() )
		return false;
	g_spbMgr.SetMemoryManager(mgr);

	//sab
	cr = g_sabMutex.Init();
	if( cr.IsFailed() ) {
		g_spbMutex.Term();
		return false;
	}
	g_sabMgr.SetMemoryManager(mgr);

	return true;
}

void dump_globals() throw()
{
	//sab
	g_sabMutex.Term();
	//spb
	g_spbMutex.Term();
}

////////////////////////////////////////////////////////////////////////////////
