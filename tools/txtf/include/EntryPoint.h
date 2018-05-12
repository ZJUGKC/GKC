/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// version
inline
void _PrintVersion() throw()
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(g_const_array_version::GetAddress(), g_const_array_version::GetCount()));
}

// help
inline
void _PrintHelp() throw()
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(g_const_array_help::GetAddress(), g_const_array_help::GetCount()));
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const ConstArray<ConstStringS>& args, const ConstArray<ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();

		//args
		if( uArgCount <= 3 ) {
			_PrintVersion();
			_PrintHelp();
			return 1;
		}

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
