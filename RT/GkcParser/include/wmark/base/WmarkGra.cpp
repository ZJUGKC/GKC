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
"document : return_set block_list return_set { do_doc_rs_block_list_rs }" "\r\n"
"	| return_set block_list { do_doc_rs_block_list }" "\r\n"
"	| block_list return_set { do_doc_block_list_rs }" "\r\n"
"	| block_list { do_doc_block_list }" "\r\n"
"	;" "\r\n"
"block_list : block_list return_set block { do_block_list_list_block }" "\r\n"
"	| block { do_block_list_block }" "\r\n"
"	;" "\r\n"
"block : block_body TK_RETURN { do_block_block_body_rt }" "\r\n"
"	| block_body { do_block_block_body }" "\r\n"
"	;" "\r\n"
"return_set : return_set TK_RETURN { do_rs_rs_rt }" "\r\n"
"	| TK_RETURN { do_rs_rt }" "\r\n"
"	;" "\r\n"
"block_body : TK_COMMENT_START { do_block_body_comment }" "\r\n"
"	| TK_INDENT { do_block_body_indent }" "\r\n"
"	| TK_TEXT { do_block_body_text }" "\r\n"
"	;" "\r\n"
	)
END_STATIC_CONST_STRING(g_wmark_gra)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
