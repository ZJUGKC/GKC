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
#ifndef __EPUB_GENERATOR_H__
#define __EPUB_GENERATOR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//generate mimetype file
inline bool _Generate_MimeType_File(const ConstStringS& strFile) throw()
{
	return _Generate_Fix_Content_File(strFile,
				ConstStringA(g_epub_mimetype_body::GetAddress(), g_epub_mimetype_body::GetCount())
			);
}

//generate container file
inline bool _Generate_Container_File(const ConstStringS& strFile) throw()
{
	return _Generate_Fix_Content_File(strFile,
				ConstStringA(g_epub_container_body::GetAddress(), g_epub_container_body::GetCount())
			);
}

//generate css file
inline bool _Generate_Css_File(const ConstStringS& strFile)
{
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_css_body::GetAddress(), g_epub_css_body::GetCount()), strContent);  //may throw
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate file-id string
inline void _Generate_FileId_String(const ConstStringA& strFile, StringA& strId)
{
	StringUtilHelper::MakeString(strFile, strId);  //may throw
	StringHelper::Replace('/', '-', strId);
	StringHelper::Replace('.', '-', strId);
	StringHelper::Replace('#', '-', strId);
}
//generate type string
inline ConstStringA _Generate_MimeType_String(const ConstStringA& str) throw()
{
	ConstStringA strType(DECLARE_TEMP_CONST_STRING(ConstStringA, "application/xhtml+xml"));
	uintptr uPos;
	if( !FsPathHelper::FindExtensionStart(str, uPos) )
		return strType;
	ConstStringA strExt(ConstArrayHelper::GetInternalPointer(str) + uPos, str.GetCount() - uPos);
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".jpg")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "image/jpeg");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".png")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "image/png");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".gif")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "image/gif");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".svg")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "image/svg+xml");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".mp4")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "video/mp4");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".mp3")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "audio/mp3");
	if( ConstStringCompareTrait<ConstStringA>::IsEQ(strExt, DECLARE_TEMP_CONST_STRING(ConstStringA, ".js")) )
		return DECLARE_TEMP_CONST_STRING(ConstStringA, "text/javascript");
	return strType;
}
//generate manifest list
inline void _Generate_Manifest_String(const DirFileList& fl, bool bMd, StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	for( uintptr i = 0; i < fl.GetCount(); i ++ ) {
		StringA strV(CS_S2U(fl.GetAt(i)).GetV());  //may throw
		StringUtilHelper::MakeString(ConstStringA(g_epub_opf_item::GetAddress(), g_epub_opf_item::GetCount()), strItem);  //may throw
		//file id
		_Generate_FileId_String(StringUtilHelper::To_ConstString(strV), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILEID$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//file
		if( bMd )
			StringUtilHelper::Replace(strV.GetLength() - 3, 3, DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strV);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strV), strItem);  //may throw
		//type
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TYPE$$"), _Generate_MimeType_String(StringUtilHelper::To_ConstString(strV)), strItem);  //may throw
		//list
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
}
//generate spine list
inline void _Generate_Spine_String(const FileListInfo& flInfo, StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	for( uintptr i = 0; i < flInfo.GetCount(); i ++ ) {
		StringUtilHelper::MakeString(ConstStringA(g_epub_opf_itemref::GetAddress(), g_epub_opf_itemref::GetCount()), strItem);  //may throw
		//file id
		_Generate_FileId_String(StringUtilHelper::To_ConstString(flInfo.GetAt(i)), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILEID$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//append
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
}
//generate guide
inline void _Generate_Guide_String(const ConstStringA& strName, const ConstStringA& strFile, const ConstStringA& strType,
								StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_opf_guide::GetAddress(), g_epub_opf_guide::GetCount()), strItem);  //may throw
	_Generate_FileUrl_String(strFile, DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TYPE$$"), strType, strItem);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), strName, strItem);  //may throw
	StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
}

//generate opf file
inline bool _Generate_Opf_File(const ConstStringS& strFile,
							const ConstStringA& strProjectName,
							const ConstStringA& strCoverName,
							const ConstStringA& strShortString,
							const ConstStringA& strTopic,
							const ConstStringA& strCoverImageFile,
							const ConstStringA& strAuthor,
							const ConstStringA& strDescription,
							const ConstStringA& strDate,
							const ConstStringA& strContributor,
							const ConstStringA& strPublisher,
							const ConstStringA& strSubject,
							const ConstStringA& strRights,
							const ConstStringA& strIdentifier,
							const DirFileList& flMd, const DirFileList& flAux,
							const FileListInfo& flInfo, const ProjectInfo& info)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_opf_body::GetAddress(), g_epub_opf_body::GetCount()), strContent);  //may throw
	//project name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strContent);  //may throw
	//cover name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVERNAME$$"), strCoverName, strContent);  //may throw
	//topic
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TOPIC$$"), strTopic, strContent);  //may throw
	//author
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$AUTHOR$$"), strAuthor, strContent);  //may throw
	//description
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DESCRIPTION$$"), strDescription, strContent);  //may throw
	//language
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCSS$$"), strShortString, strContent);  //may throw
	//date
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DATE$$"), strDate, strContent);  //may throw
	//contributor
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CONTRIBUTOR$$"), strContributor, strContent);  //may throw
	//publisher
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PUBLISHER$$"), strPublisher, strContent);  //may throw
	//subject
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$SUBJECT$$"), strSubject, strContent);  //may throw
	//rights
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$RIGHTS$$"), strRights, strContent);  //may throw
	//identifier
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$IDENTIFIER$$"), strIdentifier, strContent);  //may throw
	//cover
	_Generate_FileId_String(strCoverImageFile, strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVER$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//manifest
	StringA strList(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	_Generate_Manifest_String(flMd, true, strList);  //may throw
	_Generate_Manifest_String(flAux, false, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMLIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//spine
	strList.Clear();
	_Generate_Spine_String(flInfo, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMREFLIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//guide
	strList.Clear();
	{
		StringA str1, str2;
		if( info.GetTitleFile(str1, str2) )
			_Generate_Guide_String(StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), DECLARE_TEMP_CONST_STRING(ConstStringA, "title-page"), strList);  //may throw
		if( info.GetContentFile(str1, str2) )
			_Generate_Guide_String(StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), DECLARE_TEMP_CONST_STRING(ConstStringA, "toc"), strList);  //may throw
		if( info.GetCopyrightFile(str1, str2) )
			_Generate_Guide_String(StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), DECLARE_TEMP_CONST_STRING(ConstStringA, "copyright-page"), strList);  //may throw
	} //end block
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$GUIDELIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//integer string
typedef FixedStringT<CharA, 64>  IntegerString;

inline void _Generate_Integer_String(uintptr uValue, IntegerString& str) throw()
{
	str.SetLength(0);
	int ret = value_to_string(FixedArrayHelper::GetInternalPointer(str), IntegerString::c_size,
							"%Iu", uValue);
	if( ret >= 0 )
		str.SetLength(ret);
}
//generate nav string
inline void _Generate_Nav_String(FileTreeEnumerator& ftEnum, StringA& strTree, uintptr& uMaxLevel)
{
	uMaxLevel = 1;
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	ConstStringA strNode(g_epub_ncx_node::GetAddress(), g_epub_ncx_node::GetCount());
	ConstStringA strTail(g_epub_ncx_node_tail::GetAddress(), g_epub_ncx_node_tail::GetCount());
	uintptr uOrder = 2;
	bool bFound = ftEnum.FindFirst();
	while( bFound ) {
		//level
		intptr iDelta = ftEnum.GetDelta();
		if( iDelta < 0 ) {
			//leave
			for( intptr i = 0; i < -iDelta; i ++ )
				StringUtilHelper::Append(strTail, strTree);  //may throw
		} //end if
		if( uMaxLevel < ftEnum.GetLevel() )
			uMaxLevel = ftEnum.GetLevel();
		//item
		StringUtilHelper::MakeString(strNode, strItem);  //may throw
		//file id
		_Generate_FileId_String(ftEnum.GetFile(), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILEID$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//order
		{
			IntegerString strOrder;
			_Generate_Integer_String(uOrder, strOrder);
			StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ORDER$$"), StringUtilHelper::To_ConstString(strOrder), strItem);  //may throw
			++ uOrder;
		} //end block
		//file
		_Generate_FileUrl_String(ftEnum.GetFile(), DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//name
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), ftEnum.GetName(), strItem);  //may throw
		//append
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strTree);  //may throw
		if( ftEnum.IsLeaf() )
			StringUtilHelper::Append(strTail, strTree);  //may throw
		bFound = ftEnum.FindNext();
	} //end while
	//last
	intptr iDelta = ftEnum.GetDelta();
	if( iDelta < 0 ) {
		//leave
		for( intptr i = 0; i < -iDelta; i ++ )
			StringUtilHelper::Append(strTail, strTree);  //may throw
	} //end if
}

