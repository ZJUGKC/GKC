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
This file contains the string of file called wlang.lex,
i.e., the lexical definition of WLANG.
This string will be compiled into the SA,
and will replace the external file.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_wlang_lex

BEGIN_STATIC_CONST_STRING(g_wlang_lex)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"digit [0-9]" "\r\n"
"dot   ." "\r\n"
"num_exp ([eE][\\+\\-]?{digit}+)" "\r\n"
"float_num ({digit}+{dot}{digit}*{num_exp}?|{digit}+{num_exp}|{dot}{digit}+{num_exp}?)" "\r\n"
"TK_CONST_INTEGER  ({digit}+)" "\r\n"
"TK_CONST_FLOAT    {TK_CONST_INTEGER}f|{float_num}f" "\r\n"
"TK_CONST_DOUBLE   {float_num}" "\r\n"
"letter [a-zA-Z_]" "\r\n"
"TK_KEY_CLASS      class" "\r\n"
"TK_KEY_NAMESPACE  namespace" "\r\n"
"TK_IDENTIFIER     {letter}({letter}|{digit})*" "\r\n"
"TK_COMMENT_START  /\\*" "\r\n"
"TK_LINE_COMMENT_START  //" "\r\n"
"hex_digit    [0-9A-Fa-f]" "\r\n"
"hex_byte     ({hex_digit}{hex_digit}?)" "\r\n"
"oct_digit    [0-7]" "\r\n"
"oct_byte     ({oct_digit}({oct_digit}{oct_digit}?)?)" "\r\n"
"escape_char  (\\\\([\'\"\\\\0abfnrtv]|x{hex_byte}|{oct_byte}))" "\r\n"
"byte_char    ([^\\\\\'\"\\x00-\\x1F]|{escape_char})" "\r\n"
"TK_CONST_CHAR     (\'{byte_char}\')" "\r\n"
"TK_CONST_WCHAR    L{TK_CONST_CHAR}" "\r\n"
"TK_CONST_STRING   (\"{byte_char}*\")" "\r\n"
"TK_CONST_WSTRING  L{TK_CONST_STRING}" "\r\n"
"TK_COMMA          ," "\r\n"
"TK_BANG           !" "\r\n"
"TK_SEMI           ;" "\r\n"
"TK_HYPHEN         \\-" "\r\n"
"TK_LCURLY         \\{" "\r\n"
"TK_RCURLY         \\}" "\r\n"
"TK_LPAREN         \\(" "\r\n"
"TK_RPAREN         \\)" "\r\n"
"TK_LBRACKET       \\[" "\r\n"
"TK_RBRACKET       \\]" "\r\n"
"TK_UPARROW        \\^" "\r\n"
"TK_UPWAVE         ~" "\r\n"
"TK_STAR           \\*" "\r\n"
"TK_PLUS           \\+" "\r\n"
"TK_SLASH          /" "\r\n"
"TK_PERCENT        %" "\r\n"
"TK_LEFT_SHIFT     <<" "\r\n"
"TK_RIGHT_SHIFT    >>" "\r\n"
"TK_DOT            {dot}" "\r\n"
"TK_COLON          :" "\r\n"
"TK_EQUAL          =" "\r\n"
"TK_ET             &" "\r\n"
"TK_VERT           \\|" "\r\n"
"TK_LOG_EQ         ==" "\r\n"
"TK_LOG_NE         !=" "\r\n"
"TK_LOG_GE         >=" "\r\n"
"TK_LOG_LE         <=" "\r\n"
"TK_LOG_GT         >" "\r\n"
"TK_LOG_LT         <" "\r\n"
"TK_LOG_AND        &&" "\r\n"
"TK_LOG_OR         \\|\\|" "\r\n"
"TK_SPACE          [\\s\\t]+" "\r\n"
"TK_RETURN         \\r|\\n|\\r\\n" "\r\n"
	)
END_STATIC_CONST_STRING(g_wlang_lex)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
