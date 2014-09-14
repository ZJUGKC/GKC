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

// SharedPtrBlock
DECLARE_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>, spb_mgr)
DECLARE_SA_GLOBAL_VARIABLE(GKC::Mutex, spb_mutex)

// SharedArrayBlock
DECLARE_SA_GLOBAL_VARIABLE(GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>, sab_mgr)
DECLARE_SA_GLOBAL_VARIABLE(GKC::Mutex, sab_mutex)

//functions

bool init_globals() throw();
void dump_globals() throw();

////////////////////////////////////////////////////////////////////////////////
