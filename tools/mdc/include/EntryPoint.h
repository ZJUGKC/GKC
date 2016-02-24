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

#include "global.h"

#include "process.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// version
inline
void print_version() throw()
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(_const_array_version::GetAddress(), _const_array_version::GetCount()));
}

// help
inline
void print_help() throw()
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(_const_array_help::GetAddress(), _const_array_help::GetCount()));
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
			print_version();
			print_help();
			return 0;
		}
		if( compare_string(ConstArrayHelper::GetInternalPointer(args[1].get_Value()), ConstArrayHelper::GetInternalPointer(args[2].get_Value())) == 0 ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source directory and the destination directory cannot be the same!\n")));
			return 0;
		}
		StringS strSrc(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
		StringUtilHelper::MakeString(args[1].get_Value(), strSrc);
		StringS strDest(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
		StringUtilHelper::MakeString(args[2].get_Value(), strDest);
		//file name
		FsPathHelper::ConvertPathStringToPlatform(strSrc);
		FsPathHelper::ConvertPathStringToPlatform(strDest);

		//process
		print_version();
		process_md(strSrc, strDest);

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
