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
This file contains entry point function.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTRY_POINT_H__
#define __ENTRY_POINT_H__
////////////////////////////////////////////////////////////////////////////////

#include "Global.h"

#include "ProcessFiles.h"

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
		if( uArgCount != 3 ) {
			_PrintVersion();
			_PrintHelp();
			return 0;
		}
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), args[2].get_Value()) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source directory and the destination directory cannot be the same!\n")));
			return 0;
		}
		StringS strSrc(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
		StringUtilHelper::MakeString(args[1].get_Value(), strSrc);
		StringS strDest(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
		StringUtilHelper::MakeString(args[2].get_Value(), strDest);
		//directory name
		FsPathHelper::ConvertPathStringToPlatform(strSrc);
		FsPathHelper::ConvertPathStringToPlatform(strDest);

		//process
		_PrintVersion();
		ProcessFiles(strSrc, strDest);

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
