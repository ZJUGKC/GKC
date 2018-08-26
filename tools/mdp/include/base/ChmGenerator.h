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
#ifndef __CHM_GENERATOR_H__
#define __CHM_GENERATOR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//generate def file
inline bool _Generate_Def_File(const ConstStringS& strFile,
							const ConstStringA& strProjectName,
							const ConstStringA& strTopic,
							uint uLCID,
							FileTreeEnumerator& ftEnum)
{
	CallResult cr;
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strFile, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() )
		return false;
	ShareCom<ITextStream> spText;
	cr = StreamHelper::CreateTextStream(spText);
	if( cr.IsFailed() )
		return false;
	spText.Deref().SetStream(spDest);
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strFileList(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//LCID string
	ConstStringA strLCID;
	lcid_array arrLCID;
	{
		uintptr uSize;
		get_lcid_string(uLCID, arrLCID, uSize);
		ConstArrayHelper::SetInternalPointer(FixedArrayHelper::GetInternalPointer(arrLCID), uSize, strLCID);
	} //end block
	//file list
	{
		StringUtilHelper::Append(strProjectName, strFileList);  //may throw
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-cover.htm"), strFileList);  //may throw
		StringUtilHelper::Append(get_crlf_string(), strFileList);  //may throw
		bool bFound = ftEnum.FindFirst();
		while( bFound ) {
			StringUtilHelper::MakeString(ftEnum.GetFile(), strContent);  //may throw
			StringUtilHelper::Replace(strContent.GetLength() - 3, 3, DECLARE_TEMP_CONST_STRING(ConstStringA, ".htm"), strContent);  //may throw
			StringUtilHelper::Append(strContent, strFileList);  //may throw
			StringUtilHelper::Append(get_crlf_string(), strFileList);  //may throw
			bFound = ftEnum.FindNext();
		}
		FsPathHelper::ConvertPathStringToPlatform(strFileList);
	} //end block
	//lines
	DECLARE_CONST_STRING_ARRAY_TYPE(CharA) arr(g_chm_def_body::GetAddress(), g_chm_def_body::GetCount());
	for( auto iter(arr.GetBegin()); iter != arr.GetEnd(); iter.MoveNext() ) {
		StringUtilHelper::MakeString(ConstStringA(iter.get_Value()), strContent);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strContent);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TOPIC$$"), strTopic, strContent);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCID$$"), strLCID, strContent);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILES$$"), StringUtilHelper::To_ConstString(strFileList), strContent);  //may throw
		StringUtilHelper::Append(get_crlf_string(), strContent);  //may throw
		//save
		cr = spText.Deref().PutStringA(StringUtilHelper::To_ConstString(strContent));
		if( cr.IsFailed() )
			return false;
	} //end for
	return true;
}

//generate cover LI string
inline void _Generate_Cover_LI_String(const ConstStringA& strProjectName,
									const ConstStringA& strCoverName,
									StringA& strContent)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_chm_content_node::GetAddress(), g_chm_content_node::GetCount()), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), strCoverName, strContent);  //may throw
	StringUtilHelper::MakeString(strProjectName, strTemp);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-cover.htm"), strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
}
//generate htm LI string
inline void _Generate_Htm_LI_String(const ConstStringA& strName,
									const ConstStringA& strFile,
									bool bLeaf,
									StringA& strContent)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	if( bLeaf )
		StringUtilHelper::MakeString(ConstStringA(g_chm_content_node::GetAddress(), g_chm_content_node::GetCount()), strContent);  //may throw
	else
		StringUtilHelper::MakeString(ConstStringA(g_chm_content_folder::GetAddress(), g_chm_content_folder::GetCount()), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), strName, strContent);  //may throw
	StringUtilHelper::MakeString(strFile, strTemp);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strTemp);
	StringUtilHelper::Replace(strTemp.GetLength() - 3, 3, DECLARE_TEMP_CONST_STRING(ConstStringA, ".htm"), strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
}

