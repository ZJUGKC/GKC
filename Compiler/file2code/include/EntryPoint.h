/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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

// process file
inline
int _Cmd_ProcessFile(const ConstArray<ConstStringS>& args, bool bCpp)
{
	uintptr uPos = FsPathHelper::FindFilePartStart(args[3]);
	//check length of file part
	if ( uPos == args[3].GetCount() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The destination file name must be a C variable name!")));
		return 1;
	}
	//check name
	auto iter(args[3].GetBegin());
	iter.MoveDelta(uPos);
	for ( ; iter != args[3].GetEnd(); iter.MoveNext() ) {
		CharS ch = iter.get_Value();
		if ( !char_is_lower(ch) && !char_is_upper(ch) && !char_is_digit(ch) && ch != '_' ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The destination file name must be a C variable name!")));
			return 1;
		}
	}
	//process
	return _Process_File(args[2], args[3],
						ConstStringS(ConstArrayHelper::GetInternalPointer(args[3]) + uPos, args[3].GetCount() - uPos),
						bCpp) ? 0 : 2;
}

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static int ConsoleMain(const ConstArray<ConstStringS>& args, const ConstArray<ConstStringS>& env)
	{
		uintptr uArgCount = args.GetCount();

		//args
		if( uArgCount != 4 ) {
			_PrintVersion();
			_PrintHelp();
			return 1;
		}

		bool bCpp;
		//-c
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-c"))) ) {
			bCpp = false;
		}
		//-cpp
		else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-cpp"))) ) {
			bCpp = true;
		}
		else {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid parameters!")));
			return 1;
		}

		return _Cmd_ProcessFile(args, bCpp);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
