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
This file contains GkcSys functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SA_GKC_SYS_H__
#define __SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error gkc_sys.h requires GkcBase.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// SharedPtrBlock Pool

SA_FUNCTION GKC::SharedPtrBlock* SpbPool_Allocate() throw();
SA_FUNCTION void SpbPool_Free(GKC::SharedPtrBlock* p) throw();

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
