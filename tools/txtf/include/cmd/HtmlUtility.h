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
This file contains functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __HTML_UTILITY_H__
#define __HTML_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Generate_Complete_Html

inline int _Generate_Complete_Html(const ConstStringA& strHead, const ConstStringA& strTail,
								const ConstStringS& strSrc, const ConstStringS& strDest) throw()
{
	CallResult cr;

	//src
	ShareCom<IByteStream> spStream;
	cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, spStream);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened!")));
		return 1;
	}
	//dest
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strDest, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened!")));
		return 1;
	}

	uint uWritten;

	//head
	uintptr uSize = strHead.GetCount();
	assert( uSize <= (uintptr)Limits<uint>::Max / sizeof(CharA) );
	cr = spDest.Deref().Write((uintptr)ConstArrayHelper::GetInternalPointer(strHead), (uint)(uSize * sizeof(CharA)), uWritten);
	if( cr.IsFailed() ) {
		_Delete_Output_File(strDest, spDest);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Header writing failed!")));
		return 1;
	}
	if( uWritten != uSize * sizeof(CharA) ) {
		_Delete_Output_File(strDest, spDest);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Header writing failed!")));
		return 1;
	}

	//body
	cr = StreamHelper::Copy(spStream, spDest);
	if( cr.IsFailed() ) {
		_Delete_Output_File(strDest, spDest);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Copy failed!")));
		return 1;
	}

	//tail
	uSize = strTail.GetCount();
	assert( uSize <= (uintptr)Limits<uint>::Max / sizeof(CharA) );
	cr = spDest.Deref().Write((uintptr)ConstArrayHelper::GetInternalPointer(strTail), (uint)(uSize * sizeof(CharA)), uWritten);
	if( cr.IsFailed() ) {
		_Delete_Output_File(strDest, spDest);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Tail writing failed!")));
		return 1;
	}
	if( uWritten != uSize * sizeof(CharA) ) {
		_Delete_Output_File(strDest, spDest);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Tail writing failed!")));
		return 1;
	}

	return 0;
}

// _Cmd_Generate_GitHub_Html

inline int _Cmd_Generate_GitHub_Html(const ConstStringS& strCharset, const ConstStringS& strSrc, const ConstStringS& strDest)
{
	//charset
	if( ConstStringCompareTrait<ConstStringS>::IsNE(strCharset, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF-8"))) ) {
		ConstStringS strCodePage(_Find_CodePage_From_Charset(strCharset));
		if( strCodePage.GetCount() == 0 ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Invalid charset!")));
			return 1;
		}
	}

	//head
	StringA strHead(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_html_github_header::GetAddress(), g_html_github_header::GetCount()), strHead);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CHARSET$$"), CS_S2U(strCharset).GetC(), strHead);  //may throw

	return _Generate_Complete_Html(StringUtilHelper::To_ConstString(strHead),
								ConstStringA(g_html_tail::GetAddress(), g_html_tail::GetCount()),
								strSrc, strDest);
}

// _Cmd_Generate_XHtml_Html

inline int _Cmd_Generate_XHtml_Html(const ConstStringS& strLanguage, const ConstStringS& strCssFile, const ConstStringS& strSrc, const ConstStringS& strDest)
{
	//language
	uint uIndex;
	uint uLCID = _Find_LCID_From_ShortString(strLanguage, uIndex);
	if( uLCID == 0 ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Invalid language!")));
		return 1;
	}

	//head
	StringA strHead(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	StringUtilHelper::MakeString(ConstStringA(g_html_xhtml_header::GetAddress(), g_html_xhtml_header::GetCount()), strHead);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$LANG$$"), CS_S2U(strLanguage).GetC(), strHead);  //may throw
	StringUtilHelper::Replace(DECLARE_TEMP_CONST_STRING(ConstStringA, "$$CSSFILE$$"), CS_S2U(strCssFile).GetC(), strHead);  //may throw

	return _Generate_Complete_Html(StringUtilHelper::To_ConstString(strHead),
								ConstStringA(g_html_xhtml_tail::GetAddress(), g_html_xhtml_tail::GetCount()),
								strSrc, strDest);
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
