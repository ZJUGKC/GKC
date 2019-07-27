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

//process project file
inline
int _Cmd_ProcessProjectFile(const ConstStringS& cstrSrc, const ConstStringS& cstrDest, int type, bool bLatest)
{
	StringS strSrc(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	StringUtilHelper::MakeString(cstrSrc, strSrc);
	StringS strSrcDir(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	StringUtilHelper::MakeString(cstrSrc, strSrcDir);
	StringS strDest(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));
	{
		uintptr uPos;
		//source
		if( !_CheckFileExtension(StringUtilHelper::To_ConstString(strSrc), DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".mdp")), uPos) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The project file name must have the extension \".mdp\"!")));
			return 1;
		}
		//source directory
		uPos = FsPathHelper::FindFilePartStart(StringUtilHelper::To_ConstString(strSrcDir));
		FsPathHelper::ToPathPart(uPos, strSrcDir);
		if( strSrcDir.IsEmpty() )
			StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("./")), strSrcDir);
		//destination
		if( !cstrDest.IsNull() ) {
			StringUtilHelper::MakeString(cstrDest, strDest);
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
	FsPathHelper::AppendSeparator(strDest);
	if( type == MDP_TYPE_CHM )
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("chm")), strDest);
	else if( type == MDP_TYPE_EPUB )
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("epub")), strDest);
	else
		assert( false );

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
	return _Process_Project_File(strSrc, strSrcDir, strDest, type, bLatest) ? 0 : 2;
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
		uintptr uStartIndex = 2;
		//-m
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-m"))) ) {
			if( uArgCount > 4 ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
			}
			type = MDP_TYPE_CHM;
		}
		//-e
		else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-e"))) ) {
			//-l
			if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[2].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-l"))) )
				uStartIndex ++;
			if( (uStartIndex == 2 && uArgCount > 4) || (uStartIndex == 3 && (uArgCount == 3 || uArgCount > 5)) ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
			}
			type = MDP_TYPE_EPUB;
		}
		else {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid parameters!")));
			return 1;
		}

		return _Cmd_ProcessProjectFile(args[uStartIndex].get_Value(),
									(uStartIndex + 1 >= uArgCount) ? ConstStringS() : args[uStartIndex + 1].get_Value(),
									type, uStartIndex == 3);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
