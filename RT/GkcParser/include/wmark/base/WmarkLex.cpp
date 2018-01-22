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
This file contains the string of file called wmark.lex,
i.e., the lexical definition of WMARK.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_wmark_lex

BEGIN_STATIC_CONST_STRING(g_wmark_lex)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"TK_COMMENT_START <!\\-\\-" "\r\n"
	)
END_STATIC_CONST_STRING(g_wmark_lex)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
