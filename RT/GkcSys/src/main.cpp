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
This file contains main functions for SA.
*/

////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static bool SAMain(bool bInitOrDump)
	{
		if( bInitOrDump ) {
			//init
			GKC::CallResult cr;
			//spb
			cr = g_spbMutex.Init();
			if( cr.IsFailed() )
				return false;
			GKC::RefPtr<GKC::IMemoryManager> mgr(GKC::MemoryHelper::GetCrtMemoryManager());
			g_spbMgr.SetMemoryManager(mgr);
		}
		else {
			//dump
		}
		return true;
	}
};

#include "GkcSAMain.cpp"

////////////////////////////////////////////////////////////////////////////////
