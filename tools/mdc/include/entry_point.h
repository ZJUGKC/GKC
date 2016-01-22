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

// version
inline
void _print_version() throw()
{
	GKC::ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(GKC::_const_array_version::GetAddress(), GKC::_const_array_version::GetCount()));
}

// help
inline
void _print_help() throw()
{
	GKC::ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(GKC::_const_array_help::GetAddress(), GKC::_const_array_help::GetCount()));
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const GKC::ConstArray<GKC::ConstStringS>& args, const GKC::ConstArray<GKC::ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();
		//args
		if( uArgCount != 3 ) {
			_print_version();
			_print_help();
			return 0;
		}
		if( compare_string(GKC::ConstArrayHelper::GetInternalPointer(args[1].get_Value()), GKC::ConstArrayHelper::GetInternalPointer(args[2].get_Value())) == 0 ) {
			GKC::ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(GKC::ConstStringS, _S("Error: The source directory and the destination directory cannot be the same!\n")));
			return 0;
		}
		GKC::StringS strSrc(GKC::StringUtilHelper::MakeEmptyString<CharS>(GKC::MemoryHelper::GetCrtMemoryManager()));
		GKC::StringUtilHelper::MakeString(args[1].get_Value(), strSrc);
		GKC::StringS strDest(GKC::StringUtilHelper::MakeEmptyString<CharS>(GKC::MemoryHelper::GetCrtMemoryManager()));
		GKC::StringUtilHelper::MakeString(args[2].get_Value(), strDest);
		//file name
		GKC::FsPathHelper::ConvertPathStringToPlatform(strSrc);
		GKC::FsPathHelper::ConvertPathStringToPlatform(strDest);

		//process
		_print_version();
		GKC::process_md(strSrc, strDest);

		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
