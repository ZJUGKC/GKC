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
This file contains the string of file called wmark.gra,
i.e., the grammar definition of WMARK.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_wmark_gra

BEGIN_STATIC_CONST_STRING(g_wmark_gra)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"block_list : block_list block { do_block_list_list_block }" "\r\n"
"	| block { do_block_list_block }" "\r\n"
"	;" "\r\n"
"block : TK_COMMENT_START { do_block_comment_start }" "\r\n"
"	;" "\r\n"
	)
END_STATIC_CONST_STRING(g_wmark_gra)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
