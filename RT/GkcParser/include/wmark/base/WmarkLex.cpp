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
"TK_INDENT \\s\\s\\s\\s|\\t" "\r\n"
"TK_RETURN \\n|\\r\\n?" "\r\n" 
"TK_COMMENT_START <!\\-\\-" "\r\n"
"special_base \\r\\n\\t" "\r\n"
"text_1 ([^<{special_base}][^{special_base}]*)" "\r\n"
"text_2 (<|<!|<!\\-)" "\r\n"
"text_3 (<[^!{special_base}][^{special_base}]*)" "\r\n"
"text_4 (<![^\\-{special_base}][^{special_base}]*)" "\r\n"
"text_5 (<!\\-[^\\-{special_base}][^{special_base}]*)" "\r\n"
"TK_TEXT {text_1}|{text_2}|{text_3}|{text_4}|{text_5}" "\r\n"
	)
END_STATIC_CONST_STRING(g_wmark_lex)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
