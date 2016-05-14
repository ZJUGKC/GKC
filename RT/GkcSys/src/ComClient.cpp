/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains component client functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

#include "Globals.h"

////////////////////////////////////////////////////////////////////////////////

// functions

void _Com_SA_GetClassObject(const _StringS& strAssembly, const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw()
{
	cr = GKC::GET_SA_GLOBAL_VARIABLE(g_com_sa_cache).GetClassObject(strAssembly, cid, sp);
}

void _Com_SA_FreeUnusedLibraries() throw()
{
	GKC::GET_SA_GLOBAL_VARIABLE(g_com_sa_cache).FreeUnusedLibraries();
}

////////////////////////////////////////////////////////////////////////////////
