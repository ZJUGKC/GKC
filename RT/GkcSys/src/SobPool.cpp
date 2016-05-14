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

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

#include "Globals.h"

////////////////////////////////////////////////////////////////////////////////

//internal

template <class TElement>
inline TElement* _PoolAllocate(fixed_size_memory_pool<sizeof(TElement)>& pool) throw()
{
	TElement* p = NULL;
	try {
		GKC::MutexLock lock(GKC::GET_SA_GLOBAL_VARIABLE(g_mutex));
		p = (TElement*)pool.Allocate();
	}
	catch(...) {
		p = NULL;
	}
	//constructor
	if( p != NULL )
		call_constructor(*p);  //no throw
	return p;
}
template <class TElement>
inline void _PoolFree(fixed_size_memory_pool<sizeof(TElement)>& pool, TElement* p) throw()
{
	if( p == NULL )
		return ;
	//destructor
	p->~TElement();
	{
		GKC::MutexLock lock(GKC::GET_SA_GLOBAL_VARIABLE(g_mutex));
		pool.Free(p);
	} //end block
}

//functions

//SPB

share_ptr_block* _SpbPool_Allocate() throw()
{
	return _PoolAllocate<share_ptr_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_spb_pool));
}
void _SpbPool_Free(share_ptr_block* p) throw()
{
	_PoolFree<share_ptr_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_spb_pool), p);
}

//SAB

share_array_block* _SabPool_Allocate() throw()
{
	return _PoolAllocate<share_array_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_sab_pool));
}
void _SabPool_Free(share_array_block* p) throw()
{
	_PoolFree<share_array_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_sab_pool), p);
}

//SCB

share_com_block* _ScbPool_Allocate() throw()
{
	return _PoolAllocate<share_com_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_scb_pool));
}
void _ScbPool_Free(share_com_block* p) throw()
{
	_PoolFree<share_com_block>(GKC::GET_SA_GLOBAL_VARIABLE(g_scb_pool), p);
}

////////////////////////////////////////////////////////////////////////////////
