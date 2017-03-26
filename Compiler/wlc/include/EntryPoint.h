﻿/*
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
		if( uArgCount <= 2 ) {
			_PrintVersion();
			_PrintHelp();
			return 1;
		}
		bool ret = true;
		//-c
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-c"))) ) {
			if( uArgCount > 4 ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
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

			_PrintVersion();

			//process
			ret = _Compile_Single_File(strSrc, strDest);
		}

		return ret ? 0 : 2;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
