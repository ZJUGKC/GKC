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

////////////////////////////////////////////////////////////////////////////////

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(GKC::ConstArray<GKC::ConstStringS>& args, GKC::ConstArray<GKC::ConstStringS>& env)
	{
		//output argments
		GKC::Console::WriteLine(GKC::ConstStringS(_S("======================="), 0));
		for( auto iter = args.GetBegin(); iter != args.GetEnd(); iter.MoveNext() ) {
			GKC::Console::WriteLine(iter.get_Value());
		}
		//output environments
		GKC::Console::WriteLine(GKC::ConstStringS(_S("======================="), 0));
		for( auto iter = env.GetBegin(); iter != env.GetEnd(); iter.MoveNext() ) {
			GKC::Console::WriteLine(iter.get_Value());
		}

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
