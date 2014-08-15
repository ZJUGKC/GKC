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
extern GKC::PoolMemoryManager<sizeof(GKC::SharedPtrBlock)>  g_spbMgr;
extern GKC::Mutex  g_spbMutex;

// SharedArrayBlock
extern GKC::PoolMemoryManager<sizeof(GKC::SharedArrayBlock)>  g_sabMgr;
extern GKC::Mutex  g_sabMutex;

//functions

bool init_globals() throw();
void dump_globals() throw();

////////////////////////////////////////////////////////////////////////////////
