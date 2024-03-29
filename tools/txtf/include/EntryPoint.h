﻿/*
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

//same file
inline bool _Check_Same_File(const ConstStringS& str1, const ConstStringS& str2) throw()
{
	if( ConstStringCompareTrait<ConstStringS>::IsEQ(str1, str2) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: The source file and the destination file can not be the same!")));
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
		if( uArgCount <= 3 ) {
			_PrintVersion();
			_PrintHelp();
			return 1;
		}

		int ret = 0;

		//-bom
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-bom"))) ) {
			//-d
			if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[2], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-d"))) ) {
				if( uArgCount != 5 ) {
					_PrintVersion();
					_PrintHelp();
					return 1;
				}
				if( _Check_Same_File(args[3], args[4]) ) {
					return 1;
				}
				ret = _Cmd_Bom_Process(BOMTypes::None, args[3], args[4]);
			}
			//-a
			else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[2], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-a"))) ) {
				if( uArgCount != 6 ) {
					_PrintVersion();
					_PrintHelp();
					return 1;
				}
				int iBomType = BOMTypes::None;
				if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF8"))) ) {
					iBomType = BOMTypes::UTF8;
				}
				else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF16LE"))) ) {
					iBomType = BOMTypes::UTF16LE;
				}
				else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF16BE"))) ) {
					iBomType = BOMTypes::UTF16BE;
				}
				else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF32LE"))) ) {
					iBomType = BOMTypes::UTF32LE;
				}
				else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF32BE"))) ) {
					iBomType = BOMTypes::UTF32BE;
				}
				else {
					ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid BOM Type!")));
					return 1;
				}
				if( _Check_Same_File(args[4], args[5]) ) {
					return 1;
				}
				ret = _Cmd_Bom_Process(iBomType, args[4], args[5]);
			}
			else {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid BOM parameters!")));
				return 1;
			}
		}
		//-html
		else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-html"))) ) {
			//-g
			if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[2], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-g"))) ) {
				bool bRTL = false;
				uintptr uStartIndex = 3;
				//-rtl
				if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-rtl"))) ) {
					bRTL = true;
					uStartIndex ++;
				}
				if( uArgCount != uStartIndex + 3 ) {
					_PrintVersion();
					_PrintHelp();
					return 1;
				}
				if( _Check_Same_File(args[uStartIndex + 1], args[uStartIndex + 2]) ) {
					return 1;
				}
				ret = _Cmd_Generate_GitHub_Html(bRTL, args[uStartIndex], args[uStartIndex + 1], args[uStartIndex + 2]);  //may throw
			}
			//-x
			else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[2], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-x"))) ) {
				bool bRTL = false;
				uintptr uStartIndex = 3;
				//-rtl
				if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[3], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-rtl"))) ) {
					bRTL = true;
					uStartIndex ++;
				}
				if( uArgCount < uStartIndex + 4 ) {
					_PrintVersion();
					_PrintHelp();
					return 1;
				}
				bool bLatest = false;
				//-l
				if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[uStartIndex], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-l"))) ) {
					bLatest = true;
					uStartIndex ++;
				}
				if( uArgCount != uStartIndex + 4 ) {
					_PrintVersion();
					_PrintHelp();
					return 1;
				}
				if( _Check_Same_File(args[uStartIndex + 2], args[uStartIndex + 3]) ) {
					return 1;
				}
				ret = _Cmd_Generate_XHtml_Html(bRTL, bLatest, args[uStartIndex], args[uStartIndex + 1], args[uStartIndex + 2], args[uStartIndex + 3]);  //may throw
			}
			else {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Command error: Invalid HTML parameters!")));
				return 1;
			}
		}
		//-encoding
		else if( ConstStringCompareTrait<ConstStringS>::IsEQ(args[1], DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-encoding"))) ) {
			if( uArgCount != 6 ) {
				_PrintVersion();
				_PrintHelp();
				return 1;
			}
			if( _Check_Same_File(args[4], args[5]) ) {
				return 1;
			}
			ret = _Cmd_Convert_Encoding(args[2], args[3], args[4], args[5]);  //may throw
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
