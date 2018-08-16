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
	//tree
	if( info.GetFileTree().IsEmpty() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The file tree cannot be empty!")));
		return false;
	}
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

	if( !_Copy_MD_Files(_Get_Html_Ext_Name(iType), info,
						StringUtilHelper::To_ConstString(strSrcDir),
						StringUtilHelper::To_ConstString(strDestRoot),
						flMd)
		) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The md files cannot be copied to destination file tree!")));
		return false;
	}

	//generate description files
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Generate description files...")));

	if( iType == MDP_TYPE_CHM ) {
		if( !_Chm_Generate_Description_Files(StringUtilHelper::To_ConstString(strDestRoot),
											info) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The description files cannot be generated!")));
			return false;
		}
	}
	else if( iType == MDP_TYPE_EPUB ) {
		if( !_Epub_Generate_Description_Files(StringUtilHelper::To_ConstString(strDest),
											StringUtilHelper::To_ConstString(strDestRoot),
											info, hlInfo, flMd, flAux) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The description files cannot be generated!")));
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
