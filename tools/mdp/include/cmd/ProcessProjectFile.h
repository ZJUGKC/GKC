/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PROCESS_PROJECT_FILE_H__
#define __PROCESS_PROJECT_FILE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// check project

inline bool _Check_ProjectFile(const ProjectInfo& info, HelpLanguageInfo& hlInfo) throw()
{
	//LCID
	if( !HelpAuthoringHelper::FindLCID(info.GetLCID(), hlInfo) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The LCID is not supported!")));
		return false;
	}
	//Name
	if( info.GetProjectName().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The project name cannot be empty!")));
		return false;
	}
	//document directory
	if( info.GetDocumentDirectory().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The document directory cannot be empty!")));
		return false;
	}
	//command array
	if( info.GetCommandArray().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The command array cannot be empty!")));
		return false;
	}
	//cover
	if( info.GetCoverImageFile().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The cover image file cannot be empty!")));
		return false;
	}
	//cover name
	if( info.GetCoverName().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The cover name cannot be empty!")));
		return false;
	}
	//topic
	if( info.GetTopic().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The topic name cannot be empty!")));
		return false;
	}
	//identifier
	if( info.GetIdentifier().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The identifier cannot be empty!")));
		return false;
	}
	//tree
	if( info.GetFileTree().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The file tree cannot be empty!")));
		return false;
	}
	return true;
}

// check project file list

inline bool _Check_Project_FileList(const FileListInfo& flInfo, const DirFileList& flMd)
{
	//check project files
	uintptr uCount = flInfo.GetCount();
	for( uintptr i = 0; i < uCount; i ++ ) {
		StringS strV(CS_U2S(flInfo.GetAt(i)).GetV());  //may throw
		if( !flMd.Find(strV) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: [")));
			ConsoleHelper::WriteLine(strV);
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("] is not in file tree!")));
			return false;
		}
	} //end for
	return true;
}

// process project file

inline bool _Process_Project_File(StringS& strSrc, StringS& strSrcDir,
								StringS& strDest, int iType)
{
	//load
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Load project file...")));

	ProjectInfo info;
	if( !info.Load(StringUtilHelper::To_ConstString(strSrc)) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot load project file!")));
		return false;
	}
	//check
	HelpLanguageInfo hlInfo;
	if( !_Check_ProjectFile(info, hlInfo) )
		return false;
	//file list
	FileListInfo flInfo;
	_Generate_Project_FileList(info, flInfo);

	//output
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Create output file tree...")));

	StringS strDestRoot;
	if( !_Create_Output_FileTree(iType, StringUtilHelper::To_ConstString(strDest), strDestRoot) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The output file tree cannot be created!")));
		return false;
	}

	//source root
	FsPathHelper::AppendSeparator(strSrcDir);
	StringUtilHelper::Append(CS_U2S(info.GetDocumentDirectory()).GetC(), strSrcDir);

	//scan
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Scan the source file tree...")));

	DirFileList flMd, flAux;
	if( !_Scan_FileTree(StringUtilHelper::To_ConstString(strSrcDir),
						StringUtilHelper::To_ConstString(strDestRoot),
						flMd, flAux)
		) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The output file tree creation is failed!")));
		return false;
	}

	//copy aux files
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Copy aux files...")));

	if( !_Copy_Aux_Files(StringUtilHelper::To_ConstString(strSrcDir),
						StringUtilHelper::To_ConstString(strDestRoot),
						flAux)
		) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The aux files cannot be copied to destination file tree!")));
		return false;
	}

	//copy md files
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Copy md files...")));

	if( !_Check_Project_FileList(flInfo, flMd) )
		return false;

	if( !_Copy_MD_Files(_Get_Html_Ext_Name(iType), info,
						StringUtilHelper::To_ConstString(strSrcDir),
						StringUtilHelper::To_ConstString(strDestRoot),
						flInfo, flMd)
		) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The md files cannot be copied to destination file tree!")));
		return false;
	}

	//generate description files
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Generate description files...")));

	if( iType == MDP_TYPE_CHM ) {
		if( !_Chm_Generate_Description_Files(StringUtilHelper::To_ConstString(strDestRoot),
											info, flInfo) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The description files cannot be generated!")));
			return false;
		}
	}
	else if( iType == MDP_TYPE_EPUB ) {
		if( !_Epub_Generate_Description_Files(StringUtilHelper::To_ConstString(strDest),
											StringUtilHelper::To_ConstString(strDestRoot),
											info, hlInfo, flInfo, flMd, flAux) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The description files cannot be generated!")));
			return false;
		}
	}

	//generate script file
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Generate script file...")));

	if( !_Generate_Script_File(StringUtilHelper::To_ConstString(strDest),
							_Get_FileTree_Prefix(iType),
							iType == MDP_TYPE_CHM,
							_Get_Html_Ext_Name(iType),
							_Get_Html_Options_Template(iType),
							_Get_EBook_CMD_Template(iType),
							info, hlInfo, flInfo) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The script file cannot be generated!")));
		return false;
	}

	//done
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Done.")));

	return true;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
