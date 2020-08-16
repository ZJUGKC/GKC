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
inline bool _Generate_Css_File(bool bRTLorder, bool bVerticalLine, bool bLatest,
							const ConstStringS& strFile)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_css_body::GetAddress(), g_epub_css_body::GetCount()), strContent);  //may throw
	//layout
	if( bRTLorder || bVerticalLine ) {
		StringUtilHelper::Append(ConstStringA(g_epub_css_layout::GetAddress(), g_epub_css_layout::GetCount()), strItem);  //may throw
		// mode
		if( bVerticalLine ) {
			StringUtilHelper::Append(bLatest ? DECLARE_TEMP_CONST_STRING(ConstStringA, "vertical")
											: DECLARE_TEMP_CONST_STRING(ConstStringA, "tb"),
									strTemp);  //may throw
			if( bRTLorder )
				StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-rl"), strTemp);  //may throw
			else
				StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-lr"), strTemp);  //may throw
		}
		else {
			StringUtilHelper::Append(bLatest ? DECLARE_TEMP_CONST_STRING(ConstStringA, "horizontal-tb")
											: DECLARE_TEMP_CONST_STRING(ConstStringA, "lr-tb"),
									strTemp);  //may throw
		}
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$MODE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		// text
		strTemp.Clear();
		if( bVerticalLine ) {
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "text-orientation: upright;\r\n"), strTemp);  //may throw
		}
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TEXT-LAYOUT$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
	}
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LAYOUT$$"), StringUtilHelper::To_ConstString(strItem), strContent);  //may throw
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate time string
inline void _Generate_Time_String(StringA& str)
{
	TimeValue tv;
	TimeHelper::FetchCurrentTime(tv);
	TimeDetail td = { 0 };
	bool bRet = TimeHelper::ToGmtDetail(tv, td);
	(void)bRet;
	assert( bRet );
	FixedStringT<CharA, 32> buffer;
	int ret = value_to_string(FixedArrayHelper::GetInternalPointer(buffer), FixedStringT<CharA, 32>::c_size,
							"%04d-%02d-%02dT%02d:%02d:%02dZ",
							td.iYear, td.iMonth, td.iDay,
							td.iHour, td.iMinute, td.iSecond);
	if( ret >= 0 )
		buffer.SetLength(ret);
	StringUtilHelper::MakeString(buffer, str);  //may throw
}

