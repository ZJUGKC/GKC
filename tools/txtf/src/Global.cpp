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
This file contains global variables.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "Global.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//version
BEGIN_STATIC_CONST_ARRAY(g_const_array_version)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Text File Processing (version : 1.0.1.1001)")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(g_const_array_version)

//help
BEGIN_STATIC_CONST_ARRAY(g_const_array_help)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  Usage : txtf <options> [sub-functions] source-file destination-file")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  options : -bom Process the BOM of text file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            sub-functions: -d Remove the BOM.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                           -a <BOM Type> Add the BOM. The BOM Type can be UTF8, UTF16LE, UTF16BE, UTF32LE, UTF32BE.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            -html Process the html file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            sub-functions: -g <charset string> Generate the complete github-style html file. The charset string specifies the character set, such as UTF-8.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                           -x <language> <css-file> Generate the complete xhtml file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                                                    The language specifies the language short-string, such as en-US.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                                                    The css-file specifies the relative path of css file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            -encoding <source encoding> <destination encoding> Convert the encoding of text.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                                                               The encoding is the same as charset string with the additional UTF16LE, UTF16BE, UTF32LE, UTF32BE.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  supported charset : UTF-8 US-ASCII Arabic GB2312 Big5 ISO-8859-2 Greek Hebrew Shift-JIS ISO-2022-KR ISO-8859-5 ISO-8859-9")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  supported language : en-US ar-SA zh-CN zh-TW cs-CZ el-GR he-IL hu-HU ja-JP ko-KR pl-PL ru-RU sk-SK sl-SI tr-TR bg-BG")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(g_const_array_help)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
