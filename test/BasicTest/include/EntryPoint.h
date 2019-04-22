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

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const ConstArray<ConstStringS>& args, const ConstArray<ConstStringS>& env)
	{
		//const strings
		DECLARE_LOCAL_CONST_STRING(CharS, c_szSep, c_uSepLen, _S("======================="))

		//output argments
		ConsoleHelper::WriteLine(ConstStringS(c_szSep, c_uSepLen));
		for( auto iter(args.GetBegin()); iter != args.GetEnd(); iter.MoveNext() ) {
			ConsoleHelper::WriteLine(iter.get_Value());
		}
		//output environments
		ConsoleHelper::WriteLine(ConstStringS(c_szSep, c_uSepLen));
		for( auto iter(env.GetBegin()); iter != env.GetEnd(); iter.MoveNext() ) {
			ConsoleHelper::WriteLine(iter.get_Value());
		}

		//unit test
		UNIT_TEST_MAIN_PROCESS(args)
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
