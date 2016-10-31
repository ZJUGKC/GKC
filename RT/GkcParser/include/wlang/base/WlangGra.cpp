/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains the string of file called wlang.gra,
i.e., the grammar definition of WLANG.
This string will be compiled into the SA,
and will replace the external file.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_wlang_gra

BEGIN_STATIC_CONST_STRING(g_wlang_gra)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"program : namespace_block_list { do_program_ns_list }" "\r\n"
"	;" "\r\n"
"\r\n"
"namespace_block_list : namespace_block_list namespace_block { do_ns_list_block }" "\r\n"
"	| namespace_block { do_ns_block }" "\r\n"
"	;" "\r\n"
"\r\n"
"namespace_block : TK_KEY_NAMESPACE TK_IDENTIFIER TK_LCURLY TK_RCURLY  { do_ns_null_body }" "\r\n"
"	| TK_KEY_NAMESPACE TK_IDENTIFIER TK_LCURLY body TK_RCURLY  { do_ns_body }" "\r\n"
"	;" "\r\n"
"\r\n"
"body : TK_SEMI { do_body_semi }" "\r\n"
"	;" "\r\n"
	)
END_STATIC_CONST_STRING(g_wlang_gra)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