//generate content file
inline bool _Generate_Content_File(const ConstStringS& strFile,
								const ConstStringA& strProjectName,
								const ConstStringA& strCoverName,
								FileTreeEnumerator& ftEnum)
{
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//tree
	StringA strTree(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//cover
	_Generate_Cover_LI_String(strProjectName, strCoverName, strContent);  //may throw
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strContent), strTree);  //may throw
	//files
	bool bFound = ftEnum.FindFirst();
	while( bFound ) {
		//level
		intptr iDelta = ftEnum.GetDelta();
		if( iDelta > 0 ) {
			//enter
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "<UL>\r\n"), strTree);  //may throw
		}
		else if( iDelta < 0 ) {
			//leave
			for( intptr i = 0; i < -iDelta; i ++ )
				StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "</UL>\r\n"), strTree);  //may throw
		} //end if
		//node
		_Generate_Htm_LI_String(ftEnum.GetName(), ftEnum.GetFile(), ftEnum.IsLeaf(), strContent);  //may throw
		StringUtilHelper::Append(strContent, strTree);  //may throw
		bFound = ftEnum.FindNext();
	} //end while
	intptr iDelta = ftEnum.GetDelta();
	if( iDelta < 0 ) {
		//leave
		for( intptr i = 0; i < -iDelta; i ++ )
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "</UL>\r\n"), strTree);  //may throw
	} //end if
	//content
	StringUtilHelper::MakeString(ConstStringA(g_chm_content_body::GetAddress(), g_chm_content_body::GetCount()), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$GENERATOR$$"), get_chm_generator(), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TREE$$"), StringUtilHelper::To_ConstString(strTree), strContent);  //may throw
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate index file
inline bool _Generate_Index_File(const ConstStringS& strFile,
								const ConstStringA& strProjectName,
								const ConstStringA& strCoverName,
								FileTreeEnumerator& ftEnum)
{
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//tree
	StringA strFileList(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//cover
	_Generate_Cover_LI_String(strProjectName, strCoverName, strContent);  //may throw
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strContent), strFileList);  //may throw
	//files
	bool bFound = ftEnum.FindFirst();
	while( bFound ) {
		//node
		_Generate_Htm_LI_String(ftEnum.GetName(), ftEnum.GetFile(), true, strContent);  //may throw
		StringUtilHelper::Append(strContent, strFileList);  //may throw
		bFound = ftEnum.FindNext();
	} //end while
	//content
	StringUtilHelper::MakeString(ConstStringA(g_chm_index_body::GetAddress(), g_chm_index_body::GetCount()), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$GENERATOR$$"), get_chm_generator(), strContent);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILELIST$$"), StringUtilHelper::To_ConstString(strFileList), strContent);  //may throw
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}
	
//generate description files

inline bool _Chm_Generate_Description_Files(const ConstStringS& strDestRoot,
											ProjectInfo& info)
{
	StringS strFile(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringS strRoot(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw

	FileTreeEnumerator ftEnum(info.GetFileTree());

	StringUtilHelper::MakeString(strDestRoot, strRoot);  //may throw
	FsPathHelper::AppendSeparator(strRoot);  //may throw
	StringUtilHelper::Append(CS_U2S(info.GetProjectName()).GetC(), strRoot);  //may throw

	//project file
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strRoot), strFile);  //may throw
	StringUtilHelper::Append(CS_U2S(get_def_file_ext()).GetC(), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Def_File(StringUtilHelper::To_ConstString(strFile),
							StringUtilHelper::To_ConstString(info.GetProjectName()),
							StringUtilHelper::To_ConstString(info.GetTopic()),
							info.GetLCID(),
							ftEnum) )  //may throw
		return false;

	//content file
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strRoot), strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".hhc")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Content_File(StringUtilHelper::To_ConstString(strFile),
								StringUtilHelper::To_ConstString(info.GetProjectName()),
								StringUtilHelper::To_ConstString(info.GetCoverName()),
								ftEnum) )  //may throw
		return false;

	//index file
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strRoot), strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".hhk")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Index_File(StringUtilHelper::To_ConstString(strFile),
							StringUtilHelper::To_ConstString(info.GetProjectName()),
							StringUtilHelper::To_ConstString(info.GetCoverName()),
							ftEnum) )  //may throw
		return false;

	//cover html
	StringUtilHelper::MakeString(StringUtilHelper::To_ConstString(strRoot), strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("-cover.html")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Cover_Html_File(StringUtilHelper::To_ConstString(strFile),
								StringUtilHelper::To_ConstString(info.GetCoverImageFile())) )  //may throw
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