//generate file-id string
inline void _Generate_FileId_String(const ConstStringA& strFile, StringA& strId)
{
	StringUtilHelper::MakeString(strFile, strId);  //may throw
	StringOpHelper::Replace('/', '-', strId);
	StringOpHelper::Replace('.', '-', strId);
	StringOpHelper::Replace('#', '-', strId);
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
//generate meta list
inline void _Generate_MetaList_String(const ConstStringA& strCoverImageFile,
									const ConstStringA& strAuthor,
									const ConstStringA& strDescription,
									const ConstStringA& strDate,
									const ConstStringA& strContributor,
									const ConstStringA& strPublisher,
									const ConstStringA& strSubject,
									const ConstStringA& strRights,
									bool bRTLorder, bool bVerticalLine,
									bool bLatest,
									StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//author
	if( !strAuthor.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:creator>$$AUTHOR$$</dc:creator>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$AUTHOR$$"), strAuthor, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//description
	if( !strDescription.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:description>$$DESCRIPTION$$</dc:description>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DESCRIPTION$$"), strDescription, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//date
	if( !strDate.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:date>$$DATE$$</dc:date>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DATE$$"), strDate, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//contributor
	if( !strContributor.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:contributor>$$CONTRIBUTOR$$</dc:contributor>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CONTRIBUTOR$$"), strContributor, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//publisher
	if( !strPublisher.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:publisher>$$PUBLISHER$$</dc:publisher>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PUBLISHER$$"), strPublisher, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//subject
	if( !strSubject.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:subject>$$SUBJECT$$</dc:subject>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$SUBJECT$$"), strSubject, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//rights
	if( !strRights.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<dc:rights>$$RIGHTS$$</dc:rights>\r\n"), strItem);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$RIGHTS$$"), strRights, strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
	//meta
	if( bLatest ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<meta property=\"dcterms:modified\">$$TIME$$</meta>\r\n"), strItem);  //may throw
		//time
		_Generate_Time_String(strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TIME$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
	}
	else {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<meta name=\"cover\" content=\"$$COVER$$\" />\r\n"), strItem);  //may throw
		//cover
		_Generate_FileId_String(strCoverImageFile, strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVER$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
	}
	StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	//writing-mode
	if( !bLatest && (bRTLorder || bVerticalLine) ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<meta name=\"primary-writing-mode\" content=\"$$MODE$$\" />\r\n"), strItem);  //may throw
		strTemp.Clear();
		if( bVerticalLine )
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "vertical"), strTemp);  //may throw
		else
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "horizontal"), strTemp);  //may throw
		if( bRTLorder )
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-rl"), strTemp);  //may throw
		else
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "-lr"), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$MODE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
	}
}
//generate manifest list
inline void _Generate_Manifest_String(const DirFileList& fl, bool bMd, bool bLatest,
									const ConstStringA& strCoverImageFile,
									StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	for( uintptr i = 0; i < fl.GetCount(); i ++ ) {
		StringA strV(CS_S2U(StringUtilHelper::To_ConstString(fl.GetAt(i))).GetV());  //may throw
		StringUtilHelper::MakeString(ConstStringA(g_epub_opf_item::GetAddress(), g_epub_opf_item::GetCount()), strItem);  //may throw
		//file id
		_Generate_FileId_String(StringUtilHelper::To_ConstString(strV), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILEID$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//file
		if( bMd )
			StringUtilHelper::Replace(strV.GetLength() - 3, 3, DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strV);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strV), strItem);  //may throw
		//prop
		strTemp.Clear();
		if( bLatest ) {
			if( bMd ) {
				StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "properties=\"mathml svg\""), strTemp);  //may throw
			}
			else {
				if( ConstStringCompareTrait<ConstStringA>::IsEQ(StringUtilHelper::To_ConstString(strV), strCoverImageFile) )
					StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "properties=\"cover-image\""), strTemp);  //may throw
			}
		}
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROP$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
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
inline void _Generate_GuideItem_String(const ConstStringA& strTemplate,
									const ConstStringA& strName, const ConstStringA& strFile, const ConstStringA& strType,
									StringA& strList)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(strTemplate, strItem);  //may throw
	_Generate_FileUrl_String(strFile, DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TYPE$$"), strType, strItem);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), strName, strItem);  //may throw
	StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strList);  //may throw
}
inline void _Generate_GuideList_String(const ConstStringA& strListTemplate, const ConstStringA& strItemTemplate,
									const ConstStringA& strTitleType, const ConstStringA& strContentType, const ConstStringA& strCopyrightType,
									const ConstStringA& strProjectName,
									const ConstStringA& strCoverName,
									const ProjectInfo& info,
									StringA& strList)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(strListTemplate, strList);  //may throw
	//project name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strList);  //may throw
	//cover name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVERNAME$$"), strCoverName, strList);  //may throw
	//list
	StringA str1, str2;
	if( info.GetTitleFile(str1, str2) )
		_Generate_GuideItem_String(strItemTemplate, StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), strTitleType, strTemp);  //may throw
	if( info.GetContentFile(str1, str2) )
		_Generate_GuideItem_String(strItemTemplate, StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), strContentType, strTemp);  //may throw
	if( info.GetCopyrightFile(str1, str2) )
		_Generate_GuideItem_String(strItemTemplate, StringUtilHelper::To_ConstString(str1), StringUtilHelper::To_ConstString(str2), strCopyrightType, strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LIST$$"), StringUtilHelper::To_ConstString(strTemp), strList);  //may throw
}

