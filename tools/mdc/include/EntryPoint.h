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

//compile single file
inline
int _Cmd_CompileSingleFile(const ConstArray<ConstStringS>& args)
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
		if( !_CheckFileExtension(StringUtilHelper::To_ConstString(strSrc), DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".md")), uPos) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The source file name must have the extension \".md\"!")));
			return 1;
		}
		//destination
		if( uArgCount == 4 ) {
			//extension
			ConstStringS c_strDest(args[3].get_Value());
			if( !_CheckFileExtension(c_strDest, DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".htm")), uPos)
				&& !_CheckFileExtension(c_strDest, DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".html")), uPos)
				) {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The destination file name must have the extension \".htm\" or \".html\"!")));
				return 1;
			}
			StringUtilHelper::MakeString(c_strDest, strDest);
		}
		else {
			strDest = StringHelper::Clone(strSrc);
			FsPathHelper::RemoveExtension(uPos, strDest);
			StringUtilHelper::Insert(uPos, DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".html")), strDest);
		}
	} //end block

	//file names
	FsPathHelper::ConvertPathStringToPlatform(strSrc);
	FsPathHelper::ConvertPathStringToPlatform(strDest);

	_PrintVersion();

	//process
	return _Compile_Single_File(strSrc, strDest) ? 0 : 2;
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

		int ret = 0;
		//-c
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1].get_Value(), DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-c"))) ) {
			ret = _Cmd_CompileSingleFile(args);
		}
		else {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid parameters!")));
			return 1;
		}

		return ret;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
