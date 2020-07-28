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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SCRIPT_GENERATOR_H__
#define __SCRIPT_GENERATOR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//generate script FILES string
inline void _Generate_Script_FILES_String(const ConstStringA& strDirPrefix,
										const FileListInfo& flInfo,
										StringA& strList)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	uintptr uCount = flInfo.GetCount();
	for( uintptr i = 0; i < uCount; ) {
		StringUtilHelper::MakeString(strDirPrefix, strTemp);  //may throw
		FsPathHelper::AppendSeparator(strTemp);  //may throw
		StringUtilHelper::Append(flInfo.GetAt(i), strTemp);  //may throw
		FsPathHelper::RemoveExtension(strTemp.GetLength() - 3, strTemp);
		FsPathHelper::ConvertPathStringToPlatform(strTemp);
		StringOpHelper::Insert(0, '\"', strTemp);  //may throw
		StringOpHelper::Append('\"', strTemp);  //may throw
		i ++;
		if( i < uCount )
			StringUtilHelper::Append(get_array_delimiter(), strTemp);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strTemp), strList);  //may throw
	}
}

//generate script CSSPREFIX string
inline void _Generate_Script_CSSPREFIX_String(const FileListInfo& flInfo,
											StringA& strList)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	uintptr uCount = flInfo.GetCount();
	for( uintptr i = 0; i < uCount; ) {
		strTemp.Clear();
		for( auto iter(flInfo.GetAt(i).GetBegin()); iter != flInfo.GetAt(i).GetEnd(); iter.MoveNext() ) {
			if( iter.get_Value() == '/' )
				StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "../"), strTemp);  //may throw
		}
		StringOpHelper::Insert(0, '\"', strTemp);  //may throw
		StringOpHelper::Append('\"', strTemp);  //may throw
		i ++;
		if( i < uCount )
			StringUtilHelper::Append(get_array_delimiter(), strTemp);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strTemp), strList);  //may throw
	}
}

//generate script COMMANDS string
inline void _Generate_Script_COMMANDS_String(const ShareArray<StringA>& arrCommand,
											StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	for( auto iter(arrCommand.GetBegin()); iter != arrCommand.GetEnd(); iter.MoveNext() ) {
		StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(iter.get_Value()), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$SRC$"), get_command_src(), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$DEST$"), get_command_dest(), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$DEL$"), get_command_del(), strTemp);  //may throw
		StringUtilHelper::MakeString(ConstStringA(g_script_command_item::GetAddress(), g_script_command_item::GetCount()), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CMDITEM$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
}

//generate encoding string
inline void _Generate_Script_Encoding_String(const ConstStringA& strDirPrefix,
											const ConstStringA& strProjectName,
											const ConstStringA& strCharset,
											StringA& strList)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_script_chm_encoding::GetAddress(), g_script_chm_encoding::GetCount()), strList);  //may throw
	StringUtilHelper::MakeString(strDirPrefix, strTemp);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strTemp);
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DIRPREFIX$$"), StringUtilHelper::To_ConstString(strTemp), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DEF$$"), get_def_file_ext(), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CHARSET$$"), strCharset, strList);  //may throw
}

//generate script file

inline bool _Generate_Script_File(const ConstStringS& strDest,
								const ConstStringA& strDirPrefix,
								bool bEncoding,
								const ConstStringA& strExt,
								const ConstStringA& strHtmlOptions,
								const ConstStringA& strEBookCmd,
								const ProjectInfo& info, const HelpLanguageInfo& hlInfo,
								const FileListInfo& flInfo)
{
	StringS strFile(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(strDest, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(get_script_filename(), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);

	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_script_body::GetAddress(), g_script_body::GetCount()), strContent);  //may throw

	//files
	StringA strList(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	_Generate_Script_FILES_String(strDirPrefix, flInfo, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILES$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw

	//cssprefix
	strList.Clear();
	_Generate_Script_CSSPREFIX_String(flInfo, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CSSPREFIX$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw

	//commands
	strList.Clear();
	_Generate_Script_COMMANDS_String(info.GetCommandArray(), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COMMANDS$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw

	StringA strCharset(CS_S2U(hlInfo.strCharset).GetV());  //may throw
	StringA strShortString(CS_S2U(hlInfo.strShortString).GetV());  //may throw

	//encoding
	strList.Clear();
	if( bEncoding )
		_Generate_Script_Encoding_String(strDirPrefix,
										StringUtilHelper::To_ConstString(info.GetProjectName()),
										StringUtilHelper::To_ConstString(strCharset),
										strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ENCODING$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw

	//html
	StringUtilHelper::MakeString(strHtmlOptions, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CHARSET$$"), StringUtilHelper::To_ConstString(strCharset), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LANG$$"), StringUtilHelper::To_ConstString(strShortString), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$OPTIONS$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$HTMLEXT$$"), strExt, strContent);  //may throw
	StringUtilHelper::MakeString(strDirPrefix, strList);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strList);
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DIRPREFIX$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), StringUtilHelper::To_ConstString(info.GetProjectName()), strContent);  //may throw

	//generate eBook
	StringUtilHelper::MakeString(strEBookCmd, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), StringUtilHelper::To_ConstString(info.GetProjectName()), strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$EBOOKCMD$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw

	//save
	return _Generate_Fix_Content_File(StringUtilHelper::To_ConstString(strFile), StringUtilHelper::To_ConstString(strContent));
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