inline void _Generate_Opf_Guide_String(const ConstStringA& strProjectName,
									const ConstStringA& strCoverName,
									const ProjectInfo& info,
									StringA& strList)
{
	_Generate_GuideList_String(
			ConstStringA(g_epub_opf_guide_body::GetAddress(), g_epub_opf_guide_body::GetCount()),
			ConstStringA(g_epub_opf_guide_item::GetAddress(), g_epub_opf_guide_item::GetCount()),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "title-page"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "toc"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "copyright-page"),
			strProjectName, strCoverName, info, strList
		);  //may throw
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
							bool bRTLorder, bool bVerticalLine,
							const DirFileList& flMd, const DirFileList& flAux,
							const FileListInfo& flInfo, const ProjectInfo& info,
							bool bLatest)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strList(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_opf_body::GetAddress(), g_epub_opf_body::GetCount()), strContent);  //may throw
	//project name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strContent);  //may throw
	//topic
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TOPIC$$"), strTopic, strContent);  //may throw
	//language
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCSS$$"), strShortString, strContent);  //may throw
	//identifier
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$IDENTIFIER$$"), strIdentifier, strContent);  //may throw
	//meta-list
	_Generate_MetaList_String(strCoverImageFile, strAuthor, strDescription, strDate, strContributor,
							strPublisher, strSubject, strRights,
							bRTLorder, bVerticalLine,
							bLatest, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$METALIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//version
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$VERSION$$"),
		bLatest
		? DECLARE_TEMP_CONST_STRING(ConstStringA, "3.0")
		: DECLARE_TEMP_CONST_STRING(ConstStringA, "2.0"),
		strContent);  //may throw
	//xml:lang
	if( bLatest ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "xml:lang=\"$$LCSS$$\""), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCSS$$"), strShortString, strTemp);  //may throw
	}
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$XMLLANG$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//toc item
	if( bLatest )
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMTOC$$"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "<item href=\"toc.xhtml\" properties=\"nav\" media-type=\"application/xhtml+xml\" id=\"toc-nav\"/>"),
			strContent);  //may throw
	else
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMTOC$$"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "<item href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" id=\"ncx\"/>"),
			strContent);  //may throw
	//manifest
	strList.Clear();
	_Generate_Manifest_String(flMd, true, bLatest, strCoverImageFile, strList);  //may throw
	_Generate_Manifest_String(flAux, false, bLatest, strCoverImageFile, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMLIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//spine
	strTemp.Clear();
	if( bRTLorder )
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "page-progression-direction=\"rtl\""), strTemp);  //may throw
	if( !bLatest )
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, " toc=\"ncx\""), strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$SPINE-TOC$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	strList.Clear();
	_Generate_Spine_String(flInfo, strList);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$ITEMREFLIST$$"), StringUtilHelper::To_ConstString(strList), strContent);  //may throw
	//guide
	strTemp.Clear();
	if( !bLatest )
		_Generate_Opf_Guide_String(strProjectName, strCoverName, info, strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$GUIDETAG$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
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
		bool bNegative;
		uintptr uDelta = ftEnum.GetDelta(bNegative);
		if( bNegative ) {
			//leave
			for( uintptr i = 0; i < uDelta; i ++ )
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
	bool bNegative;
	uintptr uDelta = ftEnum.GetDelta(bNegative);
	if( bNegative ) {
		//leave
		for( uintptr i = 0; i < uDelta; i ++ )
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
	if( !strAuthor.IsEmpty() ) {
		StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(ConstStringA, "<docAuthor><text>$$AUTHOR$$</text></docAuthor>\r\n"), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$AUTHOR$$"), strAuthor, strTemp);  //may throw
	}
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$AUTHOR-LIST$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//identifier
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$IDENTIFIER$$"), strIdentifier, strContent);  //may throw
	//tree
	uintptr uMaxLevel;
	strTemp.Clear();
	_Generate_Nav_String(ftEnum, strTemp, uMaxLevel);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TREE$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//depth
	{
		IntegerString strDepth;
		_Generate_Integer_String(uMaxLevel, strDepth);
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$DEPTH$$"), StringUtilHelper::To_ConstString(strDepth), strContent);  //may throw
	} //end block
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate tree string
inline void _Generate_Tree_String(FileTreeEnumerator& ftEnum, StringA& strTree)
{
	StringA strItem(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	bool bFound = ftEnum.FindFirst();
	while( bFound ) {
		//level
		bool bNegative;
		uintptr uDelta = ftEnum.GetDelta(bNegative);
		if( bNegative ) {
			//leave
			for( uintptr i = 0; i < uDelta; i ++ )
				StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "</ol></li>\r\n"), strTree);  //may throw
		}
		else if( uDelta > 0 ) {
			//enter
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "<ol>\r\n"), strTree);  //may throw
		} //end if
		//item
		StringUtilHelper::MakeString(ConstStringA(g_epub_END_item::GetAddress(), g_epub_END_item::GetCount()), strItem);  //may throw
		//file id
		_Generate_FileId_String(ftEnum.GetFile(), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILEID$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//file
		_Generate_FileUrl_String(ftEnum.GetFile(), DECLARE_TEMP_CONST_STRING(ConstStringA, ".xhtml"), strTemp);  //may throw
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$FILE$$"), StringUtilHelper::To_ConstString(strTemp), strItem);  //may throw
		//name
		StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$NAME$$"), ftEnum.GetName(), strItem);  //may throw
		//append
		StringUtilHelper::Append(StringUtilHelper::To_ConstString(strItem), strTree);  //may throw
		if( ftEnum.IsLeaf() )
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "</li>\r\n"), strTree);  //may throw
		//next
		bFound = ftEnum.FindNext();
	} //end while
	//last
	bool bNegative;
	uintptr uDelta = ftEnum.GetDelta(bNegative);
	if( bNegative ) {
		//leave
		for( uintptr i = 0; i < uDelta; i ++ )
			StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringA, "</ol></li>\r\n"), strTree);  //may throw
	} //end if
}
//generate landmarks
inline void _Generate_landmarks_String(const ConstStringA& strProjectName,
									const ConstStringA& strCoverName,
									const ProjectInfo& info,
									StringA& strList)
{
	_Generate_GuideList_String(
			ConstStringA(g_epub_END_landmarks_body::GetAddress(), g_epub_END_landmarks_body::GetCount()),
			ConstStringA(g_epub_END_landmarks_item::GetAddress(), g_epub_END_landmarks_item::GetCount()),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "titlepage"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "toc"),
			DECLARE_TEMP_CONST_STRING(ConstStringA, "copyright-page"),
			strProjectName, strCoverName, info, strList
		);  //may throw
}

