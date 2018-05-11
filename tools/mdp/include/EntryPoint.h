/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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

// tools
inline
bool _CheckFileExtension(const ConstStringS& str, const ConstStringS& strExt, uintptr& uPos) throw()
{
	if( !FsPathHelper::FindExtensionStart(str, uPos) )
		return false;
	return ConstStringCompareTrait<ConstStringS>::IsEQ(
		ConstStringS(ConstArrayHelper::GetInternalPointer(str) + uPos, str.GetCount() - uPos),
		strExt);
}

//process project file
inline
int _Cmd_ProcessProjectFile(const ConstArray<ConstStringS>& args, int type)
{
	uintptr uArgCount = args.GetCount();

	if( uArgCount > 4 ) {
		_PrintVersion();
		_PrintHelp();
		return 1;
	}

	StringS strSrc(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	StringUtilHelper::MakeString(args[2].get_Value(), strSrc);
	StringS strDest(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	{
		uintptr uPos;
		//source
		if( !_CheckFileExtension(StringUtilHelper::To_ConstString(strSrc), DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".mdp")), uPos) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The project file name must have the extension \".mdp\"!")));
			return 1;
		}
		//destination
		if( uArgCount == 4 ) {
			StringUtilHelper::MakeString(args[3].get_Value(), strDest);
		}
		else {
			if( !FileManagementHelper::GetCurrentDirectory(strDest) ) {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The current directory cannot be obtained!")));
				return 1;
			}
			FsPathHelper::AppendSeparator(strDest);
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("mdb-output")), strDest);
		}
	} //end block

	//directory/file names
	FsPathHelper::ConvertPathStringToPlatform(strSrc);
	FsPathHelper::ConvertPathStringToPlatform(strDest);

	//create directory
	if( !FileManagementHelper::ForceDirectory(strDest) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The destination directory cannot be created!")));
		return 1;
	}

	_PrintVersion();

	//process
	return _Process_Project_File(strSrc, strDest, type) ? 0 : 2;
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

		int type = MDP_TYPE_CHM;
		//-m
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-m"))) ) {
			type = MDP_TYPE_CHM;
		}
		//-e
		else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-e"))) ) {
			type = MDP_TYPE_EPUB;
		}
		else {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid parameters!")));
			return 1;
		}

		return _Cmd_ProcessProjectFile(args, type);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
