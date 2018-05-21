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
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  options : -html Process the html file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            sub-functions: -g <charset string> Generate the complete github-style html file. The charset string specifies the character set, such as UTF-8.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("                           -x Generate the complete xhtml file.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  supported charset : UTF-8 US-ASCII Arabic GB2312 Big5 ISO-8859-2 Greek Hebrew Shift-JIS ISO-2022-KR ISO-8859-5 ISO-8859-9")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(g_const_array_help)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