//generate END file
inline bool _Generate_END_File(const ConstStringS& strFile,
							const ConstStringA& strProjectName,
							const ConstStringA& strCoverName,
							const ConstStringA& strShortString,
							const ConstStringA& strTopic,
							const ProjectInfo& info,
							FileTreeEnumerator& ftEnum)
{
	StringA strTemp(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	//content
	StringA strContent(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_epub_END_body::GetAddress(), g_epub_END_body::GetCount()), strContent);  //may throw
	//project name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$PROJECTNAME$$"), strProjectName, strContent);  //may throw
	//cover name
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$COVERNAME$$"), strCoverName, strContent);  //may throw
	//language
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LCSS$$"), strShortString, strContent);  //may throw
	//topic
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TOPIC$$"), strTopic, strContent);  //may throw
	//tree
	_Generate_Tree_String(ftEnum, strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$TREE$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//landmarks
	strTemp.Clear();
	_Generate_landmarks_String(strProjectName, strCoverName, info, strTemp);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LANDMARKS$$"), StringUtilHelper::To_ConstString(strTemp), strContent);  //may throw
	//save
	return _Generate_Fix_Content_File(strFile, StringUtilHelper::To_ConstString(strContent));
}

//generate description files

inline bool _Epub_Generate_Description_Files(const ConstStringS& strDest, const ConstStringS& strDestRoot,
											const ProjectInfo& info, const HelpLanguageInfo& hlInfo,
											const FileListInfo& flInfo,
											const DirFileList& flMd, const DirFileList& flAux,
											bool bLatest)
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
	if( !_Generate_Css_File(info.IsRTLorder(), info.IsVerticalLine(), bLatest,
							StringUtilHelper::To_ConstString(strFile)) )  //may throw
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
							info.IsRTLorder(), info.IsVerticalLine(),
							flMd, flAux,
							flInfo, info,
							bLatest) )  //may throw
		return false;

	//toc
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	if( bLatest )
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("toc.xhtml")), strFile);  //may throw
	else
		StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("toc.ncx")), strFile);  //may throw
	FsPathHelper::ConvertPathStringToPlatform(strFile);
	if( bLatest ) {
		//END
		if( !_Generate_END_File(StringUtilHelper::To_ConstString(strFile),
								StringUtilHelper::To_ConstString(info.GetProjectName()),
								StringUtilHelper::To_ConstString(info.GetCoverName()),
								StringUtilHelper::To_ConstString(strShortString),
								StringUtilHelper::To_ConstString(info.GetTopic()),
								info, ftEnum) )  //may throw
			return false;
	}
	else {
		//ncx
		if( !_Generate_Ncx_File(StringUtilHelper::To_ConstString(strFile),
								StringUtilHelper::To_ConstString(info.GetProjectName()),
								StringUtilHelper::To_ConstString(info.GetCoverName()),
								StringUtilHelper::To_ConstString(strShortString),
								StringUtilHelper::To_ConstString(info.GetTopic()),
								StringUtilHelper::To_ConstString(info.GetAuthor()),
								StringUtilHelper::To_ConstString(info.GetIdentifier()),
								ftEnum) )  //may throw
			return false;
	}

	//cover html
	StringUtilHelper::MakeString(strDestRoot, strFile);  //may throw
	FsPathHelper::AppendSeparator(strFile);  //may throw
	StringUtilHelper::Append(CS_U2S(StringA(info.GetProjectName())).GetC(), strFile);  //may throw
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
