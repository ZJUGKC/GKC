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

#include "Global.h"

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
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY(CharS)(g_const_array_help::GetAddress(), g_const_array_help::GetCount()));
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const ConstArray<ConstStringS>& args, const ConstArray<ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();
		//args
		if( uArgCount <= 2 ) {
			_PrintVersion();
			_PrintHelp();
			return 0;
		}
		int ret = 0;
		//-c
		if( ConstStringCompareTrait::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-c"))) ) {
			if( uArgCount > 4 ) {
				_PrintVersion();
				_PrintHelp();
				return 0;
			}
			StringS strSrc(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
			StringUtilHelper::MakeString(args[2].get_Value(), strSrc);
			StringS strDest(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
			if( uArgCount == 4 ) {
				StringUtilHelper::MakeString(args[3].get_Value(), strDest);
			}
			else {
				strDest = StringHelper::Clone(strSrc);
				uintptr uPos;
				if( FsPathHelper::FindExtensionStart(StringUtilHelper::To_ConstString(strDest), uPos) ) {
					StringHelper::Delete(uPos, strDest.GetLength() - uPos, strDest);
					StringUtilHelper::Insert(uPos, DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".wo")), strDest);
				}
				else {
					StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".wo")), strDest);
				}
			}
			//file name
			FsPathHelper::ConvertPathStringToPlatform(strSrc);
			FsPathHelper::ConvertPathStringToPlatform(strDest);

			//process
			_PrintVersion();
			ret = compile_single_file(strSrc, strDest);
		}

		return ret;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
