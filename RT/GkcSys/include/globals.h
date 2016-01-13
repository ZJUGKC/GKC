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

// CrtMemoryManager

class CrtMemoryManager : public GKC::IMemoryManager
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
DECLARE_SA_GLOBAL_VARIABLE(CrtMemoryManager, crt_mem_mgr)

inline GKC::IMemoryManager* get_crt_mem_mgr() throw()
{
	return static_cast<GKC::IMemoryManager*>(&(GET_SA_GLOBAL_VARIABLE(crt_mem_mgr)));
}

// SharedPtrBlock
DECLARE_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>, spb_mgr)
DECLARE_SA_GLOBAL_VARIABLE(GKC::Mutex, spb_mutex)

// SharedArrayBlock
DECLARE_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedArrayBlock)>, sab_mgr)
DECLARE_SA_GLOBAL_VARIABLE(GKC::Mutex, sab_mutex)

//functions

bool init_globals() throw();
void dump_globals() throw();

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
