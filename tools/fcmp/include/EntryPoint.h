/*
** Copyright (c) 2022, Xin YUAN, courses of Zhejiang University
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
void _PrintVersion() noexcept
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(g_const_array_version::GetAddress(), g_const_array_version::GetCount()));
}

// help
inline
void _PrintHelp() noexcept
{
	ConsoleHelper::PrintConstStringArray(DECLARE_CONST_STRING_ARRAY_TYPE(CharS)(g_const_array_help::GetAddress(), g_const_array_help::GetCount()));
}

// same file
inline
bool _Check_Same_File(const ConstStringS& str1, const ConstStringS& str2) noexcept
{
	if( ConstStringCompareTrait<ConstStringS>::IsEQ(str1, str2) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Two files can not be the same!")));
		return true;
	}
	return false;
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const ConstArray<ConstStringS>& args, const ConstArray<ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();

		//args
		if ( uArgCount < 3 || uArgCount > 4 ) {
			_PrintVersion();
			_PrintHelp();
			return 1;
		}

		const ConstStringS* pstr1 = &args[1];
		const ConstStringS* pstr2 = &args[2];
		bool  bCaseIgnore = false;

		//-i
		if ( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-i"))) ) {
			if ( uArgCount != 4 ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
			}
			pstr1 = &args[2];
			pstr2 = &args[3];
			bCaseIgnore = true;
		}
		else {
			if ( uArgCount != 3 ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
			}
		}

		//check
		if ( _Check_Same_File(*pstr1, *pstr2) ) {
			return 1;
		}

		//process
		return _Process_File(*pstr1, *pstr2, bCaseIgnore);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
