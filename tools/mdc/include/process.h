/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains a process function.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MDC_PROCESS_H__
#define __MDC_PROCESS_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//process

inline void process_md(const StringS& strSrc, const StringS& strDest)
{
	StringS strCurrentPath(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	StringS strFile(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));

	List<StringS> stack(MemoryHelper::GetCrtMemoryManager());
	while( stack.GetCount() > 0 ) {

	} //end while
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
