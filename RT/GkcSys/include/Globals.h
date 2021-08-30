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
#ifndef __GLOBALS_H__
#define __GLOBALS_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CrtMemoryManager

class CrtMemoryManager : public IMemoryManager
{
public:
	// IMemoryManager methods
	virtual uintptr Allocate(const uintptr& uBytes) throw()
	{
		return (uintptr)crt_alloc(uBytes);
	}
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw()
	{
		return (uintptr)crt_realloc((void*)p, uBytes);
	}
	virtual void    Free(const uintptr& p) throw()
	{
		crt_free((void*)p);
	}
};

// CrtMemoryManager
DECLARE_SA_GLOBAL_VARIABLE(CrtMemoryManager, g_crt_mem_mgr)

inline IMemoryManager* get_crt_mem_mgr() throw()
{
	return static_cast<IMemoryManager*>(&(GET_SA_GLOBAL_VARIABLE(g_crt_mem_mgr)));
}

// Mutex
DECLARE_SA_GLOBAL_VARIABLE(Mutex, g_mutex)

inline Mutex* get_mem_mutex() throw()
{
	return &(GET_SA_GLOBAL_VARIABLE(g_mutex));
}

// share_ptr_block
DECLARE_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_ptr_block)>, g_spb_pool)

// share_array_block
DECLARE_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_array_block)>, g_sab_pool)

// share_com_block
DECLARE_SA_GLOBAL_VARIABLE(fixed_size_memory_pool<sizeof(share_com_block)>, g_scb_pool)

// mutex for Com_SA_Cache
DECLARE_SA_GLOBAL_VARIABLE(Mutex, g_ca_mutex)

// Com_SA_Cache
DECLARE_SA_GLOBAL_VARIABLE(Com_SA_Cache, g_com_sa_cache)

//functions

bool _InitGlobals() throw();
void _DumpGlobals() throw();

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
