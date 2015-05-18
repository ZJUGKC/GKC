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
This file contains entry point function.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTRY_POINT_H__
#define __ENTRY_POINT_H__
////////////////////////////////////////////////////////////////////////////////

#include "cmd/compile_single_file.h"

////////////////////////////////////////////////////////////////////////////////

// help
static
void _help() throw()
{
	static const CharS* l_szHelp[] = {
		_S("wlc version : 1.0.1.1001"),
		_S("  Usage : wlc options source-dir-or-file [destination dir]"),
		_S("  options: -c compile the source file."),
		NULL
	};
	const CharS** pp = l_szHelp;
	while( (*pp) != NULL ) {
		print_string(*pp);
		print_string(_S("\n"));
		++ pp;
	}
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const GKC::ConstArray<GKC::ConstStringS>& args, const GKC::ConstArray<GKC::ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();
		//args
		if( uArgCount <= 2 ) {
			_help();
			return 0;
		}
		int ret = 0;
		//-c
		if( compare_string(GKC::ConstHelper::GetInternalPointer(args[1].get_Value()), _S("-c")) ) {
			if( uArgCount > 4 ) {
				_help();
				return 0;
			}
			GKC::StringS strSrc(GKC::StringUtilHelper::MakeEmptyString<CharS>(GKC::MemoryHelper::GetCrtMemoryManager()));
			GKC::StringUtilHelper::MakeString(args[2].get_Value(), strSrc);
			GKC::StringS strDest(GKC::StringUtilHelper::MakeEmptyString<CharS>(GKC::MemoryHelper::GetCrtMemoryManager()));
			if( uArgCount == 4 ) {
				GKC::StringUtilHelper::MakeString(args[3].get_Value(), strDest);
			}
			//file name

			GKC::FsPathHelper::ConvertPathStringToPlatform(strSrc);
			GKC::FsPathHelper::ConvertPathStringToPlatform(strDest);
			ret = GKC::compile_single_file(strSrc, strDest);
		}

		return ret;
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