//generate ncx file
inline bool _Generate_Ncx_File(const ConstStringS& strFile,
							const ConstStringA& strProjectName,
							const ConstStringA& strCoverName,
							const ConstStringA& strShortString,
							const ConstStringA& strTopic,
							const ConstStringA& strAuthor,
							const ConstStringA& strIdentifier,
							FileTreeEnumerator& ftEnum)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_ncx_body::GetAddress(), g_epub_ncx_body::GetCount()), strContent);  //may throw
	//project name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strContent);  //may throw
	//cover name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVERNAME$$"), strCoverName, strContent);  //may throw
	//language
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCSS$$"), strShortString, strContent);  //may throw
	//topic
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TOPIC$$"), strTopic, strContent);  //may throw
	//author
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$AUTHOR$$"), strAuthor, strContent);  //may throw
	//identifier
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$IDENTIFIER$$"), strIdentifier, strContent);  //may throw
	//tree
	StringA strTree(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	uintptr uMaxLevel;
	_Generate_Nav_String(ftEnum, strTree, uMaxLevel);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TREE$$"), StringUtilHelper::To_ConstString(strTree), strContent);  //may throw
	//depth
	{
		IntegerString strDepth;
		_Generate_Integer_String(uMaxLevel, strDepth);
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DEPTH$$"), StringUtilHelper::To_ConstString(strDepth), strContent);  //may throw
	} //end block
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate description files

inline bool _Epub_Generate_Description_Files(const ConstStringS& strDest, const ConstStringS& strDestRoot,
											const ProjectInfo& info, const HelpLanguageInfo& hlInfo,
											const FileListInfo& flInfo,
											const DirFileList& flMd, const DirFileList& flAux)
{
	StringS strFile(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw

	FileTreeEnumerator ftEnum(info.GetFileTree());

	//mimetype
	StringUtilHelper::MakeString(strDest, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("doc")), strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("mimetype")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_MimeType_File(StringUtilHelper::To_ConstString(strFile)) )
		return false;

	//container
	StringUtilHelper::MakeString(strDest, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("doc")), strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("META-INF")), strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("container.xml")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Container_File(StringUtilHelper::To_ConstString(strFile)) )
		return false;

	//css
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("stylesheet.css")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Css_File(StringUtilHelper::To_ConstString(strFile)) )  //may throw
		return false;

	StringA strShortString(CS_S2U(hlInfo.strShortString).GetV());  //may throw

	//opf
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("content.opf")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Opf_File(StringUtilHelper::To_ConstString(strFile),
							StringUtilHelper::To_ConstString(info.GetProjectName()),
							StringUtilHelper::To_ConstString(info.GetCoverName()),
							StringUtilHelper::To_ConstString(strShortString),
							StringUtilHelper::To_ConstString(info.GetTopic()),
							StringUtilHelper::To_ConstString(info.GetCoverImageFile()),
							StringUtilHelper::To_ConstString(info.GetAuthor()),
							StringUtilHelper::To_ConstString(info.GetDescription()),
							StringUtilHelper::To_ConstString(info.GetDate()),
							StringUtilHelper::To_ConstString(info.GetContributor()),
							StringUtilHelper::To_ConstString(info.GetPublisher()),
							StringUtilHelper::To_ConstString(info.GetSubject()),
							StringUtilHelper::To_ConstString(info.GetRights()),
							StringUtilHelper::To_ConstString(info.GetIdentifier()),
							flMd, flAux,
							flInfo, info) )  //may throw
		return false;

	//ncx
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("toc.ncx")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( !_Generate_Ncx_File(StringUtilHelper::To_ConstString(strFile),
							StringUtilHelper::To_ConstString(info.GetProjectName()),
							StringUtilHelper::To_ConstString(info.GetCoverName()),
							StringUtilHelper::To_ConstString(strShortString),
							StringUtilHelper::To_ConstString(info.GetTopic()),
							StringUtilHelper::To_ConstString(info.GetAuthor()),
							StringUtilHelper::To_ConstString(info.GetIdentifier()),
							ftEnum) )  //may throw
		return false;

	//cover html
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(CS_U2S(info.GetProjectName()).GetC(), strFile);  //may throw
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
