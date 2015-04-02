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

#include "parser/SourceAnalyzer.h"

////////////////////////////////////////////////////////////////////////////////

// help
static
void _help() throw()
{
	static const CharS* l_szHelp[] = {
		_S("Usage : wlc [options] source-dir-or-file [destination dir]"),
		NULL
	};
	const CharS** pp = l_szHelp;
	while( (*pp) != NULL ) {
		print_string(*pp);
		++ pp;
	}
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const GKC::ConstArray<GKC::ConstStringS>& args, const GKC::ConstArray<GKC::ConstStringS>& env)
	{
		//args
		if( args.GetCount() <= 1 ) {
			_help();
			return 0;
		}

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
