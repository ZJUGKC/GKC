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
#ifndef __ENTRY_H__
#define __ENTRY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const GKC::ConstArray<GKC::ConstStringS>& args, const GKC::ConstArray<GKC::ConstStringS>& env)
	{
		//const strings
		DECLARE_LOCAL_CONST_STRING(CharS, c_szSep, c_iSepLen, _S("======================="))

		//output argments
		GKC::Console::WriteLine(GKC::ConstStringS(c_szSep, c_iSepLen));
		for( auto iter = args.GetBegin(); iter != args.GetEnd(); iter.MoveNext() ) {
			GKC::Console::WriteLine(iter.get_Value());
		}
		//output environments
		GKC::Console::WriteLine(GKC::ConstStringS(c_szSep, c_iSepLen));
		for( auto iter = env.GetBegin(); iter != env.GetEnd(); iter.MoveNext() ) {
			GKC::Console::WriteLine(iter.get_Value());
		}

		//unit test
		UNIT_TEST_MAIN_PROCESS(args);

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
