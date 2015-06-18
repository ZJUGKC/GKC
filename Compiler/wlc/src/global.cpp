/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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

#include "global.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//version
BEGIN_STATIC_CONST_ARRAY(_const_array_version)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("WLANG compiler (wlc) version : 1.0.1.1001")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(_const_array_version)

//help
BEGIN_STATIC_CONST_ARRAY(_const_array_help)
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  Usage : wlc options source-dir-or-file [destination dir]")))
	STATIC_CONST_ARRAY_ENTRY(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("  options: -c compile the source file.")))
	STATIC_CONST_ARRAY_ENTRY_LAST(IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")))
END_STATIC_CONST_ARRAY(_const_array_help)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
