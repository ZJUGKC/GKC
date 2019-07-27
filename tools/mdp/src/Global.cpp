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
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Process Markdown Project (version : 1.0.1.1001)")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(g_const_array_version)

//help
BEGIN_STATIC_CONST_ARRAY(g_const_array_help)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  Usage : mdp <options> file [destination]")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  destination : The destination is a directory.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  options : -m Generate the script for chm.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("            -e [-l] Generate the script for epub.")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("               -l: Generate the latest version.")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(g_const_array_help)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
